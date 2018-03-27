/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Sets up system hardware */
typedef struct
{

	int 	gpio_p;
	int     gpio_port;
	LPC_GPIO_T  *base;

    bool s;
}gppin;
void gppinset(gppin gp)
		{
			Chip_GPIO_WritePortBit(gp.base,gp.gpio_p,gp.gpio_port,1);
		}
	void gppinreset(gppin gp)
		{
			Chip_GPIO_WritePortBit(gp.base,gp.gpio_p,gp.gpio_port,0);
		}
	bool gppinread(gppin gp)

		{
			return	Chip_GPIO_ReadPortBit(gp.base,gp.gpio_port,gp.gpio_p);
		}

static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, false);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 9, true);
}

/* LED1 toggle thread */
static void vLEDTask1(void *pvParameters)
{
	bool LedState = false;
	int i_1=0;
	bool On=1;
	bool Off=0;
	bool state =true;
	bool state2=true;
	int mulv=1;
	int di=1;
	while (1) {

		Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;
		if (i_1==0)
		{
			Chip_GPIO_WritePortBit(LPC_GPIO, 2, 9, On);
			Chip_GPIO_WritePortBit(LPC_GPIO,	 4, 28, On);
		}

		else
		{
			Chip_GPIO_WritePortBit(LPC_GPIO, 2, 9, Off);
			Chip_GPIO_WritePortBit(LPC_GPIO, 4, 28, On);
		}
		state =Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 20);
		state2 =Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 19);
		if (state)
		{
         di=10;
         mulv=1;
		}

		if(!state2)
		{
			di=1;
		    mulv=2;
		}

         if (i_1==1)
         {
        	 i_1=0;
         }
         else
         {
        	 i_1=1;
         }
		/* About a 3Hz on/off toggle rate */
		vTaskDelay((configTICK_RATE_HZ*mulv)/di);
	}
}

/* LED2 toggle thread */
static void vLEDTask2(void *pvParameters) {
	bool LedState = false;

	    bool st[4];
	    gpiopin gp[4];
	while (1) {
		Board_LED_Set(1, LedState);
		LedState = (bool) !LedState;
		Board_LED_Set(1, LedState);
				LedState = (bool) !LedState;

				  {
		    		     for(int i=0;i<4;i++)
				         {
					      st[i]=gpio_pin_read(gp[i]);
				          }


			    if(st[1]&&st[2])
			    {
			    //	 do stuff
			    }
			    // do stuff
		           }

		/* About a 7Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 14);
	}
}

/* UART (or output) thread */
static void vUARTTask(void *pvParameters) {
	int tickCnt = 0;

	while (1) {
		DEBUGOUT("Tick: %d\r\n", tickCnt);
		tickCnt++;

		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{
	prvSetupHardware();

	/* LED1 toggle thread */
	xTaskCreate(vLEDTask1, (signed char *) "vTaskLed1",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* LED2 toggle thread*/
	xTaskCreate(vLEDTask2, (signed char *) "vTaskLed2",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* UART output thread, simply counts seconds */
	xTaskCreate(vUARTTask, (signed char *) "vTaskUart",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
