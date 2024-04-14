#define SC_INCLUDE_FX
#include <systemc.h>
#include "AlexNet.h"
#include "Pattern.h"
#include "Monitor.h"
#include "clockreset.h"
#include <iostream>
#include <vector>

using namespace std;

int sc_main( int argc, char* argv[] ) {
	sc_signal < bool > clk, rst;

	sc_signal < bool > in_valid;
	sc_signal < bool > linear3_valid;

	sc_vector < sc_signal < sc_fixed_fast<45,17> > > image{"image", 150528};
	sc_vector < sc_signal < sc_fixed_fast<45,17> > > linear3_result{"linear3_result", 1000};
	
	Reset m_Reset( "m_Reset", 10 );
	Clock m_clock( "m_clock", 5, 80 );
	Monitor m_Monitor( "m_Monitor" );
	Pattern m_Pattern( "m_Pattern" );
	ALEXNET m_ALEXNET( "m_ALEXNET" );
	
	m_Reset( rst );
	m_clock( clk );
	
	m_Monitor( rst, clk );
	m_Monitor.in_valid(in_valid);
	m_Monitor.linear3_valid(linear3_valid);
	m_Monitor.linear3_result(linear3_result);


	m_Pattern( rst, clk );
	m_Pattern.image(image);
	m_Pattern.in_valid(in_valid);

	m_ALEXNET.clk(clk);
	m_ALEXNET.rst(rst);
	m_ALEXNET.in_valid(in_valid);
	m_ALEXNET.image(image);
	m_ALEXNET.linear3_valid(linear3_valid);
	m_ALEXNET.linear3_result(linear3_result);
	
	
	sc_start( 500, SC_NS );
	return 0;

}
