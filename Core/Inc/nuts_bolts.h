/*
  nuts_bolts.h - Header file for shared definitions, variables, and functions
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

#ifndef nuts_bolts_h
#define nuts_bolts_h
#include "stm32f4xx_hal.h"
#include "float.h"
#define false 0
#define true 1

#define SOME_LARGE_VALUE FLT_MAX

// CoreXY motor assignments. DO NOT ALTER.
// NOTE: If the A and B motor axis bindings are changed, this effects the CoreXY equations.
#ifdef COREXY
#define A_MOTOR X_AXIS // Must be X_AXIS
#define B_MOTOR Y_AXIS // Must be Y_AXIS
#endif

// Conversions
#define MM_PER_INCH (25.40f)
#define INCH_PER_MM (0.0393701f)
#define TICKS_PER_MICROSECOND (SystemCoreClock / 1000000)

// math constant
#define pi 3.1415926

// Useful macros
#define clear_vector(a) memset(a, 0, sizeof(a))
#define clear_vector_float(a) memset(a, 0.0, sizeof(float) * N_AXIS)
// #define clear_vector_long(a) memset(a, 0.0, sizeof(long)*N_AXIS)
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define isequal_position_vector(a, b) !(memcmp(a, b, sizeof(float) * N_AXIS))

// Bit field and masking macros
#define bit(n) (1 << n)
#define bit_true(x, mask) (x) |= (mask)
#define bit_false(x, mask) (x) &= ~(mask)
#define bit_istrue(x, mask) ((x & mask) != 0)
#define bit_isfalse(x, mask) ((x & mask) == 0)

// Bit operations
#define bit_set(reg, bit) ((reg) |= (1 << (bit)))
#define bit_clr(reg, bit) ((reg) &= ~(1 << (bit)))
#define bit_tgl(reg, bit) ((reg) ^= (1 << (bit)))
#define bit_get(reg, bit) ((reg) & (1 << (bit)))
#define bit_put(reg, bit, val) ((reg) = (reg) & ~(1 << (bit)) | ((val) << (bit)))
inline uint16_t htons(const uint16_t v)
{
  return v << 8 | v >> 8;
}

#define ARRAY_LEN(x) (sizeof(x) / sizeof((*x)))

#define reverse_array(data, size) _Generic((data), float *            \
                                           : reverse_float, int16_t * \
                                           : reverse_int16, int32_t * \
                                           : reverse_int32, default   \
                                           : reverse_int)(data, size)
void reverse_float(float *data, uint8_t n);
void reverse_int16(int16_t *data, uint8_t n);
void reverse_int32(int32_t *data, uint8_t n);
void reverse_int(int *data, uint8_t n);
// Read a floating point value from a string. Line points to the input buffer, char_counter
// is the indexer pointing to the current character of the line, while float_ptr is
// a pointer to the result variable. Returns true when it succeeds
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr);

// Computes hypotenuse, avoiding avr-gcc's bloated version and the extra error checking.
float hypot_f(float x, float y);

float convert_delta_vector_to_unit_vector(float *vector);
float limit_value_by_axis_maximum(float *max_value, float *unit_vec);
uint16_t ModRTU_CRC(uint8_t *buf, int len);
uint32_t hash(char *str);
void upperString(uint8_t *s);
char **split(char *string, char *seperators, int *count);

typedef enum _STRING_NUM_TYPE_HandleTypedef
{
  IsNotNumber,
  IsInteger,
  IsFloat
} STRING_NUM_TYPE_HandleTypedef;
extern STRING_NUM_TYPE_HandleTypedef STRING_NUM_TYPE;

STRING_NUM_TYPE_HandleTypedef string_number_type(char *string);

#endif /* nuts_bolts_h */
