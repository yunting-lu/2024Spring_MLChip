`timescale 1ns/10ps
module router (
    rst,
    clk,
    out_flit_0, out_flit_1, out_flit_2, out_flit_3, out_flit_4,
    out_req_0, out_req_1 ,out_req_2, out_req_3, out_req_4,
    in_ack_0, in_ack_1, in_ack_2, in_ack_3, in_ack_4,
    in_flit_0, in_flit_1, in_flit_2, in_flit_3, in_flit_4,
    in_req_0, in_req_1, in_req_2, in_req_3, in_req_4,
    out_ack_0, out_ack_1, out_ack_2, out_ack_3, out_ack_4
);

input               rst, clk;
input               in_ack_0, in_ack_1, in_ack_2, in_ack_3, in_ack_4;
input       [33:0]  in_flit_0, in_flit_1, in_flit_2, in_flit_3, in_flit_4;
input               in_req_0, in_req_1, in_req_2, in_req_3, in_req_4;

output reg  [33:0]  out_flit_0, out_flit_1, out_flit_2, out_flit_3, out_flit_4;
output reg          out_req_0, out_req_1 ,out_req_2, out_req_3, out_req_4;
output reg          out_ack_0, out_ack_1, out_ack_2, out_ack_3, out_ack_4;


// ===============================================================
//  					WIRE/REG DECLARATION 
// ===============================================================

parameter router_id = 0; //TODO
parameter x_coor = 0;
parameter y_coor = 0;
parameter DIR = 5;
parameter NORTH = 0;
parameter SOUTH = 1;
parameter WEST  = 2;
parameter EAST  = 3;
parameter LOCAL = 4;

integer i, j;

//INPUT
reg         in_ack[0:4];
reg [33:0]  in_flit[0:4];
reg         in_req[0:4];

//OUTPUT
reg         out_ack_comb[0:4];
reg [33:0]  out_flit_comb[0:4];
reg         out_req_comb[0:4];
reg [33:0]  out_flit[0:4];

//DESIGN
reg [33:0]  in_buffer[0:4];
reg         buf_full[0:4];
// reg [1:0]   x_coor, y_coor;
reg [3:0]   dest_xy[0:4];
reg [1:0]   x_dest[0:4];
reg [1:0]   y_dest[0:4];

reg [2:0]   out_dir_comb[0:4];
reg         dir_ready_comb[0:4];
reg [2:0]   out_dir[0:4];
reg         dir_ready[0:4];
reg         chout_busy[0:4];
reg [2:0]   occupied_by[0:4];



// initial begin
//     $fsdbDumpfile("router.fsdb");
//     $fsdbDumpvars(0,"+mda");
// end



// ===============================================================
//  					INPUT 
// ===============================================================

always @(*) begin
    in_ack[0] = in_ack_0;   in_flit[0] = in_flit_0; in_req[0] = in_req_0;
    in_ack[1] = in_ack_1;   in_flit[1] = in_flit_1; in_req[1] = in_req_1;
    in_ack[2] = in_ack_2;   in_flit[2] = in_flit_2; in_req[2] = in_req_2;
    in_ack[3] = in_ack_3;   in_flit[3] = in_flit_3; in_req[3] = in_req_3;
    in_ack[4] = in_ack_4;   in_flit[4] = in_flit_4; in_req[4] = in_req_4;
end

// ===============================================================
//  					OUTPUT 
// ===============================================================

always @(posedge clk) begin
    if(rst) begin
        out_ack_0 <= 1'b0;  out_flit_0 <= 34'b0;    out_req_0 <= 1'b0;
        out_ack_1 <= 1'b0;  out_flit_1 <= 34'b0;    out_req_1 <= 1'b0;
        out_ack_2 <= 1'b0;  out_flit_2 <= 34'b0;    out_req_2 <= 1'b0;
        out_ack_3 <= 1'b0;  out_flit_3 <= 34'b0;    out_req_3 <= 1'b0;
        out_ack_4 <= 1'b0;  out_flit_4 <= 34'b0;    out_req_4 <= 1'b0;
        $display("router id: %5d", router_id);
        $display("x_coor: %2d, y_coor: %2d", x_coor, y_coor);
    end
    else begin
        out_ack_0 <= out_ack_comb[0];   out_flit_0 <= out_flit_comb[0]; out_req_0 <= out_req_comb[0];
        out_ack_1 <= out_ack_comb[1];   out_flit_1 <= out_flit_comb[1]; out_req_1 <= out_req_comb[1];
        out_ack_2 <= out_ack_comb[2];   out_flit_2 <= out_flit_comb[2]; out_req_2 <= out_req_comb[2];
        out_ack_3 <= out_ack_comb[3];   out_flit_3 <= out_flit_comb[3]; out_req_3 <= out_req_comb[3];
        out_ack_4 <= out_ack_comb[4];   out_flit_4 <= out_flit_comb[4]; out_req_4 <= out_req_comb[4];
    end
end

always @(*) begin
    out_flit[0] = out_flit_0;
    out_flit[1] = out_flit_1;
    out_flit[2] = out_flit_2;
    out_flit[3] = out_flit_3;
    out_flit[4] = out_flit_4;
end

genvar in_idx;
generate
    for(in_idx=0; in_idx<DIR; in_idx=in_idx+1) begin: GEN_OUT_ACK
        always @(*) begin
            if(in_req[in_idx] && buf_full[in_idx]==1'b0)    out_ack_comb[in_idx] = 1'b1;
            else                                            out_ack_comb[in_idx] = 1'b0;
        end
    end
endgenerate

// ===============================================================
//  					DESIGN 
// ===============================================================

//in_buffer
generate
    for(in_idx=0; in_idx<DIR; in_idx=in_idx+1) begin: GEN_IN_BUFFER
        always @(posedge clk) begin
            if(rst) begin
                buf_full[in_idx] <= 1'b0;
                in_buffer[in_idx] <= 34'd0;
            end
            else if(in_req[in_idx]==1'b1 && buf_full[in_idx]==1'b0) begin
                buf_full[in_idx] <= 1'b1;
                in_buffer[in_idx] <= in_flit[in_idx];
            end
            //clear
            else if(in_ack[0] && chout_busy[0] && in_idx==occupied_by[0]) begin
                buf_full[in_idx] <= 1'b0;
                in_buffer[in_idx] <= 34'd0;
            end
            else if(in_ack[1] && chout_busy[1] && in_idx==occupied_by[1]) begin
                buf_full[in_idx] <= 1'b0;
                in_buffer[in_idx] <= 34'd0;
            end
            else if(in_ack[2] && chout_busy[2] && in_idx==occupied_by[2]) begin
                buf_full[in_idx] <= 1'b0;
                in_buffer[in_idx] <= 34'd0;
            end
            else if(in_ack[3] && chout_busy[3] && in_idx==occupied_by[3]) begin
                buf_full[in_idx] <= 1'b0;
                in_buffer[in_idx] <= 34'd0;
            end
            else if(in_ack[4] && chout_busy[4] && in_idx==occupied_by[4]) begin
                buf_full[in_idx] <= 1'b0;
                in_buffer[in_idx] <= 34'd0;
            end
            else begin
                buf_full[in_idx] <= buf_full[in_idx];
                in_buffer[in_idx] <= in_buffer[in_idx];
            end
        end
    end
endgenerate

//direction
// always @(*) begin
//     x_coor = router_id[1:0];
//     y_coor = router_id / 3;
// end

generate
    for(in_idx=0; in_idx<DIR; in_idx=in_idx+1) begin: GEN_XY
        always @(*) begin
            dest_xy[in_idx] = in_buffer[in_idx][27:24];
            x_dest[in_idx] = dest_xy[in_idx] % 3;
            y_dest[in_idx] = dest_xy[in_idx] / 3;
        end
    end
endgenerate

//dir_ready, out_dir
generate
    for(in_idx=0; in_idx<DIR; in_idx=in_idx+1) begin: GEN_DIR_COMB
        always @(*) begin
            if(in_buffer[in_idx][33]==1'b1) begin
                // $display("dir: x_coor: %2d, y_coor: %2d", x_coor, y_coor);
                // $display("dir: x_dest: %2d, y_dest: %2d", x_dest[in_idx], y_dest[in_idx]);
                if(x_dest[in_idx] != x_coor) begin
                    if((3+x_dest[in_idx]-x_coor) % 3 <= 1)  out_dir_comb[in_idx] = EAST;
                    else                                    out_dir_comb[in_idx] = WEST;
                end
                else if(y_dest[in_idx] != y_coor) begin
                    if((3+y_dest[in_idx]-y_coor) % 3 <= 1)  out_dir_comb[in_idx] = SOUTH;
                    else                                    out_dir_comb[in_idx] = NORTH;
                end
                else                                        out_dir_comb[in_idx] = LOCAL;
                // $display("out_dir[%2d] is %2d", in_idx, out_dir_comb[in_idx]);
                dir_ready_comb[in_idx] = 1'b1;
            end
            //clear
            else if(in_ack[0] && chout_busy[0] && in_buffer[occupied_by[0]][32] && in_idx==occupied_by[0]) begin
                dir_ready_comb[in_idx] = 1'b0;
                out_dir_comb[in_idx] = 'd5;
            end
            else if(in_ack[1] && chout_busy[1] && in_buffer[occupied_by[1]][32] && in_idx==occupied_by[1]) begin
                dir_ready_comb[in_idx] = 1'b0;
                out_dir_comb[in_idx] = 'd5;
            end
            else if(in_ack[2] && chout_busy[2] && in_buffer[occupied_by[2]][32] && in_idx==occupied_by[2]) begin
                dir_ready_comb[in_idx] = 1'b0;
                out_dir_comb[in_idx] = 'd5;
            end
            else if(in_ack[3] && chout_busy[3] && in_buffer[occupied_by[3]][32] && in_idx==occupied_by[3]) begin
                dir_ready_comb[in_idx] = 1'b0;
                out_dir_comb[in_idx] = 'd5;
            end
            else if(in_ack[4] && chout_busy[4] && in_buffer[occupied_by[4]][32] && in_idx==occupied_by[4]) begin
                dir_ready_comb[in_idx] = 1'b0;
                out_dir_comb[in_idx] = 'd5;
            end
            else begin
                out_dir_comb[in_idx] = out_dir[in_idx];
                dir_ready_comb[in_idx] = dir_ready[in_idx];
            end
        end
    end
endgenerate
generate
    for(in_idx=0; in_idx<DIR; in_idx=in_idx+1) begin: GEN_DIR
        always @(posedge clk) begin
            if(rst) begin
                dir_ready[in_idx] <= 1'b0;
                out_dir[in_idx] <= 3'd5;
            end
            else begin
                dir_ready[in_idx] <= dir_ready_comb[in_idx];
                out_dir[in_idx] <= out_dir_comb[in_idx];
            end
        end
    end
endgenerate

//chout_busy, occupied_by
genvar out_idx;
generate
    for(out_idx=0; out_idx<DIR; out_idx=out_idx+1) begin: GEN_CHOUT
        always @(*) begin
            if(dir_ready[0]==1'b1 && out_dir[0]==out_idx) begin
                chout_busy[out_idx] = 1'b1;
                occupied_by[out_idx] = 0;
            end
            else if(dir_ready[1]==1'b1 && out_dir[1]==out_idx) begin
                chout_busy[out_idx] = 1'b1;
                occupied_by[out_idx] = 1;
            end
            else if(dir_ready[2]==1'b1 && out_dir[2]==out_idx) begin
                chout_busy[out_idx] = 1'b1;
                occupied_by[out_idx] = 2;
            end
            else if(dir_ready[3]==1'b1 && out_dir[3]==out_idx) begin
                chout_busy[out_idx] = 1'b1;
                occupied_by[out_idx] = 3;
            end
            else if(dir_ready[4]==1'b1 && out_dir[4]==out_idx) begin
                chout_busy[out_idx] = 1'b1;
                occupied_by[out_idx] = 4;
            end
            else begin
                chout_busy[out_idx] = 1'b0;
                occupied_by[out_idx] = 'd0;
            end
        end
    end
endgenerate

//output
generate
    for(out_idx=0; out_idx<DIR; out_idx=out_idx+1) begin: GEN_OUT_REQ
        always @(*) begin
            if(chout_busy[out_idx]==1'b1 && buf_full[occupied_by[out_idx]]==1'b1 && in_ack[out_idx]==1'b0) begin
                out_req_comb[out_idx] = 1'b1;
                out_flit_comb[out_idx] = in_buffer[occupied_by[out_idx]];
            end
            else begin
                out_req_comb[out_idx] = 1'b0;
                out_flit_comb[out_idx] = 'd0;
            end
        end
    end
endgenerate


// ===============================================================
//  					DEBUG 
// ===============================================================

always @(posedge clk) begin
    for(i=0; i<DIR; i=i+1) begin
        if(in_req[i]==1'b1) begin
            // $display("router %2d receives input from %2d: %34b", router_id, i, in_flit[i]);
        end
        if(in_ack[i]==1'b1) begin
            // $display("router %2d          outputs to %2d: %34b", router_id, i, out_flit[i]);
        end
    end
end


endmodule
