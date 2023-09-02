#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <string>
#include <cstring>

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

	for(uint32_t i = 1; i <= ITERATIONS; i++) {
		uint8_t encode = (i % 3 == 0) ? 1 : 0;
		encode += (i % 5 == 0) ? 2 : 0;

		if(encode == 0) {
			accum.concat(std::to_string(i).c_str());
			accum.concat(" ", 1);
		}else if(encode == 1) {
			accum.concat("fizz ", 5);
		}else if(encode == 2) {
			accum.concat("buzz ", 5);
		}else if(encode == 3) {
			accum.concat("fizzbuzz ", 9);
		}
	}

	accum.print_last(20);
	#ifdef INFO_MODE
		printf("Total size string: %ld\n", std::strlen(accum.string));
		printf("HASH: %ld\n", std::hash<std::string>{}(accum.string));
	#endif

	return 0;
}