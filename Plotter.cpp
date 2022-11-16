#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <stdbool.h>
#include <iomanip>

#define WIDTH 25
#define HEIGHT 25

#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))

typedef struct {
    int x, y;
    char symbol;
} Point;

float calculate_slope(Point p1, Point p2) {
    return (max(p1.y, p2.y) - min(p1.y, p2.y)) / (max(p1.x, p2.x) - min(p1.x, p2.x));
}

float calculate_y_intercept(Point p, float m) {
    return p.y - m * p.x;
}

void plot(Point p, char *canvas) {
    canvas[p.y * WIDTH + p.x] = p.symbol;
}

void plot_line(Point p1, Point p2, char* canvas) {
    int dx = abs(p2.x - p1.x);
    int sx = p1.x < p2.x ? 1 : -1;
    int dy = -abs(p2.y - p1.y);
    int sy = p1.y < p2.y ? 1 : -1;
    int error = dx + dy;
    
    int calc_x = p1.x;
    int calc_y = p1.y;

    while (true) {
        plot(Point{calc_x, calc_y, '.'}, canvas);

        if (calc_x == p2.x && calc_y == p2.y) break;
        int e2 = 2 * error;

        if (e2 >= dy) {
            if (calc_x == p2.x) break;
            error += dy;
            calc_x += sx;
        }
        if (e2 <= dx) {
            if (calc_y == p2.y) break;
            error += dx;
            calc_y += sy;
        }
    }
}

void render(char canvas[]) {
    int y_v = HEIGHT - 1;
    for (int x = 0; x < WIDTH ; ++x) {
        if (y_v < 10) {
            printf("%i  |", y_v);
        }
        else {
            printf("%i |", y_v);
        }

        y_v -= 1;

        for (int y = 0; y < WIDTH; ++y) {
             printf(" %c ", canvas[x * WIDTH + y]);
        }
        printf("%c", '\n');
    }

    printf("    ");

    for (int x = 0; x < WIDTH * 3; ++x) {
        printf("-"); 
    }

    printf("\n");
    printf("     ");

    for (int x = 0; x < WIDTH; ++x) {
        if (x < 10) {
            printf(" %i ", x);
        } else if (x < 100 && x >= 10) {
            printf("%i ", x);
        } else if (x < 1000 && x >= 100) {
            printf("%i", x);
        }
    }
}

int main()
{
    char canvas[WIDTH*HEIGHT];

    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        canvas[i] = ' ';
    }

    Point points[] = { { 10, 10, '0' }, { 15, 17, '0' }, { 19, 5, '0' }, { 21, 22, '0' } };
    Point toPlot1 = { 1, 22, '0' };
    Point toPlot2 = { 9, 17, '0' };

    plot_line(toPlot1, toPlot2, canvas);
    plot_line(toPlot2, points[0], canvas);

    for (int i = 0; i < my_sizeof(points) / my_sizeof(points[0]); ++i) {
        if (i > 0) {
            plot_line(points[i - 1], points[i], canvas);
        }
    }

    for (int i = 0; i < my_sizeof(points) / my_sizeof(points[0]); ++i) {
        plot(points[i], canvas);
    }

    plot(toPlot1, canvas);
    plot(toPlot2, canvas);

    render(canvas);
}
