/**
 * @file    display.h
 * @author  Dingkun <https://github.com/oct19>
 * @brief   This file contains all the function prototypes for
 *           the display.c file
 * @version 0.1
 * @date    2022-11-09
 *
 * @copyright Copyright (c) 2022
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OLEDDISPLAY_H__
#define __OLEDDISPLAY_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define OLED_WARNING_SIZE OLED_SHORT_MESSAGE_LENGTH
#define OLED_TX_SIZE OLED_LONG_MESSAGE_LENGTH
#define OLED_RX_SIZE OLED_LONG_MESSAGE_LENGTH
#define OLED_INFO_SIZE OLED_LONG_MESSAGE_LENGTH


    typedef struct _OLED_HandleTypeDef
    {
        uint8_t Warning[OLED_WARNING_SIZE]; // priority High
        uint8_t Tx[OLED_TX_SIZE];           // priority Normal
        uint8_t Rx[OLED_RX_SIZE];           // priority Normal
        uint8_t Info[OLED_INFO_SIZE];       // priority Low
    } OLED_HandleTypeDef;

    extern OLED_HandleTypeDef OLED;

    void OLED_Init(void);
    void OLED_display_welcome(void);
    void OLED_display_off(void);
    void OLED_Update_Warning(char *msg);
    void OLED_Update_Rx(char *msg);
    void OLED_Update_Tx(char * msg);
    void OLED_Update_Info(char *msg);

#ifdef __cplusplus
}
#endif

#endif /* __OLEDDISPLAY_H__ */