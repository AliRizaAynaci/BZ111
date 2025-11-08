#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int faktoriyel(int num) {
	int f = 1;
	for (int i = 2; i <= num; i++) {
		f = f * i;
	}
	return f;
}

double sin_hesapla(double x) {
	double sum = 0;
	for (int i  = 0; i < 4; i++) {
		sum += pow(-1, (double)i) * (pow(x, (double)(2*i+1)) / (double)faktoriyel(2*i+1));
	}
	return sum;
}

int main() {
	double x = 5;
	do {
		printf("Radyan cinsinden aci degerini giriniz: ");
		scanf("%lf", &x);
	} while ((x > 3.14001) || (x < -3.14001));
	printf("\n\n%.5f radyanlik acinin sinusu %.5f'dir\n\n", x, sin_hesapla(x));

	return 0;
}
