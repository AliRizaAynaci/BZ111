#include <stdio.h>
#include <math.h>

int asal_mi(int sayi) {
	if (sayi < 2) return 0;
	for (int i = 2; i <= sqrt(sayi); i++) {
		if (sayi % i == 0) return 0;
	}
	return 1;
}

int main() {
	int sayi;
	printf("Bir sayi giriniz: ");
	scanf("%d", &sayi);

	if (asal_mi(sayi)) {
		printf("%d asal bir sayidir.\n", sayi);
	} else {
		printf("%d asal bir sayi degildir.\n", sayi);
	}

	return 0;
}
