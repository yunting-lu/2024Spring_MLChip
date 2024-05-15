#ifndef CORE_H
#define CORE_H

#include "systemc.h"
#include "pe.h"
#include <bitset>

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


    sc_lv<34> temp;
    vector<sc_lv<34>> flit;
    sc_lv<34> flit_rec;
    int cnt_rx;
    int count;
    int pat_num;
    bool get_p;

    sc_buffer < bool > handshake;


    Core(sc_module_name name, int id, sc_trace_file *tf=nullptr) : sc_module(name), pe()
    {
        core_id = id;

        pe.init(core_id);
        // cout << "Core " << core_id << " init" << endl;

        this->tf = tf;
        SC_THREAD(do_it);
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

    Packet* p;
    Packet* p_rx = nullptr;

    void do_it(){

        while(true){
            
            if (rst.read() == 1) {
                count = 0;
                pat_num = 0;
                get_p = 0;
                p = nullptr;
            } 
            else {

                //* tx
                // get_packet
                if(p == nullptr)
                    p = pe.get_packet();
                    
                if(p != nullptr && get_p==0){
                    flit.clear();
                    pat_num++;
                    // cout << "Core " << core_id << " is getting packet, pat_num = " << pat_num << ", ";
                    for(int i = 0; i < (p->datas.size())+1; i++){
                        if(i == 0){ //header
                            temp[33] = 1;
                            temp[32] = 0;
                            temp.range(31, 28) = p->source_id;
                            temp.range(27, 24) = p->dest_id;
                            temp.range(23, 0) = 0;
                        }
                        else if(i == (p->datas.size())){ //tail
                            temp[33] = 0;
                            temp[32] = 1;
                            temp.range(31, 0) = floatToLogicvector(p->datas[i-1]);
                        }
                        else{ //body
                            temp[33] = 0;
                            temp[32] = 0;
                            temp.range(31, 0) = floatToLogicvector(p->datas[i-1]);
                            
                        }
                        flit.push_back(temp);
                    }
                    // output header
                    // cout << "flit size: " << flit.size() << endl;
                    req_tx.write(1);
                    flit_tx.write(flit[count]);
                    get_p = 1;
                }
                else if(ack_tx.read()==1 && count<flit.size()-1 && get_p==1){
                    count++;
                    req_tx.write(1);
                    flit_tx.write(flit[count]);
                }
                else if(ack_tx.read()==1 && count==flit.size()-1 && get_p==1){
                    delete p;
                    p = nullptr; 
                    count = 0;
                    get_p = 0;
                    req_tx.write(0);
                    flit_tx.write(0);
                    // cout << "Core_" << core_id << ": p deleted" << endl;
                }

                //* rx
                //receiving rx
                if(p_rx == nullptr){
                    p_rx = new Packet();
                }

                //ack_rx
                if(handshake.read()==1){
                    ack_rx.write(0);
                    handshake.write(0);
                }
                else if(req_rx.read() == 1){
                    ack_rx.write(1);
                    handshake.write(1);
                }
                else{
                    ack_rx.write(0);
                    handshake.write(0);
                }
                
                //packeting
                if(req_rx.read() == 1 && ack_rx == 1){
                    flit_rec = flit_rx.read();
                    if(flit_rec[33] == 1){ //header
                        p_rx->source_id = flit_rec.range(31, 28).to_uint();
                        p_rx->dest_id = flit_rec.range(27, 24).to_uint();
                        p_rx->datas.clear();
                    }
                    else{
                        (p_rx->datas).push_back(logicvectorToFloat(flit_rec.range(31, 0)));
                        if(flit_rec[32]==1){ //tail
                            // cout << "\033[31mcore " << core_id << "'s check point!!!\033[0m" << " ";
                            // cout << "at time " << sc_time_stamp() << " ";
                            // cout<<"From: "<<p_rx->source_id<<", ";
                            // cout<<"To: "<<p_rx->dest_id<<endl;
                            // for(int inx=0;inx<p_rx->datas.size();inx++){
                            //     cout<< p_rx->datas[inx]<<" ";
                            // }
                            pe.check_packet(p_rx);
                            // cout << "\033[31mchecked\033[0m" << endl;
                        }
                    }
                }        

            }
            wait();
        }
    }

};

#endif