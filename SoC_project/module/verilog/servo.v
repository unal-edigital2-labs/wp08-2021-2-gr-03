`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.06.2021 21:11:04
// Design Name: 
// Module Name: PWM
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module servo(clk, pwm, pos);
input clk;
input [1:0]pos;
output reg pwm;

reg [20:0]contador = 0;

parameter  I = 0, C = 1, D = 2;

always@(posedge clk)begin
	contador = contador + 1;
	if(contador =='d1_000_000) begin
	   contador = 0;
	end
	
	case(pos)
        I:  pwm = (contador < 'd50_000) ? 1:0;
        
        C:  pwm = (contador < 'd150_000) ? 1:0;
        
        D:  pwm = (contador < 'd200_000) ? 1:0;
        
        default:
                pwm = (contador < 'd150_000) ? 1:0;
    endcase

end
endmodule
