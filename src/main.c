/**
 * Copyright (C) 2013 Chetan Patil, http://chetanpatil.info
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * @author Chetan Patil | http://chetanpatil.info
 */

//Include required ST sytem header files, FreeRTOS header files & standar C libraries
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Decalare variables to capture ADC conversion,convert to required voltage level and store it
uint32_t adcValue=0;
uint32_t adcVoltage=0;

//Task to handle ADC conversion
static void AdcTask(void *pvParameters){

      	//ADC1 regular channel16 configuration
        ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_3Cycles);

	while(1){

		//Start ADC1 Conversion
		ADC_SoftwareStartConv(ADC1);

		//NOTE :: Make sure that decimals value is being multipied with adcValue
		adcValue= ADC_GetConversionValue(ADC1);
		adcVoltage = adcValue * (5.0/4096.0);

		//Wait Until Transmit Finishes
	        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		//Send the data to USART2
			USART_SendData(USART2,adcVoltage);

	}

}

//Initialise required ADC1, USART2 & GPIOS
void initx(void){
  
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	
       	//Enable ADC1 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//Enable the ADC1 GPIO Clock	
      	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 

	//Initializing GPIO for ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//Initializing GPIO 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//ADC Common Init

	//Use Independent mode
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	//ADC1 Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	//Enabling Temperature sensor
	ADC_TempSensorVrefintCmd(ENABLE);

	//Enable ADC1
	ADC_Cmd(ADC1, ENABLE);

	//Structure With Data For USART Configuration
	USART_InitTypeDef USART_InitStructure;

	//Setup USART2 TX Pin As Alternate Function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Enable GPIO Clocks For USART2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//Enable Clocks for USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//USART Parameters
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No ;
        USART_InitStructure.USART_Mode = USART_Mode_Tx;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	//Configuring And Enabling USART2
	USART_Init(USART2,&USART_InitStructure);
        USART_Cmd(USART2, ENABLE);

	//Comment out below line if you need full FPU support	
	//SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2)); 

}

int main(void){

	//Call initialize
	initx();
	
	//AdcTask to capture data from Temperature Sensor and convert to Digital
	xTaskCreate(AdcTask, (signed char*)"AdcTask", 128, NULL, tskIDLE_PRIORITY+1, NULL);

	//Give control scheduler
	vTaskStartScheduler();

}
