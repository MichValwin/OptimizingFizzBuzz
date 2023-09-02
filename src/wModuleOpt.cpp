#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <string>
#include <cstring>
#include <thread>

static constexpr size_t ITERATIONS = 500000000; 

struct CustomString {
	char* string;
	size_t total_size_reserved;
	size_t position_null_char; // End of string

	void initialize(size_t size_bytes) {
		string = (char*)malloc(size_bytes);
		string[0] = '\0';
		total_size_reserved = size_bytes;
		position_null_char = 0;
	}

	void concat(const char* str_to_concat) {
		size_t length = std::strlen(str_to_concat);

		memcpy(string+position_null_char, str_to_concat, length+1); // length+1 to include null terminator
		position_null_char = position_null_char+length;
	}

	void concat(const char* str_to_concat, size_t length) {
		memcpy(string+position_null_char, str_to_concat, length+1); // length+1 to include null terminator
		position_null_char = position_null_char+length;
	}

	void print_last(size_t last_chars) const {
		char* points_last_chars = string + position_null_char - last_chars;
		printf("%s\n", points_last_chars);
	}
};


int main() {
	CustomString accum;
	accum.initialize(ITERATIONS*8);
	
	uint8_t c3 = 1;
	uint8_t c5 = 1;
	for(uint32_t i = 1; i <= ITERATIONS; i++) {
		if(c3 == 3 && c5 == 5) {
			accum.concat("fizzbuzz ", 9);
			c3 = 0;
			c5 = 0;
		}else if(c3 == 3) {
			accum.concat("fizz ", 5);
			c3 = 0;
		}else if(c5 == 5) {
			accum.concat("buzz ", 5);
			c5 = 0;
		}else{
			accum.concat(std::to_string(i).c_str());
			accum.concat(" ", 1);
		}
		c3++;
		c5++;
	}

	accum.print_last(20);
	#ifdef INFO_MODE
		printf("Total size string: %ld\n", std::strlen(accum.string));
		printf("HASH: %ld\n", std::hash<std::string>{}(accum.string));
	#endif
	return 0;
}