`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:				UNAL
// Engineer:			Julian Andres Castro Pardo
// 
// Create Date:		05:31:14 28/01/2022
// Design Name: 
// Module Name:    	motores
// Project Name:		SOC Robot Cartógrafo
// Target Devices:
// Tool versions:
// Description:		Driver de control de motores DC con Puente H
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//	
//					FPGA PIN MAPPING TO L298 MODULE
//							
//							FPGA	->	L298
//							IN[3]	->	IN1
//							IN[2]	->	IN2
//							IN[1]	->	IN3
//							IN[0]	->	IN4
//							
//							Motor A{IN1,IN2}
//							Motor B{IN3,IN4}
//							
//////////////////////////////////////////////////////////////////////////////////

module motores(
    input clk,
    input [2:0] movimiento,
    output reg [3:0] IN			// Input del módulo puente H L298
);


parameter PAUSA=0, RETROCESO=1, AVANCE=2, GIROD=3, GIROI=4;

always @(posedge clk) 
begin
      case(movimiento)
        AVANCE:		IN = 4'b0110;
        RETROCESO:  IN = 4'b1001;
        PAUSA:	    IN = 4'b0000;
        GIROD:	    IN = 4'b0101;
        GIROI:	    IN = 4'b1010;
      default:
					IN = 4'b0000;
      endcase
  end
endmodule