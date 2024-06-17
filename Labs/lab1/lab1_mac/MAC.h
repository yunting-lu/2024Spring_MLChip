#include <systemc.h>

SC_MODULE(Adder)
{
	sc_in<sc_uint<8>> in1, in2;
	sc_out<sc_uint<9>> out;

	void run()
	{
		// vvvvv put your code here vvvvv
    out.write(in1.read()+in2.read());
		// ^^^^^ put your code here ^^^^^
	}

	SC_CTOR(Adder)
	{
		SC_METHOD(run);
		sensitive << in1 << in2;
	}
};

SC_MODULE(Multiplier)
{
	sc_in<sc_uint<4>> in1, in2;
	sc_out<sc_uint<8>> out;

	void run()
	{
		// vvvvv put your code here vvvvv
    //out.write(in1.read()*in2.read());
    out = in1.read()*in2.read();
		// ^^^^^ put your code here ^^^^^
	}

	SC_CTOR(Multiplier)
	{
		SC_METHOD(run);
		sensitive << in1 << in2;
	}
};

SC_MODULE(MAC)
{
	sc_in<sc_uint<4>> in1, in2;
	sc_in<sc_uint<8>> in3;
	sc_out<sc_uint<9>> out;

	Adder ADD_1;
	Multiplier MUL_1;

	// vvvvv put your code here vvvvv
  sc_signal<sc_uint<8>> mul_result;
	// ^^^^^ put your code here ^^^^^

	SC_CTOR(MAC) : MUL_1("Mul"), ADD_1("Adder")
	{
		// vvvvv put your code here vvvvv
    MUL_1(in1, in2, mul_result);
    ADD_1(mul_result, in3, out);
    // sensitive << in1 << in2;
		// ^^^^^ put your code here ^^^^^
	}
};
