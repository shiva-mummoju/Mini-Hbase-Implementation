#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include "myString.h"
#include "data_store.h"
#include "server_api.h"

int main(){
	int k = 0;
	char buff[30];
	kickstart();
	get_response* gt;
	/*create_user("shiva", "shiva.sairam97@gmail.com", "shiva", "900", "23", "link");
	create_user("somesh", "somesh.sairam97@gmail.com", "somesh", "900", "23", "link");
	create_user("somesh", "somesh.sairafwefm97@gmail.com", "somwefweesh", "90wefwe0", "wef23", "lwefink");
	create_user("abhinav", "abhinav.sairam97@gmail.com", "abhinav", "900", "23", "link");
	create_user("abhinav", "abhinav.97@gmail.com", "abhin", "300", "232", "ligernk");
	create_user("abhinav", "abhi5enav.97@gmail.com", "abhi34n", "30430", "23442", "ligcernk");*/
	//put_one("post", 1, "profile", "newprofile", 5);
	//put_one("follow", 2, "followers", "hello", 34);
	//put_one("testing", 3, "followers", "hello", 234);
//	printf("List of columns of follow\n");
//	print_list_cols("user");
	/*for (int i = 0; i < 80; i++){
		create_user("abhinav", "abhi5enav.97@gmail.com", "abhi34n", "30430", "23442", "ligcernk");
	}*/
	 
	dump_to_file();

	load_all();

	put_one("mynewtable", 1, "newcol", "newval", 1);
	display_response(get("mynewtable", 1));
	
//	printf("Printing values of shiva ************************\n");
	//display_response(get("follow", "shiva"));

	display_all();
	//printf("\ndumping done\n");
///	printf("\n");

	scanf("%d", k);

	return 0;
}