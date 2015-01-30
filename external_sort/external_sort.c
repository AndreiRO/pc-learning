#include<stdio.h>
#include<stdlib.h>

#define BATCH 30

int main(int argc, char *argv[]) {

	FILE *in, *out;
	if(argc < 3) {
		if(argc == 1) {
			in = stdin;
			out = stdout;
		} else {
			fprintf(stderr, "Either supply both arguments"\
							"(input output), or none\n");
			return -1;
		}
	} else {
		in = fopen(argv[1], "rb");
		out = fopen(argv[2], "wb");
		if(in == NULL || out == NULL) {
			fprintf(stderr, "Error while opening files...\n");
			return -2;
		}
	}

	int rez, found;
	long k = 0;
	long temp[BATCH];

	while(1) {
		found = fread(temp, sizeof(long), BATCH, in);	

		char filename[64];
		sprintf(filename, "temp%ld.bin", k ++);
		FILE *fp = fopen(filename, "wb");

		if(!fp) {
			return fprintf(stderr, "Could not create file:"\
								   " %s. Aborting.\n", filename);
				
			if(in != stdin) {
				fclose(in);
			}
			if(out != stdout) {
				fclose(out);
			}

			return -3;
		}

		int i, j, min;
		for(i = 0; i < found - 1; ++ i) {
			min = i;
			for(j = i + 1; j < found; ++ j) {
				if(temp[j] < temp[min]) {
					min = j;
				}
			}

			if(min != i) {
				long aux = temp[i];
				temp[i] = temp[min];
				temp[min] = aux;
			}
		}

		rez = fwrite(temp, sizeof(long), found, fp);

		if(rez < found) {
			fprintf(stderr, "Could not create file:"\
								   " %s. Aborting.\n", filename);
				
			if(in != stdin) {
				fclose(in);
			}
			if(out != stdout) {
				fclose(out);
			}
			if(fp != NULL) {
				fclose(fp);
			}

			return -4;
		}
		
		fclose(fp);
		if(found < BATCH) {
			break;
		}
	}

	FILE **fps;
	fps = (FILE**)malloc(k * sizeof(FILE*));
	long it;
	long min_index;

	for(it = 0; it < k; ++ it) {
		char buf[64];
		sprintf(buf, "temp%ld.bin", it);

		if((fps[it] = fopen(buf, "rb")) == NULL) {
			goto end;
		} 
	}

	while(1) {
		it = 0;
		while(it < k) {
			long check;
			rez = fread(&check, sizeof(long), 1, fps[it]);
			if(rez == 1) {
				fseek(fps[it], -sizeof(long), SEEK_CUR);
				break;
			}
			++ it;
		}

		if(it == k) {
			goto end;
		}

		min_index = it;
		long temp_var, min_var;
		for(; it < k; ++ it) {
			rez = fread(&temp_var, sizeof(long), 1, fps[it]);

			if(rez < 1) {
				if(min_index == it) {
					++ min_index;
				}
				continue;
			}

			if(it == min_index) {
				fwrite(&temp_var, sizeof(long), 1, out);
			} else {
				fseek(fps[min_index], -sizeof(long), SEEK_CUR);
				fread(&min_var, sizeof(long), 1, fps[min_index]);

				if(temp_var < min_var) {
					fseek(out, -sizeof(long), SEEK_CUR);
					fwrite(&temp_var, sizeof(long), 1, out);
					fseek(fps[min_index], -sizeof(long), SEEK_CUR);
					min_index = it;
					min_var = temp_var;
				} else {
					fseek(fps[it], -sizeof(long), SEEK_CUR);
				}
			}

		}	
	}

end:
	for(it = 0; it < k; ++ it) {
		if(fps[it] != NULL) {
			fclose(fps[it]);
		}
	}

	free(fps);
	if(in != stdin) {
		fclose(in);
	}
	if(out != stdout) {
		fclose(out);
	}

	return 0;
}
