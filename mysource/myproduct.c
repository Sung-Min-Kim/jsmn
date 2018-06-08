#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../myproduct.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

char *JSON_STRING;

char *readJSONFile(){
	FILE *fp;
	char input[256];
	int len = 0;

	fp = fopen("myproduct.json" , "rt");

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
//strncpy(price, jsonstr + t[nameTokenInfo[i].tok_index+5].start, t[nameTokenInfo[i].tok_index+5].end - t[nameTokenInfo[i].tok_index+5].start);
int makeProduct(const char *jsonstr, jsmntok_t *t, int tokcount, product_t * p[]){
		int i;
		int pcount = 0; // 구조체 개수 세기
		char company[128];
		char name[128];
		char price[128];
		char quantity[128];
		for(i = 0; i < tokcount; i++){
			if(t[i].type == JSMN_STRING && t[i].size == 1 && t[t[t[i].parent].parent].parent == 1 && t[0].type == JSMN_OBJECT && t[i+1].type != JSMN_ARRAY){
				if(jsoneq(JSON_STRING, &t[i], "company") == 0){
					p[pcount] = (product_t *)malloc(sizeof(product_t));
					strncpy(company, jsonstr + t[i+1].start, t[i+1].end - t[i+1].start); // 회사 이름 받기
					company[t[i+1].end - t[i+1].start] = '\0';
					//sprintf(company, "%s", company);
					if(strcmp(company, "롯데") == 0){
						p[pcount]->company = LOTTE;
					}else if(strcmp(company, "조지아") == 0){
						p[pcount]->company = GEORGIA;
					}else if(strcmp(company, "맥심") == 0){
						p[pcount]->company = MAXIM;
					}else if(strcmp(company, "스타벅스") == 0){
						p[pcount]->company = STARBUCKS;
					}
				}else if(jsoneq(JSON_STRING, &t[i], "name") == 0){
					strncpy(name, jsonstr + t[i+1].start, t[i+1].end - t[i+1].start); // 이름 구조체에 넣어주기
					name[t[i+1].end - t[i+1].start] = '\0';
					strcpy(p[pcount]->name, name);
				}else if(jsoneq(JSON_STRING, &t[i], "price") == 0){
					strncpy(price, jsonstr + t[i+1].start, t[i+1].end - t[i+1].start); // 제품 가격
					price[t[i+1].end - t[i+1].start] = '\0';
					p[pcount]->price = atoi(price);
				}else if(jsoneq(JSON_STRING, &t[i], "quantity") == 0){
					strncpy(quantity, jsonstr + t[i+1].start, t[i+1].end - t[i+1].start); // 제품 용량
					quantity[t[i+1].end - t[i+1].start] = '\0';
					p[pcount]->quantity = atoi(quantity);
					pcount++;
				}
			}
		}
		return pcount;
}

void printProduct(product_t * p[], int pcount){
	int i;
	printf("*******************************************************\n");
	printf("번호\t제품명\t\t\t  제조사  가격\t용량  \n");
	printf("*******************************************************\n");
	for(i = 0; i < pcount; i++){
		if(p[i]->company == LOTTE){
			printf("%d\t%s\t  %s\t  %d\t%d\n", i + 1, p[i]->name, "롯데", p[i]->price, p[i]->quantity);
		}else if(p[i]->company == GEORGIA){
			printf("%d\t%s\t  %s  %d\t%d\n", i + 1, p[i]->name, "조지아", p[i]->price, p[i]->quantity);
		}else if(p[i]->company == MAXIM){
			printf("%d\t%s\t\t  %s\t  %d\t%d\n", i + 1, p[i]->name, "맥심", p[i]->price, p[i]->quantity);
		}else if(p[i]->company == STARBUCKS){
			printf("%d\t%s\t\t  %s%d\t%d\n", i + 1, p[i]->name, "스타벅스", p[i]->price, p[i]->quantity);
		}
	}
}

NameTokenInfo *jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo, int *len){
	int count = 0;
	int i;

	nameTokenInfo = (NameTokenInfo *)malloc(sizeof(NameTokenInfo) * 1);
	printf("***** Name List *******\n");
	for (i = 1; i < tokcount; i++){
		if(t[i].type == JSMN_STRING && t[i].size == 1 && t[t[t[i].parent].parent].parent == 1 && t[0].type == JSMN_OBJECT && t[i+1].type != JSMN_ARRAY){
			printf("[NAME %d] %.*s\n", count + 1, t[i].end-t[i].start,
				jsonstr + t[i].start);

			nameTokenInfo = (NameTokenInfo *)realloc(nameTokenInfo, sizeof(NameTokenInfo) * (count + 1));
			nameTokenInfo[count].tok_index = i;
			count++;
		}
	}

	*len = count; // 몇 개의 구조체가 선언되었는지 알기위해
	return nameTokenInfo;
}

int printNameList(char *jsonstr, jsmntok_t *t,  NameTokenInfo *nameTokenInfo, int len){
	printf("***** Name List *******\n");
	int i, count = 0;

	for(i = 0; i < len; i++){
		printf("Tok Index: %d\n", nameTokenInfo[i].tok_index);
	}
	for(i = 0; nameTokenInfo[i].tok_index != 0 && i < len; i++){
		printf("Index: %d, ", nameTokenInfo[i].tok_index);
		printf("[NAME %d] %.*s\n", ++count, t[nameTokenInfo[i].tok_index].end-t[nameTokenInfo[i].tok_index].start,
		 jsonstr + t[nameTokenInfo[i].tok_index].start);
	}
	printf("\n");
	return count;
}

void selectNameList(char *jsonstr, jsmntok_t *t, NameTokenInfo *nameTokenInfo, int len){
	int choice;

	while(1){
		printf("Select Name's no (exit:0) >> ");
		scanf("%d", &choice);
		if(choice == 0)
			break;
		else if(nameTokenInfo[choice - 1].tok_index != 0 && choice <= len && choice > 0){
			printf("[NAME %d] %.*s\n", choice, t[nameTokenInfo[choice - 1].tok_index].end-t[nameTokenInfo[choice - 1].tok_index].start,
				jsonstr + t[nameTokenInfo[choice - 1].tok_index].start);
			printf("%.*s\n", t[nameTokenInfo[choice - 1].tok_index + 1].end-t[nameTokenInfo[choice - 1].tok_index + 1].start,
				jsonstr + t[nameTokenInfo[choice - 1].tok_index + 1].start);
		}else
			printf("Invalid Input!\n");

		printf("\n");
	}
}

NameTokenInfo *printObjectList(char *jsonstr, jsmntok_t *t, NameTokenInfo *nameTokenInfo, int len, int numTokens, int *objectNum){
	printf("***** Object List *******\n");
	int i, count = 0;

	for(i = 0; nameTokenInfo[i].tok_index != 0 && i < len; i++){ // nameTokIndex는 이름부분이 되는 부분의 index를 저장하는 배열이다.
		if((jsoneq(jsonstr, &t[nameTokenInfo[i].tok_index], "company") == 0)){
			printf("[NAME %d] %.*s\n", ++count, t[nameTokenInfo[i].tok_index+1].end-t[nameTokenInfo[i].tok_index+1].start,
				jsonstr + t[nameTokenInfo[i].tok_index+1].start);

			nameTokenInfo[i].object_index = nameTokenInfo[i].tok_index;
		}else
			nameTokenInfo[i].object_index = 0;
	}

	int count_object = 0;
	for(i = 0; i < numTokens; i++){
		if(nameTokenInfo[i].object_index != 0){
			nameTokenInfo[i+1].object_index = count_object+1;
			nameTokenInfo[i+2].object_index = count_object+1;
			nameTokenInfo[i+3].object_index = count_object+1;
			nameTokenInfo[i].object_index = count_object+1;
			i+=3;
			count_object++;
		}
	}

	*(objectNum) = count_object;
	for(i = 0; i < len; i++){
		printf("object index: %d\n", nameTokenInfo[i].object_index);
	}
	printf("Count: %d\n", count);
	return nameTokenInfo;
}

int printTokIndex(char *jsonstr, jsmntok_t *t, NameTokenInfo *nameTokenInfo, int len){
	int objectNo = 0;
	char name[20];

	printf("Object의 No를 입력하세요: ");
	scanf("%d", &objectNo);
	printf("Object의 Name을 입력하세요: ");
	scanf("%s", name);

	int i;
	int tokindex = 0;
	bool found = false;
	for(i = 0; i < len; i++){
		if(nameTokenInfo[i].object_index == objectNo){
			if(jsoneq(jsonstr, &t[nameTokenInfo[i].tok_index], name) == 0){
				tokindex = nameTokenInfo[i].tok_index;
				found = true;
				break;
			}
		}
	}
	if(found)
		printf("찾는 name의 tokindex는 %d 입니다.\n", tokindex);
	else
		printf("그런게 없는데요? ㅎㅎ\n");
	return tokindex;
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

void print_object_chart(char *jsonstr, jsmntok_t *t, NameTokenInfo *nameTokenInfo, int len){
	printf("**************************************************\n");
	printf("번호\t제품명  제조사  가격\t개수  \n");
	printf("**************************************************\n");
	int i;
	int count = 0;
	for(i = 0; i < len; i+=4, count++){
		int total_price = 0;
		char price[20];
		char amount[20];
		//strncpy(price, jsonstr + t[nameTokenInfo[i].tok_index+5].start, t[nameTokenInfo[i].tok_index+5].end - t[nameTokenInfo[i].tok_index+5].start);
		//printf("price: %s\n", price);
		//strncpy(amount, jsonstr + t[nameTokenInfo[i].tok_index+7].start, t[nameTokenInfo[i].tok_index+7].end - t[nameTokenInfo[i].tok_index+7].start);
		//printf("price: %s\n", amount);
		//total_price = atoi(price) * atoi(amount);
		//printf("Total Price = %d\n", total_price);
		printf("%d\t%.*s  %.*s\t%.*s\t%.*s\n", count+1, t[nameTokenInfo[i].tok_index+3].end-t[nameTokenInfo[i].tok_index+3].start,
				JSON_STRING + t[nameTokenInfo[i].tok_index+3].start, t[nameTokenInfo[i].tok_index+1].end-t[nameTokenInfo[i].tok_index+1].start,
						JSON_STRING + t[nameTokenInfo[i].tok_index+1].start, t[nameTokenInfo[i].tok_index+5].end-t[nameTokenInfo[i].tok_index+5].start,
								JSON_STRING + t[nameTokenInfo[i].tok_index+5].start, t[nameTokenInfo[i].tok_index+7].end-t[nameTokenInfo[i].tok_index+7].start,
										JSON_STRING + t[nameTokenInfo[i].tok_index+7].start);
	}
}

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	JSON_STRING = readJSONFile();


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
	int nameNum = 0;
	NameTokenInfo *nameTokenInfo;
	int len = 0;
	int tokindex = 0;
	int objectNum = 0;
	int pcount = 0;

	product_t *coffee_list[20];

	printf("Number of Tokens: %d\n", r);
	pcount =  makeProduct(JSON_STRING, t, r, coffee_list);
	printf("제품의 개수는: %d\n", pcount);
	printProduct(coffee_list, pcount);
	return EXIT_SUCCESS;
	nameTokenInfo = jsonNameList(JSON_STRING, t, r, nameTokenInfo, &len);
	printf("구조체 갯수: %d\n", len);
	//nameNum = printNameList(JSON_STRING, t, nameTokenInfo, len);
	selectNameList(JSON_STRING, t, nameTokenInfo, len);
	nameTokenInfo = printObjectList(JSON_STRING, t, nameTokenInfo, len, r, &objectNum);
	tokindex = printTokIndex(JSON_STRING, t, nameTokenInfo, len);
	print_object_chart(JSON_STRING, t, nameTokenInfo, len);
	return EXIT_SUCCESS;
	selectObjectList(JSON_STRING, t, nameTokIndex, objectTokIndex, objectNum, nameNum);
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
