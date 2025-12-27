#include <stdio.h>
#include <string.h>

int main() {
	char str1[30], str2[30];
	printf("String Giriniz: ");
	gets(str1);
	int size = strlen(str1);
	for (int i = 0; i <= size; i++) {
		strncpy(&str2[i], &str1[size-i-1], 1);
	}
	puts(str2);
	return 0;
}
