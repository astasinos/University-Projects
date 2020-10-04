

module fanari (output reg RED, YLW, GRN,
	      input wire Clock, Reset, CAR);

	reg [1:0] current_state, next_state;	// Two bit signals to describe the current or the next state.
	
	parameter is_GREEN = 2'b00, is_YELLOW = 2'b01, is_RED = 2'b10;	// State encodings
	
	reg [14:0] counter = 15000;		// This timer will begin counting to zero when the light is red. It will reach zero
						// after 15 seconds.
	
	reg [11:0] ylw_counter = 2000;		// This timer will begin counting to zero when the light is yellow. It will reach zero
						// after 2 seconds.
	
	wire Timeout, ylw_Timeout;		// Timeout signals
	
	assign Timeout = (counter == 0);	// Connect the value of the Timeout signal to this boolean expression. Essentialy if
						// the counter reached zero and thus 15 seconds have passed Timeout will become 1.
	
	assign ylw_Timeout = (ylw_counter == 0); // Same logic here. When the ylw_counter reaches 0 ylw_Timeout will become 1.

	
	always @ (posedge Clock or negedge Reset)	// State memory block. Triggered on positive edge clock and on negative edge reset.
		begin: STATE_MEMORY
			if(!Reset)			// If reset is enabled then the light will return to and remain green.
				current_state <= is_GREEN;
			else
				current_state <= next_state;	// If reset is not enabled then every posedge the current state will become 
								// what the next_state signal is.
		end

	// The next block is triggered when the curren state changes or the input CAR changes or any of the timeout signals change.
	// It will calculate the correct next state.
	always @ (current_state or CAR or posedge Timeout or posedge ylw_Timeout)
		begin: NEXT_STATE_LOGIC
			case(current_state)		
				is_GREEN  :  
					      if(CAR == 1'b1)		// If the light is green and a car is detected then next state is yellow.
						next_state = is_YELLOW;
					   	else
						next_state = is_GREEN; // else stay green.
				is_YELLOW  :  if(ylw_Timeout == 1'b1)		// If the light is yellow and 2 seconds have passed
						next_state = is_RED;		// then the next state it the light to be red.
					      else
					      	next_state = is_YELLOW;  	// else stay yellow until 2 seconds have passed.
				is_RED    :  if(Timeout == 1'b1)		// If the light is red and 15 secs have passed
						next_state = is_GREEN;		// (Timeout = 1) then next state is for the light
					  else					// to be green.
					  	next_state = is_RED;		// Else stay red.
       				default :  next_state = is_GREEN;		// Default case is for the light to be green.
			endcase
     		end

	// This next block is the timer that will start counting down if the light turns red , every posedge clock.

	always @ (posedge Clock)
		begin: RED_DOWN_COUNTER
			if(current_state == is_RED)		// If the light is red.
				begin
					if(!Timeout)		// If the Timeout signal has not rised yet 
						counter <= counter - 1;	// Decrease the counter at each posedge. Non blocking call.
					else
						counter = 15000;	// If the Timeout signal was raised (counter gets to 0), reset the counter.
									// Given the previous declarations when the timer is reset
									// the Timeout signal will fall again.
				end
			
		end

	// This block describes the down counter that will count down for 2 seconds when the light turns yellow.
	
	always @ (posedge Clock) 
		begin: YELLOW_DOWN_COUNTER
			if(current_state == is_YELLOW)			// If the light is yellow 
				begin
					if(!ylw_Timeout)		// If the ylw_timeout signal has not been raised ( aka 2 secs have not passed ).
						ylw_counter <= ylw_counter - 1; // then decrease the ylw_counter by one every posedge
					else
						ylw_counter = 2000;		// if the signal has been raised, which happens when ylw_counter = 0,
										// then reset the counter and the signal drops again.
				end
		end
	
	// ouput logic block.
	always @ (current_state)		// only curren_state in sensitivity list because moore FSM.
		begin:  OUTPUT_LOGIC
			case(current_state)
				is_GREEN  :  {RED,YLW,GRN} = 3'b001;	// If light is green then only GRN ouput is 1.
				is_YELLOW :  {RED,YLW,GRN} = 3'b010;	// If light is yellow then only YLW output is 1.
				is_RED    :  {RED,YLW,GRN} = 3'b100;	// If light is red then only RED output is 1.
				default:  {RED,YLW,GRN} = 3'b001;	// default value is for the light to be green.
			endcase
		end

				

endmodule 
