#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Game area size
#define WIDTH 20
#define HEIGHT 20

int carPos = WIDTH / 2;
int obsX = 5, obsY = 0;
int score = 0;

// ----------- Non-blocking key detection (cross-platform) ----------
int kbhit(void) {
    struct termios oldt, newt;
    int ch, oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// ---------------------- Draw screen ------------------------
void draw() {
    system("clear"); // use "cls" for Windows

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            if (x == 0 || x == WIDTH - 1)
                printf("|");                 // WALLS
            else if (y == HEIGHT - 2 && x == carPos)
                printf("A");                 // CAR
            else if (y == obsY && x == obsX)
                printf("O");                 // OBSTACLE
            else
                printf(" ");
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
}

// ---------------------- Update game -----------------------
void update() {

    if (kbhit()) {
        char key = getchar();
        if (key == 'a' && carPos > 1) carPos--;
        if (key == 'd' && carPos < WIDTH - 2) carPos++;
    }

    // Move obstacle
    obsY++;

    if (obsY >= HEIGHT - 2) {
        if (obsX == carPos) {
            printf("\n💥 Crash! Game Over\n");
            printf("Final Score: %d\n", score);
            exit(0);
        }

        obsY = 0;
        obsX = rand() % (WIDTH - 2) + 1;
        score++;
    }
}

// ------------------------ MAIN ----------------------------
int main() {
    while (1) {
        draw();
        update();
        usleep(100000); // 100 ms
    }

    return 0;
}
