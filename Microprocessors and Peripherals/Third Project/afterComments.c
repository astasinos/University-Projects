#include <platform.h>
#include <gpio.h>
#include "delay.h"
#include "lcd.h"
#include "switches.h"
#include "leds.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include <stdio.h>



#define ECHO PA_4
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
volatile int startedTempMeasuring = 0;
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
	delay_ms(30);	
	printZesthKryo = 1;
	startedTempMeasuring = 1;
	return Temperature;
	
	

}

void printTemperature(char *str,float temperature){
	lcd_clear();
	lcd_set_cursor(0,0);
	char array[20] = {0};
	sprintf(array,"%s:%.2f C",str,temperature);
	lcd_print(array);

}

// End of Temperature Sensor Drivers


void TIM4_IRQHandler(){
	HAL_TIM_IRQHandler(&TIM4_Handle);
	sendPulseNow = 1;
}
void TIM3_IRQHandler(){// Executed every 5 sec.
	HAL_TIM_IRQHandler(&TIM3_Handle);
	
	getTempNow  = 1;
	
}
void timer_isr(){			// If systick timer enabled will be executed once every second.
	
	tenSec = tenSec -1;
	
	
}

int main(void) {
	
	// Initializations
	
	leds_init();
	lcd_init();
	__enable_irq();									// Enable interrupts.
	gpio_set_mode(ECHO,PullDown);		// Connect echo pin of proximity sensor to pull-down resistor.
  gpio_set_mode(TRIGGER,Output);
  gpio_set(TRIGGER,0);						// Begin by setting trigger pin to zero.
	
	
	
										
	
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
	
	
	getTempNow = 0;
	uint32_t echoUp;
	uint32_t echoDown;
	float mean = 0;						// Initialize mean temperature value at zero.
	float pulse_width;								//  Will be used to calculate the distance in cm.
	float distance; 
	float temperature = 0;
	int showMeanTen = 0;		// If 1 it will print mean.
	int tempCalls = 0;					// Initialize at zero.
	getTempNow = 0;					// Initialize as zero so first temperature measurement occurs after 5 secs.
	printZesthKryo = 1;				// Initialize at 1 so that if no object is close enough it will display KRYo or ZESTH.
	for(int i = 0; i < 24 ; i++){		// Initialize temperature array to zero.
		tempArray[i] = 0;
	
	}
	
	
	while (1) {
		
		if(sendPulseNow){					// sendPulseNow will be 1 every 100 ms . So every 100 ms we send a pulse and check if something is close enough.
			
			sendPulseNow = 0;				// set it again to zero.
			
			// Send a pulse by toggling the TRIGGER pin . Set to 1 for 10 us and then to 0.
			gpio_set(TRIGGER,1);
			delay_us(10);
			gpio_set(TRIGGER,0);
			
			
			/* Based on the working principle of the proximity sensor we know that when we send a pulse the ECHO pin will be set high.
			Through polling we capture the value of the timer at the moment the ECHO pin went HIGH ( 1 ) and the moment it dropped LOW ( 0 ).
			The ECHO pin drops to zero when the pulse comes back and thus measuring the time difference between the two captured values and
			multiplying with 0.0017 , in the end the pulse_width variable will contain how many centimeters away the nearest object is. */
			
			while(!gpio_get(ECHO)){}
			echoUp = TIM4->CNT;
			while(gpio_get(ECHO)){}
			echoDown = TIM4->CNT;
			pulse_width = echoDown - echoUp;
			
			pulse_width *= 100;
			distance = pulse_width / 58;
			}
		
		
		// Temperature
		
		if(tempCalls == 24){						// Check if we have measure 24 temperatures in a row. Since we capture a temperature value every 5 seconds.
			mean = 0;											// this means that 2 minutes have passed so it is time to calculate and display the mean value of the last 2 minutes.
			for(int j = 0 ; j < 24 ; j++){
			
				mean += tempArray[j];
			
			}
			mean = mean / 24;											// Calc mean.
			tempCalls = 0;												// Reset to zero.
			timer_enable();												// Enable the timer that will count the 10 seconds that the mean value will be shown.
			showMeanTen = 1;											// Flag to signal showing mean for 10 seconds.
			meanPrint = 1;												// Flag to check if mean has already been printed in those 10 secs so as to not print it again.
			tenSec = 10;													// Set tenSec to 10.
		}
		
		if(getTempNow){												// If 5 seconds have passed from the last captured temperature call getTemperature. 
			
			getTempNow = 0;											// Reset flag to zero.
			temperature = getTemperature();			// get a temperature reading through getTemperature() which returns a float value and was built according to the datasheet.
			tempArray[tempCalls] = temperature;		// Store in the temp array.
			tempCalls += 1;												// Increase the number of temperature readings we have.
			
			}
		
		
			
		if(distance <= 10 && pulsePrint){			// If distance <= 10 cm and the following message has not been printed.
				
				pulsePrint = 0;												// Set to zero because now the message has been printed.
				char strMean[20] = {0};											
				printTemperature("TEMP NOW",temperature);		
				sprintf(strMean,"MEAN: %.2f C",mean);
				lcd_print(strMean);
				meanPrint = 1;
				
			}
		
		if(distance > 10){											// If distance > 10 cm.
		
			
			
			if(showMeanTen){												// If it is time for the mean temperature to be displayed.
			
					printZesthKryo = 0;									// Stop the KRYO/ZESTH message from being displayed.
				
				if((tenSec !=0 ) && meanPrint) {			// If tenseconds have not passed since timer_enable() and mean has not yet been printed.
					
					printTemperature("MEAN",mean);			// print the mean value.
					meanPrint = 0;											// Set to zero since mean has already been printed.
					pulsePrint = 1;											// Give permission to print the TEMP MEAN message if distance gets to <= 10 cm.
					
				} 

					if (tenSec <= 0) {								// If ten seconds have passed.
						lcd_clear();										// Clear the screen.
						lcd_set_cursor(0,0); 
						timer_disable();								// Stop the timer.
					showMeanTen = 0;									// Mean should stop being displayed.
						tenSec = 10;										// Reset ten seconds.
						pulsePrint = 1;									// Give permission to print the message that is normally displayed when distance <= 10 cm.
						printZesthKryo = 1;							// Give permission to print the message that is normally displayed when distance > 10 cm.
						meanPrint = 1;									// lcd was cleared so mean is not being printed so reset to 1.
						
			
				}
			}
			
			
			if(( temperature < 26) && startedTempMeasuring && printZesthKryo){		// if temperature is below 26 C and KRYO message has not already been printed
																																						// and we have started getting temperature readings.
				
				
																																						// The second flag in the if is there because at the first 5 secs of the program
																																						// we have temperature = 0 because we haven't yet called getTemperature so
																																						// this block could potentialy be triggered because 0 < 26
				
				printZesthKryo = 0;																									// message will now be printed so don't print again
				char strKaneiZesthKryo[30] = {0};
				lcd_clear();																												// clear screen.
				lcd_set_cursor(0,0);
				sprintf(strKaneiZesthKryo,"KPYO %.2f C",temperature);
				lcd_print(strKaneiZesthKryo);
				pulsePrint = 1;																											// Give permission to print the TEMP MEAN message again if distance <= 10 cm.
			
			}
			
			if((temperature >= 29)  && printZesthKryo){														// If temperature is > 27 C and the messsage ZESTH has not already been printed.
			
				printZesthKryo = 0;																									// message will now be printed so don't print again
				char strKaneiZesthKryo[30] = {0};
				lcd_clear();																												// Clear screen.
				lcd_set_cursor(0,0);
				sprintf(strKaneiZesthKryo,"ZESTH %.2f C",temperature);
				lcd_print(strKaneiZesthKryo);
				pulsePrint = 1;																											// Give permission to print the TEMP MEAN message again if distance <= 10 cm.
				
			
			}
			
			if(((temperature < 29) && (temperature >= 26))  && printZesthKryo){						// If temperature is between 26 and 27 degrees just clear screen.
			
				printZesthKryo = 0;
				lcd_clear();
				lcd_set_cursor(0,0);
				pulsePrint = 1;
			
			
			}
			
		}
		// LEDs
		if(temperature >= 33) {															// If temp > 28 C then turn on the green light as well as the red. I didn't have a switch.
		
			leds_set(1,1,0);						// Turn green led on and red.
		
		
		}
		if((temperature >= 29) && (temperature < 33) ) {			// if temp only between 27 and 28 C then turn only red light on.
		
			leds_set(0,1,0);						// Turn red led on.
		
		
		}
		if((temperature < 26) && startedTempMeasuring){								// If temperature is below 26 C and we have started getting temperature measurements.
																																// turn the blue light on.
		
				leds_set(0,0,1);					// Turn Blue on.
			
		}
		if((temperature >= 26 ) && (temperature < 29)){								// Else if the temp is between 26 and 27 turn them all of . The screen here prints nothing.
		
			leds_set(0,0,0);
		
		}
	
	}
}
