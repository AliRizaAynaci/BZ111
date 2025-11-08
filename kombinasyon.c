#include <stdio.h>

int faktoriyel(int k) {
	if (k < 0) return 0;
	int sonuc = 1;
	for (int i = 2; i <= k; i++) {
		sonuc *= i;
	}
	return sonuc;
}

int kombinasyon(int n, int r) {
	if (n < r || r < 0) return 0;
	int pay = faktoriyel(n);
	int payda = faktoriyel(r) * faktoriyel(n-r);
	if (payda == 0) return 0;
	return pay / payda;
}

int main() {
	int A[10] = {5, 8, 4, 6, 9, 7, 10, 3, 5, 8};
	int B[10] = {3, 2, 0, 1, 5, 7, 3, 3, 5, 1};
	int sonuclar[10];
	for (int i = 0; i < 10; i++) {
		if (A[i] >= B[i]) sonuclar[i] = kombinasyon(A[i], B[i]);
		else sonuclar[i] = 0;
	}

	printf("Kombinasyon Sonuclari:\n");
	for (int i = 0; i < 10; i++) {
		printf("C(%d, %d) = %d\n", A[i], B[i], sonuclar[i]);
	}
	return 0;
}
