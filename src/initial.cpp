#include <cstdio>
#include <cstdint>
#include <string>
#include <functional>

static constexpr uint32_t ITERATIONS = 500000000;

int main() {
	std::string accum;
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
	
	//printf(accum.c_str());

	std::string subs = accum.substr(accum.length()-20, 20);
	printf("%s\n", subs.c_str());
	#ifdef INFO_MODE
		printf("Total size string: %ld\n", accum.length());
		printf("HASH: %ld\n", std::hash<std::string>{}(accum));
	#endif

	return 0;
}
