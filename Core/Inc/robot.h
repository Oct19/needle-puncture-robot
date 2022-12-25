/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ROBOT_H__
#define __ROBOT_H__

#ifdef __cplusplus
extern "C"
{
#endif


/* Version system */
#define ROBOT_NAME "needle-puncture-robot"
#define ROBOT_VERSION 1.0

/* Define standard libraries used by robot */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "stdlib.h"
#include "stdbool.h"
#include "ctype.h"
#include "string.h"
#include "stdint.h"
#include "inttypes.h"
#include "math.h"

/* Config file */
#include "config.h"

/* Define the system include files. NOTE: Do not alter organization */
/* Generated by CubeMX */
#include "main.h"
#include "gpio.h"
#include "dma.h"
#include "tim.h"
#include "i2c.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

/* Imported scripts and libraries */
#include "printf.h"
#include "nuts_bolts.h"
#include "ssd1306.h"

/* User written */
#include "buzzer.h"
#include "communication.h"
#include "command.h"
#include "stepper.h"
#include "force_sensor.h"
#include "motion.h"
#include "OLEDdisplay.h"
#include "state.h"

#ifdef __cplusplus
}
#endif

#endif /* __ROBOT_H__ */
