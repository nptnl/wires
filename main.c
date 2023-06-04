#include <stdio.h>
#include <math.h>

static int DIMX = 16;
static int DIMY = 8;

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
    for (int line = DIMY-1; line > 0; line -= 1) {
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
    { 2., 2. },
    { 13., 3. },
};

void grad_line(vx2 c1, vx2 c2, char canvas[DIMY][DIMX*2+1]) {
    double slope = (c2.y - c1.y) / (c2.x - c1.x);
    vx2 current = c1;
    while (current.x < c2.x) {
        current.x += 1.;
        current.y += slope;
        add_point(current, canvas);
    }
}
void steep_line(vx2 c1, vx2 c2, char canvas[DIMY][DIMX*2+1]) {
    double slope = (c2.x - c1.x) / (c2.y - c1.y);
    vx2 current = c1;
    while (current.y < c2.y) {
        current.y += 1.;
        current.x += slope;
        add_point(current, canvas);
    }
}

void make_line(vx2 c1, vx2 c2, char canvas[DIMY][DIMX*2+1]) {
    vx2 diff = {c2.x - c1.x, c2.y - c1.y};
    if (diff.y > -diff.x) {
        if (diff.y > diff.x) {
            steep_line(c1, c2, canvas);
        } else {
            grad_line(c1, c2, canvas);
        }
    } else {
        if (diff.y > diff.x) {
            grad_line(c2, c1, canvas);
        } else {
            steep_line(c2, c1, canvas);
        }
    }
}

int main() {
    char canvas[DIMY][DIMX * 2 + 1];
    make_spaces(canvas);
    make_line(coords[0], coords[1], canvas);
    display(canvas);
    return 0;
}