#include <cstdio>
#include <cstdint>
#include <string>

static constexpr size_t ITERATIONS = 500000000; 

int main() {
	std::string accum;
	accum.reserve(ITERATIONS*8);

	for(uint32_t i = 1; i <= ITERATIONS; i++) {
		if(i % 3 == 0 && i % 5 == 0) {
			accum += "fizzbuzz ";
		}else if(i % 3 == 0) {
			accum += "fizz ";
		}else if(i % 5 == 0) {
			accum += "buzz ";
		}else {
			accum += std::to_string(i);
			accum += " ";
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

