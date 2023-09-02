#include <cstdio>
#include <cstdint>
#include <string>

static constexpr uint32_t ITERATIONS = 500000000; 

int main() {
	std::string accum;
	accum.reserve(ITERATIONS*8);

	for(uint32_t i = 1; i <= ITERATIONS; i++) {
		uint8_t encode = (i % 3 == 0) ? 1 : 0;
		encode += (i % 5 == 0) ? 2 : 0;

		if(encode == 0) {
			accum += std::to_string(i);
			accum += " ";
		}else if(encode == 1) {
			accum += "fizz ";
		}else if(encode == 2) {
			accum += "buzz ";
		}else if(encode == 3) {
			accum += "fizzbuzz ";
		}
	}

	std::string subs = accum.substr(accum.length()-20, 20);
	printf("%s\n", subs.c_str());
	#ifdef INFO_MODE
		printf("Total size string: %ld\n", accum.length());
		printf("HASH: %ld\n", std::hash<std::string>{}(accum));
	#endif

	return 0;
}