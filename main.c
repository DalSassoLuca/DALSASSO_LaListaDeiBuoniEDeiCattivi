#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define R 0
#define W 1

void elfoBuoni(int N, int w) {
    int buoni = (N + 1) / 2;
    printf("Elfo 1 (Buoni): trovati %d\n", buoni);
    write(w, &buoni, sizeof(buoni));
    close(w);
    _exit(0);
}

void elfoCattivi(int N, int w) {
    int cattivi = N / 2;
    printf("Elfo 2 (Cattivi): trovati %d\n", cattivi);
    write(w, &cattivi, sizeof(cattivi));
    close(w);
    _exit(0);
}

int main() {

    int N;
    printf("Inserisci N: ");
    if (scanf("%d", &N) != 1 || N <= 0) return 1;

    int in[2], outB[2], outC[2];
    pipe(in); pipe(outB); pipe(outC);

    pid_t p1 = fork();
    if (p1 == 0) {
        close(in[W]); close(outB[R]); close(outC[R]); close(outC[W]);
        read(in[R], &N, sizeof(N));
        close(in[R]);
        elfoBuoni(N, outB[W]);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        close(in[W]); close(outC[R]); close(outB[R]); close(outB[W]);
        read(in[R], &N, sizeof(N));
        close(in[R]);
        elfoCattivi(N, outC[W]);
    }

    close(in[R]);
    write(in[W], &N, sizeof(N));
    write(in[W], &N, sizeof(N));
    close(in[W]);

    close(outB[W]); close(outC[W]);

    int regali = 0, carbone = 0;
    read(outB[R], &regali, sizeof(regali));
    read(outC[R], &carbone, sizeof(carbone));
    close(outB[R]); close(outC[R]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    printf("Quest'anno consegnerò %d regali e %d pezzi di carbone!\n", regali, carbone);
    return 0;
}
