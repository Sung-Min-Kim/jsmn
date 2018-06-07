#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

char *JSON_STRING;

char *readJSONFile(int *file_check){
	FILE *fp;
	char input[256];
	int len = 0;

	char file_name[256];
	printf("원하는 파일의 이름을 입력하시오: ");
	scanf("%s", file_name);

	if(strcmp(file_name, "data4.json") == 0)
		*file_check = 4; // 데이터가 data4.json인 경우와 아닌 경우를 나눈다.
	else if(strcmp(file_name, "data3.json") == 0)
		*file_check = 3;
	else if(strcmp(file_name, "data2.json") == 0)
		*file_check = 2;
	else if(strcmp(file_name, "data.json") == 0)
		*file_check = 1;
	else
		*file_check = 0;

	printf("file_check: %d\n", *file_check);

	fp = fopen(file_name , "rt");

	if(fp == NULL){
		printf("Cannot read the file!\n");
		return 1;
	}else{
		printf("The file has been read succesfully.\n");
	}

	JSON_STRING = (char *)malloc(sizeof(char) * 256);
	while(1){
		fgets(input , sizeof(input) , fp);
		if( feof(fp) ) {
         break ;
      }
		len += strlen(input);
		realloc(JSON_STRING, len + 1);
		strcat(JSON_STRING, input);
		//malloc, ralloc
	}

	//printf("%s\n", JSON_STRING);

	fclose(fp);
	return JSON_STRING;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

static int jsoneq2(const char *json, jsmntok_t *tok, jsmntok_t *tok2) {
	char s[512];

	strncpy(s, json + tok2->start, tok2->end - tok2->start);
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int *printObjectList(char *jsonstr, jsmntok_t *t, int *nameTokIndex, int *objectTokIndex, int *len){
	printf("***** Object List *******\n");
	int i, count = 0;

	objectTokIndex = (int *)malloc(sizeof(int) * (*len));

	for(i = 0; nameTokIndex[i] != 0 && i < 100; i++){ // nameTokIndex는 이름부분이 되는 부분의 index를 저장하는 배열이다.
		if((jsoneq2(jsonstr, &t[nameTokIndex[i]], &t[nameTokIndex[0]]) == 0)){
			printf("[NAME %d] %.*s\n", ++count, t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start,
				jsonstr + t[nameTokIndex[i]+1].start);

			(*len)++;
			objectTokIndex = (int *)realloc(objectTokIndex, sizeof(int) * (*len));
			objectTokIndex[count - 1] = nameTokIndex[i];
		}

		// int index = nameTokIndex[i];
		// if (t[t[index].parent].type == JSMN_OBJECT){
		// 	printf("[NAME %d] %.*s\n", ++count, t[index].end-t[index].start,
		// 		jsonstr + t[index].start);
		//
		// 	(*len)++;
		// 	objectTokIndex = (int *)realloc(objectTokIndex, sizeof(int) * (*len));
		// 	objectTokIndex[count - 1] = index;
		// }
	}
	(*len) = count;

	// for(i = 0; i < count; i++){
	// 	printf("object index: %d\n", objectTokIndex[i]);
	// }
	// printf("Count: %d\n", count);
	return objectTokIndex;
}

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex, int file_check){
	int count = 0;
	int i;

	printf("***** Name List *******\n");
	for (i = 1; i < tokcount; i++){
		switch(file_check){
					case 1: case 2: case 3:
						if(t[i].type == JSMN_STRING && t[i].size == 1 && t[i].parent == 0 && t[0].type == JSMN_OBJECT){ // data파일이 JSMN_OBJECT로 시작할 때, name들의 부모가 0이라고 가정.
							printf("[NAME %d] %.*s\n", count + 1, t[i].end-t[i].start,
								jsonstr + t[i].start);
							nameTokIndex[count] = i;
							count++;
						}else	if(t[i].type == JSMN_STRING && t[i].size == 1 && t[t[i].parent].type == JSMN_OBJECT && t[0].type == JSMN_ARRAY){// data파일이 JSMN_ARRAY로 시작할 때, name들의 부모의 타입이 JSMN_OBJECT
							printf("[NAME %d] %.*s\n", count + 1, t[i].end-t[i].start,
								jsonstr + t[i].start);
							nameTokIndex[count] = i;
							count++;
						}
						break;

					case 4:
						if(t[i].type == JSMN_STRING && t[i].size == 1 && t[0].type == JSMN_OBJECT && t[i+1].type != JSMN_ARRAY){
							printf("[NAME %d] %.*s\n", count + 1, t[i].end-t[i].start,
								jsonstr + t[i].start);
							nameTokIndex[count] = i;
							count++;
						}
						break;

					default:
						break;
				}
		//((t[i].parent == 0) || (t[i - 1].parent == 0))
		// if(t[i].type == JSMN_STRING && t[i].size == 1 && t[i].parent == 0 && t[0].type == JSMN_OBJECT){ // data파일이 JSMN_OBJECT로 시작할 때, name들의 부모가 0이라고 가정.
		// 	printf("[NAME %d] %.*s\n", count + 1, t[i].end-t[i].start,
		// 		jsonstr + t[i].start);
		// 	nameTokIndex[count] = i;
		// 	count++;
		// }else	if(t[i].type == JSMN_STRING && t[i].size == 1 && t[t[i].parent].type == JSMN_OBJECT && t[0].type == JSMN_ARRAY){// data파일이 JSMN_ARRAY로 시작할 때, name들의 부모의 타입이 JSMN_OBJECT
		// 	printf("[NAME %d] %.*s\n", count + 1, t[i].end-t[i].start,
		// 		jsonstr + t[i].start);
		// 	nameTokIndex[count] = i;
		// 	count++;
		// }
		// else if(t[i].type == JSMN_STRING && t[i].size == 1 && t[0].type == JSMN_OBJECT && t[i+1].type != JSMN_ARRAY){
		// 	printf("[NAME %d] %.*s\n", count + 1, t[i].end-t[i].start,
		// 		jsonstr + t[i].start);
		// 	nameTokIndex[count] = i;
		// 	count++;
		// }
	}
}

int printNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
	printf("***** Name List *******\n");
	int i, count = 0;
	for(i = 0; nameTokIndex[i] != 0 && i < 100; i++){
		printf("[NAME %d] %.*s\n", ++count, t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,
			jsonstr + t[nameTokIndex[i]].start);
	}
	printf("\n");
	return count;
}

void selectNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
	int choice;

	while(1){
		printf("Select Name's no (exit:0) >> ");
		scanf("%d", &choice);
		if(choice == 0)
			break;
		else if(nameTokIndex[choice - 1] != 0 && choice < 100 && choice > 0){
			printf("[NAME %d] %.*s\n", choice, t[nameTokIndex[choice - 1]].end-t[nameTokIndex[choice - 1]].start,
				jsonstr + t[nameTokIndex[choice - 1]].start);
			printf("%.*s\n", t[nameTokIndex[choice - 1] + 1].end-t[nameTokIndex[choice - 1] + 1].start,
				jsonstr + t[nameTokIndex[choice - 1] + 1].start);
		}else
			printf("Invalid Input!\n");

		printf("\n");
	}
}

void selectObjectList(char *jsonstr, jsmntok_t *t, int *nameTokIndex, int *objectTokIndex, int len, int nameNum){
	int choice;

	int i;

	while(1){
		printf("원하는 번호 입력 (Exit:0) >> ");
		scanf("%d", &choice);
		int index_start;
		int index_end;

		for(i = 0; nameTokIndex[i] != 0 && i < 100; i++){
			if(choice < len){
				if(nameTokIndex[i] == objectTokIndex[choice - 1])
					index_start = i;

				if(nameTokIndex[i] == objectTokIndex[choice])
					index_end = i;
			}else if(choice == len){
				if(nameTokIndex[i] == objectTokIndex[choice - 1])
					index_start = i;

					index_end = nameNum;
			}
		} // 선택한 객체의 시작과 끝을 나타내는 name의 인덱스를 구함.

		if(choice == 0)
			break;
		else if(choice <= len){
			printf("%.*s : %.*s\n", t[objectTokIndex[choice - 1]].end-t[objectTokIndex[choice - 1]].start,
				jsonstr + t[objectTokIndex[choice - 1]].start, t[objectTokIndex[choice - 1] + 1].end-t[objectTokIndex[choice - 1] + 1].start,
					jsonstr + t[objectTokIndex[choice - 1] + 1].start);

			int number = index_end - index_start;
			for(i = 0; i < number - 1; i++, index_start++){
				printf("\t[%.*s]  %.*s\n", t[nameTokIndex[index_start + 1]].end-t[nameTokIndex[index_start + 1]].start, jsonstr + t[nameTokIndex[index_start + 1]].start,
					t[nameTokIndex[index_start + 1] + 1].end-t[nameTokIndex[index_start + 1] + 1].start, jsonstr + t[nameTokIndex[index_start + 1] + 1].start);
			}
		}else
			printf("Invalid Input!\n");

		printf("\n");
	}

}

void print_object_chart(char *jsonstr, jsmntok_t *t, int *objectTokIndex, int objectNum){
	printf("**************************************************\n");
	printf("번호\t제품명  제조사  가격\t개수  \n");
	printf("**************************************************\n");
	int i;
	for(i = 0; i < objectNum; i++){
		printf("%d\t%.*s  %.*s\t%.*s\t%.*s\n", i+1, t[objectTokIndex[i]+3].end-t[objectTokIndex[i]+3].start,
				JSON_STRING + t[objectTokIndex[i]+3].start, t[objectTokIndex[i]+1].end-t[objectTokIndex[i]+1].start,
						JSON_STRING + t[objectTokIndex[i]+1].start, t[objectTokIndex[i]+5].end-t[objectTokIndex[i]+5].start,
								JSON_STRING + t[objectTokIndex[i]+5].start, t[objectTokIndex[i]+7].end-t[objectTokIndex[i]+7].start,
										JSON_STRING + t[objectTokIndex[i]+7].start);
	}
}

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	int file_check = 0;
	JSON_STRING = readJSONFile(&file_check);


	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || (t[0].type != JSMN_OBJECT && t[0].type != JSMN_ARRAY)) {
		printf("Object or Array expected\n");
		return 1;
	}
	/* Loop over all keys of the root object */

	int nameTokIndex[100] = {0}; // 인덱스 값이 다 0 아니니까 0이면 break
	int *objectTokIndex; // object의 인덱스를 담는 배열을 가르킬 포인터
	int objectNum = 5;
	int nameNum = 0;

	printf("Number of Tokens: %d\n", r);
	jsonNameList(JSON_STRING, t, r, nameTokIndex, file_check);
	nameNum = printNameList(JSON_STRING, t, nameTokIndex);
	selectNameList(JSON_STRING, t, nameTokIndex);

	switch(file_check){
		case 1:
			objectTokIndex = printObjectList(JSON_STRING, t, nameTokIndex, objectTokIndex, &objectNum);
			selectObjectList(JSON_STRING, t, nameTokIndex, objectTokIndex, objectNum, nameNum);
			break;
		case 3: case 4:
			objectTokIndex = printObjectList(JSON_STRING, t, nameTokIndex, objectTokIndex, &objectNum);
			selectObjectList(JSON_STRING, t, nameTokIndex, objectTokIndex, objectNum, nameNum);
			print_object_chart(JSON_STRING, t, objectTokIndex, objectNum);
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;


	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- name: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "keywords") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- keywords: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "description") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "examples") == 0) {
			int j;
			printf("- examples:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		}	else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			printf("  \"repository\"\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			printf("  {\n");
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			printf("  {\n");
			i += t[i+1].size + 1;
		} else {
			//printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					//JSON_STRING + t[i].start);
		}
	}
	return EXIT_SUCCESS;
}
