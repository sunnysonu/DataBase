#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

char ** resize_memory_double_pointer(char ** memory, int new_size)
{
	memory = (char **)realloc(memory, sizeof(char *) * new_size);
	return memory;
}

void resize_memory_single_pointer(char * memory, int new_size)
{
	memory = (char *)realloc(memory, sizeof(char) * new_size);
}

char ** split_str(char * str, char delimiter, int * length)
{
	char ** words = (char **)malloc(sizeof(char));
	int str_pointer = 0, temp_pointer = 0;
	int capacity = 64;

	while (str[str_pointer] != '\0' && str[str_pointer - 1] != '\0')
	{
		temp_pointer = 0;

		words = resize_memory_double_pointer(words, *length + 1);
		words[*length] = (char *)malloc(sizeof(char) * capacity);

		while (str[str_pointer] != delimiter && str[str_pointer] != '\0')
		{
			if (temp_pointer >= capacity)
			{
				capacity = capacity * 2;
				resize_memory_single_pointer(words[*length], capacity);
			}
			words[*length][temp_pointer++] = str[str_pointer++];
		}
		if (temp_pointer >= capacity)
		{
			capacity = capacity * 2;
			resize_memory_single_pointer(words[*length], capacity);
		}
		words[*length][temp_pointer] = '\0';
		(*length)++;
		str_pointer++;
	}

	return words;
}
