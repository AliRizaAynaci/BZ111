#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonksiyon prototipleri
void kodlaveyacoz(char Dzg1[], char Mat1[8][8], char Mat2[8][8], char Dzg2[], int satir, int sutun);
void arr2mat(char dzg1[], char mat1[8][8], int satir, int sutun);
void trnsmat(char Mat1[8][8], char Mat2[8][8], int satir, int sutun);
void mat2arr(char mat2[8][8], char dzg2[], int satir, int sutun);

// i. işlem: 64 uzunluklu dizgiyi 8x8 matrise çevirir. (Satır yönünde ilerleyerek)
void arr2mat(char dzg1[], char mat1[8][8], int satir, int sutun) {
    int i, j;
    int k = 0; // Dizgi indeksi
    int len = strlen(dzg1); 

    // 64 karakter (8x8) matrise doldurulur. Dizgi kısa ise boşluk (' ') kullanılır.
    for (i = 0; i < satir; i++) {
        for (j = 0; j < sutun; j++) {
            if (k < len) {
                mat1[i][j] = dzg1[k];
            } else {
                mat1[i][j] = ' '; 
            }
            k++;
        }
    }
}

// ii. işlem: Matrisin transpozunu alır
void trnsmat(char Mat1[8][8], char Mat2[8][8], int satir, int sutun) {
    int i, j;
    for (i = 0; i < satir; i++) {
        for (j = 0; j < sutun; j++) {
            Mat2[j][i] = Mat1[i][j];
        }
    }
}

// iii. işlem: 8x8 matrisi tekrar 64 uzunluklu dizgiye çevirir (Satır yönünde ilerleyerek)
void mat2arr(char mat2[8][8], char dzg2[], int satir, int sutun) {
    int i, j;
    int k = 0; // Dizgi indeksi

    for (i = 0; i < satir; i++) {
        for (j = 0; j < sutun; j++) {
            dzg2[k] = mat2[i][j];
            k++;
        }
    }
    dzg2[k] = '\0'; // Dizgi sonu karakteri eklenir
}

// Kodlama ve Kod Çözme İşlemini Yönlendiren Ana Fonksiyon
void kodlaveyacoz(char Dzg1[], char Mat1[8][8], char Mat2[8][8], char Dzg2[], int satir, int sutun) {
    arr2mat(Dzg1, Mat1, satir, sutun); // Adım i: Dizgi -> Matris
    trnsmat(Mat1, Mat2, satir, sutun); // Adım ii: Matris -> Transpoz
    mat2arr(Mat2, Dzg2, satir, sutun); // Adım iii: Transpoz -> Yeni Dizgi
}

int main(void) {
    // satir ve sutun değişkenleri main içinde tanımlanır.
    int satir = 8;
    int sutun = 8;

    // Giriş dizgisi
    char dzg1[64] = "Erciyes Universitesi a.b.1978 tarihinde Kayseri'de kurulmustur.";
    char dzg2[64], dzg3[64];
    char mat1[8][8], mat2[8][8];

    printf("Orijinal dizgi: %s\n", dzg1);

    // Kodlama işlemi: dzg1 -> dzg2
    kodlaveyacoz(dzg1, mat1, mat2, dzg2, satir, sutun);
    printf("Kodlanmis dizgi: %s\n", dzg2);

    // Kod çözme işlemi: dzg2 -> dzg3 (Aynı fonksiyon tekrar çağrılır)
    kodlaveyacoz(dzg2, mat1, mat2, dzg3, satir, sutun);
    printf("Kodu cozulmus dizgi: %s\n", dzg3);

    system("PAUSE");
    return 0;
}