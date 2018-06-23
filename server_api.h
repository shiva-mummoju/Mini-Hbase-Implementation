#pragma once
#include "data_store.h"

int global_current_number_user = 1;
int global_current_number_post = 1;
int global_current_number_follow = 1;
int global_post_id = 0;
int global_user_id = 0;


void add_post_to_user(int user1, int postid);
void add_following_to_user(int user2, int user1);
char* create_filename(char* table_name, char* col_name);

void kickstart(){
	load_db();
}

//void kickstart(){
//
//	s->number_of_tables = 3;
//	s->list_of_tables = (mytable**)malloc(sizeof(mytable*)*s->number_of_tables);
//		for (int i = 0; i < s->number_of_tables; i++){
//			s->list_of_tables[i] = (mytable*)malloc(sizeof(mytable));
//		}
//

	//schema* s = (schema*)malloc(sizeof(schema));
	//s->number_of_tables = 3;
	//s->list_of_tables = (mytable**)malloc(sizeof(mytable*)*s->number_of_tables);
	//
	//for (int i = 0; i < s->number_of_tables; i++){
	//	s->list_of_tables[i] = (mytable*)malloc(sizeof(mytable));

	//}
	//s->table_names[0] = "user";
	//s->table_names[1] = "follow";
	//s->table_names[2] = "post";

	//initialize_schema(s);
//}

void create_user(char* username,char* email,char* name, char* phone, char* dob, char* profile){
	global_user_id++;
	put_request pr[6] = {
		{ "username", username },
		{ "email", email },
		{ "name", name },
		{ "phone",phone },
		{ "dob" , dob },
		{ "profile" , profile }
	};
	put("user", global_user_id, pr, 6, global_current_number_user++);

}



int add_post_by_user(int user, char* text, char* photo, char* comments, int views, int likes ){
	char buff1[30] , buff2[30], buff3[30];
	global_post_id++;
	char* name = get_value("user", user, "name");
	put_request pr[6] = {
		{ "user", name },
		{ "text", text },
		{ "photo", photo },
		{ "comments", comments },
		{ "views", _itoa(views,buff1,10) },
		{ "likes", _itoa(likes, buff2, 10) }
	};
	put("post", global_user_id, pr, 6, global_current_number_user++);
	int post_id = global_post_id;


	add_post_to_user(user, global_post_id);


	char* follow_list = get_value("follow", user, "followers");
	if (strlen(follow_list) == 0){
		return global_post_id;
	}

	char* temp_follow_list = (char*)malloc(sizeof(char*)*strlen(follow_list));
	/*for (int i = 0; i < strlen(follow_list); i++){
		temp_follow_list[i] = follow_list[i];
	}*/
	strcpy(temp_follow_list, follow_list);
	char** token = strtok(temp_follow_list, ",");

	int token_count = sizeof(token) / sizeof(char*);

	for (int i = 0; i < token_count; i++){
		printf("\n*** tokens ***\n");
		add_post_to_user(atoi(token[i]), global_post_id);
		get_response* gt = get("user", atoi(token[i]));
		display_response(gt);
	}
	return global_post_id;
}

void add_post_to_user(int user1, int postid){
	int user_index = user1;
	char* buff  = (char*)malloc(sizeof(10));
	global_current_number_follow++;
	char* follow_list_user1 = get_value("follow", user1, "post");
	if (strlen(follow_list_user1) == 0){
		 buff = (char*)malloc(sizeof(char) * 50);
		put_request* pr = (put_request*)malloc(sizeof(put_request));
		pr->col = (char*)malloc(sizeof(char)*strlen("post"));
		strcpy(pr->col, "post");
		pr->value = (char*)malloc(sizeof(char)*(strlen(_itoa(postid,buff,10)) + 1));
		strcpy(buff, _itoa(postid,buff,10));
		buff = strconcat(buff, ",");
		//pr->value = user2;
		strcpy(pr->value, buff);
		put("follow", user1, pr, 1, global_current_number_follow);
		return;
		//free(buff);
	}
	else{
		 buff = (char*)malloc(sizeof(char)*(strlen(follow_list_user1) + strlen(_itoa(postid,buff,10)) + 1));
		put_request* pr = (put_request*)malloc(sizeof(put_request));
		pr->col = (char*)malloc(sizeof(char)*strlen("post"));
		strcpy(pr->col, "post");
		pr->value = (char*)malloc(sizeof(char)*(strlen(follow_list_user1) + strlen(_itoa(postid,buff,10)) + 1));
		strcpy(buff, follow_list_user1);
		buff = strconcat(buff, _itoa(postid, buff, 10));
		buff = strconcat(buff, ",");
		strcpy(pr->value, buff);
		put("follow", user1, pr, 1, global_current_number_follow);
		return;
	}

}

void add_follower_to_user(int user1,int user2){
	char* buff = (char*)malloc(sizeof(char) * 30);
	int user_index = user1;
	char* follow_list_user1 = get_value("follow", user1, "followers");
	if (strlen(follow_list_user1) == 0){
		 buff = (char*)malloc(sizeof(char)*50);
		put_request* pr = (put_request*)malloc(sizeof(put_request));
		pr->col = (char*)malloc(sizeof(char)*strlen("followers"));
		strcpy(pr->col , "followers");
		pr->value = (char*)malloc(sizeof(char)*(strlen(_itoa(user2,buff,10)) + 1));
		strcpy(buff, _itoa(user2, buff, 10));
		buff = strconcat(buff, ",");
		//pr->value = user2;
		strcpy(pr->value, buff);
		put("follow", user1, pr, 1, global_current_number_follow++);
		free(buff);
	}
	else{
		 buff = (char*)malloc(sizeof(char)*(strlen(follow_list_user1) + strlen(_itoa(user2, buff, 10)) + 1));
		put_request* pr = (put_request*)malloc(sizeof(put_request));
		pr->col = (char*)malloc(sizeof(char)*strlen("followers"));
		strcpy(pr->col, "followers");
		pr->value = (char*)malloc(sizeof(char)*(strlen(follow_list_user1) + strlen(_itoa(user2, buff, 10)) + 1));
		strcpy(buff, follow_list_user1);
		buff = strconcat(buff, _itoa(user2, buff, 10));
		buff = strconcat(buff, ",");
		strcpy(pr->value,buff);
		put("follow", user1, pr, 1, global_current_number_follow++);
	}

	add_following_to_user(user2, user1);
	
}


void add_following_to_user(int user1, int user2){
	int user_index = user1;
	char* buff = (char*)malloc(sizeof(char) * 10);
	char* follow_list_user1 = get_value("follow", user1, "following");
	if (strlen(follow_list_user1) == 0){
		 buff = (char*)malloc(sizeof(char) * 50);
		put_request* pr = (put_request*)malloc(sizeof(put_request));
		pr->col = (char*)malloc(sizeof(char)*strlen("following"));
		strcpy(pr->col, "following");
		pr->value = (char*)malloc(sizeof(char)*(strlen(_itoa(user2, buff, 10)) + 1));
		strcpy(buff, _itoa(user2, buff, 10));
		buff = strconcat(buff, ",");
		//pr->value = user2;
		strcpy(pr->value, buff);
		put("follow", user1, pr, 1, global_current_number_follow++);
		free(buff);
	}
	else{
		buff = (char*)malloc(sizeof(char)*(strlen(follow_list_user1) + strlen(_itoa(user2, buff, 10)) + 1));
		put_request* pr = (put_request*)malloc(sizeof(put_request));
		pr->col = (char*)malloc(sizeof(char)*strlen("following"));
		strcpy(pr->col, "following");
		pr->value = (char*)malloc(sizeof(char)*(strlen(follow_list_user1) + strlen(_itoa(user2, buff, 10)) + 1));
		strcpy(buff, follow_list_user1);
		buff = strconcat(buff, _itoa(user2, buff, 10));
		buff = strconcat(buff, ",");
		strcpy(pr->value, buff);
		put("follow", user1, pr, 1, global_current_number_follow++);
	}

}

void show_post_for_user(int key){

	char* post_list = get_value("follow", key, "post");
	char* temp_post_list = (char*)malloc(sizeof(char)*strlen(post_list));
	strcpy(temp_post_list, post_list);

	char** token = strtok(temp_post_list, ",");

	int token_count = sizeof(token) / sizeof(char*);

	for (int i = 0; i < token_count; i++){
		get_response* gt = get("user", atoi(token[i]));
		display_response(gt);
	}

}

void show_follower_list(int key){
	char* follow_list = get_value("follow", key, "followers");
	char* temp_list = (char*)malloc(sizeof(char)*strlen(follow_list));
	strcpy(temp_list, follow_list);

	char** token = strtok(temp_list, ",");

	int token_count = sizeof(token) / sizeof(char*);

	for (int i = 0; i < token_count; i++){
		get_response* gt = get("user", atoi(token[i]));
		display_response(gt);
	}

	/*while (token != NULL && token != "" && strlen(token) != 0){
		get_response* gt = get("user", token);
		display_response(gt);
		token = strtok(NULL, ",");
	}*/
}

void show_following_list(){

}

void get_user_details(int key){
	display_response(get("user", key));
}

void get_post(int id){
	char buff[50];
	get_response* gt = get("post", id);
	display_response(gt);

}





//void save_the_column(FILE* fp, )


void display_all(){
	for (int i = 0; i < s->number_of_tables; i++){
		for (int j = 0; j < s->list_of_tables[i]->row_count; j++){
			if (s->list_of_tables[i]->data[j] != NULL){
				node* row = s->list_of_tables[i]->data[j];
				while (row != NULL){
					node* version = row;
					while (version != NULL){
						printf("%s %s %d\n", version->column_name, version->value, version->version);
						version = version->next_version;
					}
					row = row->next_column;
				}
			}
		}
	}
}


