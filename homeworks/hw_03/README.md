# BZ111 Bilgisayar Programlama - 1
## Ödev 3: Labirent (badMaze)

### Derleme ve Çalıştırma

```bash
gcc -o labirent labirent.c
./labirent
```

### Özellikler

1. **Yol Çizme (F1)**: Yön tuşları ile labirentte yol çizin
2. **Yol Silme (F2)**: Yön tuşları ile çizilen yolları silin
3. **Gezinme (F3)**: Çizilen yollarda gezinin
4. **Duvar Yerleştirme (F4)**: Boş alanları duvar ile doldurun
5. **Kaydetme/Yükleme (F5/F6)**: Labirenti dosyaya kaydedin/yükleyin
6. **Temizleme (F7)**: Tüm labirenti temizleyin
7. **En Kısa Yol (F8)**: BFS algoritması ile en kısa yolu bulun
8. **Rastgele Labirent (F9)**: DFS algoritması ile otomatik labirent oluşturun
9. **Boyut Değiştirme (F10)**: Labirent boyutlarını değiştirin

### Tuş Kontrolleri

#### Yön Tuşları
- `↑` `↓` `←` `→` : İmleç hareketi / Gezinme

#### Fonksiyon Tuşları
| Tuş | İşlev |
|-----|-------|
| F1  | Çiz - Yol çizme modu |
| F2  | Sil - Yol silme modu |
| F3  | Gez - Labirentte gezinme |
| F4  | Duvar - Boşlukları duvara çevir |
| F5  | Kaydet |
| F6  | Yükle |
| F7  | Temizle |
| F8  | En kısa yolu bul |
| F9  | Rastgele labirent oluştur |
| F10 | Yeni boyut belirle |

#### Diğer Tuşlar
| Tuş | İşlev |
|-----|-------|
| Boşluk | Duraklat |
| ESC | Çıkış |
| 1-0 | F1-F10 tuşlarına alternatif |

### Hücre Tipleri
- `█` (Gri) : Duvar
- `.` (Sarı) : Yol
- `<` (Cyan) : Giriş (sol kenarda)
- `>` (Kırmızı) : Çıkış (sağ kenarda)
- `@` (Yeşil) : Oyuncu (gezinme modunda)

### Algoritmalar

- **Rastgele Labirent**: Depth-First Search (DFS) ile recursive backtracking
- **En Kısa Yol**: Breadth-First Search (BFS) algoritması

### Dosya Formatı

Labirent dosyası (.maze) aşağıdaki formatta kaydedilir:
```
genişlik yükseklik
başlangıç_x başlangıç_y
bitiş_x bitiş_y
[labirent içeriği]
```

### Varsayılan Boyut
- Genişlik: 60 karakter
- Yükseklik: 12 satır
- Maksimum: 80x30

### Ekran Görünümü

```
┌────────────────────────────────────────────────────────────┐
<..........                                                  │
│          ██████████                                        │
│                    ██████████                              │
│                              ██████████                    │
│                                        ██████████          │
│                                                  ..........>
└────────────────────────────────────────────────────────────┘

  >> Ciz Modu: Yon tuslari ile yol cizin.

F1: Ciz | F2: Sil | F3: Gez | F4: Duvar | F5: Kaydet | F6: Yukle
F7: Temizle | F8: En kisa yolu bul | F9: Rastgele | F10: Yeni boyut
Bosluk: Duraklat | Esc: Cikis                 https://dr.alperbasturk.com
```
