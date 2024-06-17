`timescale 1ns/10ps
module MAC (
    clock,
    reset,
    a,
    b,
    c,
    a_out,
    b_out,
    c_out,
    result,
    done
);

input                       clock, reset;
input           [  3:0 ]    a, b;
input           [  7:0 ]    c;

output reg      [  3:0 ]    a_out, b_out;
output reg      [  7:0 ]    c_out;
output reg      [  8:0 ]    result;
output reg                  done;

// Implement your HDL here
wire [8:0] result_comb;


assign result_comb = a * b + c;


always @(posedge clock or posedge reset) begin
    if(reset) begin
        a_out <= 0;
        b_out <= 0;
        c_out <= 0;
        result <= 0;
        done <= 0;
    end
    else begin
        a_out <= a;
        b_out <= b;
        c_out <= c;
        result <= result_comb;
        done <= 1;
    end
end


endmodule
