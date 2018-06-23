#pragma once
#include<stdio.h>
#include<stdlib.h>

#include "myString.h"



struct row_metadata{
	char* primary_key;
	int current;
	int commit;
};

struct node{
	char* column_name;
	char* value;
	//int type;
	int version;

	struct node* next_column;
	struct node* next_version;
};






struct column_metadata{
	char* col_name;
	column_metadata* next_column;
};


struct mytable{
	char* table_name;
	row_metadata** row_metadata_table;
	node** data;
	int row_count;
	column_metadata* list_cols;
	int number_of_cols;
};

//struct mytable** database;
//int number_of_tables;

struct get_response{
	node* data;
	int version;
};

struct put_request{
	char* col;
	char* value;
};

struct schema{
	int number_of_tables;
	mytable** list_of_tables;
};

schema* s;

int get_table_index(char* table_name);
void make_sure_the_row_exists(int table_index, int rowid){

	int new_size  =0;
	if (rowid >= s->list_of_tables[table_index]->row_count){

		new_size = s->list_of_tables[table_index]->row_count * 2;


		node** newtable = (node**)malloc(sizeof(node*)*new_size);
		row_metadata** newrow_metadata = (row_metadata**)malloc(sizeof(row_metadata*)*new_size);

		int count = 0;

		for (int i = 0; i < s->list_of_tables[table_index]->row_count; i++){
			newtable[count] = s->list_of_tables[table_index]->data[i];
			newrow_metadata[count] = s->list_of_tables[table_index]->row_metadata_table[i];
			count++;
			//printf("along with count %d\n", count);
		}
		for (int i = count; i < new_size; i++){
			newtable[i] = NULL;
			
			newrow_metadata[i] = (row_metadata*)malloc(sizeof(row_metadata));
			newrow_metadata[i]->commit = 0;
			newrow_metadata[i]->current = 0;
		//	printf("along with i %d\n", i);
		}

		free(s->list_of_tables[table_index]->data);
		s->list_of_tables[table_index]->data = newtable;
		s->list_of_tables[table_index]->row_metadata_table = newrow_metadata;

		s->list_of_tables[table_index]->row_count = new_size;

	}
}


//int hash(char *str)
//{
//	int hash = 5381;
//	int c;
//
//	while (c = *str++)
//		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//
//	return hash % 10000;
//}

void add_col(char* table_name,column_metadata** source, column_metadata* col){
	int table_index = get_table_index(table_name);

	column_metadata* temp = *source;
	if (temp == NULL){
		*source = col;
		s->list_of_tables[table_index]->number_of_cols = 1;
		return;
	}if (!strcmp(temp->col_name, col->col_name)){
		return;
	}
	while (temp != NULL && temp->next_column != NULL){
		if (!strcmp(temp->col_name, col->col_name)){
			return;
		}
		temp = temp->next_column;
	}
	if (strcmp(temp->col_name, col->col_name)){
		s->list_of_tables[table_index]->number_of_cols++;
			temp->next_column = col;
	}
		
}

void load_db(){
	s = (schema*)malloc(sizeof(schema));
	s->number_of_tables = 0;
	/*for (int i = 0; i < s->number_of_tables; i++){

		s->list_of_tables[i]->data = (node**)malloc(sizeof(node) * 100);
		s->list_of_tables[i]->row_metadata_table = (row_metadata**)malloc(sizeof(row_metadata*) * 100);

		for (int j = 0; j < 10000; j++){
			s->list_of_tables[i]->row_metadata_table[j] = (row_metadata*)malloc(sizeof(row_metadata));
			s->list_of_tables[i]->row_metadata_table[j]->commit = 0;
			s->list_of_tables[i]->row_metadata_table[j]->current = 0;
		}

		for (int j = 0; j < 10000; j++){
			s->list_of_tables[i]->data[j] = NULL;
		}

		s->list_of_tables[i]->list_cols = NULL;
	}*/
}

void load_the_table(int i){

	s->list_of_tables[i]->data = (node**)malloc(sizeof(node) * 50);
	s->list_of_tables[i]->row_metadata_table = (row_metadata**)malloc(sizeof(row_metadata*) * 50);

	for (int j = 0; j < 50; j++){
		s->list_of_tables[i]->row_metadata_table[j] = (row_metadata*)malloc(sizeof(row_metadata));
		s->list_of_tables[i]->row_metadata_table[j]->commit = 0;
		s->list_of_tables[i]->row_metadata_table[j]->current = 0;
	}

	for (int j = 0; j < 50; j++){
		s->list_of_tables[i]->data[j] = NULL;
	}
	s->list_of_tables[i]->row_count = 50;
	s->list_of_tables[i]->list_cols = NULL;
}


void initialize_schema(schema* q){
	int number_of_tables = s->number_of_tables;
	//char buff[30];
	mytable** database = (struct mytable**)malloc(sizeof(struct mytable*)*number_of_tables);
	for (int i = 0; i < s->number_of_tables; i++){
		database[i] = (mytable*)malloc(sizeof(mytable));

		//scanf("%s", buff);
		database[i]->table_name = (char*)malloc(sizeof(char)*strlen(s->list_of_tables[i]->table_name));
		strcpy(s->list_of_tables[i]->table_name, q->list_of_tables[i]->table_name);

	}
	load_db();
}

int get_table_index(char* table_name){
	int i = 0;
	for ( i = 0; i < s->number_of_tables; i++){
		if (!strcmp(s->list_of_tables[i]->table_name, table_name)){
			return i;
		}
	}
	int num = s->number_of_tables;
	mytable** temp = (mytable**)malloc(sizeof(mytable*) * (num + 1));
	for ( i = 0; i < num; i++){
		temp[i] = (mytable*)malloc(sizeof(mytable));
		temp[i]->data = s->list_of_tables[i]->data;
		temp[i]->list_cols = s->list_of_tables[i]->list_cols;
		temp[i]->number_of_cols = s->list_of_tables[i]->number_of_cols;
		temp[i]->row_count = s->list_of_tables[i]->row_count;
		temp[i]->row_metadata_table = s->list_of_tables[i]->row_metadata_table;
		temp[i]->table_name = s->list_of_tables[i]->table_name;
	}
	temp[i] = (mytable*)malloc(sizeof(mytable));
	temp[i]->data = NULL;
	temp[i]->list_cols = NULL;
	temp[i]->number_of_cols = 0;
	temp[i]->row_count = 0;
	temp[i]->row_metadata_table = (row_metadata**)malloc(sizeof(row_metadata*));
	temp[i]->table_name = table_name;
	s->number_of_tables++;
	s->list_of_tables = temp;
	load_the_table(i);

	
	
	return i;
	
}




void put_one(char*  table_name  ,   int key, char* col_name, char* value, int current_number){
	int table_index = get_table_index(table_name);
	make_sure_the_row_exists(table_index, key);
	//int index = hash(key);
	int index = key;
	//row_metadata_table[index]->current++;
	int my_current_version = current_number;
	//int my_current_version = row_metadata_table[index]->current;
	int col_found = 0;

	if (my_current_version < s->list_of_tables[table_index]->row_metadata_table[index]->commit){
		return;
	}

	if (s->list_of_tables[table_index]->data[index] == NULL){

		node* temp = (node*)malloc(sizeof(node));
		temp->column_name = col_name;
		temp->next_column = NULL;
		temp->next_version = NULL;
		temp->value = value;
		temp->version = 1;
		column_metadata* new_col = (column_metadata*)malloc(sizeof(char));
		new_col->col_name = col_name;
		new_col->next_column = NULL;
		add_col(table_name,&(s->list_of_tables[table_index]->list_cols), new_col);
		s->list_of_tables[table_index]->data[index] = temp;
		return;
	}

	node* temp = s->list_of_tables[table_index]->data[index];
	//first node itself is matching 
	if (temp != NULL && !strcmp(temp->column_name, col_name)){
		col_found = 1;

		node* temp1 = (node*)malloc(sizeof(node));
		
		temp1->column_name = col_name;
		temp1->next_column = temp->next_column;
		temp1->next_version = temp;
		temp1->value = (char*)malloc(sizeof(char)*strlen(value));
		strcpy(temp1->value , value);
		temp1->version = temp->version + 1;

		s->list_of_tables[table_index]->data[index] = temp1;
		return;
	}//second and more rows matching
	while (temp != NULL && temp->next_column!=NULL){
		if (!strcmp(temp->next_column->column_name, col_name)){
			col_found = 1;
			node* temp1 = (node*)malloc(sizeof(node));
			temp1->column_name = col_name;
			temp1->next_column = temp->next_column->next_column;
			temp1->next_version = temp->next_column;
			//free(temp1->value);
			temp1->value = (char*)malloc(sizeof(char*)*strlen(value));
			strcpy(temp1->value ,value);
			temp1->version = temp->next_column->version+ 1;
			temp->next_column = temp1;
			
			return;

		}
		temp = temp->next_column;
	}
	if (col_found == 0){
		//add new node to the data at index

		//adding column to list of cols
		column_metadata* new_col = (column_metadata*)malloc(sizeof(column_metadata));
		new_col->col_name = (char*)malloc(sizeof(char)*strlen(col_name));
		strcpy(new_col->col_name, col_name);
		new_col->next_column = NULL;
		add_col(table_name,&(s->list_of_tables[table_index]->list_cols), new_col);

		//add new node to the data at index
		node* new_node = (node*)malloc(sizeof(node));
		new_node->column_name = (char*)malloc(sizeof(char)*strlen(col_name));
		strcpy(new_node->column_name, col_name);
		new_node->next_column = NULL;
		new_node->next_version = NULL;
		new_node->value = (char*)malloc(sizeof(char)*strlen(value));
		strcpy(new_node->value ,value);
		new_node->version = 1;

		temp = s->list_of_tables[table_index]->data[index];

			while (temp->next_column != NULL){
				temp = temp->next_column;
			}
			temp->next_column = new_node;

	}

	if (s->list_of_tables[table_index]->row_metadata_table[index]->commit < my_current_version)
		s->list_of_tables[table_index]->row_metadata_table[index]->commit = my_current_version;

}

void put(char* table_name, int key, put_request* put_array, int put_count, int current_number){
	for (int i = 0; i < put_count; i++){
		put_one(table_name, key, put_array[i].col, put_array[i].value, current_number);
	}
}


char* get_value(char* table_name,  int key, char* column_name){
	int table_index = get_table_index(table_name);
	//int index = hash(key);
	int index = key;
	make_sure_the_row_exists(table_index, key);

	if (s->list_of_tables[table_index]->data[index] == NULL){
		return "";
	}

	int committed_version = s->list_of_tables[table_index]->row_metadata_table[index]->commit;

	node* temp = s->list_of_tables[table_index]->data[index];
	while (temp != NULL){
		if (!strcmp(temp->column_name, column_name)){
			//if the first node is itself the latest node
			if (temp->version = committed_version){
				return temp->value;
			}
			else{
				//traverse to get the latest node
				node* version_temp = temp;
				while (version_temp->next_version != NULL){
					if (version_temp->version <= committed_version){
						return version_temp->value;
					}
					version_temp = version_temp->next_version;
				}
			}
		}
		temp = temp->next_column;
	}
	return "";

}
node* get_node(char* table_name, int key, char* column_name){
	int table_index = get_table_index(table_name);
	int committed_version = s->list_of_tables[table_index]->row_metadata_table[key]->commit;

	node* temp = s->list_of_tables[table_index]->data[key];
	while (temp != NULL){
		if (!strcmp(temp->column_name, column_name)){
			//if the first node is itself the latest node
			if (temp->version = committed_version){
				return temp;
			}
			else{
				//traverse to get the latest node
				node* version_temp = temp;
				while (version_temp->next_version != NULL){
					if (version_temp->version <= committed_version){
						return version_temp;
					}
					version_temp = version_temp->next_version;
				}
			}
		}
		temp = temp->next_column;
	}

}

void add_to_result(  node** head, node* val){
	if ( (*head) == NULL){
		(*head) = (node*)malloc(sizeof(node));
		(*head)->column_name = val->column_name;
		(*head)->next_column = NULL;
		(*head)->next_version = NULL;
		(*head)->value = val->value;
		(*head)->version = val->version;
		return;
		
	}

	else{
		node* temp = *head;
		while (temp->next_column != NULL){

			temp = temp->next_column;
		}
		temp->next_column = (node*)malloc(sizeof(node));
		temp->next_column->column_name = val->column_name;
		temp->next_column->next_column = NULL;
		temp->next_column->next_version = NULL;
		temp->next_column->value = val->value;
		temp->next_column->version = val->version;
	}
}

get_response* get( char* table_name,  int key){
	int table_index = get_table_index(table_name);
	//int index = hash(key);
	int index = key;
	make_sure_the_row_exists(table_index, key);
	get_response* response = (get_response*)malloc(sizeof(get_response));
	column_metadata* col = s->list_of_tables[table_index]->list_cols;
	node* result = NULL;

	while (col != NULL){
		add_to_result(&result, get_node(s->list_of_tables[table_index]->table_name, key, col->col_name));
		col = col->next_column;
	}

	response->data = result;
	response->version = s->list_of_tables[table_index]->row_metadata_table[index]->commit;
	return response;
	
}




void display(char* table_name , int key){
	int table_index = get_table_index(table_name);
 	//int index = hash(key);
	int index = key;
	make_sure_the_row_exists(table_index, key);
	printf("Current: %d\n", s->list_of_tables[table_index]->row_metadata_table[index]->current);
	printf("Commit: %d\n", s->list_of_tables[table_index]->row_metadata_table[index]->commit);
	node* temp = s->list_of_tables[table_index]->data[index];
	while (temp != NULL){
		printf("%s ", temp->column_name);
		printf("%s ", temp->value);
		printf("%d\n", temp->version);
		temp = temp->next_column;
	}
}

void print_list_cols(char* table_name){
	int table_index = get_table_index(table_name);
	column_metadata* temp = s->list_of_tables[table_index]->list_cols;
	printf("List of columns\n");
	while (temp != NULL){
		printf("%s ", temp->col_name);
		temp = temp->next_column;
	}
	printf("\n");
}



void delete_row(char* table_name,int key){
	int table_index = get_table_index(table_name);
	s->list_of_tables[table_index]->data[key] = NULL;
}




void display_response(get_response* gt){
	printf("Version number %d\n", gt->version);
	node* temp = gt->data;
	while (temp != NULL){
		printf("Column_name: %s value: %s\n", temp->column_name, temp->value);
		printf("strlen %d %d\n", strlen(temp->column_name), strlen(temp->value));
		temp = temp->next_column;
	}
	printf("\n");
}


char* create_filename(char* table_name, char* col_name){
	return strconcat(strconcat(strconcat(table_name, "_"), col_name), ".txt");
}


void dump_to_file(){
	node* dump_stack = NULL;
	printf("dumping to file\n");
	for (int j = 0; j < s->number_of_tables; j++){
		column_metadata* col_count = s->list_of_tables[j]->list_cols;
		//printf("%s\n", s->list_of_tables[j]->table_name);
		while (col_count != NULL){
			//printf("%s\n", col_count->col_name);
			FILE* fp = fopen(create_filename(s->list_of_tables[j]->table_name, col_count->col_name), "w");
			for (int i = 0; i < s->list_of_tables[j]->row_count; i++){
				if (s->list_of_tables[j]->data[i] != NULL){
					node* row = s->list_of_tables[j]->data[i];;
					while (row != NULL){
						if (strcmp(row->column_name, col_count->col_name) == 0){
							node* version_count = row;
							dump_stack == NULL;
							// need stack here to have correct order of saving
							while (version_count != NULL){
								//if dumpstack is empty
								if (dump_stack == NULL){
									dump_stack = version_count;
								}
								//if dumpstack is not empty
								else{
									version_count->next_version = dump_stack;
									dump_stack = version_count;
								}
								version_count = version_count->next_version;
								
							}
							//print from dump stack
							node* temp_dump_stack = dump_stack;
							while (temp_dump_stack != NULL){
								fprintf(fp, "%d %s %d\n", i, temp_dump_stack->value, temp_dump_stack->version);
								version_count = version_count->next_version;
							}

						}
						row = row->next_column;
					}
				}
			}
			fclose(fp);
			col_count = col_count->next_column;
		}
	}
	printf("Dumped all data to files");

	FILE* f = fopen(create_filename("schema", "metadata"), "w");
	fprintf(f, "%d\n", s->number_of_tables);
	for (int i = 0; i < s->number_of_tables; i++){
		fprintf(f, "%s\n", s->list_of_tables[i]->table_name);
		FILE* ft = fopen(create_filename(s->list_of_tables[i]->table_name, "metadata"), "w");
	//	fprintf(ft, "%s\n", s->list_of_tables[i]->table_name);
		fprintf(ft, "%d\n", s->list_of_tables[i]->row_count);
		for (int j = 0; j < s->list_of_tables[i]->row_count; j++){
			fprintf(ft, "%d %d\n", s->list_of_tables[i]->row_metadata_table[j]->current, s->list_of_tables[i]->row_metadata_table[j]->commit);
		}
		fprintf(ft, "%d\n", s->list_of_tables[i]->number_of_cols);
		column_metadata* temp = s->list_of_tables[i]->list_cols;
		while(temp!=NULL){
			fprintf(ft, "%s\n", temp->col_name);
			temp = temp->next_column;
		}
		
	}
}

void load_all(){
	FILE* fp = fopen(create_filename("schema", "metadata"), "r");
	int n = 0;
	char table[30];
	fscanf(fp, "%d", &n);
	printf("%d\n", n);
	s->number_of_tables = n;
	s->list_of_tables = (mytable**)malloc(sizeof(mytable*));
	for (int i = 0; i < n; i++){
		s->list_of_tables[i] = (mytable*)malloc(sizeof(mytable));
		fscanf(fp, "%s", table);
		strcpy(s->list_of_tables[i]->table_name, table);
		FILE* f_table = fopen(create_filename(s->list_of_tables[i]->table_name, "metadata"), "r");

		printf("%s\n", table);
	}
	
	fclose(fp);
}