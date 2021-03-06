// Practica7CarlosHD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


const char INCLUDE_STRING[] = "#include ";
const unsigned int INCLUDE_LENGTH = strlen(INCLUDE_STRING);


/**
Searches and saves in a file all the header files included starting a source file.

@param target_file Identifier of the target file. Must be opened and set to write.
@param file_name Name of the source file to be inspected.
@param level Deep of the file which is inspected during the process.
*/
void inspect_headers(FILE * target_file, const char * file_name, const unsigned int level)
{
	if (target_file && file_name)
	{
		fwrite("\n", 1, 1, target_file);

		FILE * file = fopen(file_name, "rb");
		unsigned int file_size = 0;
		fseek(file, 0, SEEK_END);
		file_size = static_cast<unsigned int>(ftell(file));
		fseek(file, 0, SEEK_SET);
		char * buffer = new char[file_size + 1];
		fread(buffer, 1, file_size, file);
		fclose(file);
		buffer[file_size] = '\0';

		char * token = nullptr;
		unsigned int i = 0;
		char * next_token = nullptr;
		char * next_file_name = nullptr;
		unsigned int next_file_name_length = 0;
		char delimiter[] = "\n";
		unsigned int file_name_start_pos = 0;

		token = strtok_s(buffer, delimiter, &next_token);
		while (token)
		{
			i = 0;
			while (*(token + i) == INCLUDE_STRING[i] && i < INCLUDE_LENGTH) 
				++i;

			if (i == INCLUDE_LENGTH) // Include string has been found. Now, file name is extracted.
			{
				// File name may be surrounded by "" or by <>.
				while ('\"' != *(token + i) && '<' != *(token + i))
					++i;

				++i;
				file_name_start_pos = i;
				while ('\"' != *(token + i) && '>' != *(token + i))
					++i;

				next_file_name_length = (i - 1) - file_name_start_pos + 1;
				next_file_name = new char[next_file_name_length + 1];
				for (unsigned int j = file_name_start_pos; j < i; j++)
					next_file_name[j - file_name_start_pos] = *(token + j);

				next_file_name[next_file_name_length] = '\0';

				for (unsigned int j = 1; j < level; j++)
					fwrite("\t", 1, 1, target_file);

				fwrite(next_file_name, 1, next_file_name_length, target_file);

				inspect_headers(target_file, next_file_name, (level + 1));

				delete []next_file_name;
			}

			token = strtok_s(NULL, delimiter, &next_token);
		}
		delete[]buffer;
	}
}

int main()
{
	FILE * headers_file = fopen("headers.txt", "w");
	if (headers_file)
	{
		inspect_headers(headers_file, "Fuente1.cpp", 1);
		printf("\nHeaders file successfully created");

		fclose(headers_file);
	} else
		printf("\nFile could not be opened");

	printf("\n\nPress Enter to quit...");
	getchar();

	return 0;
}

