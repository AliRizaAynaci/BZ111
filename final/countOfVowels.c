#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
	char dizgi[100];
	printf("Bir metin giriniz: ");
	gets(dizgi);
	int len = strlen(dizgi);
	int sessizHarfSayac = 0, sesliHarfSayac = 0;
	for (int i = 0; i < len; i++) {
		char ch = dizgi[i];
		ch = tolower(ch);
		if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
			sesliHarfSayac += 1;
		} else if (isalpha(ch)) {
			sessizHarfSayac += 1;
		}
	}
	printf("Sesli Harf Sayisi: %d\n", sesliHarfSayac);
	printf("Sessiz Harf Sayisi: %d\n\n", sessizHarfSayac);
	return 0;
}
