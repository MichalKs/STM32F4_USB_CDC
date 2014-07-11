/**
 * @file:   main.c
 * @brief:  USB CDC example
 * @date:   11 July 2014
 * @author: Michal Ksiezopolski
 *
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the
 * accompanying materials are made available
 * under the terms of the GNU Public License
 * v3.0 which accompanies this distribution,
 * and is available at
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <stm32f4xx.h>
#include <stdio.h>
#include <string.h>
#include "timers.h"
#include "led.h"
#include "uart.h"

// USB includes
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"

#define SYSTICK_FREQ 1000 ///< Frequency of the SysTick.

void softTimerCallback(void);

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END; ///< USB device handle

/**
  * @brief  Main function
  * @retval None
  */
int main(void) {

  UART2_Init(); // Initialize USART2 (for printf)
  TIMER_Init(SYSTICK_FREQ); // Initialize timer

  LED_TypeDef led;
  led.nr = LED0;
  led.gpio = GPIOD;
  led.pin = 12;
  led.clk = RCC_AHB1Periph_GPIOD;

  LED_Add(&led); // Add an LED
  printf("Starting program\r\n");

  int8_t timerID = TIMER_AddSoftTimer(2000, softTimerCallback);
  TIMER_StartSoftTimer(timerID); // start soft timer

  // Initialize USB device stack
  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
  

  while (1) {

    TIMER_SoftTimersUpdate();
  }
} 

/**
 * @brief Callback for soft timer
 */
void softTimerCallback(void) {
  LED_Toggle(LED0); // Toggle LED

  uint8_t* str = "Test string sent from STM32F4!!!\r\n";

  VCP_DataTx (str, strlen(str));
  printf("%s", str); // Print test string

}
