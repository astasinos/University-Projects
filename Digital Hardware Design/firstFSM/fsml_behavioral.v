
module fsml_behavioral (output reg Dout,			// reg for outputs.
			input wire Clock, Reset, Din);		// wire for the input signals.

 	reg [1:0] current_state, next_state;			// 2 bit vectors to describe the current and the next state.
	parameter Start = 2'b00, Midway = 2'b01, Done = 2'b10;	// State encodings based on simple binary encoding.
	
	// State memory block.

	always @ (posedge Clock or negedge Reset) // Triggered on positive edge clock and on negative edge reset.
		begin: STATE_MEMORY
			if(!Reset)		// If reset is enabled the return to and remain at Start.
				current_state <= Start;
			else				// Else every posedge clock the current state will become whatever value the
				current_state <= next_state; // next_state signal contains.
		end
	
	// Next state logic block.
	always @ (current_state or Din)	// triggered every posedge clock or if the input signal changes.
		begin: NEXT_STATE_LOGIC
			case(current_state)
				Start  :  if(Din == 1'b1)	// If the current state is Start and Din = 1 then next state should be Midway.
						next_state = Midway; 
					   else
						next_state = Start;	// Else if Din = 0 remain at Start.
				Midway  :  next_state = Done;	// If current state is Midway then next state should always be Done.
				Done    :  next_state = Start;  // Same here. If current state is Done then next state should always be Start.
       				default :  next_state = Start;	// Default state is Start.
			endcase
     		end

	// Output logic block.
	always @ (current_state or Din) // Din is included in the sensitivity list because we have a Mealy FSM.
		begin:  OUTPUT_LOGIC
			case(current_state)
				Done  :  if(Din == 1'b1) // If the current state is Done and Din = 1 then Dout has to be 1.
					 	Dout = 1'b1;
					 else			// Else Dout = 0.
					 	Dout = 1'b0;
				default  :  Dout = 1'b0;		// Default value of the output signal is zero.
			endcase
		end

				

endmodule 