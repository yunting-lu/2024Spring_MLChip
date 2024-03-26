#define SC_INCLUDE_FX
#include "Pattern.h"
#include <time.h>
#include <fstream>

void Pattern::run() {

	if ( rst.read() == 1 )
		return;

	const int CHANNELS = 3;
    const int HEIGHT = 224;
    const int WIDTH = 224;
	//sc_vector < sc_out < sc_fixed_fast<40,17> > > image("image", 150528);

    
	
	temp_uint = temp_uint.to_uint() + 1;

	cycle++;
	if(cycle == 2){
		//* Open the input file
    	std::ifstream file("data/cat.txt");
		double value;
		int cnt = 0;
		while(file >> value) {
			image[cnt] = (sc_fixed_fast<40,17>) (value);
			cnt++;
			//std::cout << "image[" << cnt << "]: " << image[cnt] << std::endl;
		}

    	file.close();

		in_valid = 1;
	}
	else if(cycle == 3){
		//* Open the input file
    	std::ifstream file("data/dog.txt");
		double value;
		int cnt = 0;
		while(file >> value) {
			image[cnt] = (sc_fixed_fast<40,17>) (value);
			cnt++;
			//std::cout << "image[" << cnt << "]: " << image[cnt] << std::endl;
		}

    	file.close();

		in_valid = 1;
	}
	else {
		in_valid = 0;
	}

	if ( cycle == CYCLE )
		sc_stop();
}


