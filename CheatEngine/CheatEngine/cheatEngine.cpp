#include "cheatEngine.h"

#define size 0x00000808

// The search function scans all memory from 0x00000000 to 0x7FFFFFFF for the passed value
void search(const HANDLE process, const int passed_val) {
	FILE* temp_file = NULL;
	fopen_s(&temp_file, "hex.txt", "w");

	unsigned char* buffer = (unsigned char*)calloc(1, size);
	size_t bytes_read = 0;

	for (DWORD i = 0x00000000; i < 0x7FFFFFFF; i += size) {
		ReadProcessMemory(process, (void*)i, buffer, size, &bytes_read);

		for (int j = 0; j < size - 4; j += 4) {
			DWORD val = 0;
			memcpy(&val, &buffer[j], 4);
			if (val == passed_val) {
				fprintf(temp_file, "%x\n", i + j);
			}
		}
	}

	fclose(temp_file);

	free(buffer);
}

// The filter function takes a list of addresses in hex.txt and checks to see
// if they match the provided value. If so, they are written to res_fil.txt
// After the initial pass, filter writes all the addresses in res_fil.txt to hex.txt
void next(const HANDLE process, const int passed_val) {
	FILE* temp_file = NULL;
	FILE* temp_file_filter = NULL;
	fopen_s(&temp_file, "hex.txt", "r");
	fopen_s(&temp_file_filter, "res_fil.txt", "w");

	DWORD address = 0;
	while (fscanf_s(temp_file, "%x\n", &address) != EOF) {
		DWORD val = 0;
		size_t bytes_read = 0;

		ReadProcessMemory(process, (void*)address, &val, 4, &bytes_read);
		if (val == passed_val) {
			fprintf(temp_file_filter, "%x\n", address);
		}
	}

	fclose(temp_file);
	fclose(temp_file_filter);

	fopen_s(&temp_file, "hex.txt", "w");
	fopen_s(&temp_file_filter, "res_fil.txt", "r");
	while (fscanf_s(temp_file_filter, "%x\n", &address) != EOF) {
		fprintf(temp_file, "%x\n", address);
	}

	fclose(temp_file);
	fclose(temp_file_filter);

	remove("res_fil.txt");
}

// The write function writes a value to every address in hex.txt
void write(const HANDLE process, const int passed_val) {
	FILE* temp_file = NULL;
	fopen_s(&temp_file, "hex.txt", "r");

	DWORD address = 0;
	while (fscanf_s(temp_file, "%x\n", &address) != EOF) {
		size_t bytes_written = 0;

		WriteProcessMemory(process, (void*)address, &passed_val, 4, &bytes_written);
	}

	fclose(temp_file);
}