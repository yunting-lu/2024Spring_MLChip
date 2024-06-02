#ifndef CORE_H
#define CORE_H

#include "systemc.h"
#include "pe.h"
#include <iostream>
#include <string>
using namespace std;

SC_MODULE( Core ) {
    sc_in  < bool >  rst;
    sc_in  < bool >  clk;
    // receive
    sc_in  < sc_lv<34> > flit_rx;	// The input channel
    sc_in  < bool > req_rx;	        // The request associated with the input channel
    sc_out < bool > ack_rx;	        // The outgoing ack signal associated with the input channel
    // transmit
    sc_out < sc_lv<34> > flit_tx;	// The output channel
    sc_out < bool > req_tx;	        // The request associated with the output channel
    sc_in  < bool > ack_tx;	        // The outgoing ack signal associated with the output channel

    PE pe;

    int core_id;
    sc_trace_file *tf;
    SC_HAS_PROCESS(Core);

    sc_buffer < bool > handshake;
    sc_lv<34> flit_rec;
    bool rec_input, rec_weight, rec_bias;
    bool out_ready;
    int cnt_out = 0;
    sc_lv<34> flit_send;



    Packet* p_input = new Packet();
    Packet* p_weight = new Packet();
    Packet* p_bias = new Packet();

    Packet p_output;


    Core(sc_module_name name, int id, sc_trace_file *tf=nullptr) : sc_module(name), pe()
    {
        core_id = id;

        pe.init(core_id);
        // cout << "Core " << core_id << " init" << endl;

        this->tf = tf;
        SC_METHOD(run);
        sensitive << clk.pos();
        dont_initialize();

        //tracing
        if (tf != nullptr) {
            sc_trace(tf, clk, "core_" + std::to_string(id) + ".clk");
            sc_trace(tf, rst, "core_" + std::to_string(id) + ".rst");
            sc_trace(tf, flit_rx, "core_" + std::to_string(id) + ".flit_rx");
            sc_trace(tf, req_rx, "core_" + std::to_string(id) + ".req_rx");
            sc_trace(tf, ack_rx, "core_" + std::to_string(id) + ".ack_rx");
            sc_trace(tf, flit_tx, "core_" + std::to_string(id) + ".flit_tx");
            sc_trace(tf, req_tx, "core_" + std::to_string(id) + ".req_tx");
            sc_trace(tf, ack_tx, "core_" + std::to_string(id) + ".ack_tx");
        }
    }

    sc_lv<32> floatToLogicvector(float d) {
        // Convert float to IEEE 754 logicfield
        sc_dt::scfx_ieee_float id(d);
        // Prepare parts
        bool               sgn = id.negative();
        sc_dt::sc_uint<8>  exp = id.exponent();
        sc_dt::sc_uint<23> mnt = id.mantissa();
        // Concatenate parts to bitvector
        // sc_lv<32> lv = (sgn, exp, mnt);
        sc_lv<32> lv;
        lv[31] = sgn;
        lv.range(30, 23) = exp;
        lv.range(22, 0) = mnt;

        return lv;
    }

    float logicvectorToFloat(const sc_lv<32>& lv) {
        // Extract parts from the logic vector
        bool sgn = lv[31].to_bool();
        sc_uint<8> exp = lv.range(30, 23).to_uint();
        sc_uint<23> mnt = lv.range(22, 0).to_uint();
        // Convert to IEEE 754 floating-point format
        sc_dt::scfx_ieee_float id;
        id.negative(sgn);
        id.exponent(exp);
        id.mantissa(mnt);
        // Convert to float
        float result = id; //.to_float()

        return result;
    }

    void run(){
        if(rst.read()){
            flit_tx.write(0);
            req_tx.write(0);
            ack_rx.write(0);
            flit_rec = 0;
            out_ready = 0;
        }
        else{

            //* rx
            //ack_rx
                if(handshake.read()==1){
                    ack_rx.write(0);
                    handshake.write(0);
                }
                else if(req_rx.read()==1){
                    ack_rx.write(1);
                    handshake.write(1);
                }
                else{
                    ack_rx.write(0);
                    handshake.write(0);
                }
            //packeting & computing
            if(req_rx.read() == 1 && ack_rx == 1){
                flit_rec = flit_rx.read();
                // cout << "Core " << core_id << " received flit: " << flit_rec << endl;
                // cout << "Core " << core_id << " received flit: " << logicvectorToFloat(flit_rec.range(31, 0)) << endl;
                if(flit_rec[33]==1){ //header
                    cout << "Core " << core_id << " received packet from " << flit_rec.range(31, 28) << endl;
                    if(flit_rec[23]==1) rec_input = 1;
                    else                rec_input = 0;
                    if(flit_rec[22]==1) rec_weight = 1;
                    else                rec_weight = 0;
                    if(flit_rec[21]==1) rec_bias = 1;
                    else                rec_bias = 0;
                }
                else{
                    //data
                    if(rec_input){
                        p_input->datas.push_back(logicvectorToFloat(flit_rec.range(31, 0)));
                    }
                    else if(rec_weight){
                        p_weight->datas.push_back(logicvectorToFloat(flit_rec.range(31, 0)));
                    }
                    else if(rec_bias){
                        p_bias->datas.push_back(logicvectorToFloat(flit_rec.range(31, 0)));
                    }
                    else{
                        cout << "Core " << core_id << " received unknown packet" << endl;
                    }
                    //input tail
                    if(rec_input && flit_rec[32]==1){
                        // start_computing
                        // cout << "Core " << core_id << " received input tail" << endl;
                        p_output = pe.alexnet_layer(*p_input, *p_weight, *p_bias);
                        // cout << "p_output.datas[0]: " << p_output.datas[0] << endl;
                        out_ready = 1; //todo reset
                    }
                }
            }
            //output
            if(out_ready && cnt_out==0){
                flit_send[33] = 1;
                flit_send[32] = 0;
                flit_send.range(31, 28) = p_output.source_id;
                flit_send.range(27, 24) = p_output.dest_id;
                flit_send[23] = 1;
                flit_send.range(22, 0) = 0;
                req_tx.write(1);
                flit_tx.write(flit_send);
                cnt_out++;
                // cout << "Core " << core_id << " sent flit: " << flit_send << endl;
                // cout << "----------------------" << endl;
                // cout << "From: " << p_output.source_id << endl;
                // cout << "To:   " << p_output.dest_id << endl;
                // cout << "----------------------" << endl;
            }
            else if(ack_tx.read() && out_ready && cnt_out<=p_output.datas.size()){
                flit_send[33] = 0;
                if(cnt_out==p_output.datas.size())  flit_send[32] = 1;
                else                                flit_send[32] = 0;
                flit_send.range(31, 0) = floatToLogicvector(p_output.datas[cnt_out-1]);
                req_tx.write(1);
                flit_tx.write(flit_send);
                // cout << "Core " << core_id << " sent flit: " << p_output.datas[cnt_out] << endl;
                cnt_out++;
            }
            else if(ack_tx.read() && out_ready && cnt_out>p_output.datas.size()){
                req_tx.write(0);
                flit_tx.write(0);
                out_ready = 0;
                cnt_out = 0;
                // clear packet
                p_input->datas.clear();
                p_weight->datas.clear();
                p_bias->datas.clear();
            }
            
            
        }
    }



};

#endif