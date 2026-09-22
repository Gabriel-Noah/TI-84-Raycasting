#include <math.h>

/***********************
 * Function prototypes *
 ***********************/
double degToRad(double);
long fixedCosLookUp(int);
long fixedSinLookUp(int);
int doubleToFixed(double);
double fixedToDouble(long);
void rayCast();
void drawBackground();
void movePlayer(int);
void rotatePlayer(int);

/*************
 * Constants *
 *************/
const int SCREEN_W = 320;
const int SCREEN_H = 240;
const int HALF_SCREEN_H = SCREEN_H / 2;
const int SQ_HALF_SCREEN_H = HALF_SCREEN_H * HALF_SCREEN_H;
const double RAYCAST_INC = 60.0 / SCREEN_W;
const int WALL_COLOR = 224;
const int TOP_COLOR = 30;                   // Color code for blue
const int BOTTOM_COLOR = 70;                // Color code for green
const double RAD = M_PI / 180;
// Fixed point stuff
const int SHIFT = 16;
const int SHIFT_MASK = ((1 << SHIFT) - 1);

// Player struct
struct Player {
    double x;
    double y;
    double angle;
    double speed;
    double rotateSpeed;
    int fov;
};

// Raycast map
const int MAP[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
