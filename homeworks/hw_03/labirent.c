/**
 * BZ111 Bilgisayar Programlama - 1
 * Ödev 3: Labirent (badMaze)
 * 
 * Özellikler:
 * - F1: Çiz (yol çizme modu)
 * - F2: Sil (yol silme modu)
 * - F3: Gez (labirentte gezinme)
 * - F4: Duvar (duvarları yerleştir)
 * - F5: Kaydet
 * - F6: Yükle
 * - F7: Temizle
 * - F8: En kısa yolu bul
 * - F9: Rastgele labirent
 * - F10: Yeni boyut
 * - Boşluk: Duraklat
 * - ESC: Çıkış
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

// Labirent boyutları
#define MAX_WIDTH 80
#define MAX_HEIGHT 30
#define DEFAULT_WIDTH 60
#define DEFAULT_HEIGHT 12
#define MAX_FILENAME 256

// Hücre tipleri
#define CELL_WALL   '#'
#define CELL_PATH   '.'
#define CELL_EMPTY  ' '
#define CELL_START  '<'
#define CELL_END    '>'
#define CELL_CURSOR '@'
#define CELL_SOLUTION '*'

// ANSI Renk kodları
#define RESET       "\x1b[0m"
#define BOLD        "\x1b[1m"

// Ön plan renkleri
#define FG_BLACK    "\x1b[30m"
#define FG_RED      "\x1b[31m"
#define FG_GREEN    "\x1b[32m"
#define FG_YELLOW   "\x1b[33m"
#define FG_BLUE     "\x1b[34m"
#define FG_MAGENTA  "\x1b[35m"
#define FG_CYAN     "\x1b[36m"
#define FG_WHITE    "\x1b[37m"

// Parlak ön plan renkleri
#define FG_BRIGHT_BLACK   "\x1b[90m"
#define FG_BRIGHT_RED     "\x1b[91m"
#define FG_BRIGHT_GREEN   "\x1b[92m"
#define FG_BRIGHT_YELLOW  "\x1b[93m"
#define FG_BRIGHT_BLUE    "\x1b[94m"
#define FG_BRIGHT_MAGENTA "\x1b[95m"
#define FG_BRIGHT_CYAN    "\x1b[96m"
#define FG_BRIGHT_WHITE   "\x1b[97m"

// Arka plan renkleri
#define BG_BLACK    "\x1b[40m"
#define BG_RED      "\x1b[41m"
#define BG_GREEN    "\x1b[42m"
#define BG_YELLOW   "\x1b[43m"
#define BG_BLUE     "\x1b[44m"
#define BG_MAGENTA  "\x1b[45m"
#define BG_CYAN     "\x1b[46m"
#define BG_WHITE    "\x1b[47m"
#define BG_GRAY     "\x1b[100m"

// Tuş kodları
#define KEY_ESC       27
#define KEY_SPACE     32
#define KEY_ENTER     10

// Modlar
typedef enum {
    MODE_DRAW,      // F1: Çiz - yol çizme
    MODE_ERASE,     // F2: Sil - yol silme
    MODE_WALK,      // F3: Gez - labirentte gezinme
    MODE_WALL,      // F4: Duvar - duvar yerleştirme
    MODE_SOLVE,     // F8: Çözüm gösterme
    MODE_PAUSED     // Duraklatma
} EditMode;

// Pozisyon yapısı
typedef struct {
    int x, y;
} Position;

// Labirent yapısı
typedef struct {
    char cells[MAX_HEIGHT][MAX_WIDTH];
    char solution[MAX_HEIGHT][MAX_WIDTH];
    int width;
    int height;
    Position start;
    Position end;
    Position cursor;
    EditMode mode;
    EditMode prevMode;
    bool solved;
    bool solving;
    bool running;
    char filename[MAX_FILENAME];
    char statusMsg[256];
} Maze;

// Global değişkenler
Maze maze;
struct termios orig_termios;

// Fonksiyon prototipleri
void initTerminal(void);
void resetTerminal(void);
void clearScreen(void);
void moveCursor(int x, int y);
void hideCursor(void);
void showCursor(void);
int getKey(void);

void initMaze(int width, int height);
void drawScreen(void);
void drawMaze(void);
void drawStatusBar(void);
void drawHelpBar(void);

void handleInput(int key);
void handleArrowKey(int dir);
void handleFunctionKey(int fkey);

void setPath(int x, int y);
void erasePath(int x, int y);
void placeWalls(void);
void clearMaze(void);
void generateRandomMaze(void);
void generateMazeDFS(int x, int y, bool visited[MAX_HEIGHT][MAX_WIDTH]);

bool solveMaze(void);
bool solveMazeBFS(void);
void showSolutionAnimated(void);
void clearSolution(void);

void saveMaze(const char* filename);
void loadMaze(const char* filename);
void setStatus(const char* msg);
void promptNewSize(void);
void promptFilename(char* buffer, const char* prompt);

// Terminal fonksiyonları
void initTerminal(void) {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void resetTerminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    showCursor();
    printf(RESET);
}

void clearScreen(void) {
    printf("\033[2J\033[H");
}

void moveCursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void hideCursor(void) {
    printf("\033[?25l");
}

void showCursor(void) {
    printf("\033[?25h");
}

int getKey(void) {
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return -1;
    
    if (c == 27) {
        unsigned char seq[5];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return KEY_ESC;
        
        if (seq[0] == '[') {
            if (read(STDIN_FILENO, &seq[1], 1) != 1) return KEY_ESC;
            
            // Yön tuşları: ESC [ A/B/C/D
            if (seq[1] == 'A') return 1001; // Yukarı
            if (seq[1] == 'B') return 1002; // Aşağı
            if (seq[1] == 'C') return 1003; // Sağ
            if (seq[1] == 'D') return 1004; // Sol
            
            // Fonksiyon tuşları
            if (seq[1] >= '1' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return KEY_ESC;
                if (seq[2] == '~') {
                    // F5-F8: 15~, 17~, 18~, 19~
                    if (seq[1] == '5') return 2005; // F5
                    if (seq[1] == '7') return 2006; // F6
                    if (seq[1] == '8') return 2007; // F7
                    if (seq[1] == '9') return 2008; // F8
                }
                if (seq[2] >= '0' && seq[2] <= '9') {
                    if (read(STDIN_FILENO, &seq[3], 1) != 1) return KEY_ESC;
                    int code = (seq[1] - '0') * 10 + (seq[2] - '0');
                    // F9: 20~, F10: 21~
                    if (code == 20) return 2009; // F9
                    if (code == 21) return 2010; // F10
                    if (code == 23) return 2011; // F11
                    if (code == 24) return 2012; // F12
                    if (code == 15) return 2005; // F5
                    if (code == 17) return 2006; // F6
                    if (code == 18) return 2007; // F7
                    if (code == 19) return 2008; // F8
                }
            }
        } else if (seq[0] == 'O') {
            // F1-F4: ESC O P/Q/R/S
            if (read(STDIN_FILENO, &seq[1], 1) != 1) return KEY_ESC;
            if (seq[1] == 'P') return 2001; // F1
            if (seq[1] == 'Q') return 2002; // F2
            if (seq[1] == 'R') return 2003; // F3
            if (seq[1] == 'S') return 2004; // F4
        }
        return KEY_ESC;
    }
    
    return c;
}

// Labirent fonksiyonları
void initMaze(int width, int height) {
    memset(&maze, 0, sizeof(Maze));
    
    maze.width = (width > MAX_WIDTH) ? MAX_WIDTH : width;
    maze.height = (height > MAX_HEIGHT) ? MAX_HEIGHT : height;
    
    // Tüm hücreleri duvar yap
    for (int y = 0; y < maze.height; y++) {
        for (int x = 0; x < maze.width; x++) {
            maze.cells[y][x] = CELL_WALL;
            maze.solution[y][x] = CELL_EMPTY;
        }
    }
    
    // Başlangıç (sol kenar ortası) ve bitiş (sağ kenar ortası)
    maze.start.x = 0;
    maze.start.y = maze.height / 2;
    maze.end.x = maze.width - 1;
    maze.end.y = maze.height / 2;
    
    maze.cells[maze.start.y][maze.start.x] = CELL_START;
    maze.cells[maze.end.y][maze.end.x] = CELL_END;
    
    // Cursor başlangıçta start'ın yanında
    maze.cursor.x = 1;
    maze.cursor.y = maze.start.y;
    
    maze.mode = MODE_DRAW;
    maze.prevMode = MODE_DRAW;
    maze.solved = false;
    maze.solving = false;
    maze.running = true;
    strcpy(maze.filename, "labirent.maze");
    setStatus("Hosgeldiniz! Yon tuslari ile hareket edin.");
}

void drawScreen(void) {
    moveCursor(0, 0);
    drawMaze();
    drawStatusBar();
    drawHelpBar();
    fflush(stdout);
}

void drawMaze(void) {
    // Üst çerçeve
    printf(FG_BRIGHT_BLACK);
    printf("┌");
    for (int i = 0; i < maze.width; i++) printf("─");
    printf("┐\n");
    
    // Labirent içeriği
    for (int y = 0; y < maze.height; y++) {
        // Sol kenar
        if (y == maze.start.y) {
            printf(FG_BRIGHT_CYAN "%c" RESET, CELL_START);
        } else {
            printf(FG_BRIGHT_BLACK "│" RESET);
        }
        
        for (int x = 0; x < maze.width; x++) {
            char cell = maze.cells[y][x];
            
            // Cursor pozisyonu
            if (x == maze.cursor.x && y == maze.cursor.y && maze.mode != MODE_SOLVE) {
                if (maze.mode == MODE_WALK) {
                    printf(FG_BRIGHT_GREEN "@" RESET);
                } else {
                    printf(BG_WHITE FG_BLACK " " RESET);
                }
                continue;
            }
            
            // Çözüm yolu gösterimi - kırmızı yıldız ile göster
            if (maze.solved && maze.solution[y][x] == CELL_SOLUTION) {
                printf(FG_BRIGHT_RED "*" RESET);
                continue;
            }
            
            switch (cell) {
                case CELL_WALL:
                    printf(BG_GRAY " " RESET);
                    break;
                case CELL_PATH:
                    printf(FG_BRIGHT_YELLOW "." RESET);
                    break;
                case CELL_START:
                    printf(FG_BRIGHT_CYAN "%c" RESET, CELL_START);
                    break;
                case CELL_END:
                    printf(FG_BRIGHT_RED "%c" RESET, CELL_END);
                    break;
                case CELL_EMPTY:
                    printf(" ");
                    break;
                default:
                    printf("%c", cell);
            }
        }
        
        // Sağ kenar
        if (y == maze.end.y) {
            printf(FG_BRIGHT_RED "%c" RESET, CELL_END);
        } else {
            printf(FG_BRIGHT_BLACK "│" RESET);
        }
        printf("\n");
    }
    
    // Alt çerçeve
    printf(FG_BRIGHT_BLACK);
    printf("└");
    for (int i = 0; i < maze.width; i++) printf("─");
    printf("┘" RESET "\n");
}

void drawStatusBar(void) {
    printf("\n");
    
    if (maze.solving) {
        printf("  " FG_BRIGHT_CYAN ">> Cozuluyor..." RESET ": Bosluk ile durdur.\n");
    } else if (maze.mode == MODE_PAUSED) {
        printf("  " FG_BRIGHT_YELLOW ">> Duraklatildi" RESET ": Bosluk ile devam et.\n");
    } else {
        // Mod bilgisi
        printf("  >> ");
        switch (maze.mode) {
            case MODE_DRAW:
                printf(FG_BRIGHT_GREEN "Ciz Modu" RESET);
                break;
            case MODE_ERASE:
                printf(FG_BRIGHT_RED "Sil Modu" RESET);
                break;
            case MODE_WALK:
                printf(FG_BRIGHT_CYAN "Gez Modu" RESET);
                break;
            case MODE_WALL:
                printf(FG_BRIGHT_MAGENTA "Duvar Modu" RESET);
                break;
            case MODE_SOLVE:
                printf(FG_BRIGHT_YELLOW "Cozum Gosteriliyor" RESET);
                break;
            default:
                break;
        }
        printf(": %s\n", maze.statusMsg);
    }
    printf("\n");
}

void drawHelpBar(void) {
    // İlk satır
    printf(FG_WHITE "F1:" RESET " Ciz" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "F2:" RESET " Sil" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "F3:" RESET " Gez" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "F4:" RESET " Duvar" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "F5:" RESET " Kaydet" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "F6:" RESET " Yukle\n");
    
    // İkinci satır
    printf(FG_WHITE "F7:" RESET " Temizle" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_BRIGHT_CYAN "F8:" RESET FG_BRIGHT_CYAN " En kisa yolu bul" RESET FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "F9:" RESET " Rastgele" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "F10:" RESET " Yeni boyut\n");
    
    // Üçüncü satır
    printf(FG_WHITE "Bosluk:" RESET " Duraklat" FG_BRIGHT_BLACK " | " RESET);
    printf(FG_WHITE "Esc:" RESET " Cikis");
    
    // Sağ tarafa URL
    int padding = maze.width - 25;
    if (padding > 0) {
        for (int i = 0; i < padding; i++) printf(" ");
    }
}

void setStatus(const char* msg) {
    strncpy(maze.statusMsg, msg, sizeof(maze.statusMsg) - 1);
    maze.statusMsg[sizeof(maze.statusMsg) - 1] = '\0';
}

// Giriş işleme
void handleInput(int key) {
    // Yön tuşları
    if (key >= 1001 && key <= 1004) {
        handleArrowKey(key);
        return;
    }
    
    // Fonksiyon tuşları
    if (key >= 2001 && key <= 2012) {
        handleFunctionKey(key - 2000);
        return;
    }
    
    // Normal tuşlar
    switch (key) {
        case KEY_SPACE:
            if (maze.solving) {
                maze.solving = false;
                maze.mode = maze.prevMode;
                setStatus("Cozum durduruldu.");
            } else if (maze.mode == MODE_PAUSED) {
                maze.mode = maze.prevMode;
                setStatus("Devam ediliyor...");
            } else {
                maze.prevMode = maze.mode;
                maze.mode = MODE_PAUSED;
                setStatus("Duraklatildi. Bosluk ile devam.");
            }
            break;
            
        case KEY_ESC:
            maze.running = false;
            break;
            
        case '1': handleFunctionKey(1); break;
        case '2': handleFunctionKey(2); break;
        case '3': handleFunctionKey(3); break;
        case '4': handleFunctionKey(4); break;
        case '5': handleFunctionKey(5); break;
        case '6': handleFunctionKey(6); break;
        case '7': handleFunctionKey(7); break;
        case '8': handleFunctionKey(8); break;
        case '9': handleFunctionKey(9); break;
        case '0': handleFunctionKey(10); break;
    }
}

void handleArrowKey(int dir) {
    int dx = 0, dy = 0;
    
    switch (dir) {
        case 1001: dy = -1; break; // Yukarı
        case 1002: dy = 1;  break; // Aşağı
        case 1003: dx = 1;  break; // Sağ
        case 1004: dx = -1; break; // Sol
    }
    
    int newX = maze.cursor.x + dx;
    int newY = maze.cursor.y + dy;
    
    // Sınır kontrolü
    if (newX < 0 || newX >= maze.width || newY < 0 || newY >= maze.height) {
        return;
    }
    
    // Gezinme modunda duvardan geçemez
    if (maze.mode == MODE_WALK) {
        char cell = maze.cells[newY][newX];
        if (cell == CELL_WALL) {
            return;
        }
        
        // Çıkışa ulaştı mı?
        if (newX == maze.end.x && newY == maze.end.y) {
            setStatus("TEBRIKLER! Cikisa ulastiniz!");
        }
    }
    
    maze.cursor.x = newX;
    maze.cursor.y = newY;
    
    // Çizim modunda yol çiz
    if (maze.mode == MODE_DRAW) {
        setPath(newX, newY);
    }
    // Silme modunda yol sil
    else if (maze.mode == MODE_ERASE) {
        erasePath(newX, newY);
    }
}

void handleFunctionKey(int fkey) {
    switch (fkey) {
        case 1: // F1 - Çiz modu
            maze.mode = MODE_DRAW;
            maze.solved = false;
            clearSolution();
            setStatus("Ciz modu: Yon tuslari ile yol cizin.");
            break;
            
        case 2: // F2 - Sil modu
            maze.mode = MODE_ERASE;
            maze.solved = false;
            clearSolution();
            setStatus("Sil modu: Yon tuslari ile yol silin.");
            break;
            
        case 3: // F3 - Gez modu
            maze.mode = MODE_WALK;
            maze.cursor.x = maze.start.x + 1;
            maze.cursor.y = maze.start.y;
            // Başlangıç noktasının yanında yol yoksa uyar
            if (maze.cells[maze.cursor.y][maze.cursor.x] == CELL_WALL) {
                setStatus("Uyari: Baslangic noktasinda yol yok!");
            } else {
                setStatus("Gez modu: Labirentte gezinin.");
            }
            break;
            
        case 4: // F4 - Duvar modu (yollardan geri kalan yerlere duvar koy)
            placeWalls();
            maze.mode = MODE_DRAW;
            setStatus("Duvarlar yerlestirildi.");
            break;
            
        case 5: // F5 - Kaydet
            saveMaze(maze.filename);
            break;
            
        case 6: // F6 - Yükle
            loadMaze(maze.filename);
            break;
            
        case 7: // F7 - Temizle
            clearMaze();
            setStatus("Labirent temizlendi.");
            break;
            
        case 8: // F8 - En kısa yolu bul
            maze.solving = true;
            maze.prevMode = maze.mode;
            if (solveMaze()) {
                maze.solved = true;
                maze.mode = MODE_SOLVE;
                setStatus("En kisa yol bulundu!");
            } else {
                setStatus("Cozum bulunamadi! Yol yok.");
            }
            maze.solving = false;
            break;
            
        case 9: // F9 - Rastgele labirent
            generateRandomMaze();
            setStatus("Rastgele labirent olusturuldu.");
            break;
            
        case 10: // F10 - Yeni boyut
            promptNewSize();
            break;
    }
}

// Hücre işlemleri
void setPath(int x, int y) {
    if (x >= 0 && x < maze.width && y >= 0 && y < maze.height) {
        if (maze.cells[y][x] == CELL_WALL) {
            maze.cells[y][x] = CELL_PATH;
        }
    }
}

void erasePath(int x, int y) {
    if (x >= 0 && x < maze.width && y >= 0 && y < maze.height) {
        char cell = maze.cells[y][x];
        if (cell == CELL_PATH) {
            maze.cells[y][x] = CELL_WALL;
        }
    }
}

void placeWalls(void) {
    // Yol olmayan ve başlangıç/bitiş olmayan tüm hücreleri duvar yap
    for (int y = 0; y < maze.height; y++) {
        for (int x = 0; x < maze.width; x++) {
            if (maze.cells[y][x] == CELL_EMPTY) {
                maze.cells[y][x] = CELL_WALL;
            }
        }
    }
}

void clearMaze(void) {
    for (int y = 0; y < maze.height; y++) {
        for (int x = 0; x < maze.width; x++) {
            maze.cells[y][x] = CELL_WALL;
            maze.solution[y][x] = CELL_EMPTY;
        }
    }
    
    maze.cells[maze.start.y][maze.start.x] = CELL_START;
    maze.cells[maze.end.y][maze.end.x] = CELL_END;
    maze.cursor.x = 1;
    maze.cursor.y = maze.start.y;
    maze.solved = false;
}

void clearSolution(void) {
    for (int y = 0; y < maze.height; y++) {
        for (int x = 0; x < maze.width; x++) {
            maze.solution[y][x] = CELL_EMPTY;
        }
    }
}

// Rastgele labirent oluşturma (DFS algoritması)
void generateRandomMaze(void) {
    // Tüm hücreleri duvar yap
    for (int y = 0; y < maze.height; y++) {
        for (int x = 0; x < maze.width; x++) {
            maze.cells[y][x] = CELL_WALL;
        }
    }
    
    bool visited[MAX_HEIGHT][MAX_WIDTH] = {false};
    
    // Başlangıç noktasından DFS
    int startX = 1;
    int startY = maze.start.y;
    
    generateMazeDFS(startX, startY, visited);
    
    // Başlangıç ve bitiş noktalarını ayarla
    maze.cells[maze.start.y][maze.start.x] = CELL_START;
    maze.cells[maze.end.y][maze.end.x] = CELL_END;
    
    // Başlangıç ve bitişe bağlantı sağla
    maze.cells[maze.start.y][1] = CELL_PATH;
    maze.cells[maze.end.y][maze.width - 2] = CELL_PATH;
    
    maze.cursor.x = 1;
    maze.cursor.y = maze.start.y;
    maze.solved = false;
    clearSolution();
}

void generateMazeDFS(int x, int y, bool visited[MAX_HEIGHT][MAX_WIDTH]) {
    visited[y][x] = true;
    maze.cells[y][x] = CELL_PATH;
    
    // Yönler: yukarı, aşağı, sol, sağ (2 adım)
    int dirs[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
    
    // Yönleri karıştır
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int tempX = dirs[i][0], tempY = dirs[i][1];
        dirs[i][0] = dirs[j][0]; dirs[i][1] = dirs[j][1];
        dirs[j][0] = tempX; dirs[j][1] = tempY;
    }
    
    // Her yöne git
    for (int i = 0; i < 4; i++) {
        int nx = x + dirs[i][0];
        int ny = y + dirs[i][1];
        
        if (nx > 0 && nx < maze.width - 1 && ny > 0 && ny < maze.height - 1 && !visited[ny][nx]) {
            // Aradaki duvarı kaldır
            int midX = x + dirs[i][0] / 2;
            int midY = y + dirs[i][1] / 2;
            maze.cells[midY][midX] = CELL_PATH;
            generateMazeDFS(nx, ny, visited);
        }
    }
}

// BFS ile en kısa yol bulma
bool solveMaze(void) {
    clearSolution();
    return solveMazeBFS();
}

bool solveMazeBFS(void) {
    typedef struct {
        int x, y;
    } Point;
    
    Point queue[MAX_WIDTH * MAX_HEIGHT];
    int parent[MAX_HEIGHT][MAX_WIDTH][2];
    bool visited[MAX_HEIGHT][MAX_WIDTH];
    
    memset(visited, false, sizeof(visited));
    memset(parent, -1, sizeof(parent));
    
    int front = 0, rear = 0;
    
    // Başlangıç noktasının yanından başla
    int startX = maze.start.x + 1;
    int startY = maze.start.y;
    
    // Eğer başlangıç noktasının yanı duvarsa, başlangıcın kendisinden başla
    if (maze.cells[startY][startX] == CELL_WALL) {
        startX = maze.start.x;
    }
    
    queue[rear].x = startX;
    queue[rear].y = startY;
    rear++;
    visited[startY][startX] = true;
    
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    
    while (front < rear) {
        Point current = queue[front++];
        
        // Bitiş noktasına veya yanına ulaştık mı?
        if ((current.x == maze.end.x && current.y == maze.end.y) ||
            (current.x == maze.end.x - 1 && current.y == maze.end.y)) {
            
            // Yolu geri izle
            int px = current.x, py = current.y;
            while (parent[py][px][0] != -1) {
                maze.solution[py][px] = CELL_SOLUTION;
                int tempX = parent[py][px][0];
                int tempY = parent[py][px][1];
                px = tempX;
                py = tempY;
            }
            maze.solution[py][px] = CELL_SOLUTION;
            return true;
        }
        
        // Komşuları kontrol et
        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            
            if (nx >= 0 && nx < maze.width && ny >= 0 && ny < maze.height &&
                !visited[ny][nx]) {
                char cell = maze.cells[ny][nx];
                if (cell != CELL_WALL) {
                    visited[ny][nx] = true;
                    parent[ny][nx][0] = current.x;
                    parent[ny][nx][1] = current.y;
                    queue[rear].x = nx;
                    queue[rear].y = ny;
                    rear++;
                }
            }
        }
    }
    
    return false;
}

// Dosya işlemleri
void saveMaze(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        setStatus("Dosya kaydedilemedi!");
        return;
    }
    
    fprintf(file, "%d %d\n", maze.width, maze.height);
    fprintf(file, "%d %d\n", maze.start.x, maze.start.y);
    fprintf(file, "%d %d\n", maze.end.x, maze.end.y);
    
    for (int y = 0; y < maze.height; y++) {
        for (int x = 0; x < maze.width; x++) {
            fputc(maze.cells[y][x], file);
        }
        fputc('\n', file);
    }
    
    fclose(file);
    
    char msg[128];
    snprintf(msg, sizeof(msg), "Kaydedildi: %s", filename);
    setStatus(msg);
}

void loadMaze(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        setStatus("Dosya bulunamadi!");
        return;
    }
    
    int w, h;
    if (fscanf(file, "%d %d\n", &w, &h) != 2) {
        fclose(file);
        setStatus("Dosya formati hatali!");
        return;
    }
    
    if (w > MAX_WIDTH) w = MAX_WIDTH;
    if (h > MAX_HEIGHT) h = MAX_HEIGHT;
    
    maze.width = w;
    maze.height = h;
    
    if (fscanf(file, "%d %d\n", &maze.start.x, &maze.start.y) != 2) {
        fclose(file);
        setStatus("Dosya formati hatali!");
        return;
    }
    
    if (fscanf(file, "%d %d\n", &maze.end.x, &maze.end.y) != 2) {
        fclose(file);
        setStatus("Dosya formati hatali!");
        return;
    }
    
    for (int y = 0; y < maze.height; y++) {
        for (int x = 0; x < maze.width; x++) {
            int c = fgetc(file);
            if (c == EOF || c == '\n') {
                maze.cells[y][x] = CELL_WALL;
            } else {
                maze.cells[y][x] = (char)c;
            }
        }
        // Satır sonunu atla
        int c;
        while ((c = fgetc(file)) != '\n' && c != EOF);
    }
    
    fclose(file);
    
    maze.cursor.x = 1;
    maze.cursor.y = maze.start.y;
    maze.solved = false;
    clearSolution();
    
    char msg[128];
    snprintf(msg, sizeof(msg), "Yuklendi: %s", filename);
    setStatus(msg);
}

void promptNewSize(void) {
    showCursor();
    clearScreen();
    
    int newWidth, newHeight;
    
    printf(FG_CYAN "=== Yeni Labirent Boyutu ===" RESET "\n\n");
    printf("Mevcut boyut: %d x %d\n", maze.width, maze.height);
    printf("Maksimum boyut: %d x %d\n\n", MAX_WIDTH, MAX_HEIGHT);
    
    printf("Yeni genislik (10-%d): ", MAX_WIDTH);
    fflush(stdout);
    
    // Terminal modunu geçici olarak değiştir
    struct termios old, new;
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    
    if (scanf("%d", &newWidth) != 1 || newWidth < 10 || newWidth > MAX_WIDTH) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        hideCursor();
        clearScreen();
        setStatus("Gecersiz genislik!");
        return;
    }
    
    printf("Yeni yukseklik (5-%d): ", MAX_HEIGHT);
    fflush(stdout);
    
    if (scanf("%d", &newHeight) != 1 || newHeight < 5 || newHeight > MAX_HEIGHT) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        hideCursor();
        clearScreen();
        setStatus("Gecersiz yukseklik!");
        return;
    }
    
    // Temizle
    while (getchar() != '\n');
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    hideCursor();
    clearScreen();
    
    initMaze(newWidth, newHeight);
    
    char msg[128];
    snprintf(msg, sizeof(msg), "Yeni boyut: %d x %d", newWidth, newHeight);
    setStatus(msg);
}

// Ana fonksiyon
int main(void) {
    srand(time(NULL));
    
    initTerminal();
    atexit(resetTerminal);
    
    hideCursor();
    clearScreen();
    
    initMaze(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    // Ana döngü
    while (maze.running) {
        drawScreen();
        
        int key = getKey();
        if (key != -1) {
            handleInput(key);
        }
        
        usleep(30000); // 30ms gecikme
    }
    
    clearScreen();
    printf("Labirent programindan ciktiniz. Gorusmek uzere!\n");
    
    return 0;
}
