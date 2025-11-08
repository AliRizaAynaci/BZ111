#include <stdio.h>
#include <stdlib.h>

char mukemmel_sayi_mi(int sayi) {
	int toplam = 0;
	if (sayi <= 1) return 0;
	for (int i=1; i<=sayi/2; i++) {
		if (sayi % i == 0) toplam += i;
	}
	if (toplam == sayi) return 1;
	return 0;
}

int main() {
	int sayi;
	printf("sayi giriniz: ");
	scanf("%d", &sayi);
	if (mukemmel_sayi_mi(sayi)) printf("%d sayisi mukemmeldir.\n", sayi);
	else printf("%d sayisi mukemmel degildir.\n", sayi);
	return 0;
}
