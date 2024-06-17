#ifndef _HPG_router_proxy_h_H_
#define _HPG_router_proxy_h_H_

#include "systemc.h"

SC_HDL_MODULE( router) {
    sc_in<bool> rst;
    sc_in<bool> clk;
    sc_out<sc_lv<34> > out_flit_0;
    sc_out<sc_lv<34> > out_flit_1;
    sc_out<sc_lv<34> > out_flit_2;
    sc_out<sc_lv<34> > out_flit_3;
    sc_out<sc_lv<34> > out_flit_4;
    sc_out<bool> out_req_0;
    sc_out<bool> out_req_1;
    sc_out<bool> out_req_2;
    sc_out<bool> out_req_3;
    sc_out<bool> out_req_4;
    sc_in<bool> in_ack_0;
    sc_in<bool> in_ack_1;
    sc_in<bool> in_ack_2;
    sc_in<bool> in_ack_3;
    sc_in<bool> in_ack_4;
    sc_in<sc_lv<34> > in_flit_0;
    sc_in<sc_lv<34> > in_flit_1;
    sc_in<sc_lv<34> > in_flit_2;
    sc_in<sc_lv<34> > in_flit_3;
    sc_in<sc_lv<34> > in_flit_4;
    sc_in<bool> in_req_0;
    sc_in<bool> in_req_1;
    sc_in<bool> in_req_2;
    sc_in<bool> in_req_3;
    sc_in<bool> in_req_4;
    sc_out<bool> out_ack_0;
    sc_out<bool> out_ack_1;
    sc_out<bool> out_ack_2;
    sc_out<bool> out_ack_3;
    sc_out<bool> out_ack_4;

    std::string hpg_log_lib;
    std::string hpg_module_name;
    std::string hpg_hdl_src_path;

    std::string libraryName() { return hpg_log_lib; }

    std::string moduleName() { return hpg_module_name; }

    cwr_hdlLangType hdl_language_type() { return sc_hdl_module::cwr_verilog; }

    void getVerilogSourceFiles(std::vector<std::string>& verilog_files) {
        verilog_files.push_back(hpg_hdl_src_path + std::string("/home/2024MLChip/mlchip098/fp_try/router.v"));
    }

    router(sc_module_name name, int router_id = 0, int x_coor = 0, int y_coor = 0
        , int DIR = 5, int NORTH = 0, int SOUTH = 1, int WEST = 2, int EAST = 3
        , int LOCAL = 4, const char* hdlSrcPath="") : 
        sc_hdl_module(name), hpg_log_lib("CWR_HDL_WORK"), hpg_module_name("router"), hpg_hdl_src_path()
        , rst("rst"), clk("clk"), out_flit_0("out_flit_0"), out_flit_1("out_flit_1")
        , out_flit_2("out_flit_2"), out_flit_3("out_flit_3"), out_flit_4("out_flit_4")
        , out_req_0("out_req_0"), out_req_1("out_req_1"), out_req_2("out_req_2")
        , out_req_3("out_req_3"), out_req_4("out_req_4"), in_ack_0("in_ack_0"), in_ack_1("in_ack_1")
        , in_ack_2("in_ack_2"), in_ack_3("in_ack_3"), in_ack_4("in_ack_4"), in_flit_0("in_flit_0")
        , in_flit_1("in_flit_1"), in_flit_2("in_flit_2"), in_flit_3("in_flit_3")
        , in_flit_4("in_flit_4"), in_req_0("in_req_0"), in_req_1("in_req_1"), in_req_2("in_req_2")
        , in_req_3("in_req_3"), in_req_4("in_req_4"), out_ack_0("out_ack_0"), out_ack_1("out_ack_1")
        , out_ack_2("out_ack_2"), out_ack_3("out_ack_3"), out_ack_4("out_ack_4") {

        if (hdlSrcPath != 0 && strlen(hdlSrcPath) != 0) {
          hpg_hdl_src_path = std::string(hdlSrcPath) + "/";
        }

        ncwr_set_hdl_param("router_id", router_id);
        ncwr_set_hdl_param("x_coor", x_coor);
        ncwr_set_hdl_param("y_coor", y_coor);
        ncwr_set_hdl_param("DIR", DIR);
        ncwr_set_hdl_param("NORTH", NORTH);
        ncwr_set_hdl_param("SOUTH", SOUTH);
        ncwr_set_hdl_param("WEST", WEST);
        ncwr_set_hdl_param("EAST", EAST);
        ncwr_set_hdl_param("LOCAL", LOCAL);

    }
};

#endif
