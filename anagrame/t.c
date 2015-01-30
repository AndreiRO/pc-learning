#include<stdio.h>
#include<ctype.h>
#include<string.h>

int main() {

	char a[256];
	strcpy(a, "a123456");
	printf("%s\n", a);
	printf("%c %c %c %c %c %c", tolower(a[1]), tolower(a[2]), tolower(a[3]), tolower(a[4]), tolower(a[5]), tolower(a[6]));
	int i;
	for(i=0; i < 7; ++ i) {
		a[i] = tolower(a[i]);
	}

	printf("%s\n", a);

	return 0;
}
