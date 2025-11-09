#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main() {
	int sayac = 0;
	// sayac < 100 oldugu surece sayi++
	// bu sayede sadece asal olanlari 100'e kadar saymis oluruz
	for (int sayi = 2; sayac < 100; sayi++) {
		bool asal_mi = true;
		// sayinin karekokunu hesaplar
		// orn: sayi = 16 icin -> sqrt(16) = 4
		// 	1, 2, 3, 4 icin kontrol etmesi yeterlidir
		// 	cunku sonrasindaki sayilarin carpanlari yine bu sayilar olacaktir
		// orn: 8 icin diger carpan 16 / 8 = 2 (2 yi kontrol ettigimizde aslinda 8'i de kontrol etmis oluruz)
		for (int i = 2; i <= sqrt(sayi); i++) {
			if (sayi % i == 0) { 
				asal_mi = false;
				break; // tam bolen sayi bir kere bulduk mu gerisine bakmaya gerek kalmaz
			}
		}
		if (asal_mi) {
			printf("%3d. asal %4d dir \n", sayac + 1, sayi);
			sayac++;
		}
	}
	printf("\n");
	return 0;
}
