#include<stdio.h>
#include<string.h>


int main(int argc, char *argv[]) {
	char filenames[80][50];
	if(argc < 2) {
		fprintf(stderr, "Usage: %s catalog\n", argv[0]);
		return -1;
	}

	FILE *fp = fopen(argv[1], "rt");
	char o_filename[50];
	strcpy(o_filename, argv[1]);

	FILE *out, *temp;
	if(strrchr(o_filename, '.') == NULL) {
		strcat(o_filename, ".cpl");
	} else {
		strcpy(strrchr(o_filename, '.'), ".cpl");
	}

	if(fp == NULL) {
		return -1;
	}

	out = fopen(o_filename, "wt");
	if(out == NULL) {
		fclose(fp);
		return -1;
	}

	int i = 0;
	while(1) {
		/* get filename
 		*  get from
 		*  get how much
 		*  update read filenamse
 		**/
		char buffer[50];
		int rez = fscanf(fp, "%s", buffer);
		if(rez == 0 || rez == EOF) {
			break;
		}

		int from;
		int to;
		fscanf(fp, "%d %d", &from, &to);
		temp = fopen(buffer, "rt");

		if(temp == NULL) {
			fprintf(stderr, "File: %s not fountd!...\n", buffer);
			continue;
		}

		int l = 1;
		while(l != from) {
			int c = getc(temp);

			if(c == EOF) {
				fprintf(stderr, "Not enough lines in: %s!...\n", buffer);
				goto next;

			} else if(c == '\n') {
				++ l;
			}
		}
		while(l != from + to) {
			int c = getc(temp);

			if(c == EOF) {
				fprintf(stderr, "Not enough lines in: %s!...\n", buffer);
				goto next;

			} else if(c == '\n') {
				++ l;
			}

			int error = fputc(c, out);
			if(error == EOF) {
				goto next;
			} 
		}
	

	next:
		fclose(temp);		
	}	

	fclose(fp);

	return 0;
}
