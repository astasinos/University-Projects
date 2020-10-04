

module fsml_behavioral_onehot (output reg Dout,
			input wire Clock, Reset, Din);

 	reg [2:0] current_state, next_state;	
	parameter Start = 3'b001, Midway = 3'b010, Done = 3'b100;
	
	always @ (posedge Clock or negedge Reset)
		begin: STATE_MEMORY
			if(!Reset)
				current_state <= Start;
			else
				current_state <= next_state;
		end

	always @ (posedge Clock or Din)
		begin: NEXT_STATE_LOGIC
			case(current_state)
				Start  :  if(Din == 1'b1)
						next_state = Midway;
					   else
						next_state = Start;
				Midway  :  next_state = Done;
				Done    :  next_state = Start;
       				default :  next_state = Start;
			endcase
     		end


	always @ (current_state or Din)
		begin:  OUTPUT_LOGIC
			case(current_state)
				Done  :  if(Din == 1'b1)
					 	Dout = 1'b1;
					 else
					 	Dout = 1'b0;
				default  :  Dout = 1'b0;
			endcase
		end

				

endmodule 