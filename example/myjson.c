#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
<<<<<<< HEAD
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";
=======
	"{\"User\": \"johndoe\", \"Admin\": false, \"UID\": 1000, \"User\": jerry1004, \"Admin\": true, \"UID\": 2000\n  "
	"\"Groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";
>>>>>>> 8ad699a03c5b27ecf8edbf1765a1f36cce56c842

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
<<<<<<< HEAD
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0])); // r은 토큰개수
#ifdef DEBUG_MODE
	printf("Token's count: %d\n", r); // 토큰의 개수
#endif

=======
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
>>>>>>> 8ad699a03c5b27ecf8edbf1765a1f36cce56c842
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			/* We may use strndup() to fetch string value */
<<<<<<< HEAD
			#ifdef DEBUG_MODE
				printf("%d %d\n", t[i+1].start, t[i+1].end); // data의 시작위치와 끝나는 위치.
				printf("[%d] ", i + 1); // data의 위치
			#endif
=======
>>>>>>> 8ad699a03c5b27ecf8edbf1765a1f36cce56c842
			printf("- User: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
<<<<<<< HEAD
			#ifdef DEBUG_MODE
				printf("%d %d\n", t[i+1].start, t[i+1].end); // data의 시작위치와 끝나는 위치.
				printf("[%d] ", i + 1); // data의 위치
			#endif
=======
>>>>>>> 8ad699a03c5b27ecf8edbf1765a1f36cce56c842
			printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
<<<<<<< HEAD
			#ifdef DEBUG_MODE
				printf("%d %d\n", t[i+1].start, t[i+1].end); // data의 시작위치와 끝나는 위치.
				printf("[%d] ", i + 1); // data의 위치
			#endif
=======
>>>>>>> 8ad699a03c5b27ecf8edbf1765a1f36cce56c842
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
<<<<<<< HEAD
			#ifdef DEBUG_MODE
				printf("%d %d\n", t[i+1].start, t[i+1].end); // data의 시작위치와 끝나는 위치.
			#endif

=======
>>>>>>> 8ad699a03c5b27ecf8edbf1765a1f36cce56c842
			printf("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
<<<<<<< HEAD
				#ifdef DEBUG_MODE
					printf("[%d]   ", i+j+2); // data의 위치
				#endif
=======
>>>>>>> 8ad699a03c5b27ecf8edbf1765a1f36cce56c842
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	return EXIT_SUCCESS;
}
