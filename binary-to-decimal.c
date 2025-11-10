#include <stdio.h>

int main() {
	long long binarySayi;
	printf("Decimale cevirmek istediginiz binary sayiyi giriniz: \n");
	scanf("%lld", &binarySayi);

	int decimalSayi = 0;
	int powerOf2 = 1;
	int basamak;

	while (binarySayi != 0) {
		basamak = binarySayi % 10;
		binarySayi /= 10;
		decimalSayi += basamak * powerOf2;
		powerOf2 *= 2;
	}
	printf("Decimal Karsiligi: %d\n", decimalSayi);
}
