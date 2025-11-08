#include <stdio.h>

int main() {

	int num;
	int a = 0, b = 1;
	int c = 0;
	printf("Fibonacci degerini hesaplamak istediginiz sayiyi girin: ");
	scanf("%d", &num);
	if (num >= 1) printf("%d ", b);
	for (int i = 2; i <= num; ++i) {
		c = a + b;
		a = b;
		b = c;
		printf("%d ", c);
	}
	printf("\nSonuc: %d\n", b);
	return 0;
}

