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

struct UintInString{
	char* string;
	size_t total_size_reserved;
	size_t position_null_char;

	void initialize(size_t reserve_size) {
		string = (char*)malloc(reserve_size);
		total_size_reserved = reserve_size;
		position_null_char = 1;
		string[0] = '0';
		string[1] = '\0';
	}
	
	void update(uint8_t num) {
		// CANT UPDATE WITH NUMBERS THAT ARE more than 9
		// '0' --> 48
		// '9' --> 57
		uint8_t lastChar = string[position_null_char-1];
		if(lastChar + num > 57) {
			uint8_t n = ((lastChar + num) - 57) + 47;
			string[position_null_char-1] = n;
			updateByOne(position_null_char-2);
		}else{
			string[position_null_char-1] = lastChar + num;
		}
	}

	void updateByOne(int32_t position) {
		if(position < 0) {
			// Move everything to right
			for(int32_t i = position_null_char; i >= 0; i--) { // WE SHOULD ALWAYS HAVE ENOUGH SPACE
				string[i+1] = string[i]; 
			}
			position_null_char++;
			string[0] = '1';
		}else{
			uint8_t lastChar = string[position];
			if(lastChar + 1 > 57) {
				uint8_t n = ((lastChar + 1) - 57) + 47;
				string[position] = n;
				updateByOne(position-1);
			}else{
				string[position] = lastChar + 1;
			}
		}
	}

};

int main() {
	CustomString accum;
	accum.initialize(ITERATIONS*8);

	// nninuinniuninnf
	// Loop unrolling

	uint32_t iterToUnroll = ((size_t)(ITERATIONS / 15)) * 15;
	
	#ifdef INFO_MODE
		uint32_t iterLeft = ITERATIONS - iterToUnroll;
		printf("Iters to unroll: %d\n", iterToUnroll);
		printf("Iters left: %d\n", iterLeft);
	#endif

	UintInString num_string_buffer;
	num_string_buffer.initialize(11);

	for(uint32_t i = 1; i < iterToUnroll; i += 15) {
		// nninuinniuninnf
		num_string_buffer.update(1);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		num_string_buffer.update(1);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		accum.concat("fizz ", 5); // 3

		num_string_buffer.update(2);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		accum.concat("buzz ", 5); // 5

		accum.concat("fizz ", 5); // 6

		num_string_buffer.update(3);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		num_string_buffer.update(1);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		accum.concat("fizz ", 5); // 9

		accum.concat("buzz ", 5); // 10

		num_string_buffer.update(3);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		accum.concat("fizz ", 5); // 12

		num_string_buffer.update(2);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		num_string_buffer.update(1);
		accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
		accum.concat(" ", 1);

		accum.concat("fizzbuzz ", 9); // 15

		num_string_buffer.update(1);
	}
	
	for(uint32_t i = iterToUnroll+1; i <= ITERATIONS; i++) {
		uint8_t encode = (i % 3 == 0) ? 1 : 0;
		encode += (i % 5 == 0) ? 2 : 0;
		
		num_string_buffer.update(1);

		if(encode == 0) {
			accum.concat(num_string_buffer.string, num_string_buffer.position_null_char);
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

	//printf("\n\n%s\n", accum.string);

	free(accum.string);
	free(num_string_buffer.string);
	return 0;
}