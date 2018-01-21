#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

char parse_string[100][100][100] = { { "createtable", "{table_name}", "{column_names}" },
									 { "insert", "into", "{table_name}" },
									 { "select", "{col_names}", "from", "{table_name}" },
									 { "exit" },
									 { "delete", "{table_name}" }, 
									 {"cls"} };
int parse_lengths[100] = { 3, 3, 4, 1, 2, 1 };
int parse_string_length = 6;

hash_global * grammar_hash;

void initialize_grammar_hash()
{
	grammar_hash = (hash_global *)malloc(sizeof(hash_global));
	grammar_hash->hash_item = (hash **)calloc(100, sizeof(hash *));
	for (int i = 0; i < 100; i++)
	{
		grammar_hash->hash_item[i] = (hash *)calloc(1, sizeof(hash));
	}
}

char ** split_str(char * string, char delimiter)
{
	int str_len = 0, length = 0, temp_len = 0;
	char ** splitted_string = (char **)malloc(sizeof(char *));
	splitted_string[0] = (char *)malloc(sizeof(char));

	while (true)
	{
		temp_len = 0;
		splitted_string = (char **)realloc(splitted_string, sizeof(char *) * (str_len + 1));
		splitted_string[str_len] = (char *)malloc(sizeof(char) * 100);
		while (string[length] != '\0' && string[length] != delimiter)
		{
			splitted_string[str_len][temp_len] = string[length];
			length++;
			temp_len++;
		}
		splitted_string[str_len][temp_len] = '\0';
		str_len++;
		if (string[length] == '\0')
		{
			break;
		}
		length++;
	}
	return splitted_string;
}

int get_command_number(char * string)
{
	for (int i = 0; i < parse_string_length; i++)
	{
		if (strcmp(parse_string[i][0], string) == 0)
		{
			return i;
		}
	}
	return -1;
}

int get_command_length(char * command, char delimiter)
{
	int length = 0;
	for (int i = 0; command[i] != '\0'; i++)
	{
		if (command[i] == delimiter)
		{
			length++;
		}
	}
	return length + 1;
}

int is_valid_syntax_and_add_to_hash_map(char * command)
{
	char ** splitted_string = split_str(command, ' ');
	int command_number = get_command_number(splitted_string[0]);
	if (command_number == -1)
	{
		return -1;
	}
	else if (parse_lengths[command_number] != get_command_length(command, ' '))
	{
		return -1;
	}

	for (int i = 0; i < parse_lengths[command_number]; i++)
	{
		if (parse_string[command_number][i][0] == '{')
		{
			put_into_hash_map(grammar_hash, parse_string[command_number][i], splitted_string[i]);
		}
		else if (strcmp(parse_string[command_number][i], splitted_string[i]) != 0)
		{
			return -1;
		}
	}
}


