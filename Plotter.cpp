#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <stdbool.h>
#include <iomanip>
#include <iostream>
#include <vector>

#define WIDTH 50
#define HEIGHT 50

#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))

using std::vector;

struct Point {
    int x, y;
    char symbol;
};

struct Vector3 {
    float x, y, z;
};

struct Triangle {
    Vector3 p[3];
};

struct Mesh {
    vector<Triangle> triangles;
};

struct Matrix4 {
    float m[4][4] = {};
};

float calculate_y_intercept(Point p, float m) {
    return p.y - m * p.x;
}

void plot(Point p, char *canvas) {
    canvas[p.y * WIDTH + p.x] = p.symbol;
}

void clear(void) {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
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

void plot_triangle(Triangle triangel, char* canvas) {
    Point p1 = { triangel.p[0].x, triangel.p[0].y, '0'};
    Point p2 = { triangel.p[1].x, triangel.p[1].y, '0' };
    Point p3 = { triangel.p[2].x, triangel.p[2].y, '0' };

    plot_line(p1, p2, canvas);
    plot_line(p2, p3, canvas);
    plot_line(p1, p3, canvas);
}

void render(char canvas[]) {
    //int y_v = 1;
    for (int x = 0; x < WIDTH ; ++x) {
        //if (y_v < 10) {
        //    printf("%i  |", y_v);
        //}
        //else {
        //    printf("%i |", y_v);
        //}

        //y_v += 1;

        for (int y = 0; y < HEIGHT; ++y) {
             printf(" %c ", canvas[x * WIDTH + y]);
        }
        printf("%c", '\n');
    }

    //printf("    ");

    //for (int x = 0; x < WIDTH * 3; ++x) {
    //    printf("-"); 
    //}

    printf("\n");
    //printf("     ");

    //for (int x = 0; x < WIDTH; ++x) {
    //    if (x < 10) {
    //        printf(" %i ", x);
    //    } else if (x < 100 && x >= 10) {
    //        printf("%i ", x);
    //    } else if (x < 1000 && x >= 100) {
    //        printf("%i", x);
    //    }
    //}
}

void clear_canvas(char* canvas) {
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        canvas[i] = ' ';
    }
}

void multiply_by_matrix(Vector3& vec, Vector3& out, Matrix4& m) {
    out.x = vec.x * m.m[0][0] + vec.y * m.m[1][0] + vec.z * m.m[2][0] + m.m[3][0];
    out.y = vec.x * m.m[0][1] + vec.y * m.m[1][1] + vec.z * m.m[2][1] + m.m[3][1];
    out.z = vec.x * m.m[0][2] + vec.y * m.m[1][2] + vec.z * m.m[2][2] + m.m[3][2];
    float w = vec.x * m.m[0][3] + vec.y * m.m[1][3] + vec.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f) {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
}

int main(void)
{
    char canvas[WIDTH*HEIGHT];

    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        canvas[i] = ' ';
    }
    Triangle tri1 = { 0, 0, 0,   0, 10, 0,    10, 10, 0 };

    Mesh cubeMesh;

    cubeMesh.triangles = {
        // South
        {0.0f, 0.0f, 0.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f},

        // East
        {1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 0.0f,     1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f},

        // North
        {1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f},

        // West
        {0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f},

        // Top
        {0.0f, 1.0f, 0.0f,     0.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f},

        // Bottom
        {1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f},
    };

    // Projection Matrix
    Matrix4 projectionMatrix;
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRation = (float)HEIGHT / (float)WIDTH;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    projectionMatrix.m[0][0] = fAspectRation * fFovRad;
    projectionMatrix.m[1][1] = fFovRad;
    projectionMatrix.m[2][2] = fFar / (fFar - fNear);
    projectionMatrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    projectionMatrix.m[2][3] = 1.0f;
    projectionMatrix.m[3][3] = 0.0f;

    Matrix4 rotateZ, rotateX;
    float rotationAngle = 1.0f;

    for (int i = 0; i < 10; ++i) {
        clear_canvas(canvas);

        rotationAngle += 1.0f;

        //Rotation Matricies
        // 
        // Rotation Z
        rotateZ.m[0][0] = cosf(rotationAngle);
        rotateZ.m[0][1] = sinf(rotationAngle);
        rotateZ.m[1][0] = -sinf(rotationAngle);
        rotateZ.m[1][1] = cosf(rotationAngle);
        rotateZ.m[2][2] = 1;
        rotateZ.m[3][3] = 1;

        // Rotation X
        rotateX.m[0][0] = 1;
        rotateX.m[0][1] = cosf(rotationAngle * 0.5f);
        rotateX.m[1][0] = sinf(rotationAngle * 0.5f);
        rotateX.m[1][1] = -sinf(rotationAngle * 0.5f);
        rotateX.m[2][2] = cosf(rotationAngle * 0.5f);
        rotateX.m[3][3] = 1;

        for (auto tri : cubeMesh.triangles) {
            Triangle projected, translated, rotatedZ, rotatedZX;

            // Rotation
            multiply_by_matrix(tri.p[0], rotatedZ.p[0], rotateZ);
            multiply_by_matrix(tri.p[1], rotatedZ.p[1], rotateZ);
            multiply_by_matrix(tri.p[2], rotatedZ.p[2], rotateZ);

            //multiply_by_matrix(rotatedZ.p[0], rotatedZX.p[0], rotateX);
            //multiply_by_matrix(rotatedZ.p[1], rotatedZX.p[1], rotateX);
            //multiply_by_matrix(rotatedZ.p[2], rotatedZX.p[2], rotateX);

            // Translating
            translated = rotatedZ;
            translated.p[0].z = rotatedZ.p[0].z + 3.0f;
            translated.p[1].z = rotatedZ.p[1].z + 3.0f;
            translated.p[2].z = rotatedZ.p[2].z + 3.0f;

            // Projecting 3d triangle to 2d screen
            multiply_by_matrix(translated.p[0], projected.p[0], projectionMatrix);
            multiply_by_matrix(translated.p[1], projected.p[1], projectionMatrix);
            multiply_by_matrix(translated.p[2], projected.p[2], projectionMatrix);

            // Scale into view
            projected.p[0].x += 1.0f; projected.p[0].y += 1.0f;
            projected.p[1].x += 1.0f; projected.p[1].y += 1.0f;
            projected.p[2].x += 1.0f; projected.p[2].y += 1.0f;

            projected.p[0].x *= 0.5f * (float)WIDTH; projected.p[0].y *= 0.5f * (float)HEIGHT;
            projected.p[1].x *= 0.5f * (float)WIDTH; projected.p[1].y *= 0.5f * (float)HEIGHT;
            projected.p[2].x *= 0.5f * (float)WIDTH; projected.p[2].y *= 0.5f * (float)HEIGHT;

            plot_triangle(projected, canvas);

            //plot_line({ (int)projected.p[0].x, (int)projected.p[0].y, '#' }, { (int)projected.p[1].x, (int)projected.p[1].y, '#' }, canvas);
            //plot_line({ (int)projected.p[1].x, (int)projected.p[1].y, '#' }, { (int)projected.p[2].x, (int)projected.p[2].y, '#' }, canvas);
            //plot_line({ (int)projected.p[0].x, (int)projected.p[0].y, '#' }, { (int)projected.p[2].x, (int)projected.p[2].y, '#' }, canvas);

            plot({ (int)projected.p[0].x, (int)projected.p[0].y, '#' }, canvas);
            plot({ (int)projected.p[1].x, (int)projected.p[1].y, '#' }, canvas);
            plot({ (int)projected.p[2].x, (int)projected.p[2].y, '#' }, canvas);

            //printf("x: %f, y: %f  |  x: %f, y: %f  |  x: %f, y: %f\n", projected.p[0].x, projected.p[0].y, projected.p[1].x, projected.p[1].y, projected.p[2].x, projected.p[2].y);
        }
        
        clear();
        render(canvas);
        Sleep(5);
    }

    //Point points[] = { { 10, 10, '0' }, { 15, 17, '0' }, { 19, 5, '0' }, { 21, 22, '0' } };
    //Point toPlot1 = { 1, 24, '0' };
    //Point toPlot2 = { 4, 18, '0' };
    //Point toPlot3 = { 8, 24, '0' };

    //Point toPlot4 = { 9, 24, '0' };
    //Point toPlot5 = { 12, 18, '0' };
    //Point toPlot6 = { 16, 24, '0' };

    //plot_line(toPlot1, toPlot2, canvas);
    //plot_line(toPlot2, points[0], canvas);

    //for (int i = 0; i < my_sizeof(points) / my_sizeof(points[0]); ++i) {
    //    if (i > 0) {
    //        plot_line(points[i - 1], points[i], canvas);
    //    }
    //}

    //for (int i = 0; i < my_sizeof(points) / my_sizeof(points[0]); ++i) {
    //    plot(points[i], canvas);
    //}

    //plot_triangle(toPlot1, toPlot2, toPlot3, canvas);
    //plot_triangle(toPlot4, toPlot5, toPlot6, canvas);


    //plot(toPlot1, canvas);
    //plot(toPlot2, canvas);
    //plot(toPlot3, canvas);

    //plot(toPlot4, canvas);
    //plot(toPlot5, canvas);
    //plot(toPlot6, canvas);

    
}
