#include <platform.h>
#include <gpio.h>
#include "delay.h"
#include "switches.h"
#include "leds.h"


/* 
		If mode is defined the compiled code will work as described in the first case of the project. 
		The LED will turn on and the user will press the button to turn it off. If mode is not defined 
		then the LED will be turned OFF and the user will press the button to turn it on, working as 
		described in the second case of the project.
*/

#define mode "iTurnThemOff"


/* 
		This function is written in assembly and will store the measured value
		in a particular memory location. I chose 0x20000562.
*/


__asm void storeresults(unsigned int measure){

	MOV32 r2, 0x20000562
	STR r0, [r2]
	BX lr

}


volatile int button_pressed;  // Self-explanatory. Is declared as volatile to overcome compiler optimisations


void button_press_isr(long int sources) {
	gpio_set(P_DBG_ISR, 1);
	if ((sources << GET_PIN_INDEX(P_SW)) & (1 << GET_PIN_INDEX(P_SW))) {
		button_pressed = 1;			
		
	
	
	}
	gpio_set(P_DBG_ISR, 0);
}

int main(void) {
	leds_init();
	gpio_set_mode(P_DBG_ISR, Output);
	gpio_set_mode(P_DBG_MAIN, Output);
	gpio_set_mode(P_SW, PullUp);
	gpio_set_trigger(P_SW, Rising);
	gpio_set_callback(P_SW, button_press_isr);

	__enable_irq();

/*
		Delay the start of measurements by 2 seconds so user will be ready.
*/
	
	delay_ms(2000);
	
	
int sample_count = 0;							// Number of samples so far. When it hits 5, it will be reset to 0.
unsigned int reactions[5];				// Response times in ms will be stored in this array.
for(int i = 0 ; i < 5 ; i++) reactions[i] = 0; // Initialize elements to zero.
int delaytime;										// Milliseconds to delay next ON or OFF of LED ( depending on working mode ).
unsigned int counter;							// This variable will be increased by one 1 every 1000 cycles.
unsigned int mean;								// Mean time in ms will be stored here.


	while (1) {
		
		gpio_toggle(P_DBG_MAIN);
		
		/*
				If mode is defined set the LED to ON state and wait for the user to press the button to close it,
				else if it is not defined set it to OFF and wait for him to turn it ON by pressing the button
		*/
		
		#ifdef mode									
			leds_set(1,1,1);
		#endif
		
		#ifndef mode
			leds_set(0,0,0);
		#endif
		
		/*
				From the moment we set the LED to the appropriate state, depending on the working mode, while the button is not pressed we begin increasing
				an unsigned int variable by 1 every 1000 cycles. The default clock is set to 16 MHz. When the button is pressed, the Interrupt handler will be
				called and set button_pressed = 1 and as a result the condition inside the While statement will be false, thus we exit the loop. However, in a
				worst case scenario the button will be pressed exactly before the delay_cycles(1000) is called. Based on this we have a maximum derivation
				in our measured times by 1000 cycles, which divided by 16 MHz is 0.0625 milliseconds. I believe this precision is pretty acceptable.
		*/
		
		counter = 0;
		button_pressed = 0;
		
		while(!button_pressed){ 
			
			delay_cycles(1000);
			counter += 1;
			}
		
		// The following code is executed when we exit the loop, which happens when the button is pressed.	
		
		// Again the following lines depend on the working mode
			
		#ifdef mode
			leds_set(0,0,0);
		#endif
		
		#ifndef mode
			leds_set(1,1,1);
		#endif
		
		reactions[sample_count] = counter / 16;		// Set current reaction to counter / 16. Since counter is increased every 1000 cycles and we have a
																							// 16 MHz clock then 1000 cycles is 1000/ ( 16 * 10^6) seconds or 1/16 ms. Thus counter / 16 	is in ms.
		sample_count += 1; 												// Increase sample count.
		if(sample_count == 5) {										// Check if we have 5 samples
			mean = 0;
			for(int i = 0 ; i < 5; i++){
				mean += reactions[i] / 5;						}				// Calculate the mean.
			storeresults(mean);										// Store it.
			for(int i = 0 ; i < 5 ; i++) reactions[i] = 0;
			sample_count = 0;												// Reset samples to zero
		}
		
		/*
			Choose a random delay value between 1000-10000 milliseconds to delay the next LED state (ON/OFF depending on working mode).
			This way the user cannot estimate when the LED will turn ON/OFF and thus the sampling of his reaction times provides reliable 
			results.
*/			
			
		delaytime = 1000 + (rand(0) % 9000);			
		delay_ms(delaytime);
		

	}
}
	

   
