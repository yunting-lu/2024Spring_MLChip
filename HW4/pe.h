#ifndef PE_H
#define PE_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "systemc.h"
using namespace std;

#define DEBUG_MODE true     // You can change this value to get more information
#define TOTAL_PACKET_NUM 1   // Please don't change this value

struct Packet {
    int source_id;
    int dest_id;
    vector<float> datas;
};

class PE {
public:
    PE();
    void init(int pe_id);
    Packet alexnet_layer(const Packet& p_i, const Packet& p_w, const Packet& p_b);
    
private:
    int id;
    int des_id;
};

#endif