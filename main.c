#include <stdio.h>
#include <math.h>

static int DIMX = 8;
static int DIMY = 4;

typedef struct {
    double x;
    double y;
} vx2;

void make_spaces(char empty[DIMY][DIMX*2+1]) {
    for (int indy = 0; indy < DIMY; indy ++) {
        for (int indx = 0; indx < 2 * DIMX; indx ++) {
            empty[indy][indx] = ' ';
        }
        empty[indy][DIMX * 2] = '\0';
    }
}

void display(char canvas[DIMY][DIMX*2+1]) {
    for (int line = 0; line < DIMY; line ++) {
        printf("%s\n", canvas[line]);
    }
}

int within_range(vx2 point) {
    if (point.x < DIMX && point.y < DIMY) {
        return 0;
    }
    return 1;
}

void add_point(vx2 point, char canvas[DIMY][DIMX*2+1]) {
    if (within_range(point) != 0) { return; }
    canvas[(int) round(point.y)][(int) round(point.x) * 2] = '[';
    canvas[(int) round(point.y)][(int) round(point.x) * 2 + 1] = ']';
}

static vx2 coords[] = {
    { 1., 2. },
    { 5., 3. },
};

int main() {
    char canvas[DIMY][DIMX * 2 + 1];
    make_spaces(canvas);
    for (int indx = 0; indx < 2; indx ++) {
        add_point(coords[indx], canvas);
    }
    display(canvas);
    return 0;
}