#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NR_TARI 64
#define TARA 16
#define CLASATE 3

void swap_int(int *a, int *b);
void swap_char(char *a, char *b);
int find(char (*tari)[], char *tara, int n);
void sort(char (*tari)[TARA],int *punctaje, int n);

int main() {

	char filename[256];
	int n;
	scanf("%s %d", filename, &n);

	char *line = NULL;
	size_t sz = 0;

	FILE *in = fopen(filename, "rt");
	if(in == NULL) {
		fprintf(stderr, "Eroare la deschiderea fisierlui: %s\n", filename);
		return -1;
	}

	char tari[NR_TARI][TARA];
	int punctaje[NR_TARI];
	memset(punctaje, 0, NR_TARI * sizeof(int));

	char temp[CLASATE][TARA];
	char juriu[TARA];
	char delim[] = " \n";
	int i;

	for(i = 0; i < n; ++ i) {
		getline(&line, &sz, in);
		char *p = strtok(line, delim);
		strcpy(tari[i], p);
	}	

	fseek(in, 0, SEEK_SET);
	for(i = 0; i < n; ++ i) {
		memset(temp, '\0', CLASATE * TARA * sizeof(char));
		getline(&line, &sz, in);
		char *p = strtok(line, delim);
		strcpy(juriu, p);
		
		int k = 0;
		while((p = strtok(NULL, delim)) != NULL) {
			strcpy(temp[k ++], p);
		}
		
		if(find(temp, juriu, CLASATE) != -1) {
			printf("Juriu descalificat: %s\n", juriu);
			free(line);
			line = NULL;
			sz = 0;
			continue;
		}

		for(k = CLASATE; k > 0; -- k) {
			punctaje[find(tari, temp[CLASATE - k], n)] += k;
		}
	}


	sort(tari, punctaje, n);

	for(i = 0; i < 3; ++ i) {
		printf("Tara: %s Locul: %d punctaj: %d\n", tari[i], i + 1, punctaje[i]);
	}

	for(i = n - 1; punctaje[i] == 0 && i >= 3; -- i) {
		printf("Tara: %s punctaj 0!\n", tari[i]);
	}

	printf("\n");

	return 0;
}

void swap_int(int *a, int *b) {
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
/*	int c = *a;
	*a = *b;
	*b = c;*/
}

void swap_char(char *a, char *b) {
	char c[TARA];
	strcpy(c, a);
	strcpy(a, b);
	strcpy(b, c);
}

void sort(char (*tari)[TARA], int *punctaje, int n) {
	int i, j, max;
	for(i = 0; i < n - 1; ++ i) {
		max = i;
		for(j = i + 1; j < n; ++ j) {
			if(punctaje[max] < punctaje[j]) {
				max = j;
			}
		}

		if(max != i) {
			swap_char(tari[i], tari[max]);
			swap_int(&punctaje[i], &punctaje[max]);
		}
	}
}

int find(char tari[][TARA], char tara[TARA], int n) {
	int i;
	for(i = 0; i < n; ++ i) {
		if(strcmp(tari[i], tara) == 0){
			return i;
		}
	}

	return -1;
}
