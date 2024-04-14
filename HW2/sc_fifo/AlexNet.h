#define SC_INCLUDE_FX
#include <systemc.h>
#include "MAC.h"
#include <iostream>
#include <vector>


using namespace std;

SC_MODULE(ALEXNET)
{
    CONV_RELU_1 m_CONV_RELU_1;
    MAX_POOLING_1 m_MAX_POOLING_1;
    CONV_RELU_2 m_CONV_RELU_2;
    MAX_POOLING_2 m_MAX_POOLING_2;
    CONV_RELU_3 m_CONV_RELU_3;
    CONV_RELU_4 m_CONV_RELU_4;
    CONV_RELU_5 m_CONV_RELU_5;
    MAX_POOLING_3 m_MAX_POOLING_3;
    LINEAR_RELU_1 m_LINEAR_RELU_1;
    LINEAR_RELU_2 m_LINEAR_RELU_2;
    LINEAR_3 m_LINEAR_3;

    sc_in < bool > clk, rst;

	sc_in < bool > in_valid;
	sc_fifo < bool > conv1_valid;
	sc_fifo < bool > mp1_valid;
	sc_fifo < bool > conv2_valid;
	sc_fifo < bool > mp2_valid;
	sc_fifo < bool > conv3_valid;
	sc_fifo < bool > conv4_valid;
	sc_fifo < bool > conv5_valid;
	sc_fifo < bool > mp3_valid;
	sc_fifo < bool > linear1_valid;
	sc_fifo < bool > linear2_valid;
	sc_out < bool > linear3_valid;

	sc_vector < sc_in < sc_fixed_fast<45,17> > > image{"image", 150528};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > conv1_result{"conv1_result", 193600};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > mp1_result{"mp1_result", 46656};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > conv2_result{"conv2_result", 139968};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > mp2_result{"mp2_result", 32448};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > conv3_result{"conv3_result", 64896};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > conv4_result{"conv4_result", 43264};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > conv5_result{"conv5_result", 43264};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > mp3_result{"mp3_result", 9216};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > linear1_result{"linear1_result", 4096};
	sc_vector < sc_fifo < sc_fixed_fast<45,17> > > linear2_result{"linear2_result", 4096};
	sc_vector < sc_out < sc_fixed_fast<45,17> > > linear3_result{"linear3_result", 1000};




    SC_CTOR(ALEXNET):   m_CONV_RELU_1( "m_CONV_RELU_1" ),
                        m_MAX_POOLING_1( "m_MAX_POOLING_1" ),
                        m_CONV_RELU_2( "m_CONV_RELU_2" ),
                        m_MAX_POOLING_2( "m_MAX_POOLING_2" ),
                        m_CONV_RELU_3( "m_CONV_RELU_3" ),
                        m_CONV_RELU_4( "m_CONV_RELU_4" ),
                        m_CONV_RELU_5( "m_CONV_RELU_5" ),
                        m_MAX_POOLING_3( "m_MAX_POOLING_3" ),
                        m_LINEAR_RELU_1( "m_LINEAR_RELU_1" ),
                        m_LINEAR_RELU_2( "m_LINEAR_RELU_2" ),
                        m_LINEAR_3( "m_LINEAR_3" )
    {

        

        m_CONV_RELU_1.clk(clk);
	    m_CONV_RELU_1.rst(rst);
	    m_CONV_RELU_1.in_feature_map(image);
	    m_CONV_RELU_1.out_feature_map(conv1_result);
	    m_CONV_RELU_1.in_valid(in_valid);
	    m_CONV_RELU_1.out_valid(conv1_valid);
	    // m_CONV_RELU_1.IN_VECTOR_SIZE = 150528;
	    // m_CONV_RELU_1.OUT_VECTOR_SIZE = 193600;
	    m_CONV_RELU_1.IN_CHANNELS = 3;
	    m_CONV_RELU_1.IN_HEIGHT = 224;
	    m_CONV_RELU_1.IN_WIDTH = 224;
	    m_CONV_RELU_1.KERNAL_SIZE = 11;
	    m_CONV_RELU_1.OUT_CHANNELS = 64;
	    m_CONV_RELU_1.OUT_HEIGHT = 55;
	    m_CONV_RELU_1.OUT_WIDTH = 55;
	    m_CONV_RELU_1.STRIDE = 4;
	    m_CONV_RELU_1.PADDING = 2;
	    m_CONV_RELU_1.weight_dir = "data/conv1_weight.txt";
	    m_CONV_RELU_1.bias_dir = "data/conv1_bias.txt";


	    m_MAX_POOLING_1.clk(clk);
	    m_MAX_POOLING_1.rst(rst);
	    m_MAX_POOLING_1.in_feature_map(conv1_result);
	    m_MAX_POOLING_1.out_feature_map(mp1_result);
	    m_MAX_POOLING_1.in_valid(conv1_valid);
	    m_MAX_POOLING_1.out_valid(mp1_valid);

	    m_CONV_RELU_2.clk(clk);
	    m_CONV_RELU_2.rst(rst);
	    m_CONV_RELU_2.in_feature_map(mp1_result);
	    m_CONV_RELU_2.out_feature_map(conv2_result);
	    m_CONV_RELU_2.in_valid(mp1_valid);
	    m_CONV_RELU_2.out_valid(conv2_valid);

	    m_MAX_POOLING_2.clk(clk);
	    m_MAX_POOLING_2.rst(rst);
	    m_MAX_POOLING_2.in_feature_map(conv2_result);
	    m_MAX_POOLING_2.out_feature_map(mp2_result);
	    m_MAX_POOLING_2.in_valid(conv2_valid);
	    m_MAX_POOLING_2.out_valid(mp2_valid);

	    m_CONV_RELU_3.clk(clk);
	    m_CONV_RELU_3.rst(rst);
	    m_CONV_RELU_3.in_feature_map(mp2_result);
	    m_CONV_RELU_3.out_feature_map(conv3_result);
	    m_CONV_RELU_3.in_valid(mp2_valid);
	    m_CONV_RELU_3.out_valid(conv3_valid);

	    m_CONV_RELU_4.clk(clk);
	    m_CONV_RELU_4.rst(rst);
	    m_CONV_RELU_4.in_feature_map(conv3_result);
	    m_CONV_RELU_4.out_feature_map(conv4_result);
	    m_CONV_RELU_4.in_valid(conv3_valid);
	    m_CONV_RELU_4.out_valid(conv4_valid);

	    m_CONV_RELU_5.clk(clk);
	    m_CONV_RELU_5.rst(rst);
	    m_CONV_RELU_5.in_feature_map(conv4_result);
	    m_CONV_RELU_5.out_feature_map(conv5_result);
	    m_CONV_RELU_5.in_valid(conv4_valid);
	    m_CONV_RELU_5.out_valid(conv5_valid);

	    m_MAX_POOLING_3.clk(clk);
	    m_MAX_POOLING_3.rst(rst);
	    m_MAX_POOLING_3.in_feature_map(conv5_result);
	    m_MAX_POOLING_3.out_feature_map(mp3_result);
	    m_MAX_POOLING_3.in_valid(conv5_valid);
	    m_MAX_POOLING_3.out_valid(mp3_valid);

	    m_LINEAR_RELU_1.clk(clk);
	    m_LINEAR_RELU_1.rst(rst);
	    m_LINEAR_RELU_1.in_feature_map(mp3_result);
	    m_LINEAR_RELU_1.out_feature_map(linear1_result);
	    m_LINEAR_RELU_1.in_valid(mp3_valid);
	    m_LINEAR_RELU_1.out_valid(linear1_valid);

	    m_LINEAR_RELU_2.clk(clk);
	    m_LINEAR_RELU_2.rst(rst);
	    m_LINEAR_RELU_2.in_feature_map(linear1_result);
	    m_LINEAR_RELU_2.out_feature_map(linear2_result);
	    m_LINEAR_RELU_2.in_valid(linear1_valid);
	    m_LINEAR_RELU_2.out_valid(linear2_valid);

	    m_LINEAR_3.clk(clk);
	    m_LINEAR_3.rst(rst);
	    m_LINEAR_3.in_feature_map(linear2_result);
	    m_LINEAR_3.out_feature_map(linear3_result);
	    m_LINEAR_3.in_valid(linear2_valid);
	    m_LINEAR_3.out_valid(linear3_valid);


    }
};