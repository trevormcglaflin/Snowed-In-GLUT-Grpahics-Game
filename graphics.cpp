#include "circle.h"
#include "rect.h"
#include "graphics.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
using namespace std;



GLdouble width, height;
int wd, ballSize, userHeight, userWidth, maxJumpHeight, userSnowHeight, points, blockSpeed;
bool userUp, userDown, userDownFast;
Rect user, userSnow;
point2D userCenter, userSnowCenter;
dimensions userDimensions, userSnowDimensions;
screenLetter screen;
vector<Rect> blocks, warmingBlocks;
vector<Circle> snow;
const int distanceFromEdge = 100;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan (0, 1, 1);
const color neonYellow (0.85, 1.0, 0.2);
const color pineGreen (0.4, 0.55, 0.4);
const vector<color> colors =  { skyBlue, grassGreen, white, brickRed, darkBlue, purple, magenta, orange, cyan };

// checks if a rectangle is on the screen
bool onScreen(Rect r) {
    if (r.getRightX() > 0 && r.getLeftX() < width) {
        return true;
    }
    return false;
}

void initUser() {
    // set initial user attributes
    userHeight = 30;
    userWidth = 30;
    userCenter.x = int(distanceFromEdge);
    userCenter.y = int(height - (userHeight/2));
    userDimensions.width = userWidth;
    userDimensions.height = userHeight;
    user = Rect(orange, userCenter, userDimensions);

    // initialize userSnow attributes
    userSnowHeight = 1;
    userSnowCenter.x = userCenter.x;
    userSnowCenter.y = int(height - userHeight - (userSnowHeight/2));
    userSnowDimensions.width = userWidth;
    userSnowDimensions.height = userSnowHeight;
    userSnow = Rect(white, userSnowCenter, userSnowDimensions);

    // will be used to make user jump
    userUp = false;
    userDown = false;
    userDownFast = false;
    maxJumpHeight = 200;
}

void initBlocks() {
    int blockWidth = 20;
    int blockCount = 0;
    dimensions blockSize;
    while (blockCount < 50) {
        blockSize.height = 10 + blockCount;
        blockSize.width = blockWidth;
        blocks.push_back(Rect(pineGreen,
                              width + blockWidth,
                              height - ((blockSize.height / 2)),
                              blockSize));
        blockCount++;
    }
    // controls speed of blocks
    blockSpeed = 10;
}

void initWarmingBlocks() {
    int blockCount = 0;
    dimensions blockSize;
    int blockY = maxJumpHeight;
    color blockColor = magenta;
    while (blockY > 10) {
        if (rand() % 12 == 3) {
            blockColor = neonYellow;
        }
        else {
            blockColor = magenta;
        }
        blockSize.height = 8;
        blockSize.width = 8;
        warmingBlocks.push_back(Rect(blockColor,
                                     width + 8,
                                     height - blockY,
                                     blockSize));
        blockY -= 10;
    }
}

void initSnow() {
    for (int i = 0; i < 150; ++i) {
        snow.push_back(Circle(1, 1, 1, 1, rand() % int(width), -(rand() % int(height)), rand() % 5 + 1));
    }
}


void init() {
    srand(time(0));
    width = 700;
    height = 700;

    initUser();
    initBlocks();
    initWarmingBlocks();
    initSnow();

    screen = w;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE


    if (screen == w) {
        glColor3f(1, 1, 1);
        glRasterPos2i(200, 200);
        string message = "Brrrrr, hit 'p' to play snowed in!!";
        for (const char &letter: message) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
    }
    else if (screen == p) {

        for (Circle &flake : snow) {
            flake.draw();
        }

        user.draw();
        userSnow.draw();

        for (Rect &r : blocks) {
            r.draw();
        }

        for (Rect &w : warmingBlocks) {
            w.draw();
            cout << w.getRightX() << endl;
            cout << w.getRed() << endl;
        }

        string pointTracker = to_string(points) + " points";
        glColor3f(1, 0, 1);
        glRasterPos2i(width - 75, 20);
        for (const char &letter: pointTracker) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

    }
    else {
        glColor3f(1, 0, 1);
        glRasterPos2i(200, 200);
        string finalScore = "Score: " + to_string(points);
        for (const char &letter: finalScore) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
        glRasterPos2i(200, 250);
        string message = "Hit 'p' to play again!!";
        for (const char &letter: message) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        // reset game
        user = Rect(orange, userCenter, userDimensions);
        userSnow.setHeight(userSnowHeight);
        userSnow.setCenterY(height - user.getHeight() - (height - user.getBottomY()) - (userSnowHeight/2));
        userUp = false;
        userDown = false;
        userDownFast = false;

        for (Circle &flake : snow) {
            flake.setCenter(rand() % int(width), -(rand() % int(height)));
        }
        for (Rect &r : blocks) {
            r.setCenterX(width + 20);
        }
        for (Rect &w : warmingBlocks) {
            w.setCenterX(width + 8);
        }

    }

    glFlush();
}


void kbd(unsigned char key, int x, int y) {
    switch(key) {
        case 27: {
            // escape
            glutDestroyWindow(wd);
            exit(0);
        }

        // p: user can hit it whenever to start over
        case 112: {
            points = 0;
            screen = p;
        }
    }

    if (!(userUp) && !(userDown) && key == 32) {
        userUp = true;
    }

    glutPostRedisplay();
}

void kbdUp(unsigned char key, int x, int y) {

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    if (key = GLUT_KEY_DOWN) {
        if (userUp) {
            userUp = false;
            userDown = true;
        }
        userDownFast = true;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
     string message = "You clicked the mouse at coordinate (" + to_string(x) + ", " + to_string(y) + ")";
     glColor3f(1, 1, 1);
     glRasterPos2i(0, height);
     for (char letter : message) {
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, letter);
     }
     glutPostRedisplay();
}

void blockTimer(int dummy) {
    if (screen == p) {
        bool found = false;
        for (Rect &r : blocks) {
            if (onScreen(r)) {
                r.moveX(-blockSpeed - (1 + points/10));
                found = true;

                if (r.getRightX() <= 0) {
                    points++;
                    r.setCenterX(width + 20);
                    userSnow.setHeight(userSnow.getHeight() + 2);
                    userSnow.setCenterY(userSnow.getCenterY() - 1);
                }
            }
            if (user.isOverlapping(r)) {
                for (Rect &rect : blocks) {
                    rect.setCenterX(width + 20);
                }
                screen = e;
            }
        }

        if (!(found)) {
            blocks[rand() % blocks.size()].moveX(-blockSpeed);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, blockTimer, dummy);
}

void warmingBlockTimer(int dummy) {
    if (screen == p) {
        bool found = false;
        for (Rect &w : warmingBlocks) {
            if (onScreen(w)) {
                w.moveX(-blockSpeed * 2);
                found = true;
            }

            if (w.getRightX() < 0) {
                w.setCenterX(width + 8);
                w.setCenterY(height - (rand() % maxJumpHeight + 10));
            }

            // if user hits a warming block, it will melt some snow off of them so they can jump higher
            if (user.isOverlapping(w)) {
                w.setCenterX(width + 8);
                if (w.getRed() == 1) {
                    userSnow.setHeight(userSnow.getHeight() - 2);
                    userSnow.setCenterY(userSnow.getCenterY() + 1);
                }
                else {
                    userSnow.setHeight(userSnowHeight);
                    userSnow.setCenterY(height - user.getHeight() - (height - user.getBottomY()) - (userSnowHeight/2));
                }
            }
        }

        if (!(found)) {
            warmingBlocks[rand() % warmingBlocks.size()].moveX(-blockSpeed * 2);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, warmingBlockTimer, dummy);
}

void snowTimer(int dummy) {

    for (Circle &flake : snow) {
        flake.moveY( flake.getRadius() * 0.25 * ((points + 2)/5));

        if (flake.getTopY() > height) {
            flake.setCenter(rand() % int(width), -flake.getRadius());
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, snowTimer, dummy);
}

void userTimer(int dummy) {
    // does jumping
    if (userDown) {
        if (userDownFast && (height - user.getBottomY()) >= 60) {
            user.moveY(60);
            userSnow.moveY(60);
        }
        else if ((height - user.getBottomY()) >= 20) {
            user.moveY(20);
            userSnow.moveY(20);
        }
        else {
            user.moveY((height - user.getBottomY()));
            userSnow.moveY((height - user.getBottomY()));
            userDown = false;
            userDownFast = false;
        }
    }
    if (userUp) {
        // the more snow on the user, the lower it can jump
        if (user.getCenterY() >= (height - maxJumpHeight + (userSnow.getHeight() * 2))) {
            user.moveY(-20);
            userSnow.moveY(-20);
            userDownFast = false;
        }
        else {
            userUp = false;
            userDown = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, userTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Snowed in!");

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbdUp);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, userTimer, 0);
    glutTimerFunc(0 , blockTimer, 0);
    glutTimerFunc(0, warmingBlockTimer, 0);
    glutTimerFunc(0, snowTimer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}