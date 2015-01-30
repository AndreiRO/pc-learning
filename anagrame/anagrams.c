#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define ERROR_PARAM -1
#define ERROR_IO -2
#define ERROR_MEM -2
#define DEFAULT_WORDS 50
#define DEFAULT_WORDS_SIZE 25

#define max(a, b) ((a > b) ? a : b)

#define FREE_ALL(m) do {															\
						if(m)														\
							fprintf(stderr, "Not enough memory...%d\n", __LINE__);  \
						if(fp != NULL) fclose(fp);									\
						if(line != NULL)free(line);									\
						int i;														\
						if(words != NULL) {											\
																					\
							for(i = 0; i < maximum_size; ++ i) {					\
								if(words[0] != NULL && words[0][i] != NULL) {		\
									free(words[0][i]);								\
								}													\
								if(words[1] != NULL && words[1][i] != NULL) {		\
									free(words[1][i]);								\
								}													\
							}														\
							if(words[0] != NULL) free(words[0]);					\
							if(words[1] != NULL) free(words[1]);					\
							free(words);											\
						}															\
																					\
						if(m)														\
							return ERROR_MEM;										\
					} while(0) 


void strlower(char *s) {
	while(*s != '\0') {
		*s = tolower(*s); 

		++ s;
	}
}

int main(int argc, char *argv[]) {

	if(argc < 2) {
		fprintf(stderr, "Error. Usage: %s filename\n", argv[0]);
		return ERROR_PARAM;
	}

	FILE *fp;
	if(!(fp = fopen(argv[1], "rt"))) {
		fprintf(stderr, "Could not open: %s. Make sure it exists\n", argv[1]);
		return ERROR_IO;
	}

	char *line = NULL;
	size_t size = 0;

	int read = getline(&line, &size, fp);
	if(read == -1) {
		fprintf(stderr, "Error whil reading from: %s...\n", argv[1]);
		fclose(fp);
		if(line) {
			free(line);
		}

		return ERROR_IO;
	}
	
	char delim[] ="-\n \t,.;!?";
	int current_size, maximum_size;
	char ***words = (char***)calloc(2, sizeof(char**));
	
	if(words == NULL) {
		fprintf(stderr, "Not enough memory...\n");
		free(line);
		fclose(fp);
		return ERROR_MEM;
	}

	current_size = 0;
	maximum_size = DEFAULT_WORDS;

	words[0] = (char**)calloc(DEFAULT_WORDS, sizeof(char*));
	words[1] = (char**)calloc(DEFAULT_WORDS, sizeof(char*));

	if(words[0] == NULL || words[1] == NULL) {
		fprintf(stderr, "Not enough memory...%d\n", __LINE__); 
		fclose(fp);
		free(line);
		if(words[0] != NULL) free(words[0]);
		if(words[1] != NULL) free(words[1]);
		free(words);

		return ERROR_MEM;
	}

	int t;
	for(t = 0; t < maximum_size; ++ t) {
		words[0][t] = NULL;
		words[1][t] = NULL;
	}
	char *temp_word = strtok(line, delim);

	while(temp_word != NULL) {
		if(current_size == maximum_size) {
			char **temp = (char**)realloc(words[0], 2 * maximum_size * sizeof(char*));
			if(temp == NULL) {
				FREE_ALL(1);	
			}

			words[0] = temp;
			temp = (char**)realloc(words[1], 2 * maximum_size * sizeof(char*));
			if(temp == NULL) {
				FREE_ALL(1);	
			}

			words[1] = temp;
			maximum_size *= 2;
		}

		words[0][current_size] = (char*)calloc(
										(strlen(temp_word) + 1), 
										sizeof(char)
									);
		words[1][current_size] = (char*)calloc(
										(strlen(temp_word) + 1), 
										sizeof(char)
									);

		if(words[0][current_size] == NULL || words[1][current_size] == NULL) {
			FREE_ALL(1);
		}

		strlower(temp_word);
		strcpy(words[0][current_size ++], temp_word);
		temp_word = strtok(NULL, delim);
	}
	
	int i, j, min;
	for(i = 0; i < current_size - 1; ++ i) {
		min = i;
		for(j = i + 1; j < current_size; ++ j) {
			if(strcmp(words[0][min], words[0][j]) > 0) {
				min = j;
			}
		}

		if(min != i) {
			int sz_i = strlen(words[0][i]) + 1,
				sz_min = strlen(words[0][min]) + 1;

			char *aux_i = (char*)calloc(sz_i, sizeof(char));
			char *aux_min = (char*)calloc(sz_min, sizeof(char));

			if(aux_i == NULL || aux_min == NULL) {
				if(aux_i) free(aux_i);
				if(aux_min) free(aux_min);
				FREE_ALL(1);
			}

			strcpy(aux_i, words[0][i]);
			strcpy(aux_min, words[0][min]);

			free(words[0][i]);
			free(words[1][i]);
			free(words[0][min]);
			free(words[1][min]);

			words[0][i] = aux_min;
			words[0][min] = aux_i;

			words[1][i] = (char*)calloc(sz_min, sizeof(char));
			words[1][min] = (char*)calloc(sz_i, sizeof(char));
			
			if(words[1][i] == NULL || words[1][min] == NULL) {
				FREE_ALL(1);
			}
		}
	}
	int temp_size = current_size;
	t = i = 0;
	while(i < temp_size - 1) {
		j = i + 1;
		while(j < temp_size && !strcmp(words[0][i], words[0][j])) {
			++ t;
			++ j;
		}

		if(t) {
			j = i;
			while(j < temp_size - t) {
				free(words[0][j]);
				free(words[1][j]);
				words[0][j] = (char*)calloc(
										(strlen(words[0][j + t]) + 1), 
										sizeof(char)
									);
				words[1][j] = (char*)calloc(
										(strlen(words[0][j + t]) + 1), 
										sizeof(char)
									);
				if(words[0][j] == NULL || words[1][j] == NULL) {
					if(words[1][j]) free(words[1][j]);
					FREE_ALL(1);
				}
				strcpy(words[0][j], words[0][j + t]);
				++ j;
			}
		}
		

		temp_size -= t;
		t = 0;
		++ i;
	}

	for(i = 0;i < temp_size; ++ i) {
		strcpy(words[1][i], words[0][i]);
		int sz = strlen(words[1][i]);

		for(j = 0; j < sz - 1; ++ j) {
			min = j;
			for(t = j + 1; t < sz; ++ t) {
				if(words[1][i][t] < words[1][i][min]) {
					min = t;
				}
			}
			if(min != j) {
				char aux = words[1][i][min];
				words[1][i][min] = words[1][i][j];
				words[1][i][j] = aux;
			}

		}

		

	}

	for(i = 0; i < temp_size; ++ i) {
		if(strcmp(words[0][i], "") != 0) {
			printf("%s", words[0][i]);
			for(j = i + 1; j < temp_size; ++ j) {
				if(!strcmp(words[1][i], words[1][j])) {
					printf("-%s", words[0][j]);
					strcpy(words[0][j], "");
				}
			}
			printf("\n");
		}
		
	}	
	
	FREE_ALL(0);

	return 0;
}

