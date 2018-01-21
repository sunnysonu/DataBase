#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "Tabel.h"
#include "interpreter.h"
#include "HashMap.h"

void execute_commands(char * command);

int main()
{
	initialize_Hash();
	while (true)
	{
		char command[100];
		initialize_grammar_hash();
		fflush(stdin);
		printf("\n>> ");
		gets(command);
		fflush(stdin);
		execute_commands(command);
		fflush(stdin);
	}
	_getch();
	return 0;
}

void create_table_command()
{
	char * table_name = (char *)get_from_hash(grammar_hash, parse_string[0][1]);
	char * col_names_and_data_types = (char *)get_from_hash(grammar_hash, parse_string[0][2]);
	
	char ** splitted_col_names_and_data_types = split_str(col_names_and_data_types, ',');
	int no_of_cols = get_command_length(col_names_and_data_types, ',');

	char * data_types = (char *)malloc(sizeof(char) * no_of_cols * 100);
	char ** col_names = (char **)malloc(sizeof(char *) * no_of_cols);

	int data_type_length = 0;

	for (int i = 0; i < no_of_cols; i++)
	{
		col_names[i] = (char *)malloc(sizeof(char) * 100);
	}



	for (int i = 0; i < no_of_cols; i++)
	{
		char ** temp = split_str(splitted_col_names_and_data_types[i], ':');
		strcpy(col_names[i], temp[0]);
		if (temp[1][0] == 's')
		{
			data_types[data_type_length++] = 's';
			for (int j = 7; temp[1][j] != ')'; j++)
			{
				data_types[data_type_length++] = temp[1][j];
			}
		}
		else
		{
			data_types[data_type_length++] = temp[1][0];
		}
	}
	data_types[data_type_length] = '\0';

	create_table(table_name);

	add_fields_to_table(table_name, no_of_cols, col_names, data_types);

}

void insert_data_command()
{
	char * table_name = (char *)get_from_hash(grammar_hash, parse_string[1][2]);
	insert_data(table_name);
}

void select_command()
{
	char * table_name = (char *)get_from_hash(grammar_hash, parse_string[2][3]);
	access_full_table(table_name);
	print_table(table_name);
}

void delete_table_command()
{
	char * table_name = (char *)get_from_hash(grammar_hash, parse_string[4][2]);
	char del[10] = "del ";
	strcat(del, table_name);
	system(del);
}

void execute_commands(char * command)
{
	int command_number = is_valid_syntax_and_add_to_hash_map(command);
	switch (command_number)
	{
	case 0:
		create_table_command();
		break;

	case 1:
		insert_data_command();
		break;

	case 2:
		select_command();
		break;

	case 3:
		exit(0);

	case 4:
		delete_table_command();
		break;

	case 5:
		system("cls");
		break;

	default:
		printf("syntax error");
	}
}