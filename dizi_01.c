#include <stdio.h>


int main() {
    int size;
    printf("Dizi kac elemanli olacak: ");
    scanf("%d", &size);

    int dizi[size];
    int tekSayilarToplami = 0, ciftSayilarTOplami = 0;
    int tekSayiCounter = 0, ciftSayiCounter = 0;
    for (int i = 0; i < size; i++) {
        printf("Dizinin %d. elemanini giriniz: ", i+1);
        scanf("%d", &dizi[i]);
        if (dizi[i] % 2 == 0) {
            ciftSayilarTOplami += dizi[i];
            ciftSayiCounter++;
        } else { 
            tekSayilarToplami += dizi[i];
            tekSayiCounter++;
        }
    }
    float tekSayilarinOrtalamasi, ciftSayilarinOrtalamasi;
    tekSayilarinOrtalamasi = (float)tekSayilarToplami/tekSayiCounter;
    ciftSayilarinOrtalamasi = (float)ciftSayilarTOplami/ciftSayiCounter;
    printf("%d adet tek sayinin toplami: %d, ortalamasi: %f\n", tekSayiCounter, tekSayilarToplami, tekSayilarinOrtalamasi);
    printf("%d adet cift sayinin toplami: %d, ortalamasi: %f\n", ciftSayiCounter, ciftSayilarTOplami, ciftSayilarinOrtalamasi);
    return 0;
}