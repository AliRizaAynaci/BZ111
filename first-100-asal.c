#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int asal_mi(int sayi) {
    if (sayi < 2) return 0;
    for (int i = 2; i <= sqrt(sayi); i++) {
        if (sayi % i == 0) return 0;
    }
    return 1;
}

int main() {
    int sayi;
    int sayac = 0;
    printf("Ilk 100 asal sayi:\n");
    for (sayi = 2; sayac < 100; sayi++) {
        if (asal_mi(sayi)) {
            printf("%3d. asal %4d dir \n", sayac + 1, sayi);
            sayac++;
        }
    }
    printf("\n");
    return 0;
}
