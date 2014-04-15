
#ifdef WITH_TESTS
#include "Test.h"

#include "../oscillator/SimpleOscillator2.h"


class LUT: public ::testing::Test {

public:

	SimpleOscillator2 osc;
	std::vector<float> query;
	const unsigned int cnt = 16*1024*1024;

public:

   LUT() {
	   osc.setMode(SimpleOscillator2Mode::SINE);
	   float sum = 0;
	   for (unsigned int i = 0; i < cnt; ++i) {
		   sum += (float) rand() / (float) RAND_MAX / 10.0f;
		   query.push_back(sum);
	   }
   }

   void SetUp( ) {

   }

   void TearDown() {
	   ;
   }

   ~LUT()  {
	   ;
   }


};


TEST_F(LUT, bench1) {

	float ret = 0;
	for(unsigned int i = 0; i < cnt; ++i) {
		ret += osc.getLUT(query[i]);
	}
	std::cout << ret << std::endl;

}





#endif

