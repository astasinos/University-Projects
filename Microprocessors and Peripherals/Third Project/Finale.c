#include <platform.h>
#include <gpio.h>
#include "delay.h"
#include "lcd.h"
#include "switches.h"
#include "leds.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include <stdio.h>



#define ECHO PA_1
#define TRIGGER PA_0
#define TEMPSENSOR PC_0

TIM_HandleTypeDef  TIM4_Handle;
TIM_HandleTypeDef  TIM3_Handle;
volatile int sendPulseNow = 0;						// Has to be volatile.
float tempArray[24] = {0};
volatile int getTempNow = 0; // Get a temp measurement if 5 sec passed. Has to be volatile.
volatile int tenSec = 10;
volatile int pulsePrint = 1;  /// to print temp in first 5 secs.
volatile int iamclose = 0;
volatile int tenSecHasNotPassed = 1;
volatile int meanPrint = 0;
volatile int printZesthKryo = 1;
//volatile float temperature = 0;
// Start of Temperature Sensor Drivers


uint8_t TempSensorInit(void){
	uint8_t response = 0;
	gpio_set_mode(TEMPSENSOR,Output);
	gpio_set(TEMPSENSOR,0);
	delay_us(480);
	gpio_set_mode(TEMPSENSOR,Input);
	delay_us(80);
	
	if(!gpio_get(TEMPSENSOR)){
		response = 1;
	
	}
	else{
		
		response = -1;
	}
	
	delay_us(400);
	return response;

}

void TempSensorWrite(uint8_t value){

	gpio_set_mode(TEMPSENSOR,Output);
	for(int i = 0; i < 8 ; i++){
		
		
		if((value & (1<<i)) != 0) {
		
				gpio_set_mode(TEMPSENSOR,Output);
				gpio_set(TEMPSENSOR,0);
				delay_us(1);
				gpio_set_mode(TEMPSENSOR,Input);
				delay_us(60);
			}
		else {
				gpio_set_mode(TEMPSENSOR,Output);
				gpio_set(TEMPSENSOR,0);
				delay_us(60);
				gpio_set_mode(TEMPSENSOR,Input);
		
		}
	
	}

	

}

uint8_t TempSensorRead(void){

	
	uint8_t val = 0;
	gpio_set_mode(TEMPSENSOR,Input);
	for(int i = 0; i < 8; i++){
		
		gpio_set_mode(TEMPSENSOR,Output);
		gpio_set(TEMPSENSOR,0);
		delay_us(2);
		gpio_set_mode(TEMPSENSOR,Input);
		
		if (gpio_get(TEMPSENSOR)){
			val |= 1<<i;		// hey here.
		}
		
		
		delay_us(60);
		
		}
return val;


}
	
	
float getTemperature(){

	uint8_t first_byte;
	uint8_t second_byte;
	uint16_t final;
	uint8_t presence;
	float Temperature;
	
	presence = TempSensorInit();
	delay_ms(1);
	TempSensorWrite(0xCC);
	TempSensorWrite(0x44);
	delay_ms(800);
	presence = TempSensorInit();
	delay_ms(1);
	TempSensorWrite(0xCC);
	TempSensorWrite(0xBE);
	first_byte = TempSensorRead();
	second_byte = TempSensorRead();
	
	final = (second_byte << 8 ) | (first_byte);
	Temperature = (float)final/16;
	lcd_clear();
	lcd_set_cursor(0,0);
	delay_ms(30);	
	meanPrint = 1;
	pulsePrint = 1;					// if hand close give permission to print
	printZesthKryo = 1;
	return Temperature;
	
	

}

void printTemperature(char *str,float temperature){
	lcd_clear();
	lcd_set_cursor(0,0);
	char array[20] = {0};
	sprintf(array,"%s: %.2f C",str,temperature);
	lcd_print(array);

}

// End of Temperature Sensor Drivers


void TIM4_IRQHandler(){
	HAL_TIM_IRQHandler(&TIM4_Handle);
	sendPulseNow = 1;
	//pulsePrint = 1;
}
void TIM3_IRQHandler(){//5sec
	HAL_TIM_IRQHandler(&TIM3_Handle);
	
	getTempNow  = 1;
	
}
void timer_isr(){
	
	tenSec = tenSec -1;
	
	
}

int main(void) {
	
	// Initializations
	
	leds_init();
	lcd_init();
	__enable_irq();
	gpio_set_mode(ECHO,PullDown);		// Connect echo pin of proximity sensor to pull-down resistor.
  gpio_set_mode(TRIGGER,Output);
  gpio_set(TRIGGER,0);						// Begin by setting trigger pin to zero.
	
	/*
	// An to kanw me interrupts
	//gpio_set_callback(ECHO,timeEchoIR);
  //gpio_set_trigger(echopin,Rising);
	*/
	
	getTempNow = 0;									// This should probably remain volatile.
	
	/* Systick timer initialization.
		 This timer triggers an interrupt every second.
	   When the mean temperature is calculated then this timer will begin to tick.
	   and every second in its interrupt handler will decrease the tenSec variable by 1.
	   The tenSec variable is initialized to 10 because the mean value must remain in the lcd
	   for 10 seconds. 
		 */
		 
	timer_init(1000000);	
	timer_set_callback(timer_isr);
	timer_disable();								// Disable the timer for now. It will be enabled only when it is time for the mean value to be shown.
	tenSec=10;											// How many seconds the mean value must be shown.
	
	/*
	
		In the following two blocks of code we initialize two different timers, TIM4 and TIM3.
		Our microcontroller has a clock of 16 MHz and as we have tested these timers tick according 
		to that clock. TIM4 has a prescaler value of 1599 and a period of 900, meaning it will generate
		an interrupt every 90 ms. TIM4 is used for the distance sensor. Every 90 ms a trigger pulse 
		will be sent and we will measure the proximity of any object our sensor is facing.
		
		TIM3 will generate an interrupt every 5 seconds. This timer is used to get temperature measurements
		every 5 seconds.
	
	*/
	
	__TIM4_CLK_ENABLE();
	TIM4_Handle.Init.Prescaler = 1599;
	TIM4_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM4_Handle.Init.Period = 900;
	TIM4_Handle.Instance=TIM4;
	HAL_TIM_Base_Init(&TIM4_Handle);
	HAL_TIM_Base_Start_IT(&TIM4_Handle);
	HAL_NVIC_SetPriority(TIM4_IRQn,0,1);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	
	__TIM3_CLK_ENABLE();
	TIM3_Handle.Init.Prescaler = 1599;
	TIM3_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM3_Handle.Init.Period = 50000;
	TIM3_Handle.Instance=TIM3;
	HAL_TIM_Base_Init(&TIM3_Handle);
	HAL_TIM_Base_Start_IT(&TIM3_Handle);
	HAL_NVIC_SetPriority(TIM3_IRQn,0,1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	
	
	uint32_t echoUp;
	uint32_t echoDown;
	float mean = 0;
	float diff;
	float temperature = 0;
	int showMeanTen = 0;		// If 1 it will print mean.
	int tempCalls = 0;					// Initialize at zero.
	getTempNow = 0;					// Initialize as zero so first temperature measurement occurs after 5 secs.
	printZesthKryo = 1;
	for(int i = 0; i < 24 ; i++){
		tempArray[i] = 0;
	
	}
	
	
	while (1) {
		
		if(sendPulseNow){
			
			sendPulseNow = 0;
			//triggerPulse();
			
			gpio_set(TRIGGER,1);
			delay_us(10);
			gpio_set(TRIGGER,0);
			
			//lcd_clear();
			//lcd_set_cursor(0,0);
			while(!gpio_get(ECHO)){}
			echoUp = TIM4->CNT;
			while(gpio_get(ECHO)){}
			echoDown = TIM4->CNT;
			diff = echoDown - echoUp;
			
			diff *= 100;
			diff = diff / 58;
			}
		
		
		// Temperature
		
		if(tempCalls == 24){
			mean = 0;
			for(int j = 0 ; j < 24 ; j++){
			
				mean += tempArray[j];
			
			}
			mean = mean / 24;
			tempCalls = 0;
			timer_enable();
			showMeanTen = 1;											// Flag to signal showing mean for 10 seconds.
			meanPrint = 1;												// Flag to check if mean has already been printed in those 10 secs so as to not print it again.
			tenSec = 10;													// Set tenSec again to 10.
		}
		
		if(getTempNow){
			
			getTempNow = 0;
			temperature = getTemperature();						// Clear screen.
			tempArray[tempCalls] = temperature;
			tempCalls += 1;
			
			}
		
		
			
		if(diff <= 10 && pulsePrint){
				
				pulsePrint = 0;
				char strMean[20] = {0};
				printTemperature("TEMP",temperature);		// Clears screen.
				lcd_print("   ");
				sprintf(strMean,"MEAN: %.2f C",mean);
				lcd_print(strMean);
				meanPrint = 1;
				//printZesthKryo = 1;											// 10/6 1:27			Asto comment an thes otan vazeis to xeri na menei ligo.
				
			}
		
		if(diff > 10){
		
			
			
			if(showMeanTen){
			
					printZesthKryo = 0;
				
				if((tenSec !=0 ) && meanPrint) {
					
					printTemperature("MEAN",mean);
					meanPrint = 0;
					pulsePrint = 1;
					
				} 

					if (tenSec <= 0) {								// LOOK HERE!!!
						lcd_clear();
						lcd_set_cursor(0,0); // just did this.
						timer_disable();
					showMeanTen = 0;
						tenSec = 10;
						pulsePrint = 1;
						printZesthKryo = 1;
						meanPrint = 1;									// 10/6 1:56
						
			
				}
			}
			
			
			if((( temperature < 25) && (temperature > 0 )) && printZesthKryo){
			
				printZesthKryo = 0;
				char strKaneiZesthKryo[30] = {0};
				lcd_clear();
				lcd_set_cursor(0,0);
				sprintf(strKaneiZesthKryo,"BRRRR %.2f C",temperature);
				lcd_print(strKaneiZesthKryo);
				// LOOOOOOK
				pulsePrint = 1;
			
			}
			
			if((temperature >= 26)  && printZesthKryo){
			
				printZesthKryo = 0;
				char strKaneiZesthKryo[30] = {0};
				lcd_clear();
				lcd_set_cursor(0,0);
				sprintf(strKaneiZesthKryo,"ZESTA %.2f C",temperature);
				lcd_print(strKaneiZesthKryo);
				// LOOOOOOK
				pulsePrint = 1;
				
			
			}
			
		}
		// LEDs
		if(temperature >= 26) {
		
			leds_set(0,1,0);						// Turn red led on.
		
		
		}
		if(temperature < 25){
		
				leds_set(0,0,1);					// Turn Blue on.
			
		}
		if((temperature >= 25 ) && (temperature < 26)){
		
			leds_set(0,0,0);
		
		}
	
	}
}
   
