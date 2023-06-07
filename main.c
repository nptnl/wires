#include <stdio.h>
#include <math.h>
#include <unistd.h>

static int DIMX = 64;
static int DIMY = 64;
static double FOCAL = 64;

typedef struct {
    double x;
    double y;
} vx2;
typedef struct {
    double x;
    double y;
    double z;
} vx3;

vx2 project(vx3 point) {
    double xs = 0.5 * DIMX;
    double ys = 0.5 * DIMY;
    double multi = FOCAL / (point.z + FOCAL);
    vx2 result = { (point.x - xs) * multi + xs, (point.y - ys) * multi + ys };
    return result;
}

void list_vx2(vx2 point) {
    printf("%f, %f\n", point.x, point.y);
}
void list_vx3(vx3 point) {
    printf("%f, %f, %f\n", point.x, point.y, point.z);
}
void list_proj(vx3 point) {
    vx2 result = project(point);
    printf("%f, %f\n", result.x, result.y);
}

void make_spaces(char empty[DIMY][DIMX*2+1]) {
    for (int indy = 0; indy < DIMY; indy ++) {
        for (int indx = 0; indx < 2 * DIMX; indx ++) {
            empty[indy][indx] = ' ';
        }
        empty[indy][DIMX * 2] = '\0';
    }
}

void display(char canvas[DIMY][DIMX*2+1]) {
    for (int line = DIMY-1; line >= 0; line -= 1) {
        printf("%s\n", canvas[line]);
    }
}

int within_range(vx2 point) {
    if (point.x < DIMX
    && point.y < DIMY
    && point.x > 0
    && point.y > 0) {
        return 0;
    }
    return 1;
}

void add_point(vx2 point, char canvas[DIMY][DIMX*2+1], char rep1, char rep2) {
    if (within_range(point) != 0) { return; }
    canvas[(int) round(point.y)][(int) round(point.x) * 2] = rep1;
    canvas[(int) round(point.y)][(int) round(point.x) * 2 + 1] = rep2;
}
void blank_point(vx2 point, char canvas[DIMY][DIMX*2+1]) {
    add_point(point, canvas, '[', ']');
}

void grad_line(vx2 c1, vx2 c2, char canvas[DIMY][DIMX*2+1]) {
    double slope = (c2.y - c1.y) / (c2.x - c1.x);
    vx2 current = c1;
    while (current.x < c2.x) {
        current.x += 1.;
        current.y += slope;
        blank_point(current, canvas);
    }
}
void steep_line(vx2 c1, vx2 c2, char canvas[DIMY][DIMX*2+1]) {
    double slope = (c2.x - c1.x) / (c2.y - c1.y);
    vx2 current = c1;
    while (current.y < c2.y) {
        current.y += 1.;
        current.x += slope;
        blank_point(current, canvas);
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

typedef struct {
    vx2 left;
    vx2 mid;
    vx2 right;
} tri2;
typedef struct {
    vx3 p1;
    vx3 p2;
    vx3 p3;
} tri3;

void order(tri2 set) {
    vx2 left = set.left;
    vx2 right = set.left;
    if (set.mid.x < left.x) { left = set.mid; } else { right = set.mid; }
    if (set.right.x < left.x) { left = set.right; }
    else if (set.right.x > right.x) { right = set.right; }
}
tri2 tri_project(tri3 set) {
    tri2 ret = { project(set.p1), project(set.p2), project(set.p3) };
    order(ret);
    return ret;
}

// void draw_poly(tri3 set, char canvas[DIMY][DIMX*2+1]) {
//     tri2 proj = tri_project(set);
//     double slope_ac = (proj.right.y - proj.left.y) / (proj.right.x - proj.left.x);
    
// }

vx3 inc_ref(vx3 point) {
    double place = (point.x - 32.) + 0.001 * (32. - point.z);
    point.z -= 0.001 * (32. - point.x);
    point.x = place + 32.;
    return point;
}

int main() {
    char canvas[DIMY][DIMX * 2 + 1];

    vx3 cube[8] = {
        { 16., 16., 16. }, // bottom front left
        { 16., 16., 48. }, // bottom back left
        { 48., 16., 48. }, // bottom back right
        { 48., 16., 16. }, // bottom front right
        { 16., 48., 16. }, // top front left
        { 16., 48., 48. }, // top back left
        { 48., 48., 48. }, // top back right
        { 48., 48., 16. }, // top front right
    };
    int cnxns[24] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    };

    while (0 == 0) {
        make_spaces(canvas);
        vx2 prj[8];
        for (int indx = 0; indx < 8; indx ++) {
            cube[indx] = inc_ref(cube[indx]);
            prj[indx] = project(cube[indx]);
        }
        for (int pair = 0; pair < 24; pair += 2) {
            make_line(prj[cnxns[pair]], prj[cnxns[pair+1]], canvas);
        }
        display(canvas);
        usleep(1000);
    }


    return 0;
}