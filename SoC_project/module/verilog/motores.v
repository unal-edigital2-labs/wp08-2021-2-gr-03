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

// A - AVANCE
// R - RETROCESO
// P - PAUSA
// GD - GIRO DERECHA
// GI - GIRO IZQUIERDA

module motores(
    input clk,
    input [2:0] movimiento,
    output reg [3:0] IN			// Input del módulo puente H L298
);


parameter A=0, R=1, P=2, GD=3, GI=4;

always @(posedge clk) 
begin
      case(movimiento)
        A:	IN = 4'b0110;
        R:  IN = 4'b1001;
        P:	IN = 4'b0000;
        GD:	IN = 4'b0101;
        GI:	IN = 4'b1010;
      default:
					IN = 4'b0000;
      endcase
  end
endmodule