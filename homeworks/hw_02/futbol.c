#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <sys/select.h>

#define SAHA_GENISLIK 80
#define SAHA_YUKSEKLIK 25

#define TAKIM_OYUNCU_SAYISI 8
#define MAC_SURESI 90
#define SIMULASYON_HIZI 80000

#define RENK_SIFIRLA  "\x1b[0m"
#define RENK_KIRMIZI  "\x1b[1;31m"
#define RENK_MAVI     "\x1b[1;34m"
#define RENK_BEYAZ_YAZI "\x1b[1;37m"
#define RENK_SARI "\x1b[1;33m"
#define ZEMIN_YESIL   "\x1b[42m"
#define ZEMIN_KOYU_YESIL "\x1b[48;5;22m"

typedef struct {
    double x, y;
} Pozisyon;

typedef struct {
    int id;
    int takim_id;
    int forma_no;
    Pozisyon poz;
    Pozisyon varsayilan_poz;
    int bekleme_suresi;
    double hiz;
    int gol_sayisi;
    int asist_sayisi;
} Oyuncu;

typedef struct {
    Pozisyon poz;
    double hiz_x, hiz_y;
    int sahibi_id;
    int son_dokunan_id;
    int onceki_dokunan_id;
} Top;

Oyuncu kirmizi_takim[TAKIM_OYUNCU_SAYISI];
Oyuncu mavi_takim[TAKIM_OYUNCU_SAYISI];
Top top;
int skor_kirmizi = 0;
int skor_mavi = 0;
char spiker_metni[256] = "Mac basliyor!";
int dakika = 0;

void temizle();
void imleci_basa_al();
void ciz();
void baslat();
void top_merkez();
void oyna();
void fizik();
double mesafe(Pozisyon p1, Pozisyon p2);
void spiker(const char *metin);
void bahis_ekrani(int *secim);

int main() {
    srand(time(NULL));
    
    int bahis_takim = 0;
    bahis_ekrani(&bahis_takim);

    for(int i=0; i<TAKIM_OYUNCU_SAYISI; i++) {
        kirmizi_takim[i].gol_sayisi = 0;
        kirmizi_takim[i].asist_sayisi = 0;
        mavi_takim[i].gol_sayisi = 0;
        mavi_takim[i].asist_sayisi = 0;
    }

    baslat();
    top_merkez();
    temizle(); 

    int tick = 0;
    int toplam_dakika = 0;
    int devre = 1;

    while (toplam_dakika <= MAC_SURESI) {
        imleci_basa_al();
        ciz();
        
        tick++;
        if (tick % 8 == 0) {
            toplam_dakika++;
            dakika = toplam_dakika;
        }

        if (toplam_dakika == 45 && devre == 1) {
            spiker("ILK YARI SONA ERDI! Devre arasi...");
            sleep(2);
            devre = 2;
            top_merkez();
            for (int i = 0; i < TAKIM_OYUNCU_SAYISI; i++) {
                kirmizi_takim[i].bekleme_suresi = 12;
                mavi_takim[i].bekleme_suresi = 12;
            }
            spiker("IKINCI YARI BASLADI!");
            continue;
        }

        if (toplam_dakika >= 90) {
            break;
        }

        oyna();
        fizik();

        usleep(SIMULASYON_HIZI);
    }

    temizle();
    ciz();
    printf(RENK_SIFIRLA "\n\nMAC SONA ERDI!\n");
    printf("SKOR: KIRMIZI %d - %d MAVI\n", skor_kirmizi, skor_mavi);

    if (skor_kirmizi > skor_mavi) {
        if (bahis_takim == 1) printf(RENK_SARI "\nTEBRIKLER! Kirmizi takim kazandi ve iddianiz tuttu!\n" RENK_SIFIRLA);
        else printf(RENK_KIRMIZI "\nUZGUNUZ! Kirmizi takim kazandi, iddianiz tutmadi.\n" RENK_SIFIRLA);
    } else if (skor_mavi > skor_kirmizi) {
        if (bahis_takim == 2) printf(RENK_SARI "\nTEBRIKLER! Mavi takim kazandi ve iddianiz tuttu!\n" RENK_SIFIRLA);
        else printf(RENK_KIRMIZI "\nUZGUNUZ! Mavi takim kazandi, iddianiz tutmadi.\n" RENK_SIFIRLA);
    } else {
        printf(RENK_BEYAZ_YAZI "\nMac berabere bitti! Iddia gecersiz.\n" RENK_SIFIRLA);
    }

    return 0;
}

void temizle() {
    printf("\033[H\033[J");
}

void imleci_basa_al() {
    printf("\033[H");
}

void bahis_ekrani(int *secim) {
    temizle();
    printf(RENK_SARI "##########################################\n");
    printf("#                                        #\n");
    printf("#           IDDIA EKRANI                 #\n");
    printf("#                                        #\n");
    printf("##########################################\n" RENK_SIFIRLA);
    printf("\nMac baslamadan once tahmininizi yapin!\n");
    
    printf("Hangi takim kazanir?\n");
    printf("1. " RENK_KIRMIZI "Kirmizi Takim" RENK_SIFIRLA "\n");
    printf("2. " RENK_MAVI "Mavi Takim" RENK_SIFIRLA "\n");
    
    int sure = 10;
    printf("\n");
    
    struct timeval tv;
    fd_set fds;
    
    while(sure > 0) {
        printf("\rSeciminiz (1 veya 2) [Kalan sure: %d]: ", sure);
        fflush(stdout);
        
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        
        int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        
        if(ret > 0) {
            scanf("%d", secim);
            printf("\nTahmininiz alindi! Mac basliyor...\n");
            sleep(1);
            return;
        }
        
        sure--;
    }
    
    printf("\nSure doldu! Bahis yapilmadi.\n");
    *secim = 0;
    sleep(2);
}

void ciz() {
    char tampon[SAHA_YUKSEKLIK][SAHA_GENISLIK];
    int renk_tampon[SAHA_YUKSEKLIK][SAHA_GENISLIK];

    for (int y = 0; y < SAHA_YUKSEKLIK; y++) {
        for (int x = 0; x < SAHA_GENISLIK; x++) {
            tampon[y][x] = ' ';
            renk_tampon[y][x] = 0;

            if (x == 0 || x == SAHA_GENISLIK - 1 || y == 0 || y == SAHA_YUKSEKLIK - 1) {
                tampon[y][x] = '#';
                renk_tampon[y][x] = 1;
            }
            else if (x == SAHA_GENISLIK / 2) {
                tampon[y][x] = '|';
                renk_tampon[y][x] = 1;
            }
            else if ((x > SAHA_GENISLIK/2 - 4 && x < SAHA_GENISLIK/2 + 4) && 
                     (y > SAHA_YUKSEKLIK/2 - 3 && y < SAHA_YUKSEKLIK/2 + 3)) {
                 if (x == SAHA_GENISLIK/2 - 3 || x == SAHA_GENISLIK/2 + 3 || 
                     y == SAHA_YUKSEKLIK/2 - 2 || y == SAHA_YUKSEKLIK/2 + 2) {
                     tampon[y][x] = '.';
                     renk_tampon[y][x] = 1;
                 }
            }
            
            if (x < 12 && y > SAHA_YUKSEKLIK/2 - 8 && y < SAHA_YUKSEKLIK/2 + 8) {
                if (x == 11 || y == SAHA_YUKSEKLIK/2 - 7 || y == SAHA_YUKSEKLIK/2 + 7) {
                    tampon[y][x] = '.';
                    renk_tampon[y][x] = 1;
                }
            }
            if (x > SAHA_GENISLIK - 13 && y > SAHA_YUKSEKLIK/2 - 8 && y < SAHA_YUKSEKLIK/2 + 8) {
                if (x == SAHA_GENISLIK - 12 || y == SAHA_YUKSEKLIK/2 - 7 || y == SAHA_YUKSEKLIK/2 + 7) {
                    tampon[y][x] = '.';
                    renk_tampon[y][x] = 1;
                }
            }
        }
    }

    int kale_y_bas = SAHA_YUKSEKLIK / 2 - 3;
    int kale_y_bit = SAHA_YUKSEKLIK / 2 + 3;
    for (int y = kale_y_bas; y <= kale_y_bit; y++) {
        tampon[y][0] = '[';
        tampon[y][SAHA_GENISLIK - 1] = ']';
        renk_tampon[y][0] = 1;
        renk_tampon[y][SAHA_GENISLIK - 1] = 1;
    }

    for (int i = 0; i < TAKIM_OYUNCU_SAYISI; i++) {
        int x = (int)kirmizi_takim[i].poz.x;
        int y = (int)kirmizi_takim[i].poz.y;
        if (x >= 0 && x < SAHA_GENISLIK && y >= 0 && y < SAHA_YUKSEKLIK) {
            tampon[y][x] = '1' + i;
            renk_tampon[y][x] = 2;
        }
    }
    for (int i = 0; i < TAKIM_OYUNCU_SAYISI; i++) {
        int x = (int)mavi_takim[i].poz.x;
        int y = (int)mavi_takim[i].poz.y;
        if (x >= 0 && x < SAHA_GENISLIK && y >= 0 && y < SAHA_YUKSEKLIK) {
            tampon[y][x] = '1' + i;
            renk_tampon[y][x] = 3;
        }
    }

    int top_x = (int)top.poz.x;
    int top_y = (int)top.poz.y;
    if (top_x >= 0 && top_x < SAHA_GENISLIK && top_y >= 0 && top_y < SAHA_YUKSEKLIK) {
        tampon[top_y][top_x] = '@';
        renk_tampon[top_y][top_x] = 4;
    }

    printf("Sure: %d dk | Skor: " RENK_KIRMIZI "Kirmizi %d" RENK_SIFIRLA " - " RENK_MAVI "Mavi %d" RENK_SIFIRLA "\n", dakika, skor_kirmizi, skor_mavi);
    printf("Spiker: %-100s\n", spiker_metni);

    for (int y = 0; y < SAHA_YUKSEKLIK; y++) {
        for (int x = 0; x < SAHA_GENISLIK; x++) {
            printf(ZEMIN_YESIL);
            
            int tip = renk_tampon[y][x];
            char c = tampon[y][x];

            if (tip == 1) printf(RENK_BEYAZ_YAZI "%c", c);
            else if (tip == 2) printf(RENK_KIRMIZI "%c", c);
            else if (tip == 3) printf(RENK_MAVI "%c", c);
            else if (tip == 4) printf(RENK_BEYAZ_YAZI "%c", c);
            else printf(" ");
        }
        printf(RENK_SIFIRLA);

        printf("  ");
        if (y == 0) {
            printf(RENK_KIRMIZI "KIRMIZI TAKIM" RENK_SIFIRLA "       | " RENK_MAVI "MAVI TAKIM" RENK_SIFIRLA);
        } else if (y == 1) {
            printf("No  Gol  Asist       | No  Gol  Asist");
        } else if (y == 2) {
            printf("---------------------+--------------------");
        } else if (y >= 3 && y < 3 + TAKIM_OYUNCU_SAYISI) {
            int i = y - 3;
            printf("%-3d %-4d %-11d | %-3d %-4d %-4d", 
                i+1, kirmizi_takim[i].gol_sayisi, kirmizi_takim[i].asist_sayisi,
                i+1, mavi_takim[i].gol_sayisi, mavi_takim[i].asist_sayisi);
        }

        printf("\n");
    }
}

void baslat() {
    kirmizi_takim[0].varsayilan_poz = (Pozisyon){3, SAHA_YUKSEKLIK/2};
    kirmizi_takim[1].varsayilan_poz = (Pozisyon){15, SAHA_YUKSEKLIK/2 - 6};
    kirmizi_takim[2].varsayilan_poz = (Pozisyon){15, SAHA_YUKSEKLIK/2};
    kirmizi_takim[3].varsayilan_poz = (Pozisyon){15, SAHA_YUKSEKLIK/2 + 6};
    kirmizi_takim[4].varsayilan_poz = (Pozisyon){35, SAHA_YUKSEKLIK/2 - 5};
    kirmizi_takim[5].varsayilan_poz = (Pozisyon){35, SAHA_YUKSEKLIK/2};
    kirmizi_takim[6].varsayilan_poz = (Pozisyon){35, SAHA_YUKSEKLIK/2 + 5};
    kirmizi_takim[7].varsayilan_poz = (Pozisyon){50, SAHA_YUKSEKLIK/2};

    for(int i=0; i<TAKIM_OYUNCU_SAYISI; i++) {
        kirmizi_takim[i].id = i;
        kirmizi_takim[i].takim_id = 0;
        kirmizi_takim[i].poz = kirmizi_takim[i].varsayilan_poz;
        kirmizi_takim[i].hiz = 0.4 + ((rand()%10)/30.0);
        kirmizi_takim[i].bekleme_suresi = 0;
    }

    mavi_takim[0].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-4, SAHA_YUKSEKLIK/2};
    mavi_takim[1].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-16, SAHA_YUKSEKLIK/2 - 6};
    mavi_takim[2].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-16, SAHA_YUKSEKLIK/2};
    mavi_takim[3].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-16, SAHA_YUKSEKLIK/2 + 6};
    mavi_takim[4].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-36, SAHA_YUKSEKLIK/2 - 5};
    mavi_takim[5].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-36, SAHA_YUKSEKLIK/2};
    mavi_takim[6].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-36, SAHA_YUKSEKLIK/2 + 5};
    mavi_takim[7].varsayilan_poz = (Pozisyon){SAHA_GENISLIK-51, SAHA_YUKSEKLIK/2};

    for(int i=0; i<TAKIM_OYUNCU_SAYISI; i++) {
        mavi_takim[i].id = i + TAKIM_OYUNCU_SAYISI;
        mavi_takim[i].takim_id = 1;
        mavi_takim[i].poz = mavi_takim[i].varsayilan_poz;
        mavi_takim[i].hiz = 0.4 + ((rand()%10)/30.0);
        mavi_takim[i].bekleme_suresi = 0;
    }
}

void top_merkez() {
    top.poz.x = SAHA_GENISLIK / 2;
    top.poz.y = SAHA_YUKSEKLIK / 2;
    top.hiz_x = 0;
    top.hiz_y = 0;
    top.sahibi_id = -1;
    top.son_dokunan_id = -1;
    top.onceki_dokunan_id = -1;
}

double mesafe(Pozisyon p1, Pozisyon p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void spiker(const char *metin) {
    if (strcmp(spiker_metni, metin) != 0) {
        strncpy(spiker_metni, metin, 255);
    }
}

void oyna() {
    for (int t = 0; t < 2; t++) {
        Oyuncu *takim = (t == 0) ? kirmizi_takim : mavi_takim;
        Oyuncu *rakip_takim = (t == 0) ? mavi_takim : kirmizi_takim;
        int rakip_kale_x = (t == 0) ? SAHA_GENISLIK - 1 : 1;

        bool top_bizde = (top.sahibi_id != -1 && top.sahibi_id / TAKIM_OYUNCU_SAYISI == t);
        bool top_rakipte = (top.sahibi_id != -1 && top.sahibi_id / TAKIM_OYUNCU_SAYISI != t);

        for (int i = 0; i < TAKIM_OYUNCU_SAYISI; i++) {
            Oyuncu *o = &takim[i];

            if (o->bekleme_suresi > 0) {
                o->bekleme_suresi--;
                continue;
            }

            if (top.sahibi_id == o->id) {
                double dx = rakip_kale_x - o->poz.x;
                double dy = (SAHA_YUKSEKLIK / 2) - o->poz.y;
                double d = sqrt(dx*dx + dy*dy);

                if (d < 20 && (rand() % 15 == 0)) {
                    spiker("SUT VE GOOOOL POZISYONU MU?");
                    top.sahibi_id = -1;
                    top.hiz_x = (dx / d) * 1.5;
                    top.hiz_y = (dy / d) * 1.5;
                    o->bekleme_suresi = 8;
                    continue;
                }

                bool rakip_yakin = false;
                for(int r=0; r<TAKIM_OYUNCU_SAYISI; r++) {
                    if (mesafe(o->poz, rakip_takim[r].poz) < 4.0) {
                        rakip_yakin = true;
                        break;
                    }
                }

                if (rakip_yakin && (rand() % 8 == 0)) {
                    int en_iyi_arkadas = -1;
                    for(int a=0; a<TAKIM_OYUNCU_SAYISI; a++) {
                        if (i == a) continue;
                        double d_kale = fabs(takim[a].poz.x - rakip_kale_x);
                        if (d_kale < fabs(o->poz.x - rakip_kale_x)) {
                            double d_bana = mesafe(o->poz, takim[a].poz);
                            if (d_bana < 30 && d_bana > 5) {
                                en_iyi_arkadas = a;
                                break;
                            }
                        }
                    }
                    if (en_iyi_arkadas != -1) {
                        spiker("Paslasiyorlar...");
                        top.sahibi_id = -1;
                        double pdx = takim[en_iyi_arkadas].poz.x - o->poz.x;
                        double pdy = takim[en_iyi_arkadas].poz.y - o->poz.y;
                        double pdist = sqrt(pdx*pdx + pdy*pdy);
                        top.hiz_x = (pdx / pdist) * 1.2;
                        top.hiz_y = (pdy / pdist) * 1.2;
                        o->bekleme_suresi = 6;
                        continue;
                    }
                }

                double move_x = (dx / d) * o->hiz * 0.7;
                double move_y = (dy / d) * o->hiz * 0.7;
                if (rand()%5 == 0) move_y += (rand()%3 - 1) * 0.3;
                o->poz.x += move_x;
                o->poz.y += move_y;
                top.poz.x = o->poz.x;
                top.poz.y = o->poz.y;

            } 
            else {
                Pozisyon hedef = o->varsayilan_poz;
                double d_top = mesafe(o->poz, top.poz);

                if (i == 0) {
                    hedef.y = top.poz.y;
                    if (hedef.y < SAHA_YUKSEKLIK/2 - 4) hedef.y = SAHA_YUKSEKLIK/2 - 4;
                    if (hedef.y > SAHA_YUKSEKLIK/2 + 4) hedef.y = SAHA_YUKSEKLIK/2 + 4;
                    
                    if (d_top < 8.0) {
                        hedef = top.poz;
                    }
                } 
                else {
                    if (d_top < 10.0) {
                        hedef = top.poz;
                    } else {
                        if (top_bizde) {
                            double shift = (t == 0) ? 15.0 : -15.0;
                            hedef.x = o->varsayilan_poz.x + shift;
                        } 
                        else if (top_rakipte) {
                            double shift = (t == 0) ? -10.0 : 10.0;
                            hedef.x = o->varsayilan_poz.x + shift;
                        }
                    }
                }

                double dx = hedef.x - o->poz.x;
                double dy = hedef.y - o->poz.y;
                double d = sqrt(dx*dx + dy*dy);

                if (d > 0.5) {
                    double hiz_carpan = (d_top < 10.0) ? 0.8 : 0.5;
                    o->poz.x += (dx/d) * o->hiz * hiz_carpan;
                    o->poz.y += (dy/d) * o->hiz * hiz_carpan;
                }

                if (d_top < 1.5 && top.sahibi_id == -1) {
                    top.sahibi_id = o->id;
                    if (top.son_dokunan_id != o->id) {
                        top.onceki_dokunan_id = top.son_dokunan_id;
                        top.son_dokunan_id = o->id;
                    }
                    spiker("Top kontrol altinda.");
                } else if (d_top < 1.5 && top.sahibi_id != -1 && top.sahibi_id / TAKIM_OYUNCU_SAYISI != t) {
                    if (rand() % 60 == 0) {
                        top.sahibi_id = o->id;
                        if (top.son_dokunan_id != o->id) {
                            top.onceki_dokunan_id = top.son_dokunan_id;
                            top.son_dokunan_id = o->id;
                        }
                        spiker("Topu soktu aldi!");
                    }
                }
                
                for(int k=0; k<TAKIM_OYUNCU_SAYISI; k++) {
                    if (i == k) continue;
                    if (mesafe(o->poz, takim[k].poz) < 2.5) {
                        o->poz.x += (o->poz.x - takim[k].poz.x) * 0.4;
                        o->poz.y += (o->poz.y - takim[k].poz.y) * 0.4;
                    }
                }
                for(int r=0; r<TAKIM_OYUNCU_SAYISI; r++) {
                    if (mesafe(o->poz, rakip_takim[r].poz) < 2.0) {
                        o->poz.x += (o->poz.x - rakip_takim[r].poz.x) * 0.3;
                        o->poz.y += (o->poz.y - rakip_takim[r].poz.y) * 0.3;
                    }
                }
            }
        }
    }
}

void fizik() {
    if (top.sahibi_id != -1) return;

    top.poz.x += top.hiz_x;
    top.poz.y += top.hiz_y;
    top.hiz_x *= 0.96;
    top.hiz_y *= 0.96;

    if (fabs(top.hiz_x) < 0.1 && fabs(top.hiz_y) < 0.1) {
        top.hiz_x = 0; top.hiz_y = 0;
    }

    if (top.poz.y <= 1 || top.poz.y >= SAHA_YUKSEKLIK - 1) {
        top.hiz_y *= -1;
        if (top.poz.y <= 1) top.poz.y = 1.1;
        if (top.poz.y >= SAHA_YUKSEKLIK - 1) top.poz.y = SAHA_YUKSEKLIK - 1.1;
    }

    if (top.poz.x <= 1) {
        if (top.poz.y >= (SAHA_YUKSEKLIK/2 - 3) && top.poz.y <= (SAHA_YUKSEKLIK/2 + 3)) {
            spiker("GOOOOOL!!! MAVI TAKIM!");
            skor_mavi++;
            
            if (top.son_dokunan_id != -1) {
                int id = top.son_dokunan_id;
                if (id >= TAKIM_OYUNCU_SAYISI) {
                     mavi_takim[id - TAKIM_OYUNCU_SAYISI].gol_sayisi++;
                     if (top.onceki_dokunan_id != -1) {
                         int as_id = top.onceki_dokunan_id;
                         if (as_id >= TAKIM_OYUNCU_SAYISI && as_id != id) {
                             mavi_takim[as_id - TAKIM_OYUNCU_SAYISI].asist_sayisi++;
                         }
                     }
                }
            }

            sleep(1);
            top_merkez();
            baslat();
        } else {
            top.hiz_x *= -1;
            top.poz.x = 1.1;
        }
    } else if (top.poz.x >= SAHA_GENISLIK - 1) {
        if (top.poz.y >= (SAHA_YUKSEKLIK/2 - 3) && top.poz.y <= (SAHA_YUKSEKLIK/2 + 3)) {
            spiker("GOOOOOL!!! KIRMIZI TAKIM!");
            skor_kirmizi++;

            if (top.son_dokunan_id != -1) {
                int id = top.son_dokunan_id;
                if (id < TAKIM_OYUNCU_SAYISI) {
                     kirmizi_takim[id].gol_sayisi++;
                     if (top.onceki_dokunan_id != -1) {
                         int as_id = top.onceki_dokunan_id;
                         if (as_id < TAKIM_OYUNCU_SAYISI && as_id != id) {
                             kirmizi_takim[as_id].asist_sayisi++;
                         }
                     }
                }
            }

            sleep(1);
            top_merkez();
            baslat();
        } else {
            top.hiz_x *= -1;
            top.poz.x = SAHA_GENISLIK - 1.1;
        }
    }
}
