#include <stdio.h>
#include <stdlib.h>

int main() {
	int toplam;
	for (int i = 1; i <= 9999; i++) {
		toplam = 0;
		for (int j = 1; j <= i / 2; j++) {
			if (i % j == 0) {
				toplam += j;
			}
		}
		if (toplam == i) printf("%4d mukemmel sayidir.\n\n", i);
	}
	return 0;
}
