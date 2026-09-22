#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <stdlib.h>
#include <math.h>
#include <debug.h>
#include <time.h>
#include "main.h"
#include "lookup.h"

struct Player player = {
    .x = 2.5,
    .y = 7,
    .angle = 270,
    .speed = 0.05,
    .rotateSpeed = 15,
    .fov = 60
};


int main(void) {
    // Clears screen then initializes the graphics library
    os_ClrHome();
    gfx_Begin();

    // Draws the initial image
    rayCast();

    int key;
    do {
        // Gets the current key pressed
        key = os_GetCSC();

        // Moves the player based on keycode and redraws the frame
        switch (key) {
            case sk_Up:
                movePlayer(1);
                rayCast();
                break;
            case sk_Down:
                movePlayer(-1);
                rayCast();
                break;
            case sk_Left:
                rotatePlayer(-1);
                rayCast();
                break;
            case sk_Right:
                rotatePlayer(1);
                rayCast();
                break;
        };

    } while(key != sk_Mode && key != sk_Clear);

    // turns off graphics mode
    gfx_End();
    return 0;
}

double degToRad(double deg) {
    return deg * RAD;
}

unsigned int root(unsigned int x) {
    // Code from: https://www.mikrocontroller.net/attachment/206695/Fast_square_root_in_C.pdf
    unsigned int a, b;
    b = x;
    a = x = 0x3f;
    x = b / x;
    a = x = (x + a) >> 1;
    x = b / x;
    a = x = (x + a) >> 1;
    x = b / x;
    x = (x + a) >> 1;
    return(x);
}

long fixedCosLookUp(int rad) {
    return cosTable[rad % 360];
}

long fixedSinLookUp(int rad) {
    return sinTable[rad % 360];
}

int doubleToFixed(double d) {
    long fixed = 0;
    double fPart, iPart;

    fPart = modf(d, &iPart);
    fixed += (long)iPart << SHIFT;

    // Not entirely ideal because we can technically get more precision but this is easier
    fixed += ((long)(fPart * 10000) << SHIFT) / 10000;

    return fixed;
}

double fixedToDouble(long fixed) {
    double fPart, iPart;
    fPart = (double)(fixed & SHIFT_MASK) / (1 << SHIFT);
    iPart = fixed >> SHIFT;
    return iPart + fPart;
}

void rayCast() {
    // Profiling stuff
    clock_t start_t, end_t, test_start_t, test_end_t;
    double aTime = 0;
    double bTime = 0;
    double cTime = 0;
    start_t = clock();

    drawBackground();

    double rayAngle = player.angle - player.fov / 2.0;
    int rayStartX = doubleToFixed(player.x);
    int rayStartY = doubleToFixed(player.y);

    for (int rayCount = 0; rayCount < SCREEN_W; rayCount++) {
        int rayX = rayStartX;
        int rayY = rayStartY;

        test_start_t = clock();

        // Fixed point numbers
        long rayCos = fixedCosLookUp(rayAngle);
        long raySin = fixedSinLookUp(rayAngle);
        test_end_t = clock();
        aTime += (double)(test_end_t - test_start_t) / CLOCKS_PER_SEC;

        test_start_t = clock();
        int wall = 0;
        while (!wall) {
            rayX += rayCos;
            rayY += raySin;

            wall = MAP[rayY >> SHIFT][rayX >> SHIFT];
        }
        test_end_t = clock();
        bTime += (double)(test_end_t - test_start_t) / CLOCKS_PER_SEC;

        test_start_t = clock();
        double distanceSq = pow(player.x - fixedToDouble(rayX), 2) + pow(player.y - fixedToDouble(rayY), 2);

        int wallHeight = root( SQ_HALF_SCREEN_H / distanceSq );
        gfx_VertLine(rayCount, HALF_SCREEN_H - wallHeight / 2, wallHeight);

        rayAngle += RAYCAST_INC;
        test_end_t = clock();
        cTime += (double)(test_end_t - test_start_t) / CLOCKS_PER_SEC;
    }
    end_t = clock();

    double timeTaken = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    dbg_printf("Time taken: %.2f\n", timeTaken);
    dbg_printf("A time: %.2f\n", aTime);
    dbg_printf("B time: %.2f\n", bTime);
    dbg_printf("C time: %.2f\n", cTime);
}

void drawBackground() {
    gfx_SetColor(TOP_COLOR);
    gfx_FillRectangle(0, 0, SCREEN_W, SCREEN_H / 2);
    gfx_SetColor(BOTTOM_COLOR);
    gfx_FillRectangle(0, SCREEN_H / 2, SCREEN_W, SCREEN_H / 2);
    gfx_SetColor(WALL_COLOR);
}

void movePlayer(int direction) {
    double newX = player.x;
    double newY = player.y;

    double playerCos = cos(degToRad(player.angle));
    double playerSin = sin(degToRad(player.angle));

    newX += playerCos * direction;
    newY += playerSin * direction;

    if (!MAP[(int)newY][(int)newX]) {
        player.x = newX;
        player.y = newY;
    }
}

void rotatePlayer(int direction) {
    player.angle += player.rotateSpeed * direction;
}
