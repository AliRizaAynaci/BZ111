#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int fibonacci(int num) {
	if (num == 0) return 0;
	if (num <= 2) return 1;
	return fibonacci(num-1) + fibonacci(num-2);
}

int main() {
	int num;
	printf("Fibonacci degerini hesaplamak istediginiz sayiyi girin: ");
	scanf("%d", &num);
	int sonuc = 1;
	sonuc = fibonacci(num);
	printf("Sonuc: %d\n", sonuc);
	return 0;
}
