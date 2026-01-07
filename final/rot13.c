#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
AliRizaAynaci

nyv
ch - 'a'
for (int i = 0; i < len; i++) {
	char ch = str[i];
	int ascii = (int)ch;
	
	sifre[i] = (char)(ascii + 13)
}

*/

void rot13(char str[], char sifre[]) {
	if (strlen(str) == 0) return;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		char ch = str[i];
		if (isalpha(ch) && isupper(ch)) {
			sifre[i] = 'A' + (ch - 'A' + 13) % 26;
		} else if (isalpha(ch) && islower(ch)) {
			sifre[i] = 'a' + (ch - 'a' + 13) % 26;
		} else {
			sifre[i] = ch;
		}
	}
	sifre[len] = '\0';
}

int main() {
	char str[100];
	printf("Bir metin giriniz: ");
	gets(str);
	int len = strlen(str);
	char sifre[100];
	rot13(str, sifre);
	printf("Sifreli Hali: %s\n\n", sifre);
	return 0;
}
