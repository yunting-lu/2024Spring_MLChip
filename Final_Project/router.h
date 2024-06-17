#ifndef ROUTER_H
#define ROUTER_H

#include "systemc.h"
#include <iostream>
#include <string>

using namespace std;

SC_MODULE( Router ) {
    sc_in  < bool >  rst;
    sc_in  < bool >  clk;

    sc_out < sc_lv<34> >  out_flit[5];
    sc_out < bool >  out_req[5];
    sc_in  < bool >  in_ack[5];

    sc_in  < sc_lv<34> >  in_flit[5];
    sc_in  < bool >  in_req[5];
    sc_out < bool >  out_ack[5];

    int router_id;
    sc_trace_file *tf;
    SC_HAS_PROCESS(Router);

    sc_lv<34> in_buffer[5];
    bool buf_full[5];
    int dest_xy[5];
    int x_dest[5];
    int y_dest[5];
    int out_dir[5];
    bool dir_ready[5];

    bool chout_busy[5];
    int occupied_by[5];


    Router(sc_module_name name, int id, sc_trace_file *tf=nullptr) : sc_module(name)
    {
        router_id = id;
        this->tf = tf;
        SC_METHOD(run);
        sensitive << clk.pos();
        dont_initialize();

        //tracing
        if (tf != nullptr) {
            sc_trace(tf, clk, "router_" + std::to_string(id) + ".clk");
            sc_trace(tf, rst, "router_" + std::to_string(id) + ".rst");
            
            for (int i = 0; i < 5; i++) {
                sc_trace(tf, in_flit[i], "router_" + std::to_string(id) + ".in_flit_" + std::to_string(i));
                sc_trace(tf, in_req[i], "router_" + std::to_string(id) + ".in_req_" + std::to_string(i));
                sc_trace(tf, in_ack[i], "router_" + std::to_string(id) + ".in_ack_" + std::to_string(i));
                sc_trace(tf, out_flit[i], "router_" + std::to_string(id) + ".out_flit_" + std::to_string(i));
                sc_trace(tf, out_req[i], "router_" + std::to_string(id) + ".out_req_" + std::to_string(i));
                sc_trace(tf, out_ack[i], "router_" + std::to_string(id) + ".out_ack_" + std::to_string(i));
                sc_trace(tf, in_buffer[i], "router_" + std::to_string(id) + ".in_buffer_" + std::to_string(i));
                sc_trace(tf, dir_ready[i], "router_" + std::to_string(id) + ".dir_ready_" + std::to_string(i));
                sc_trace(tf, chout_busy[i], "router_" + std::to_string(id) + ".chout_busy_" + std::to_string(i));
            }
        }


    }


    void run(){
        const int NORTH = 0;
        const int SOUTH = 1;
        const int WEST = 2;
        const int EAST = 3;
        const int LOCAL = 4;
        
        int x_coor = router_id % 3;
        int y_coor = router_id / 3;

        if(rst.read()==1){
            for(int i=0; i<5; i++){
                dir_ready[i] = 0;
                chout_busy[i] = 0;
                occupied_by[i] = 0;
                in_buffer[i] = 0;
            }
        }
        else{
            
            //-------------------------------------------------------------//

            //* calc out_dir
            //set dir_ready
            for(int i=0; i<5; i++){
                if(in_buffer[i][33] == 1){
                    
                    // x_dest[i] = ((in_buffer[i]).range(25,24)).to_uint();
                    // y_dest[i] = ((in_buffer[i]).range(27,26)).to_uint();
                    dest_xy[i] = (in_buffer[i].range(27,24)).to_uint();
                    x_dest[i] = dest_xy[i] % 3;
                    y_dest[i] = dest_xy[i] / 3;

                    if(x_dest[i] != x_coor){
                        if((3+x_dest[i]-x_coor) % 3 <= 1){                        
                            out_dir[i] = EAST;
                        }
                        else{
                            out_dir[i] = WEST;
                        }
                    }
                    else if(y_dest[i] != y_coor){
                        if((3+y_dest[i]-y_coor) % 3 <= 1){
                            out_dir[i] = SOUTH;
                        }
                        else{
                            out_dir[i] = NORTH;
                        }
                    }
                    else{
                        out_dir[i] = LOCAL;
                    }

                    dir_ready[i] = 1;
                    
                }
            }

            //-------------------------------------------------------------//

            //* set priority
            for(int out_idx=0; out_idx<5; out_idx++){
                for(int in_idx=0; in_idx<5; in_idx++){
                    //set chout_busy
                    if(!chout_busy[out_idx]){
                        if(out_dir[in_idx]==out_idx && dir_ready[in_idx]){
                            chout_busy[out_idx] = 1;
                            occupied_by[out_idx] = in_idx;
                        }
                    }             
                }
            }

            //-------------------------------------------------------------//

            sc_lv<34> buf_value_temp[5];
            //to avoid the loss of value of in_buffer
            for(int idx=0; idx<5; idx++){
                buf_value_temp[idx] = in_buffer[idx];
            }

            //optimize: buf_full reset before in_req
            //* clear buffer, reset output
            for(int out_idx=0; out_idx<5; out_idx++){
                if(in_ack[out_idx].read() && chout_busy[out_idx]){
                    buf_full[occupied_by[out_idx]] = 0;
                    in_buffer[occupied_by[out_idx]] = 0;
                    if(buf_full[occupied_by[out_idx]]==0){
                        out_req[out_idx].write(0);
                        out_flit[out_idx].write(0);
                    }
                }
            }

            //* reset busy
            for(int out_idx=0; out_idx<5; out_idx++){
                if(in_ack[out_idx] && buf_value_temp[occupied_by[out_idx]][32]==1){
                    dir_ready[occupied_by[out_idx]] = 0;
                    chout_busy[out_idx] = 0;
                    occupied_by[out_idx] = 0;
                }                    
            }

            //-------------------------------------------------------------//

            //* in_req, in_flit -> in_buffer, out_ack
            for(int i=0; i<5; i++){
                if(in_req[i].read() == 1 && buf_full[i] == 0){
                    out_ack[i].write(1);
                    in_buffer[i] = in_flit[i].read();
                    buf_full[i] = 1;
                }
                else{
                    out_ack[i].write(0);
                }
            }

            //-------------------------------------------------------------//

            //* output: out_req, out_flit
            for(int out_idx=0; out_idx<5; out_idx++){
                if(chout_busy[out_idx] && buf_full[occupied_by[out_idx]]){
                    out_req[out_idx].write(1);
                    out_flit[out_idx].write(in_buffer[occupied_by[out_idx]]);
                }
            }


        } //end else //(!rst.read())

    } //end run


};

#endif