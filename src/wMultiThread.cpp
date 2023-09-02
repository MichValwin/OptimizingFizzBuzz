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
		//printf("Length to concat: %ld\n", length);
		//printf("str to concat: #%s#\n", str_to_concat);
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
	char* init_of_number_string;
	size_t position_null_char;
	size_t length;

	void initialize(size_t reserve_size) {
		string = (char*)malloc(reserve_size);
		for(size_t i = 0; i < reserve_size-1; i++) {
			string[i] = '0';
		}

		position_null_char = reserve_size-1;
		string[position_null_char] = '\0';
		length = 1;
		init_of_number_string = string + (position_null_char-1);
	}
	
	void set_to_number(size_t num) {
		if(num == 0) {
			init_of_number_string = string + (position_null_char-1);
			*init_of_number_string = '0';
			length = 1;
		}else{
			init_of_number_string = string + (position_null_char);

			// Extract digits in reverse order
			length = 0;
			while (num > 0) {
				uint8_t digit = num % 10;
				init_of_number_string--;
				*init_of_number_string = '0' + digit; // Convert digit to its character representation
				num /= 10;
				length++;
			}
		}
	}

	void update_add(uint8_t num) {
		// CAN'T UPDATE WITH NUMBERS THAT ARE more than 9
		// '0' --> 48
		// '9' --> 57
		uint8_t position_char = position_null_char-1;
		uint8_t total_length = 1;
		while(num > 0) {
			uint8_t lastChar = string[position_char];
			if(lastChar + num > '9') {
				string[position_char] = lastChar + num - 10;

				position_char--;
				num = 1;

				total_length++;
				if(total_length > length) {
					length = total_length;
					init_of_number_string--;
				}
			}else{
				string[position_char] = lastChar + num;
				num = 0;
			}
		}
	}

	void print_debug() const {
		printf("CHARS: #");
		for(size_t i = 0; i < position_null_char; i++) {
			printf("%c", string[i]);
		}
		printf("%c", string[position_null_char]+'E');

		printf("#, reserved size: %ld, length: %ld, position null char: %ld, position start number: %ld, char start number: %c ",
			position_null_char+1, length, position_null_char, (uint64_t)init_of_number_string, init_of_number_string[0]);
		printf("CHARS from beginner pointer until NULL: %s\n", init_of_number_string);
		if(string[position_null_char] != '\0') {
			printf("Error, position of null char does not contain null char\n");
		}
	}
};

// It already expects the input to be aligned to 15
void fizzBuzz(size_t from_number, size_t to_number, CustomString* custom_str) {
	UintInString num_string_buffer;
	num_string_buffer.initialize(12);
	num_string_buffer.set_to_number(from_number);

	#ifdef INFO_MODE
		printf("This thread makes %ld to %ld\n", from_number, to_number);
	#endif
	
    for(uint32_t i = from_number; i < to_number; i += 15) {
		// Loop unrolling
		// nninuinniuninnf
		num_string_buffer.update_add(1);

		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		num_string_buffer.update_add(1);
		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		custom_str->concat("fizz ", 5); // 3

		num_string_buffer.update_add(1+1);
		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		custom_str->concat("buzz ", 5); // 5

		custom_str->concat("fizz ", 5); // 6

		num_string_buffer.update_add(1+2);
		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		num_string_buffer.update_add(1);
		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		custom_str->concat("fizz ", 5); // 9

		custom_str->concat("buzz ", 5); // 10

		num_string_buffer.update_add(1+2);
		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		custom_str->concat("fizz ", 5); // 12

		num_string_buffer.update_add(1+1);
		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		num_string_buffer.update_add(1);
		custom_str->concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
		custom_str->concat(" ", 1);

		custom_str->concat("fizzbuzz ", 9); // 15

		num_string_buffer.update_add(1);
	}

	free(num_string_buffer.string);
}

int main() {
	CustomString accum;
	accum.initialize(ITERATIONS*9);

	uint32_t total_iters_mult_15 = ((size_t)(ITERATIONS / 15)) * 15;
	uint32_t total_chunks = total_iters_mult_15 / 15;
	
	#ifdef INFO_MODE
		uint32_t iterLeft = ITERATIONS - total_iters_mult_15;
		printf("Iters to unroll: %d\n", total_iters_mult_15);
		printf("Iters left: %d\n", iterLeft);
		printf("Total chunks: %d\n", total_chunks);
	#endif

	UintInString num_string_buffer;
	num_string_buffer.initialize(12);

	const uint32_t total_threads =  std::thread::hardware_concurrency();
	uint32_t num_threads = std::min(total_threads, total_chunks);

	std::thread* threads = new std::thread[num_threads];
	CustomString* threads_accum = new CustomString[num_threads];

	// Start threads
	size_t chunks_per_thread = total_chunks / num_threads;
	size_t remainder_chunks = total_chunks - (chunks_per_thread * num_threads);

	#ifdef INFO_MODE
		printf("Num threads: %d\n", num_threads);
		printf("Chunks per thread (each chunk is 15): %ld, remainder chunks: %ld\n", chunks_per_thread, remainder_chunks);
	#endif

	size_t from = 0;
	size_t to = chunks_per_thread * 15;
	
	for(uint32_t i = 0; i < num_threads-1; i++) {
		threads_accum[i] = CustomString();
		threads_accum[i].initialize((ITERATIONS / num_threads) * 30);
		threads[i] = std::thread(fizzBuzz, from, to, &threads_accum[i]);
		from = to;
		to += chunks_per_thread * 15;
	}
	// Last thread do the remainder operations
	threads_accum[num_threads-1] = CustomString();
	threads_accum[num_threads-1].initialize((ITERATIONS / num_threads) * 30);
	to = to+remainder_chunks*15;
	threads[num_threads-1] = std::thread(fizzBuzz, from, to, &threads_accum[num_threads-1]);

	// Wait for every thread to finish
	for(uint32_t i = 0; i < num_threads; i++) {
		threads[i].join();
	}
	#ifdef INFO_MODE
		printf("----------------------------\n");
		printf("ALL THREADS JOINED\n");
		printf("----------------------------\n");
	#endif

	// Join all the strings
	#ifdef INFO_MODE
		size_t total_length = 0;
	#endif
	for(uint32_t i = 0; i < num_threads; i++) {
		#ifdef INFO_MODE
			total_length += threads_accum[i].position_null_char + 1;
		#endif
		accum.concat(threads_accum[i].string);
	}

	#ifdef INFO_MODE
		printf("TOTAL LENGTH %ld\n", total_length);
		printf("Total size left in string accum: %ld\n", accum.total_size_reserved - accum.position_null_char);
		printf("iters left: %ld\n", ITERATIONS - to);
	#endif

	// Finish remainder operations
	num_string_buffer.set_to_number(to);
	for(size_t i = to+1; i <= ITERATIONS; i++) {
		uint8_t encode = (i % 3 == 0) ? 1 : 0;
		encode += (i % 5 == 0) ? 2 : 0;

		num_string_buffer.update_add(1);
		
		if(encode == 0) {
			accum.concat(num_string_buffer.init_of_number_string, num_string_buffer.length);
			accum.concat(" ", 1);
		}else if(encode == 1) {
			accum.concat("fizz ", 5);
		}else if(encode == 2) {
			accum.concat("buzz ", 5);
		}else if(encode == 3) {
			accum.concat("fizzbuzz ", 9);
		}
	}

	//printf("%s\n", accum.string);
	accum.print_last(50);
	#ifdef INFO_MODE
		printf("Total size string: %ld\n", std::strlen(accum.string));
		printf("HASH: %ld\n", std::hash<std::string>{}(accum.string));
	#endif

	free(accum.string);
	free(num_string_buffer.string);

	delete[] threads;
	for(uint32_t i = 0; i < num_threads; i++) {
		free(threads_accum[i].string);
	}
	delete[] threads_accum;

	return 0;
}