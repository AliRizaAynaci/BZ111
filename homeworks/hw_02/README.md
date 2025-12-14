# Futbol Simülasyonu

Bu proje, C programlama dili ile yazılmış, terminal tabanlı eğlenceli bir futbol simülasyonudur. Kırmızı ve Mavi takımın 8'er oyuncuyla mücadelesini, anlık istatistikleri ve maç anlatımını içerir.

## Nasıl Çalıştırılır?

Kodu derlemek ve çalıştırmak için terminalde şu komutları kullanın (`math.h` kütüphanesi için `-lm` gereklidir):

```bash
gcc futbol.c -o futbol -lm
./futbol
```

## Kodun Çalışma Mantığı

Simülasyon, "Game Loop" (Oyun Döngüsü) mantığıyla çalışır. Her döngü adımında fizik hesaplamaları yapılır, yapay zeka kararları verilir ve ekran güncellenir.

### Veri Yapıları
Kodda `struct` yapıları kullanılarak nesne tabanlı bir yaklaşım taklit edilmiştir:
*   **Oyuncu:** Konum, hız, takım bilgisi, gol ve asist sayılarını tutar.
*   **Top:** Konum, hız vektörleri ve topun hangi oyuncuda olduğu bilgisini tutar.

### Temel Fonksiyonlar

Kodun işleyişini sağlayan ana fonksiyonlar şunlardır:

*   **`bahis_ekrani()`**: Maç başlamadan önce kullanıcıdan tahmin alır. `select()` fonksiyonu kullanılarak, geri sayım devam ederken kullanıcının giriş yapabilmesi sağlanmıştır (non-blocking input).
*   **`ciz()`**: Sahayı, oyuncuları ve arayüzü ekrana basar. Ekran titremesini engellemek için tüm ekranı silmek yerine `imleci_basa_al()` fonksiyonu ile imleç başa taşınır ve üzerine yazılır. Renklendirme için ANSI kaçış kodları kullanılır.
*   **`oyna()`**: Oyunun yapay zekasıdır. Oyuncuların topa hareket etmesi, pas vermesi, şut çekmesi veya rakibe pres yapması gibi kararlar burada mesafe ve olasılık hesaplarına göre verilir.
*   **`fizik()`**: Topun serbest hareketini yönetir. Sürtünme, kenarlardan sekme ve kale içine girip girmediği (gol kontrolü) burada hesaplanır.
*   **`baslat()` & `top_merkez()`**: Maç başında veya gol olduktan sonra oyuncuları ve topu başlangıç pozisyonlarına döndürür.

### Özellikler
*   **Akıcı Görsellik:** Titreşimsiz çizim ve renkli takımlar.
*   **İstatistik Takibi:** Skor tablosu ve oyuncu özelinde gol/asist sayacı.
*   **Dinamik Spiker:** Maçtaki aksiyona göre değişen yorum satırı.
