#include <stdio.h>
#include <string.h>

// struct Ogrenci {
// 	char adSoyad[30];
// 	int yas;
// 	float ortalama;
// 	char cinsiyet;
// };

typedef struct {
	char adSoyad[30];
	int yas;
	float ortalama;
	char cinsiyet;
} Ogrenci;

void yapiDoldur(Ogrenci *ogr) { 
	printf("Ad Soyad Bilgisini Giriniz: ");
	gets(ogr->adSoyad);

	printf("Yas bilginizi giriniz: ");
	scanf("%d", &ogr->yas);

	printf("Ogrenci Ortalamasini Giriniz: ");
	scanf("%f", &ogr->ortalama);

	printf("Cinsiyet Bilgisini Giriniz: ");
	scanf(" %c", &ogr->cinsiyet);
}

void yapiSifirla(Ogrenci *ogr) {
	strcpy(ogr->adSoyad, "");
	ogr->yas = 0;
	ogr->ortalama = 0.0;
	ogr->cinsiyet = '\0';
}

void yapiKopyala(Ogrenci *hedef, Ogrenci *kaynak) {
	strcpy(hedef->adSoyad, kaynak->adSoyad);
	hedef->yas = kaynak->yas;
	hedef->ortalama = kaynak->ortalama;
	hedef->cinsiyet = kaynak->cinsiyet;
}


int main() {

	Ogrenci ogr;
	yapiDoldur(&ogr);


	printf("Ogrenci Ad Soyad: %s, Yas: %d, ortalama: %.2f, cinsiyet: %c\n\n\n",
		 ogr.adSoyad, ogr.yas, ogr.ortalama, ogr.cinsiyet);

	Ogrenci ogr2;
	yapiKopyala(&ogr2, &ogr);

	printf("Kopyalanan yapi bilgileri:\n");
	printf("Ogrenci Ad Soyad: %s, Yas: %d, ortalama: %.2f, cinsiyet: %c\n",
		 ogr2.adSoyad, ogr2.yas, ogr2.ortalama, ogr2.cinsiyet);

	yapiSifirla(&ogr);

	printf("Yapi sifirlandiktan sonra:\n");
	printf("Ogrenci Ad Soyad: %s, Yas: %d, ortalama: %.2f, cinsiyet: %c\n\n\n",
		 ogr.adSoyad, ogr.yas, ogr.ortalama, ogr.cinsiyet);

	yapiSifirla(&ogr2);

	printf("Yapi sifirlandiktan sonra:\n");
	printf("Ogrenci Ad Soyad: %s, Yas: %d, ortalama: %.2f, cinsiyet: %c\n\n\n",
		 ogr2.adSoyad, ogr2.yas, ogr2.ortalama, ogr2.cinsiyet);

	return 0;
}
