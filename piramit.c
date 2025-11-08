#include <stdio.h>

int main() {
	int n;
	printf("Sayi giriniz: ");
	scanf("%d", &n);

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n-i; j++) {
			printf("- ");
		}

		for (int k = n; k >= n-i+1; k--) {
			printf("%d ", k); 
		}
		printf("\n");
	}

	return 0;
}
