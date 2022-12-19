/*
  nuts_bolts.c - Shared functions
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "config.h"

#define MAX_INT_DIGITS 8 // Maximum number of digits in int32 (and float)

// Extracts a floating point value from a string. The following code is based loosely on
// the avr-libc strtod() function by Michael Stumpf and Dmitry Xmelkov and many freely
// available conversion method examples, but has been highly optimized for Grbl. For known
// CNC applications, the typical decimal value is expected to be in the range of E0 to E-4.
// Scientific notation is officially not supported by g-code, and the 'E' character may
// be a g-code word on some CNC systems. So, 'E' notation will not be recognized.
// NOTE: Thanks to Radu-Eosif Mihailescu for identifying the issues with using strtod().
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr)
{
  char *ptr = line + *char_counter;
  unsigned char c;

  // Grab first character and increment pointer. No spaces assumed in line.
  c = *ptr++;

  // Capture initial positive/minus character
  bool isnegative = false;
  if (c == '-')
  {
    isnegative = true;
    c = *ptr++;
  }
  else if (c == '+')
  {
    c = *ptr++;
  }

  // Extract number into fast integer. Track decimal in terms of exponent value.
  uint32_t intval = 0;
  int8_t exp = 0;
  uint8_t ndigit = 0;
  bool isdecimal = false;
  while (1)
  {
    c -= '0';
    if (c <= 9)
    {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS)
      {
        if (isdecimal)
        {
          exp--;
        }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
      }
      else
      {
        if (!(isdecimal))
        {
          exp++;
        } // Drop overflow digits
      }
    }
    else if (c == (('.' - '0') & 0xff) && !(isdecimal))
    {
      isdecimal = true;
    }
    else
    {
      break;
    }
    c = *ptr++;
  }

  // Return if no digits have been read.
  if (!ndigit)
  {
    return (false);
  };

  // Convert integer into floating point.
  float fval;
  fval = (float)intval;

  // Apply decimal. Should perform no more than two floating point multiplications for the
  // expected range of E0 to E-4.
  if (fval != 0)
  {
    while (exp <= -2)
    {
      fval *= 0.01f;
      exp += 2;
    }
    if (exp < 0)
    {
      fval *= 0.1f;
    }
    else if (exp > 0)
    {
      do
      {
        fval *= 10.0f;
      } while (--exp > 0);
    }
  }

  // Assign floating point value with correct sign.
  if (isnegative)
  {
    *float_ptr = -fval;
  }
  else
  {
    *float_ptr = fval;
  }

  *char_counter = ptr - line - 1; // Set char_counter to next statement

  return (true);
}

// Simple hypotenuse computation function.
float hypot_f(float x, float y) { return (sqrtf(x * x + y * y)); }

float convert_delta_vector_to_unit_vector(float *vector)
{
  uint8_t idx;
  float magnitude = 0.0f;
  for (idx = 0; idx < N_AXIS; idx++)
  {
    if (vector[idx] != 0.0f)
    {
      magnitude += vector[idx] * vector[idx];
    }
  }
  magnitude = sqrtf(magnitude);
  float inv_magnitude = 1.0f / magnitude;
  for (idx = 0; idx < N_AXIS; idx++)
  {
    vector[idx] *= inv_magnitude;
  }
  return (magnitude);
}

float limit_value_by_axis_maximum(float *max_value, float *unit_vec)
{
  uint8_t idx;
  float limit_value = SOME_LARGE_VALUE;
  for (idx = 0; idx < N_AXIS; idx++)
  {
    if (unit_vec[idx] != 0)
    { // Avoid divide by zero.
      limit_value = min(limit_value, fabsf(max_value[idx] / unit_vec[idx]));
    }
  }
  return (limit_value);
}

/* Compute the MODBUS RTU CRC */
uint16_t ModRTU_CRC(uint8_t *buf, int len)
{
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--)
    { // Loop over each bit
      if ((crc & 0x0001) != 0)
      {            // If the LSB is set
        crc >>= 1; // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else         // Else LSB is not set
        crc >>= 1; // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;
}

/* Parse a text and return pointer to a ListWithLength words and count it */
ListWithLength *getWords(char *text)
{
  // a variable for count words
  int count = 0;

  // keep length of the text
  size_t text_len = strlen(text);

  // a flag indicating the a beginning of a word
  bool new_word = false;

  // an index of a start found a word
  int index_start_word = 0;

  // 2D-array for found word
  // it will be same memory size as the original text
  char **words = malloc(text_len * sizeof(char));

  for (int i = 0; i <= text_len; ++i)
  {

    // if found ascii letter or digits and new no traced early
    // keep index of beginning a new word
    // and change the flag
    if (isalnum(text[i]) != 0)
    {
      if (new_word == false)
      {
        new_word = true;
        index_start_word = i;
      }

      // if it is not ascii letter or digits and a word traced early
      // it means the word ended
    }
    else
    {
      if (new_word == true)
      {

        // allocate a memory for a new word in the array of words
        words[count] = malloc(i - index_start_word * sizeof(char) + 1);

        // copy the found word from the text by indexes
        strncpy(words[count], text + index_start_word, i - index_start_word);

        // change the flag
        new_word = false;

        // increase the counter of words
        count++;
      }
    };
  }

  // bind the found words and it count to a structure and return it
  ListWithLength *list_with_length = malloc(sizeof(ListWithLength));

  list_with_length->length = count;
  list_with_length->list = words;

  return list_with_length;
}

/* Print information of a ListWithLength */
void printListWithLength(ListWithLength *list_with_length)
{
  printf("Total items: %li\n", list_with_length->length);
  for (int i = 0; i < list_with_length->length; ++i)
  {
    printf("%d. %s\n", i + 1, list_with_length->list[i]);
  }
}