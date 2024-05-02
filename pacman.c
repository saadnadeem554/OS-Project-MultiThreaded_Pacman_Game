#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// include bool library
#include <stdbool.h>
#include <stdio.h>
#include "stb_image.h" 
#include <SOIL/SOIL.h> // Include SOIL library for image loading

GLuint mapTextureID; // Texture ID for the map
/* Set initial display-window size. */
GLsizei winWidth = 1000, winHeight = 800;

/* Set range for world coordinates. */
GLfloat xwcMin = 0.0, xwcMax = 600.0;
GLfloat ywcMin = 0.0, ywcMax = 600.0;

int tick = 0;
const int REFRESH_MS = 5;
int direction = 0;
bool wrapAround = false;

void drawMap();
void displayFcn(void);
void pacMan(int);
void timer(int value);
void arrowFunc(int, int, int);
void keyboardFunc(unsigned char, int, int);
void init(void);
void winReshapeFcn(GLint newWidth, GLint newHeight);
void updatePos(int);
void parseArgs(int argc, char **argv);


int xPos = 300;
int yPos = 300;

void loadTexture(const char* filename) {
    mapTextureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (mapTextureID == 0) {
        printf("Texture loading failed: '%s'\n", SOIL_last_result());
    }
}

void drawMap() {
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mapTextureID); // Bind the map texture

    // Draw a textured quad representing the map
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(xwcMax, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(xwcMax, ywcMax);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, ywcMax);
    glEnd();

    // Disable texture mapping
    glDisable(GL_TEXTURE_2D);
}

int main(int argc, char **argv) {
    parseArgs(argc, argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Pac Man");
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(arrowFunc);

    init();

    // Load the map texture
    loadTexture("pacman.png");

    glutDisplayFunc(displayFcn);
    glutReshapeFunc(winReshapeFcn);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
}

/*
int main(int argc, char **argv) {
    parseArgs(argc, argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Pac Man");
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(arrowFunc);

    init();

    glutDisplayFunc(displayFcn);
    glutReshapeFunc(winReshapeFcn);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
}
*/

void displayFcn(void) {

    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    drawMap(); // Draw the map
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xPos, yPos, 0);
    pacMan(direction); // Draw Pacman
    updatePos(direction);
    glPopMatrix();

    glFlush();
    tick++;
}

void timer(int value) {
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(REFRESH_MS, timer, 0); // next timer call milliseconds later
}

void keyboardFunc(unsigned char Key, int x, int y) {
    switch (Key) {
    case 'w':
        wrapAround = !wrapAround;
        //std::cout << "Wraparound set to " << ((wrapAround) ? "true" : "false") << std::endl;
        break;
    };
}

void arrowFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        direction = 3;
        break;
    case GLUT_KEY_DOWN:
        direction = 1;
        break;
    case GLUT_KEY_LEFT:
        direction = 2;
        break;
    case GLUT_KEY_RIGHT:
        direction = 0;
        break;
    }
}


void init(void) {
    /* Set color of display window to white. */
    glClearColor(0, 0, 0, 0.0);
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);
    glClear(GL_COLOR_BUFFER_BIT);
}

void updatePos(int direction) {
    const int SPEED = 1;
    switch (direction) {
    case 0:
        if (xPos < xwcMax + 2 || wrapAround)
            xPos += SPEED;
        break;
    case 1:
        if (yPos > -2 || wrapAround)
            yPos -= SPEED;
        break;
    case 2:
        if (xPos > -2 || wrapAround)
            xPos -= SPEED;
        break;
    case 3:
        if (yPos < ywcMax + 2 || wrapAround)
            yPos += SPEED;
        break;
    };
    if (wrapAround) {
        xPos = (xPos + 600) % 600;
        yPos = (yPos + 600) % 600;
    }
}

void pacMan(int direction) {
    int r = 11;

    glPushMatrix();
    while (direction--) {
        glRotatef(90, 0, 0, -1);
    }
    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    int detail = 30;
    int startPoint = 15 * sin(tick / 15);
    for (int i = 0; i < detail; i++) {
        double deg = (i * (360 - 2 * startPoint) / detail) + startPoint;
        double x = r * cos(deg * 3.14 / 180);
        double y = r * sin(deg * 3.14 / 180);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
}

/*
void drawMap() {
    // Draw boundaries
    glColor3f(1.0f, 1.0f, 1.0f); // White color for walls
    glBegin(GL_QUADS);
    
    // Draw walls
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for walls
    // Horizontal walls
    glVertex2f(0, 50);
    glVertex2f(600, 50);
    glVertex2f(600, 100);
    glVertex2f(0, 100);
    
    // Vertical walls
    glVertex2f(100, 100);
    glVertex2f(150, 100);
    glVertex2f(150, 550);
    glVertex2f(100, 550);
    
    // Add more walls as needed
    
    glEnd();

    // Draw pellets
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for pellets
    glPointSize(5.0f); // Set point size for pellets
    glBegin(GL_POINTS);
    
    // Draw pellets at specific locations
    glVertex2f(50, 50);
    glVertex2f(200, 50);
    glVertex2f(250, 50);
    // Add more pellets as needed
    
    glEnd();
}

*/

void parseArgs(int argc, char **argv) {
    wrapAround = true;
    
}