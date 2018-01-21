#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0601
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>
#include "HashMap.h"

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

char * create_command(char * name, char * size);
void print_available_fields_types();
void add_fields_to_table(char * filename, int no_of_fields, char ** col_names, char * data_types);
struct tb_at access_data1(char * filename);
int get_size_of_row(char * type);
int convert_to_integer(char * size);
void increment_value_in_file(char * filename);
int power(int index);
void Menu();
char get_menu_option();
void insert_data(char * filename);
void create_table(char * table_name);
void access_full_table(char * filename);
void print_data(void ** data, char * filename, struct tb_at * tb);
void ** access_data(char * str);
void data_base_operations(char option);
void print_heading(char * filename);
void access_specific_row(char * filename);
void ** get_specific_row(char * filename);
int search_row(char * filename, void * value, char type, int size);
char get_data_type_of_value(int field_number, struct tb_at tb);
int get_starting_address_of_field(int field_number, struct tb_at tb);
void * get_specific_field_value(int field_number, struct tb_at tb);
void print_available_fields(struct tb_at tb);
void print_access_options();
void print_table(char * table_name);
void implement_db();

hash_global * Hash;

void initialize_Hash()
{
	Hash = (hash_global *)malloc(sizeof(hash_global));
	Hash->hash_item = (hash **)calloc(100, sizeof(hash *));
	for (int i = 0; i < 100; i++)
	{
		Hash->hash_item[i] = (hash *)calloc(1, sizeof(hash));
	}
}

struct tb_at
{
	int no_of_fields;
	char field_type[100];
	int rows = 0;
	char field_name[100][100];
	long int size_of_fields;
};

typedef struct table
{
	void ***table_data;
}table;

void DataBase()
{
	initialize_Hash();
	while (true)
	{
		implement_db();
	}
}

void implement_db()
{
	Menu();
	char option = get_menu_option();
	data_base_operations(option);
}

void data_base_operations(char option)
{
	void ** data = NULL;
	char str[10], del[50] = "del ";
	char op;

	switch (option)
	{
	case 'c':
	case 'C':
		create_table("faculty");
		break;

	case 'a':
	case'A':
		printf("\nEnter table name : ");
		gets(str);
		print_access_options();
		op = _getche();
		if (op == 'f')
		{
			access_full_table(str);
		}
		else
		{
			access_specific_row(str);
		}
		break;

	case 'd':
	case'D':
		printf("\nEnter table name:");
		gets(str);
		strcat(del, str);
		system(del);
		break;
	
	case 'i':
	case 'I':
		printf("\nEnter table name : ");
		gets(str);
		insert_data(str);
		break;

	case 'p':
	case 'P':
		printf("Enter table name:\n");
		gets(str);
		print_table(str);
		break;

	case 'e':
	case 'E':
		exit(0);

	}
}

// retriving data from the table

void ** access_data(char * filename, int file_no)
{
	void ** data = NULL;
	FILE * fp;
	int file_pointer = 0;
	int pointer = 0, name_pointer = 0;
	struct tb_at table;

	fp = fopen(filename, "rb+");
	rewind(fp);
	fread(&table, sizeof(struct tb_at), 1, fp);

	int size = get_size_of_row(table.field_type);

	data = (void **)malloc(sizeof(void *));


	while (table.field_type[pointer] != '\0')
	{
		switch (table.field_type[pointer])
		{
		case 'i':
			int integer;

			fseek(fp, sizeof(struct tb_at) + (file_no* size) + file_pointer, SEEK_SET);

			data[name_pointer] = (int *)malloc(sizeof(int));

			fread((int *)data[name_pointer], sizeof(int), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(int);

			break;
		case 'l':
			long int long_int;

			fseek(fp, sizeof(struct tb_at) + (file_no * size) + file_pointer, SEEK_SET);

			data[name_pointer] = (long int *)malloc(sizeof(long int));

			fread((long int *)data[name_pointer], sizeof(long int), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(long int);

			break;
		case 'f':
			float floating;

			fseek(fp, sizeof(struct tb_at) + (file_no * size) + file_pointer, SEEK_SET);

			data[name_pointer] = (float *)malloc(sizeof(float));

			fread((float *)data[name_pointer], sizeof(float), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(float);

			break;

		case 'c':
			char character;

			fseek(fp, sizeof(struct tb_at) + (file_no * size) + file_pointer, SEEK_SET);

			data[name_pointer] = (char *)malloc(sizeof(char));

			fread((char *)data[name_pointer], sizeof(char), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(char);
			break;

		case 's':
			char size_of_str[10];
			int i = 0, length;
			pointer++;

			while (table.field_type[pointer] >= 48 && table.field_type[pointer] < 58)
			{
				size_of_str[i++] = table.field_type[pointer++];
			}
			size_of_str[i] = '\0';
			length = convert_to_integer(size_of_str);


			fseek(fp, sizeof(struct tb_at) + (file_no * size) + file_pointer, SEEK_SET);

			data[name_pointer] = (char *)malloc(sizeof(char) * length);

			fread((char *)data[name_pointer], sizeof(char) * length, 1, fp);
			file_pointer = file_pointer + (sizeof(char) * length);

			break;
		}
		name_pointer++;
	}
	fclose(fp);
	return data;
}

// accessing entire table

void access_full_table(char * filename)
{
	FILE * fp;
	//void ** data;
	fp = fopen(filename, "rb+");
	rewind(fp);
	struct tb_at tb;
	fread(&tb, sizeof(struct tb_at), 1, fp);
	fclose(fp);

	table data;
	data.table_data = (void ***)malloc(sizeof(void **));

	for (int i = 0; i < tb.rows; i++)
	{
		data.table_data[i] = access_data(filename, i);
	}


	put_into_hash_map(Hash, filename, data.table_data);
}


void print_table(char * table_name)
{
	FILE * fp;
	//void ** data;
	fp = fopen(table_name, "rb+");
	rewind(fp);
	struct tb_at tb;
	fread(&tb, sizeof(struct tb_at), 1, fp);
	fclose(fp);

	print_heading(table_name);

	void *** test = get_from_hash(Hash, table_name);
	for (int i = 0; i < tb.rows; i++)
	{
		printf("\n");
		print_data(test[i], table_name, &tb);
	}
}
// printing data

void print_data(void ** data, char * filename, struct tb_at * tb)
{
	int a = 10;
	int pointer = 0, data_pointer = 0;
	
	while (tb->field_type[pointer] != '\0')
	{
		switch(tb->field_type[pointer++])
		{
		case 'i':
			printf("%15d", *((int *)data[data_pointer]));
			data_pointer++;
			break;
			
		case 'l':
			printf("%15d", *((long int *)data[data_pointer]));
			data_pointer++;
			break;

		case 'f':
			printf("%15f", *((float *)data[data_pointer]));
			data_pointer++;
			break;

		case 'c':
			printf("%15c", *((char *)data[data_pointer]));
			data_pointer++;
			break;

		case 's':
			printf("%15s", (char *)data[data_pointer]);
			data_pointer++;
			break;

		default:
			break;
		}
	}
}


void print_access_options()
{
	printf("\n[f]ull table");
	printf("\n[s]pecific row\n");
}

// printing column names


void print_heading(char * filename)
{
	FILE * fp;
	fp = fopen(filename, "rb+");
	struct tb_at tb;
	fread(&tb, sizeof(struct tb_at), 1, fp);
	fclose(fp);

	printf("\n");

	for (int i = 0; i < tb.no_of_fields; i++)
	{
		printf("%15s", tb.field_name[i]);
	}

	printf("\n");

	for (int i = 0; i < tb.no_of_fields * 15; i++)
	{
		printf("-");
	}
}

// taking input for the shown options

char get_menu_option()
{
	char ch = _getche();
	fflush(stdin);
	return ch;
}

//showinf menu()

void Menu()
{
	printf("\n\n[c]reate table");
	printf("\n[a]ccess existing table");
	printf("\n[d]elete table");
	printf("\n[i]nsert data");
	printf("\n[e]xit\n");
}

// creates table by taking name and size of the file

void create_table(char * table_name)
{
	char size[10] = "10240";
	int flag;
	char * command = create_command(table_name, size);
	fflush(stdin);
	flag = system(command);
	system("pause");
	//add_fields_to_table(table_name);
	//access_data1(name);
}

// returns a string consists of command required to create file

char * create_command(char * name, char * size)
{
	char * command = (char *)malloc(sizeof(char) * 50);
	char temp[50] = "fsutil file createnew ";
	strcpy(command, temp);
 	strcat(command, name);
	strcat(command, " ");
	strcat(command, size);
	return command;
}

// adding required fields and to save and retrive data

void add_fields_to_table(char * filename, int no_of_fields, char ** col_names, char * data_types)
{
	struct tb_at tb;
	/*char size[20], str[10];

	tb.field_type[0] = '\0';

	printf("\nEnter number of fields : ");
	scanf("%d", &(tb.no_of_fields));

	fflush(stdin);

	for (int i = 0; i < tb.no_of_fields; i++)
	{
		printf("\nEnter field name : ");
		gets(tb.field_name[i]);

		print_available_fields_types();
		gets(str);
		strcat(tb.field_type, str);
		if (strcmp(str, "s") == 0)
		{
			printf("\nEnter size : ");
			gets(size);
			strcat(tb.field_type, size);
		}
	}*/

	tb.no_of_fields = no_of_fields;
	
	for (int i = 0; i < tb.no_of_fields; i++)
	{
		strcpy(tb.field_name[i], col_names[i]);
	}

	strcpy(tb.field_type, data_types);

	tb.rows = 0;

	tb.size_of_fields = get_size_of_row(tb.field_type);

	FILE * fp = fopen(filename, "rb+");
	rewind(fp);

	fwrite(&tb, sizeof(struct tb_at), 1, fp);

	fclose(fp);
}

// printint available fields

void print_available_fields_types()
{
	printf("\n[i]nteger\n[l]ong\n[f]loat\n[c]har\n[s]tring");
}

// helper function to access data

struct tb_at access_data1(char * filename)
{
	struct tb_at tb;
	FILE * fp;
	fp = fopen(filename, "rb+");
	
	rewind(fp);
	fread(&tb, sizeof(struct tb_at), 1, fp);

	fclose(fp);

	return tb;
}

// inserting data into table

void insert_data(char * filename)
{
	FILE * fp;
	int pointer = 0, name_pointer = 0, file_pointer = 0;
	struct tb_at tb = access_data1(filename);

	//int size = get_size_of_row(tb.field_type);

	fp = fopen(filename, "rb+");

	while (tb.field_type[pointer] != '\0')
	{
		printf("Enter %s:", tb.field_name[name_pointer++]);
		switch (tb.field_type[pointer])
		{
		case 'i':
			int integer;
			fseek(fp, sizeof(struct tb_at) + (tb.rows * tb.size_of_fields) + file_pointer, SEEK_SET);
			scanf("%d", &integer);
			fflush(stdin);
			fwrite(&integer, sizeof(int), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(int);

			break;

		case 'l':
			long int long_int;
			fseek(fp, sizeof(struct tb_at) + (tb.rows * tb.size_of_fields) + file_pointer, SEEK_SET);
			scanf("%ld", &long_int);
			fflush(stdin);
			fwrite(&long_int, sizeof(long int), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(long int);

			break;

		case 'f':
			float floating;
			fseek(fp, sizeof(struct tb_at) + (tb.rows * tb.size_of_fields) + file_pointer, SEEK_SET);
			scanf("%f", &floating);
			fflush(stdin);
			fwrite(&floating, sizeof(float), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(float);

			break;

		case 'c':
			char character;
			fseek(fp, sizeof(struct tb_at) + (tb.rows * tb.size_of_fields) + file_pointer, SEEK_SET);
			scanf("%c", &character);
			//fflush(stdin);
			fwrite(&character, sizeof(char), 1, fp);
			pointer++;
			file_pointer = file_pointer + sizeof(char);

			break;

		case 's':
			char size_of_str[10];
			int i = 0, length;
			pointer++;

			while (tb.field_type[pointer] >= 48 && tb.field_type[pointer] < 58)
			{
				size_of_str[i++] = tb.field_type[pointer++];
			}
			size_of_str[i] = '\0';
			length = convert_to_integer(size_of_str);

			char * string = (char *)malloc(sizeof(char) * length);
			fseek(fp, sizeof(struct tb_at) + (tb.rows * tb.size_of_fields) + file_pointer, SEEK_SET);
			scanf("%s", string);
			fwrite(string, sizeof(char) * length, 1, fp);
			file_pointer = file_pointer + (sizeof(char) * length);

			break;
		}
	}
	
	fclose(fp);

	increment_value_in_file(filename);
}

// returns size of the row

int get_size_of_row(char * type)
{
	int size = 0, pointer = 0;
	while (type[pointer] != '\0')
	{
		switch (type[pointer])
		{
		case 'i':
			size = size + sizeof(int);
			pointer++;
			break;
		
		case 'l':
			size = size + sizeof(long int);
			pointer++;
			break;

		case 'f':
			size = size + sizeof(float);
			pointer++;
			break;

		case 'c':
			size = size + sizeof(char);
			pointer++;
			break;

		case 's':
			int i = 0;
			char str[20];
			pointer++;
			while (type[pointer] >= 48 && type[pointer] < 58)
			{
				str[i++] = type[pointer++];
			}
			str[i] = '\0';
			int length = convert_to_integer(str);
			size = size + sizeof(char) * length;
		}
	}
	return size;
}

// increments number of rows value in file

void increment_value_in_file(char * filename)
{
	FILE * fp;
	struct tb_at tb;
	fp = fopen(filename, "rb+");
	rewind(fp);

	fread(&tb, sizeof(struct tb_at), 1, fp);
	tb.rows++;

	rewind(fp);
	fwrite(&tb, sizeof(struct tb_at), 1, fp);

	fclose(fp);
}

// converts string to integer

int convert_to_integer(char * size)
{
	int integer = 0;
	_strrev(size);
	
	for (int i = 0; size[i] != '\0'; i++)
	{
		integer = integer + (size[i] - 48) * power(i);
	}
	return integer;
}

// returns the powers of ten(10)

int power(int index)
{
	int ans = 1;
	for (int i = 0; i < index; i++)
	{
		ans = ans * 10;
	}
	return ans;
}

void access_specific_row(char * filename)
{
	void ** data = get_specific_row(filename);
	print_heading(filename);
	printf("\n");
	struct tb_at tb;
	print_data(data, filename, &tb);
}

void ** get_specific_row(char * filename)
{
	int field_number;
	FILE * fp;
	struct tb_at tb;

	fp = fopen(filename, "rb+");
	fread(&tb, sizeof(struct tb_at), 1, fp);
	fclose(fp);

	print_available_fields(tb);
	printf("\nselect the above field for reference : ");
	scanf("%d", &field_number);

	void * value = get_specific_field_value(field_number, tb);

	char type = get_data_type_of_value(field_number, tb);

	int size = get_starting_address_of_field(field_number, tb);

	int row_number = search_row(filename, value, type, size);

	void ** data = access_data(filename, row_number - 1);

	return data;
}

int search_row(char * filename, void * value, char type, int size)
{
	int row_number = 1;
	struct tb_at tb;
	FILE * fp;

	fp = fopen(filename, "rb+");

	rewind(fp);
	fread(&tb, sizeof(struct tb_at), 1, fp);

	while (row_number <= tb.rows)
	{
		fseek(fp, size, SEEK_SET);
		
		switch (type)
		{
		case 'i':
			int integer;
			fread(&integer, sizeof(int), 1, fp);
			if (*((int *)value) == integer)
			{
				return row_number;
			}
			break;

		case 'l':
			long int long_int;
			fread(&long_int, sizeof(long int), 1, fp);
			if (*((long int *)value) == long_int)
			{
				return row_number;
			}
			break;

		case 'f':
			float floating;
			fread(&floating, sizeof(float), 1, fp);
			if (*((float *)value) == floating)
			{
				return row_number;
			}
			break;

		case 'c':
			char character;
			fread(&character, sizeof(char), 1, fp);
			if (*((char *)value) == character)
			{
				return row_number;
			}
			break;

		case 's':
			int length = strlen((char *)value);
			char * string = (char *)malloc(sizeof(char) * (length + 1));
			fread(string, sizeof(char) * (length + 1), 1, fp);
			if (strcmp(string, (char *)value) == 0)
			{
				return row_number;
			}
			break;
		}
		size = size + tb.size_of_fields;
		row_number++;
	}
	return 0;
}

char get_data_type_of_value(int field_number, struct tb_at tb)
{
	int pointer = 1, str_pointer = 0;

	while (pointer != field_number)
	{
		str_pointer++;
		if (tb.field_type[str_pointer] < 48 || tb.field_type[str_pointer] > 57)
		{
			pointer++;
		}
	}

	return tb.field_type[str_pointer];
}

int get_starting_address_of_field(int field_number, struct tb_at tb)
{
	int size = sizeof(struct tb_at);
	int pointer = 0, str_pointer = 1;
	
	while (str_pointer != field_number)
	{
		switch (tb.field_type[pointer])
		{
		case 'i':
			size = size + sizeof(int);
			pointer++;
			break;

		case 'l':
			size = size + sizeof(long int);
			pointer++;
			break;

		case 'f':
			size = size + sizeof(float);
			pointer++;
			break;

		case 'c':
			size = size + sizeof(char);
			pointer++;
			break;

		case 's':
			int i = 0;
			char str[20];
			pointer++;
			while (tb.field_type[pointer] >= 48 && tb.field_type[pointer] < 58)
			{
				str[i++] = tb.field_type[pointer++];
			}
			str[i] = '\0';
			int length = convert_to_integer(str);
			size = size + sizeof(char) * length;
		}
		str_pointer++;
	}
	return size;
}

void * get_specific_field_value(int field_number, struct tb_at tb)
{
	int pointer = 1, str_pointer = 0;
	char type;
	while (pointer != field_number)
	{
		str_pointer++;
		if (tb.field_type[str_pointer] < 48 || tb.field_type[str_pointer] > 57)
		{
			pointer++;
		}
	}
	type = tb.field_type[str_pointer];

	void * value = NULL;

	printf("\nEnter the value : ");

	switch (type)
	{
	case 'i':
		value = (int *)malloc(sizeof(int));
		scanf("%d", (int *)value);
		break;

	case 'l':
		value = (long int *)malloc(sizeof(long int));
		scanf("%ld", (long int *)value);
		break;

	case 'f':
		value = (float *)malloc(sizeof(float));
		scanf("%f", (float *)value);
		break;

	case 'c':
		value = (char *)malloc(sizeof(char));
		scanf("%c", (char *)value);
		break;

	case 's':
		value = (char *)malloc(sizeof(char) * 50);
		scanf("%s", (char *)value);
		break;

	default:
		break;
	}
	return value;
}

void print_available_fields(struct tb_at tb)
{
	for (int i = 0; i < tb.no_of_fields; i++)
	{
		printf("\n%d - %s", i + 1, tb.field_name[i]);
	}
}