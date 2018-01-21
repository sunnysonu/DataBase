#ifndef word
#define word

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int hash_function(struct hash_global * hash_details, char * source);
void put_into_hash_map(struct hash_global * hash_details, char * key, void * value);
void *** get_from_hash(struct hash_global * hash_details, char * key);

typedef struct hash
{
	char key[50];
	void * value;
	hash * next = NULL;
}hash;

typedef struct hash_global
{
	hash ** hash_item;
	int item_count = 0;
	int capacity = 16;
}hash_global;

/*void hash_mapping()
{
	hash_global hash_details;

	hash_details.capacity = 16;
	hash_details.hash_item = (hash **)calloc(hash_details.capacity, sizeof(hash *));

	char * name1 = "sonu";
	char * name2 = "mouli";
	char * name3 = "sameer";
	char * name4 = "lee";

	put_into_hash_map(&hash_details, "1", (void *)name1);
	put_into_hash_map(&hash_details, "15", (void *)name3);
	put_into_hash_map(&hash_details, "2", (void *)name2);
	put_into_hash_map(&hash_details, "33", (void *)name4);
	void * item = get_from_hash(hash_details, "");
	printf("%s", (char *)item);
}*/

int hash_function(hash_global * hash_details, char * source)
{
	int count = 0;
	for (int i = 0; source[i] != '\0'; i++)
	{
		count = count + source[i] - 48;
	}
	return count % 100;
}

void put_into_hash_map(hash_global * hash_details, char * key, void * value)
{
	hash *var = (hash *)malloc(sizeof(hash));
	var->next = NULL;
	var->value = value;
	strcpy(var->key, key);
	int flag = 1;

	int index = hash_function(hash_details, key);

	if (hash_details->hash_item[index]->value == NULL)
	{
		hash_details->hash_item[index] = var;
	}
	else if (strcmp(hash_details->hash_item[index]->key, var->key) == 0)
	{
		var->next = hash_details->hash_item[index]->next;
		free(hash_details->hash_item[index]);
		hash_details->hash_item[index] = var;
	}
	else
	{
		hash *temp;
		for (temp = hash_details->hash_item[index]; temp->next != NULL; temp = temp->next)
		{
			if (strcmp(temp->key, var->key) == 0)
			{
				var->next = temp->next;
				free(temp);
				temp = var;
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			temp->next = var;
		}
	}
}

void *** get_from_hash(hash_global * hash_details, char * key)
{
	int index = hash_function(hash_details, key);
	hash * p;
	for (p = hash_details->hash_item[index]; p != NULL; p = p->next)
	{
		if (strcmp(p->key, key) == 0)
		{
			return (void ***)(p->value);
		}
	}
}

#endif
