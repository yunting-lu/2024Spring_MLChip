#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "systemc.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

SC_MODULE( Controller ) {
    sc_in  < bool >  rst;
    sc_in  < bool >  clk;
    
    // to ROM
    sc_out < int >   layer_id;       // '0' means input data
    sc_out < bool >  layer_id_type;  // '0' means weight, '1' means bias (for layer_id == 0, we don't care this signal)
    sc_out < bool >  layer_id_valid;

    // from ROM
    sc_in  < float > data;
    sc_in  < bool >  data_valid;
    
    // to router0
    sc_out < sc_lv<34> > flit_tx;
    sc_out < bool > req_tx;
    sc_in  < bool > ack_tx;

    // from router0
    sc_in  < sc_lv<34> > flit_rx;
    sc_in  < bool > req_rx;
    sc_out < bool > ack_rx;


    sc_trace_file *tf;
    SC_HAS_PROCESS(Controller);

    vector<string> imagenet_classes;
    int layer_id_val = 1;
    bool layer_id_type_val = 0;
    bool wait_data_from_rom = 0;
    bool fin_rec_from_rom = 0;
    bool type_input, type_weight, type_bias;
    int dest_core_id;
    bool is_header;
    bool is_data_valid;
    sc_lv<32> temp_data, temp_data_d1;
    sc_lv<34> temp_flit;
    vector<sc_lv<34>> temp_flit_vec;
    bool get_data = 0;
    int cnt = 0;
    sc_buffer < bool > handshake;
    sc_lv<34> flit_rec;
    vector<float> data_rec_vec;


    Controller(sc_module_name name, sc_trace_file *tf=nullptr) : sc_module(name)
    {
        this->tf = tf;
        SC_METHOD(run);
        sensitive << clk.pos();
        dont_initialize();

        //tracing
        if (tf != nullptr) {
            sc_trace(tf, clk, "controller.clk");
            sc_trace(tf, rst, "controller.rst");
            sc_trace(tf, layer_id, "controller.layer_id");
            sc_trace(tf, layer_id_type, "controller.layer_id_type");
            sc_trace(tf, layer_id_valid, "controller.layer_id_valid");
            sc_trace(tf, data, "controller.data");
            sc_trace(tf, data_valid, "controller.data_valid");
            sc_trace(tf, flit_tx, "controller.flit_tx");
            sc_trace(tf, req_tx, "controller.req_tx");
            sc_trace(tf, ack_tx, "controller.ack_tx");
            sc_trace(tf, flit_rx, "controller.flit_rx");
            sc_trace(tf, req_rx, "controller.req_rx");
            sc_trace(tf, ack_rx, "controller.ack_rx");
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
            layer_id.write(0);
            layer_id_type.write(0);
            layer_id_valid.write(0);
            flit_tx.write(0);
            req_tx.write(0);
            ack_rx.write(0);
            temp_flit = 0;
            // read classes
            ifstream file("../data/imagenet_classes.txt");
            string class_name;
            while(getline(file, class_name)){
                imagenet_classes.push_back(class_name);
            }
        }
        else{
            // send request to ROM
            if(data_valid.read()==0 && wait_data_from_rom==0 && fin_rec_from_rom==0){
                layer_id.write(layer_id_val);
                layer_id_type.write(layer_id_type_val);
                layer_id_valid.write(1);
                cout << "Controller sends request to ROM" << endl;
                //translate id to destination
                switch (layer_id_val)
                {
                    case 0:
                        dest_core_id = 1;
                        break;
                    case 1:
                        dest_core_id = 1;
                        break;
                    case 2:
                        dest_core_id = 2;
                        break;
                    case 3:
                        dest_core_id = 5;
                        break;
                    case 4:
                        dest_core_id = 8;
                        break;
                    case 5:
                        dest_core_id = 7;
                        break;
                    case 6:
                        dest_core_id = 4;
                        break;
                    case 7:
                        dest_core_id = 3;
                        break;
                    case 8:
                        dest_core_id = 6;
                        break;
                    default:
                        dest_core_id = 0;
                        cout << "Error: Invalid layer id " << layer_id_val << "." << endl;
                        break;
                }
                //determine type
                if(layer_id_val==0) type_input = 1;
                else                type_input = 0;
                if(layer_id_val!=0 && layer_id_type_val==0) type_weight = 1;
                else                                        type_weight = 0;
                if(layer_id_val!=0 && layer_id_type_val==1) type_bias = 1;
                else                                        type_bias = 0;

                wait_data_from_rom = 1;
                temp_flit_vec.clear();
                //header flit
                temp_flit[33] = 1;
                temp_flit[32] = 0;
                temp_flit.range(31, 28) = 0;
                temp_flit.range(27, 24) = dest_core_id;
                temp_flit[23] = type_input;
                temp_flit[22] = type_weight;
                temp_flit[21] = type_bias;
                temp_flit.range(20, 0) = 0;
                temp_flit_vec.push_back(temp_flit);
                if(layer_id_val==0){
                    fin_rec_from_rom = 1;
                }

                if(layer_id_val==8 && layer_id_type_val==1){
                    layer_id_val = 0;
                }
                else if(fin_rec_from_rom==0){
                    if(layer_id_type_val)   layer_id_val++;
                    layer_id_type_val = !layer_id_type_val;
                }
            }
            else{
                layer_id.write(0);
                layer_id_type.write(0);
                layer_id_valid.write(0);
            }
            
            
        }

        // receive data from ROM
        if(data_valid.read()){
            // cout << "Controller receive data" << endl;
            temp_data = floatToLogicvector(data.read());
            // cout << "Controller receives data: " << temp_data << endl;
            get_data = 1;
        }
        if(is_data_valid){
            temp_flit.range(31, 0) = temp_data_d1;
            temp_flit[33] = 0;
            if(data_valid.read())   temp_flit[32] = 0;
            else                    temp_flit[32] = 1;
            temp_flit_vec.push_back(temp_flit);
        }
        
        // send data to router0
        if(get_data && cnt==0){
            req_tx.write(1);
            flit_tx.write(temp_flit_vec[cnt]);
            cout << "Controller sending data: " << temp_flit_vec[cnt] << endl;
            cnt++;
        }
        else if(ack_tx.read() && get_data && cnt<temp_flit_vec.size()){
            req_tx.write(1);
            flit_tx.write(temp_flit_vec[cnt]);
            // cout << "Controller sending data: " << temp_flit_vec[cnt] << endl;
            cnt++;
            // if(cnt==5)  sc_stop();
        }
        else if(ack_tx.read() && get_data && cnt==temp_flit_vec.size()){
            req_tx.write(0);
            flit_tx.write(0);
            get_data = 0;
            cnt = 0;
            wait_data_from_rom = 0;
        }
    

        is_data_valid = data_valid.read();
        temp_data_d1 = temp_data;

        // receive data from router0
        if(handshake.read()){
            ack_rx.write(0);
            handshake.write(0);
        }
        else if(req_rx.read()){
            ack_rx.write(1);
            handshake.write(1);
        }
        else{
            ack_rx.write(0);
            handshake.write(0);
        }

        if(req_rx.read() && ack_rx==1){
            flit_rec = flit_rx.read();
            if(flit_rec[33]==1){
                data_rec_vec.clear();
            }
            else{
                data_rec_vec.push_back(logicvectorToFloat(flit_rec.range(31,0)));
                if(flit_rec[32]==1){
                    //SoftMax
                    double sum = 0;
                    vector<double> softmax_result(1000);
                    vector<pair<int, double>> softmax_result_pair(1000);

                    for(int i = 0; i < 1000; i++){
                        sum += exp(data_rec_vec[i]);
                    }
                    for(int i = 0; i < 1000; i++){
                        softmax_result[i] = exp(data_rec_vec[i]) / sum;
                        softmax_result_pair[i] = make_pair(i, softmax_result[i]);
                    }

                    sort(softmax_result_pair.begin(), softmax_result_pair.end(), [](const pair<int, double> &left, const pair<int, double> &right) {
                        return left.second > right.second;
                    });
                    
                    cout << "------------------------------------------------------------" << endl;
                    cout << " Top \t idx \t val \t        possibility \t class name" << endl;
                    cout << "------------------------------------------------------------" << endl;
                    for(int i = 0; i < 5; i++){
                        cout << fixed << setprecision(6) << "  " << i+1 << "\t " << softmax_result_pair[i].first << "\t" << (double)(data_rec_vec[softmax_result_pair[i].first]) << "\t" << 100*softmax_result_pair[i].second << "\t" << imagenet_classes[softmax_result_pair[i].first] << endl;
                    }
                    cout << "------------------------------------------------------------" << endl;
                    sc_stop();
                }
            }
        }

        
    }

};

#endif