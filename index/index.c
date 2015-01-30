#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void position(int word, FILE *fp, char freq) {
	fseek(fp, 0, SEEK_SET);

	int i = 0;
	int temp;

	while(i < word) {
		fread(&temp, sizeof(int), 1, fp);
		fseek(fp, temp, SEEK_CUR);

		if(freq == 1) {
			fseek(fp, sizeof(int), SEEK_CUR);
		}

		++ i;
	}

	
}

void strlower(char *s) {
	while(*s != '\0') {
		*s = tolower(*s);
		++ s;
	}
}

int main(int argc, char *argv[]) {

	if(argc < 5) {
		fprintf(stderr, "Usage: %s filename max min freq\n", argv[0]);
		return -1;
	}

	char buf[64];
	int max, min, freq;
	max = atoi(argv[2]);
	min = atoi(argv[3]);
	freq= atoi(argv[4]);
	
	if(strrchr(argv[1], '.') == NULL) {
		strcpy(buf, argv[1]);
		strcat(buf, ".idx");
	} else {
		strcpy(buf, argv[1]);
		strcpy(strrchr(buf, '.'), ".idx");
	}

	FILE *in = fopen(argv[1], "rt"),
		 *f1 = fopen("f1.bin", "w+b"),
		 *f2 = fopen("f2.bin", "w+b"),
		 *out= fopen(buf, "w+b");

	if(in == NULL || f1 == NULL || f2 == NULL || out == NULL) {
		if(in) fclose(in);
		if(f1) fclose(f1);
		if(f2) fclose(f2);
		if(out) fclose(out);

		return -2;
	}

	char *line = NULL;
	size_t sz = 0;
	char delim[] = "\(){}[]# .,:;-\n/?!";
	unsigned long words = 0L;
	fseek(f1, 0, SEEK_CUR);

	while(getline(&line, &sz, in) != -1) {
		char *p = strtok(line, delim);

		while(p != NULL) {
			strlower(p);
			int s = strlen(p);
			if(min <= s && s <= max) {
				++ s;
				fwrite(&s, sizeof(int), 1, f1);	
				fwrite(p, sizeof(char), s, f1);	
				++ words;
			}
			
			p = strtok(NULL, delim);
		}

		free(line);
		line = NULL;
		sz = 0;
	}
	free(line);
	
	char cuv_a[25], cuv_b[25];
	memset(cuv_a, '\0', 25);
	memset(cuv_b, '\0', 25);

	int a_size, b_size, a_freq;	
	unsigned long i, j;
	char flag = 0;
	unsigned long unique = 0;

	for(i = 0L; i < words - 1; ++ i) {
		position(i, f1, 0);

		fread(&a_size, sizeof(int), 1, f1);
		fread(cuv_a, sizeof(char), a_size, f1);

		flag = 0;

		for(j = 0L; j < unique; ++ j) {
			position(j, f2, 1);

			fread(&b_size, sizeof(int), 1, f2);
			fread(cuv_b, sizeof(char), b_size, f2);

			if(strcmp(cuv_a, cuv_b) == 0) {
				fread(&a_freq, sizeof(int), 1, f2);
				++ a_freq;
				fseek(f2, -sizeof(int), SEEK_CUR);
				fwrite(&a_freq, sizeof(int), 1, f2);
				flag = 1;
				break;
			}
		}

		if(flag == 0) {
			a_freq = 1;
			position(unique, f2, 1);			
			fwrite(&a_size, sizeof(int), 1, f2);
			fwrite(cuv_a, sizeof(char), a_size, f2);
			fwrite(&a_freq, sizeof(int), 1, f2);
			++ unique;
		}

		memset(cuv_a, '\0', 25);
		memset(cuv_b, '\0', 25);

	}

	fseek(f2, 0, SEEK_SET);
	for(i = 0; i < unique; ++ i) {
		fread(&a_size, sizeof(int), 1, f2);
		fread(cuv_a, sizeof(char), a_size, f2);
		fread(&a_freq, sizeof(int), 1, f2);

		if(a_freq >= freq) {
			fwrite(&a_size, sizeof(int), 1, out);
			fwrite(cuv_a, sizeof(char), a_size, out);
			fwrite(&a_freq, sizeof(int), 1, out);
		}

	}
	
	fclose(f1);
	fclose(f2);
	fclose(out);


	return 0;
}
