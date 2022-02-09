`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.06.2021 11:40:40
// Design Name: 
// Module Name: infraRed
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

