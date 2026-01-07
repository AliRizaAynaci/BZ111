#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int no;
    char isim[30];
    int vize;
    int odev;
    int final;
} Ogrenci;

// Gano: vize * 0.5 + odev * 0.2 + final * 0.5

// vize ve odev'den gelen ortalama  >= 30 
// final notu >= 60
// Gano >= 60
int main() {
    
    FILE* ogrenci_verileri = fopen("./ogrenci_verileri.txt", "r");
    FILE* sonuc_verileri = fopen("./sonuc_verileri.txt", "w");
    Ogrenci ogr;
    while(fscanf(ogrenci_verileri, "%d %s %d %d %d", &ogr.no, ogr.isim, &ogr.vize, &ogr.odev, &ogr.final) != EOF) {
        float ortalama = ogr.vize * 0.5 + ogr.odev * 0.2;
        if (ortalama < 30) {
            fprintf(sonuc_verileri, "%d %s %s (Vize+Odev ortalamasi: %f)\n", ogr.no, ogr.isim, "KALDI", ortalama);
        } else if (ogr.final < 60) {
            fprintf(sonuc_verileri, "%d %s %s (Final Notu: %d)\n", ogr.no, ogr.isim, "KALDI", ogr.final);
        } else {
            float genel_ortalama = ortalama + ogr.final * 0.5;
            if (genel_ortalama >= 60) {
                fprintf(sonuc_verileri, "%d %s %s (Gano: %f)\n", ogr.no, ogr.isim, "GECTI", genel_ortalama);
            }
        }
        //printf("No: %d, Isim: %s, Vize: %d, Odev: %d, Final: %d\n", ogr.no, ogr.isim, ogr.vize, ogr.odev, ogr.final);
    }



    return 0;
}