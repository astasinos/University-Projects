#include <platform.h>
#include <gpio.h>
#include "delay.h"
#include "lcd.h"
#include "switches.h"
#include "leds.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include <stdio.h>



#define ECHO PA_0
#define TRIGGER PA_1
#define TEMPSENSOR PC_1

TIM_HandleTypeDef  TIM4_Handle;
TIM_HandleTypeDef  TIM3_Handle;
volatile int sendPulseNow = 0;
float tempArray[24] = {0};
	// Every sec += 1. If got temp reset to 0.
volatile int tempCalls = 0;		// How many times I got a temp = 2 minutes
volatile int getTempNow = 0; // Get a temp measurement if 5 sec passed.
volatile float mean = 0;
volatile float prevMean = 0;
volatile int tenSec = 10;
volatile int pulsePrint = 1;  /// to print temp in first 5 secs.
volatile int iamclose = 0;
volatile int tenSecHasNotPassed = 1;
volatile int meanPrint = 0;
volatile int printZesthKryo = 1;
volatile float temperature = 0;
// Start of Temperature Sensor Drivers


int TempSensorInit(void){

	gpio_set_mode(TEMPSENSOR,Output);
	gpio_set(TEMPSENSOR,0);
	delay_us(480);
	gpio_set_mode(TEMPSENSOR,Input);
	delay_us(80);
	
	if(gpio_get(TEMPSENSOR)){
		delay_us(400);
		return -1;
	
	}
	else{
		delay_us(400);
		return 1;
	}

}

void TempSensorWrite(uint8_t value){

	gpio_set_mode(TEMPSENSOR,Output);
	for(int i = 0; i < 8 ; i++){
	
		gpio_set_mode(TEMPSENSOR,Output);
		gpio_set(TEMPSENSOR,0);
		
		
		if((value & (1<<i)) != 0) {
		
				delay_us(1);
				gpio_set_mode(TEMPSENSOR,Input);
				delay_us(60);
			}
		else {
		
				delay_us(60);
				gpio_set_mode(TEMPSENSOR,Input);
		
		}
	
	}

	

}

uint8_t TempSensorRead(void){

	gpio_set_mode(TEMPSENSOR,Input);
	uint8_t val = 0;
	for(int i = 0; i < 8; i++){
		
		gpio_set_mode(TEMPSENSOR,Output);
		gpio_set(TEMPSENSOR,0);
		delay_us(2);
		gpio_set_mode(TEMPSENSOR,Input);
		
		if (gpio_get(TEMPSENSOR))
			val |= 1 << i;
		
		delay_us(60);
		
		
		
	
	
	}
return val;


}
	
	
float getTemperature(){

	
	
	TempSensorInit();
	delay_ms(1);
	TempSensorWrite(0xCC);
	TempSensorWrite(0x44);
	delay_ms(800);
	TempSensorInit();
	delay_ms(1);
	TempSensorWrite(0xCC);
	TempSensorWrite(0xBE);
	uint8_t first_byte = TempSensorRead();
	uint8_t second_byte = TempSensorRead();
	
	uint16_t final = (second_byte << 8 ) | (first_byte);
	float Temperature = (float)final/16;
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

void triggerPulse(){
    
		// Make 10us HIGH Pulse on TRIGGER Pin.
		
		gpio_set(TRIGGER,1);
    delay_us(10);
    gpio_set(TRIGGER,0);
		
		
		
}

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
	
	leds_init();
	lcd_init();
	__enable_irq();
	gpio_set_mode(ECHO,PullDown);
  gpio_set_mode(TRIGGER,Output);
  gpio_set(TRIGGER,0);
	
	// An to kanw me interrupts
	
	//gpio_set_callback(ECHO,timeEchoIR);
  //gpio_set_trigger(echopin,Rising);
	getTempNow = 0;
	timer_init(1000000);
	timer_set_callback(timer_isr);
	timer_disable();
	tenSec=10;
	
	// Timers
	
	__TIM4_CLK_ENABLE();
	TIM4_Handle.Init.Prescaler = 1599;
	TIM4_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM4_Handle.Init.Period = 1000;
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
	// Systick timer 
	
	
	
	uint32_t echoUp;
	uint32_t echoDown;
	float diff;
	temperature = 0;
	int showMeanTen = 0;		// If 1 it will print mean.
	tempCalls = 0;
	getTempNow = 0;
	printZesthKryo = 1;
	for(int i = 0; i < 24 ; i++){
		tempArray[i] = 0;
	
	}
	
	
	while (1) {
		
		if(sendPulseNow){
			
			sendPulseNow = 0;
			triggerPulse();
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
			showMeanTen = 1;
			meanPrint = 1;
			tenSec = 10;
		}
		
		if(getTempNow){
			
			getTempNow = 0;
			temperature = getTemperature();						// Clear screen.
			// DELETE THIS 
			
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
					showMeanTen = 0;
						tenSec = 10;
						pulsePrint = 1;
						printZesthKryo = 0;
						
			
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
		
			leds_set(0,1,0);
		
		
		}
		if(temperature < 25){
		
				leds_set(0,0,1);
			
		}
		if((temperature >= 25 ) && (temperature < 26)){
		
			leds_set(0,0,0);
		
		}
	
	}
}
   
