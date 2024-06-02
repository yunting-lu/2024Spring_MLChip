#include "pe.h"

PE::PE(): id(0) {}

void PE::init(int pe_id) {
    id = pe_id;
    switch (id)
    {
    case 1:
        des_id = 2;
        break;
    case 2:
        des_id = 3;
        break;
    case 3:
        des_id = 7;
        break;
    case 7:
        des_id = 11;
        break;
    case 11:
        des_id = 15;
        break;
    case 15:
        des_id = 14;
        break;
    case 14:
        des_id = 13;
        break;
    case 13:
        des_id = 12;
        break;
    case 12:
        des_id = 8;
        break;
    case 8:
        des_id = 4;
        break;
    case 4:
        des_id = 0;
        break;
    default: //0,5,6,9,10
        des_id = 0;
        break;
    }
}

Packet PE::alexnet_layer(const Packet& p_i, const Packet& p_w, const Packet& p_b) {
    Packet new_packet;
    // cout << "PE " << id << " is computing..." << endl;
    new_packet.source_id = id;
    new_packet.dest_id = des_id;
    new_packet.datas.clear();
    // implement alexnet_layer
    //todo
    int i_ptr, w_ptr;
    int i_ptr_x, i_ptr_y;
    int IN_CHANNELS;
    int IN_HEIGHT;
    int IN_WIDTH;
    int KERNAL_SIZE;
    int OUT_CHANNELS;
    int OUT_HEIGHT;
    int OUT_WIDTH;
    int STRIDE;
    int PADDING;

    //CONV_RELU
    if(id == 1 || id == 3 || id == 11 || id == 15 || id == 14){
        if(id==1){
            IN_CHANNELS = 3;
            IN_HEIGHT = 224;
            IN_WIDTH = 224;
            KERNAL_SIZE = 11;
            OUT_CHANNELS = 64;
            OUT_HEIGHT = 55;
            OUT_WIDTH = 55;
            STRIDE = 4;
            PADDING = 2;
        }
        else if(id==3){
            IN_CHANNELS = 64;
            IN_HEIGHT = 27;
            IN_WIDTH = 27;
            KERNAL_SIZE = 5;
            OUT_CHANNELS = 192;
            OUT_HEIGHT = 27;
            OUT_WIDTH = 27;
            STRIDE = 1;
            PADDING = 2;
        }
        else if(id==11){
            IN_CHANNELS = 192;
            IN_HEIGHT = 13;
            IN_WIDTH = 13;
            KERNAL_SIZE = 3;
            OUT_CHANNELS = 384;
            OUT_HEIGHT = 13;
            OUT_WIDTH = 13;
            STRIDE = 1;
            PADDING = 1;
        }
        else if(id==15){
            IN_CHANNELS = 384;
            IN_HEIGHT = 13;
            IN_WIDTH = 13;
            KERNAL_SIZE = 3;
            OUT_CHANNELS = 256;
            OUT_HEIGHT = 13;
            OUT_WIDTH = 13;
            STRIDE = 1;
            PADDING = 1;
        }
        else if(id==14){
            IN_CHANNELS = 256;
            IN_HEIGHT = 13;
            IN_WIDTH = 13;
            KERNAL_SIZE = 3;
            OUT_CHANNELS = 256;
            OUT_HEIGHT = 13;
            OUT_WIDTH = 13;
            STRIDE = 1;
            PADDING = 1;
        }
        
        float partial_sum;
        for(int oc=0; oc<OUT_CHANNELS; oc++){
            for(int orow=0; orow<OUT_HEIGHT; orow++){
                for(int ocol=0; ocol<OUT_WIDTH; ocol++){
                    partial_sum = 0;
                    for(int ic=0; ic<IN_CHANNELS; ic++){
                        for(int krow=0; krow<KERNAL_SIZE; krow++){
                            for(int kcol=0; kcol<KERNAL_SIZE; kcol++){
                                //[ic, wrow, wcol] -> [ic, stride*orow + krow -2, stride*ocol + kcol -2]
                                i_ptr_x = STRIDE*ocol + kcol - PADDING;
                                i_ptr_y = STRIDE*orow + krow - PADDING;
                                i_ptr = ic*IN_HEIGHT*IN_WIDTH + i_ptr_y*IN_WIDTH + i_ptr_x;
                                w_ptr = oc*IN_CHANNELS*KERNAL_SIZE*KERNAL_SIZE + ic*KERNAL_SIZE*KERNAL_SIZE + krow*KERNAL_SIZE + kcol; //[oc, ic, krow, kcol]]

                                if(!(i_ptr_x<0 || i_ptr_x>=IN_WIDTH || i_ptr_y<0 || i_ptr_y>=IN_HEIGHT)){
                                    partial_sum += (p_i.datas[i_ptr] * p_w.datas[w_ptr]);
                                }
                            }
                        }
                    }
                    
                    if((partial_sum + p_b.datas[oc])>0){
                        partial_sum += p_b.datas[oc];
                        new_packet.datas.push_back(partial_sum);
                    }
                    else{
                        new_packet.datas.push_back(0);
                    }
                }
            }
        }
        return new_packet;
    }
    //MAX_POOLING
    else if(id == 2 || id == 7 || id == 13){
        if(id==2){
            IN_CHANNELS = 64;
            IN_HEIGHT = 55;
            IN_WIDTH = 55;
            KERNAL_SIZE = 3;
            OUT_CHANNELS = 64;
            OUT_HEIGHT = 27;
            OUT_WIDTH = 27;
            STRIDE = 2;
        }
        else if(id==7){
            IN_CHANNELS = 192;
            IN_HEIGHT = 27;
            IN_WIDTH = 27;
            KERNAL_SIZE = 3;
            OUT_CHANNELS = 192;
            OUT_HEIGHT = 13;
            OUT_WIDTH = 13;
            STRIDE = 2;
        }
        else if(id==13){
            IN_CHANNELS = 256;
            IN_HEIGHT = 13;
            IN_WIDTH = 13;
            KERNAL_SIZE = 3;
            OUT_CHANNELS = 256;
            OUT_HEIGHT = 6;
            OUT_WIDTH = 6;
            STRIDE = 2;
        }

        float largest;
        for(int oc=0; oc<OUT_CHANNELS; oc++){
            for(int orow=0; orow<OUT_HEIGHT; orow++){
                for(int ocol=0; ocol<OUT_WIDTH; ocol++){
                    largest = 0;
                    for(int krow=0; krow<KERNAL_SIZE; krow++){
                        for(int kcol=0; kcol<KERNAL_SIZE; kcol++){
                            //[ic, wrow, wcol] -> [ic, stride*orow + krow, stride*ocol + kcol]
                            i_ptr_x = STRIDE*ocol + kcol;
                            i_ptr_y = STRIDE*orow + krow;
                            i_ptr = oc*IN_HEIGHT*IN_WIDTH + i_ptr_y*IN_WIDTH + i_ptr_x;
                            if(p_i.datas[i_ptr] > largest){
                                largest = p_i.datas[i_ptr];
                            }
                        }
                    }
                    new_packet.datas.push_back(largest);
                }
            }
        }
        return new_packet;
    }
    //LINEAR_RELU
    else if(id == 12 || id == 8){
        if(id==12){
            IN_CHANNELS = 9216;
		    OUT_CHANNELS = 4096;
        }
        else if(id==8){
            IN_CHANNELS = 4096;
            OUT_CHANNELS = 4096;
        }

        float partial_sum;
        for(int oc=0; oc<OUT_CHANNELS; oc++){
            partial_sum = 0;
            for(int ic=0; ic<IN_CHANNELS; ic++){
                partial_sum += p_i.datas[ic] * p_w.datas[oc*IN_CHANNELS + ic];
            }
            //RELU
            if((partial_sum + p_b.datas[oc])>0){
                partial_sum += p_b.datas[oc];
                new_packet.datas.push_back(partial_sum);
            }
            else{
                new_packet.datas.push_back(0);
            }
        }
        return new_packet;
    }
    //LINEAR
    else if(id == 4){
        IN_CHANNELS = 4096;
		OUT_CHANNELS = 1000;

        float partial_sum;
        for(int oc=0; oc<OUT_CHANNELS; oc++){
            partial_sum = 0;
            for(int ic=0; ic<IN_CHANNELS; ic++){
                partial_sum += p_i.datas[ic] * p_w.datas[oc*IN_CHANNELS + ic];
            }
            partial_sum += p_b.datas[oc];
            new_packet.datas.push_back(partial_sum);
        }
        return new_packet;
    }
    else{
        cout << "Error: Invalid PE id " << id << "." << endl;
        return new_packet;
    }
}
