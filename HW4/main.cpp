#include "systemc.h"
#include "clockreset.h"
#include "ROM.h"
#include "controller.h"
#include "core.h"
#include "router.h"
#include <string>

int sc_main(int argc, char* argv[])
{
    // =======================
    //   signals declaration
    // =======================
    sc_signal < bool > clk;
    sc_signal < bool > rst;
    //* ROM signals
    sc_signal < int > layer_id;
    sc_signal < bool > layer_id_type;
    sc_signal < bool > layer_id_valid;
    sc_signal < float > data;
    sc_signal < bool > data_valid;
    //* Core signals
    sc_signal < sc_lv<34> > flit_rx_core[16];
    sc_signal < bool > req_rx_core[16];
    sc_signal < bool > ack_rx_core[16];
    sc_signal < sc_lv<34> > flit_tx_core[16];
    sc_signal < bool > req_tx_core[16];
    sc_signal < bool > ack_tx_core[16];
    //* Router signals
    sc_signal < sc_lv<34> > flit_router[4][16];
    sc_signal < bool > req_router[4][16];
    sc_signal < bool > ack_router[4][16];


    // =======================
    //   modules declaration
    // =======================

    // sc_trace_file *tf = sc_create_vcd_trace_file("wave");
    sc_trace_file *tf = nullptr;

    Core* core_ptr[16];
    Router* router_ptr[16];

    Clock m_clock("m_clock", 10);
    Reset m_reset("m_reset", 15);
    ROM m_rom("m_rom");
    Controller m_controller("m_controller", tf);

    for(int i=0; i<16; i++){
        if(i>0) core_ptr[i] = new Core(("m_core_" + to_string(i)).c_str(), i, tf);
        router_ptr[i] = new Router(("m_router_" + to_string(i)).c_str(), i, tf);
    }
    

    // =======================
    //   modules connection
    // =======================
    m_clock( clk );
    m_reset( rst );
    //rom
    m_rom.clk(clk);
    m_rom.rst(rst);
    m_rom.layer_id(layer_id);
    m_rom.layer_id_type(layer_id_type);
    m_rom.layer_id_valid(layer_id_valid);
    m_rom.data(data);
    m_rom.data_valid(data_valid);
    //controller //- replace core0, connect to router0
    m_controller.clk(clk);
    m_controller.rst(rst);
    m_controller.layer_id(layer_id);
    m_controller.layer_id_type(layer_id_type);
    m_controller.layer_id_valid(layer_id_valid);
    m_controller.data(data);
    m_controller.data_valid(data_valid);
    m_controller.flit_rx(flit_rx_core[0]);
    m_controller.req_rx(req_rx_core[0]);
    m_controller.ack_rx(ack_rx_core[0]);
    m_controller.flit_tx(flit_tx_core[0]);
    m_controller.req_tx(req_tx_core[0]);
    m_controller.ack_tx(ack_tx_core[0]);

    //core
    for(int i=1; i<16; i++){
        core_ptr[i]->clk(clk);
        core_ptr[i]->rst(rst);
        core_ptr[i]->flit_rx(flit_rx_core[i]);
        core_ptr[i]->req_rx(req_rx_core[i]);
        core_ptr[i]->ack_rx(ack_rx_core[i]);
        core_ptr[i]->flit_tx(flit_tx_core[i]);
        core_ptr[i]->req_tx(req_tx_core[i]);
        core_ptr[i]->ack_tx(ack_tx_core[i]);
    }
    //router
    int n_bound, s_bound, w_bound, e_bound;

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            router_ptr[i*4+j]->clk(clk);
            router_ptr[i*4+j]->rst(rst);
            //bound
            n_bound = (i>0)?(i-1):(3);
            s_bound = (i<3)?(i+1):(0);
            w_bound = (j>0)?(j-1):(3);
            e_bound = (j<3)?(j+1):(0);

            //out_flit
            router_ptr[i*4+j]->out_flit[0](flit_router[0][n_bound*4+j]);
            router_ptr[i*4+j]->out_flit[1](flit_router[1][s_bound*4+j]);
            router_ptr[i*4+j]->out_flit[2](flit_router[2][i*4+w_bound]);
            router_ptr[i*4+j]->out_flit[3](flit_router[3][i*4+e_bound]);
            router_ptr[i*4+j]->out_flit[4](flit_rx_core[i*4+j]);
            //out_req
            router_ptr[i*4+j]->out_req[0](req_router[0][n_bound*4+j]);
            router_ptr[i*4+j]->out_req[1](req_router[1][s_bound*4+j]);
            router_ptr[i*4+j]->out_req[2](req_router[2][i*4+w_bound]);
            router_ptr[i*4+j]->out_req[3](req_router[3][i*4+e_bound]);
            router_ptr[i*4+j]->out_req[4](req_rx_core[i*4+j]);
            //in_ack
            router_ptr[i*4+j]->in_ack[0](ack_router[1][i*4+j]);
            router_ptr[i*4+j]->in_ack[1](ack_router[0][i*4+j]);
            router_ptr[i*4+j]->in_ack[2](ack_router[3][i*4+j]);
            router_ptr[i*4+j]->in_ack[3](ack_router[2][i*4+j]);
            router_ptr[i*4+j]->in_ack[4](ack_rx_core[i*4+j]);
            //in_flit
            router_ptr[i*4+j]->in_flit[0](flit_router[1][i*4+j]);
            router_ptr[i*4+j]->in_flit[1](flit_router[0][i*4+j]);
            router_ptr[i*4+j]->in_flit[2](flit_router[3][i*4+j]);
            router_ptr[i*4+j]->in_flit[3](flit_router[2][i*4+j]);
            router_ptr[i*4+j]->in_flit[4](flit_tx_core[i*4+j]);
            //in_req
            router_ptr[i*4+j]->in_req[0](req_router[1][i*4+j]);
            router_ptr[i*4+j]->in_req[1](req_router[0][i*4+j]);
            router_ptr[i*4+j]->in_req[2](req_router[3][i*4+j]);
            router_ptr[i*4+j]->in_req[3](req_router[2][i*4+j]);
            router_ptr[i*4+j]->in_req[4](req_tx_core[i*4+j]);
            //out_ack
            router_ptr[i*4+j]->out_ack[0](ack_router[0][n_bound*4+j]);
            router_ptr[i*4+j]->out_ack[1](ack_router[1][s_bound*4+j]);
            router_ptr[i*4+j]->out_ack[2](ack_router[2][i*4+w_bound]);
            router_ptr[i*4+j]->out_ack[3](ack_router[3][i*4+e_bound]);
            router_ptr[i*4+j]->out_ack[4](ack_tx_core[i*4+j]);
        }
    }

    //tracing
    sc_trace(tf, clk, "clk");
    sc_trace(tf, rst, "rst");

    sc_start();
    sc_close_vcd_trace_file(tf);
    return 0;
}