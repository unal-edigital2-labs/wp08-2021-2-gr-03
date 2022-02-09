`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:        UNAL
// Engineer: 
// 
// Create Date:     25.06.2021 11:40:40
// Design Name: 
// Module Name:     infraRed
// Project Name:    SoC Cartographer
// Target Devices:  Nexys4DDR
// Tool Versions: 
// Description:     Driver for InfraRed sensor
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//          Sample code for the Infrared Sensor Module
//          See the Infrared Sensor Module User Guide for more information
// 
//          S = Sensor pins {S1, S2, S3, S4, S5}
// 
//////////////////////////////////////////////////////////////////////////////////


//iR IRC IC ILC IL
//L LC  C RC R


module infrarrojo(
input iC,
input iL,
input iLC,
input iR, 
input iRC,    
output reg C,
output reg L,
output reg LC,  
output reg R,
output reg RC); 
  
always @* begin
    L = iL;
    LC = iLC;
    C = iC;
    RC = iRC;
    R = iR;
end

endmodule



/*
module infrarrojo(
input [4:0]S,
output reg [4:0]dir); 
  
always @* begin
    dir = S;
end

endmodule
*/