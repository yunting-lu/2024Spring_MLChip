#ifndef _HPG_MAC_proxy_h_H_
#define _HPG_MAC_proxy_h_H_

#include "systemc.h"

SC_HDL_MODULE( MAC) {
    sc_in<bool> clock;
    sc_in<bool> reset;
    sc_in<sc_uint<4> > a;
    sc_in<sc_uint<4> > b;
    sc_in<sc_uint<8> > c;
    sc_out<sc_uint<4> > a_out;
    sc_out<sc_uint<4> > b_out;
    sc_out<sc_uint<8> > c_out;
    sc_out<sc_uint<9> > result;
    sc_out<bool> done;

    std::string hpg_log_lib;
    std::string hpg_module_name;
    std::string hpg_hdl_src_path;

    std::string libraryName() { return hpg_log_lib; }

    std::string moduleName() { return hpg_module_name; }

    cwr_hdlLangType hdl_language_type() { return sc_hdl_module::cwr_verilog; }

    void getVerilogSourceFiles(std::vector<std::string>& verilog_files) {
        verilog_files.push_back(hpg_hdl_src_path + std::string("/home/2024MLChip/mlchip098/lab3/MAC.v"));
    }

    MAC(sc_module_name name, const char* hdlSrcPath="") : 
        sc_hdl_module(name), hpg_log_lib("CWR_HDL_WORK"), hpg_module_name("MAC"), hpg_hdl_src_path()
        , clock("clock"), reset("reset"), a("a"), b("b"), c("c"), a_out("a_out")
        , b_out("b_out"), c_out("c_out"), result("result"), done("done") {

        if (hdlSrcPath != 0 && strlen(hdlSrcPath) != 0) {
          hpg_hdl_src_path = std::string(hdlSrcPath) + "/";
        }


    }
};

#endif
