#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main() {
	int sayi;
	printf("Bir sayi giriniz: ");
	scanf("%d", &sayi);

	bool asal_mi = true;

	if (sayi <= 2) asal_mi = false;

	for (int i = 2; i <= sqrt(sayi); i++) {
		if (sayi % i == 0) asal_mi = false;
	}

	if (asal_mi) {
		printf("%d asal bir sayidir.\n", sayi);
	} else {
		printf("%d asal bir sayi degildir.\n", sayi);
	}
	return 0;
}
