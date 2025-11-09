#include <stdio.h>

int main() {

	int num;
	// Fibonacci serisinin ilk iki elemanini tanimliyoruz
	int a = 0, b = 1;
	// Serinin bir sonraki elemanini tutacak
	int c = 0;

	printf("Fibonacci degerini hesaplamak istediginiz sayiyi girin: ");
	scanf("%d", &num);

	// Serinin ilk elemani her zaman 1 olacagindan en basta ekrana yazdiriyoruz
	if (num >= 1) printf("%d ", b);

	// Serinin ilk elemanini zaten yazdik, bu yuzden 2.elemandan baslayacak
	for (int i = 2; i <= num; ++i) {
		// n.eleman = (n-1) + (n-2) elemanlarinin toplami
		// Bu yuzden eski a ve eski b degerleri ile topluyoruz
		c = a + b;
		// Dongunun bir sonraki adimi icin a ve b degerlerini guncelliyoruz
		a = b;
		b = c;
		// Hesaplanan toplam degerini ekrana yazdir
		printf("%d ", c);
	}
	// Dongu bittiginde serinin son elemanini yazdir
	printf("\nSonuc: %d\n", c);
	return 0;
}
/**
Adım Adım Çalışma Akışı (num=5 için):
--------------------------------------
1. BAŞLANGIÇ DEĞERLERİ:
   num = 5
   a = 0
   b = 1 (Serinin 1. elemanı)
   c = 0

2. İLK ELEMAN KONTROLÜ:
   if (num >= 1) kontrolü doğru (5 >= 1).
   -> Ekrana b yazdırılır: ÇIKTI: "1 "

3. DÖNGÜ BAŞLIYOR: for (int i = 2; i <= 5; ++i)

   a) i = 2: (Serinin 2. elemanı hesaplanıyor)
      c = a + b  -> c = 0 + 1 = 1
      a = b      -> a = 1
      b = c      -> b = 1
      -> Ekrana c yazdırılır: ÇIKTI: "1 1 "

   b) i = 3: (Serinin 3. elemanı hesaplanıyor)
      c = a + b  -> c = 1 + 1 = 2
      a = b      -> a = 1
      b = c      -> b = 2
      -> Ekrana c yazdırılır: ÇIKTI: "1 1 2 "

   c) i = 4: (Serinin 4. elemanı hesaplanıyor)
      c = a + b  -> c = 1 + 2 = 3
      a = b      -> a = 2
      b = c      -> b = 3
      -> Ekrana c yazdırılır: ÇIKTI: "1 1 2 3 "

   d) i = 5: (Serinin 5. elemanı hesaplanıyor)
      c = a + b  -> c = 2 + 3 = 5
      a = b      -> a = 3
      b = c      -> b = 5
      -> Ekrana c yazdırılır: ÇIKTI: "1 1 2 3 5 "

4. DÖNGÜ SONU:
   i = 6 olduğu için döngü sona erer.
   Son değer b = 5'tir.

5. SONUÇ:
   -> Ekrana "Sonuc: 5" yazdırılır.
*/
