`timescale 100us/10us	// #1 delay corresponds to 100 us and the simulation will run with a 10us accuracy.

module fanari_TB();

	wire RED_TB, YLW_TB, GRN_TB;	
	reg Clock_TB, Reset_TB, CAR_TB;
	// declare the design under test with the appropriate port mapping of the testbench signals.
	fanari DUT(RED_TB, YLW_TB, GRN_TB, Clock_TB, Reset_TB, CAR_TB);
	
	// initial block to model the reset functionaliy. Reset will be active for the first 20 * 100 us = 2 ms.
	initial
		begin
			Reset_TB = 1'b0;
		    #20 Reset_TB = 1'b1;
	end
	
	// Clock starts low.
	initial
		begin
			Clock_TB = 1'b0;
	end
	// And every 5 * 100 us = 0.5 ms its value is negated. The period will thus be 1 ms for a full clock cycle.
	always
		begin
			#5 Clock_TB = ~Clock_TB;
	end
	// Input signal model.
	initial 
		begin
			CAR_TB = 1'b0;	// Start a zero.
			#10 CAR_TB = 1'b1; // A car is detected at 10 * 100 us = 1 ms.
			#20 CAR_TB = 1'b0;	// Drop to zero after 2 ms. This is done for testing purposes of course the car can't vanish in such
						// a short period.
			#50 CAR_TB = 1'b1;	// Set to 1 again after 5 ms. This corresponds at t = 8ms . 
			#40 CAR_TB = 1'b0;	// Drop again after 4 ms at t = 12 ms.
	end

endmodule 
