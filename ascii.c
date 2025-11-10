#include <stdio.h>
#include <ctype.h>

int main() {
	char karakter;
	int sira;

	printf("Lutfen bir kucuk harf (a-z) giriniz: ");
	scanf("%c", &karakter);

	if (islower(karakter)) {
		sira = karakter - 'a';
		printf("Girilen karakter: '%c'\n", karakter);
		printf("ASCII kodu (Decimal): %d\n", karakter);
		printf("(a) harfinin ASCII kodu: %d\n", 'a');
		printf("Alfabetik Sira: %d\n", sira);
	} else {
		printf("Hata: Gecerli bir kucuk harf girmeidniz. \n");
	}
	return 0;
}
