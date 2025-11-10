#include <stdio.h>

int main() {
	int decimalSayi;
	printf("Sayi Giriniz: ");
	scanf("%d", &decimalSayi);
	int tempDecimalSayi = decimalSayi;
	long long binarySayi = 0;
	int powerOf10 = 1;
	int kalan;

	while (tempDecimalSayi != 0) {
		kalan = tempDecimalSayi % 2;
		tempDecimalSayi /= 2;
		binarySayi += kalan * powerOf10;
		powerOf10 *= 10;
	}
	printf("Binary Karsiligi: %lld\n", binarySayi);
	return 0;
}
