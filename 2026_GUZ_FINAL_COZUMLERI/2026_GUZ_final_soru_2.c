#include <stdio.h>
#include <math.h>

/*  ORNEK INPUIT/OUTPUT

INPUT:
x,y koordinatlarini giriniz: 0.5 1.2
x,y koordinatlarini giriniz: 3.7 4.8
x,y koordinatlarini giriniz: 1.1 1.9
x,y koordinatlarini giriniz: 5.3 0.4
x,y koordinatlarini giriniz: 2.6 3.3

OUTPUT:
0.00    4.82    0.92    4.87    2.97 
0.00    0.00    3.89    4.68    1.86 
0.00    0.00    0.00    4.46    2.05 
0.00    0.00    0.00    0.00    3.96 
0.00    0.00    0.00    0.00    0.00 


*/


typedef struct {
    float x;
    float y;
} Nokta;

float oklidUzaklik(Nokta n1, Nokta n2) {
    float uzaklik = sqrt(pow((n1.x - n2.x),2) + pow((n1.y - n2.y), 2));
    return uzaklik;
}

void oklidMatrisOlustur(Nokta arr[5], float matris[][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = i+1; j < 5; j++) {
            float distance = oklidUzaklik(arr[i], arr[j]);
            matris[i][j] = distance;
        }
        for (int k = 0; k <= i; k++) {
            matris[i][k] = 0.0;
        }
    }
}

void matrisYazdir(float matris[][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%.2f \t", matris[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Nokta arr[5];
    for (int i = 0; i < 5; i++) {
        printf("x,y koordinatlarini giriniz: ");
        scanf("%f %f", &arr[i].x, &arr[i].y);
    }
    float matris[5][5];
    oklidMatrisOlustur(arr, matris);
    matrisYazdir(matris);
    return 0;
}