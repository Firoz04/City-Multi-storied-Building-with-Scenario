#include <cstdio>
#include <windows.h>
#include <GL/glut.h>
#include <math.h>

// Global animation variables
float busX = -150.0;  //Bus Position
float carX = -100.0;   //Car Position
float cloudX = 0.0;    //cloud Positon
float pedestrianY = 60.0f; // Pedestrian position
int lightState = 0;        // 0 = Red, 1 = Yellow, 2 = Green
int lightTimer = 0;        // For cycling light every few seconds
bool isCrossing = false;    // Control flag for crossing
float pedestrianX = 20.0;  // Aligned with zebra crossing
float man1X = 0.0f;    // 1st walking man
float man2X = 500.0f;    // 2nd walking man
float manSpeed = 0.5f; // Speed of the walking men


// Circle drawing function
void drawCircle(float cx, float cy, float r)
{
    int segments = 100;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++)
    {
        float theta = 2.0f * 3.1415926f * i / segments;
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();
}

// Sky gradient
void drawGradientSky()
{
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.8, 1.0);
    glVertex2f(0, 500);
    glColor3f(0.2, 0.6, 1.0);
    glVertex2f(600, 500);
    glColor3f(0.6, 0.9, 1.0);
    glVertex2f(600, 300);
    glColor3f(0.8, 0.95, 1.0);
    glVertex2f(0, 300);
    glEnd();
}

// Clouds
void drawCloud(float x, float y)
{
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(x, y, 20);
    drawCircle(x + 20, y + 10, 20);
    drawCircle(x + 40, y, 20);
    drawCircle(x + 20, y - 10, 20);
}

// Grass
void drawGrass()
{
    glColor3f(0.3, 0.8, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(600, 0);
    glVertex2f(600, 60);
    glVertex2f(0, 60);
    glEnd();
}

// Arsenic-colored road
void drawRoad()
{
    glColor3f(0.231, 0.267, 0.294);
    glBegin(GL_QUADS);
    glVertex2f(0, 60);
    glVertex2f(600, 60);
    glVertex2f(600, 100);
    glVertex2f(0, 100);
    glEnd();

    // White dashed lines
    glColor3f(1.0, 1.0, 1.0);
    for (int x = 0; x < 600; x += 40)
    {
        glBegin(GL_QUADS);
        glVertex2f(x + 10, 78);
        glVertex2f(x + 30, 78);
        glVertex2f(x + 30, 82);
        glVertex2f(x + 10, 82);
        glEnd();
    }

    // Zebra crossing at left corner
    for (int i = 0; i < 3; i++)
    {
        float x = 15 + i * 6;
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
        glVertex2f(x, 60);
        glVertex2f(x + 4, 60);
        glVertex2f(x + 4, 100);
        glVertex2f(x, 100);
        glEnd();
    }




}

//Big Tree
void drawTree(float x, float y)
{
    glColor3f(0.4, 0.2, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(x - 5, y);
    glVertex2f(x + 5, y);
    glVertex2f(x + 5, y + 30);
    glVertex2f(x - 5, y + 30);
    glEnd();

    glColor3f(0.0, 0.6, 0.0);
    drawCircle(x, y + 45, 20);
    drawCircle(x - 10, y + 55, 15);
    drawCircle(x + 10, y + 55, 15);
}

// Building
void drawBuilding()
{
    // Adjusted position of the main building to add spacing
    glColor3f(0.8, 0.8, 0.9);
    glBegin(GL_QUADS);
    glVertex2f(250, 100);          // Move 100 units to the right
    glVertex2f(450, 100);
    glVertex2f(450, 400);
    glVertex2f(250, 400);
    glEnd();

    // Roof
    glColor3f(0.5, 0.2, 0.1);
    glBegin(GL_POLYGON);
    glVertex2f(240, 400);         // Move roof to align with new position
    glVertex2f(460, 400);
    glVertex2f(430, 420);
    glVertex2f(270, 420);
    glEnd();

    // Side buildings
    glColor3f(0.6, 0.8, 0.9);
    glBegin(GL_QUADS);
    glVertex2f(150, 100);
    glVertex2f(250, 100);
    glVertex2f(250, 350);
    glVertex2f(150, 350);
    glVertex2f(450, 100);
    glVertex2f(550, 100);
    glVertex2f(550, 350);
    glVertex2f(450, 350);
    glEnd();

    // Windows
    glColor3f(0.2, 0.2, 0.6);
    for (int y = 110; y < 390; y += 30)
    {
        glBegin(GL_QUADS);
        glVertex2f(270, y);
        glVertex2f(430, y);
        glVertex2f(430, y + 20);
        glVertex2f(270, y + 20);
        glEnd();
    }

    for (int y = 110; y < 340; y += 30)
    {
        glBegin(GL_QUADS);
        glVertex2f(170, y);
        glVertex2f(230, y);
        glVertex2f(230, y + 20);
        glVertex2f(170, y + 20);
        glVertex2f(470, y);
        glVertex2f(530, y);
        glVertex2f(530, y + 20);
        glVertex2f(470, y + 20);
        glEnd();
    }

    // Existing left trees
    drawTree(125, 100);  // Left side tree


    // Trees on the Right**
    drawTree(530, 100); // Right side tree
    drawTree(550, 100); // Right side tree
    drawTree(570, 100); // Right side tree
}


void drawTrafficLampPost()
{
    float baseX = 30;  // Move to left

    // Pole
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(baseX - 2, 100);
    glVertex2f(baseX + 2, 100);
    glVertex2f(baseX + 2, 160);
    glVertex2f(baseX - 2, 160);
    glEnd();

    // Light box
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(baseX - 8, 160);
    glVertex2f(baseX + 8, 160);
    glVertex2f(baseX + 8, 190);
    glVertex2f(baseX - 8, 190);
    glEnd();

    // Lights
    glColor3f(lightState == 2 ? 1.0 : 0.2, 0.0, 0.0); // Red
    drawCircle(baseX, 182, 4);

    glColor3f(lightState == 1 ? 1.0 : 0.2, 1.0, 0.0); // Yellow
    drawCircle(baseX, 172, 4);

    glColor3f(lightState == 0 ? 0.0 : 0.2, 1.0, 0.0); // Green
    drawCircle(baseX, 162, 4);

}

void drawPedestrian()
{
    if (lightState == 0)   // RED light for cars → green for pedestrians
    {
        isCrossing = true;
    }

    if (isCrossing)
    {
        pedestrianY += 0.5;  // Move up the road (Y-axis)
        if (pedestrianY >= 100.0)
        {
            pedestrianY = 60.0;  // Reset after crossing
            isCrossing = false;
        }
    }

    // Draw the pedestrian (simple stick figure)
    glColor3f(0.0, 0.0, 0.0); // Head
    drawCircle(pedestrianX, pedestrianY + 10, 3);

    glColor3f(0.0, 0.2, 0.8); // Body
    glBegin(GL_LINES);
    glVertex2f(pedestrianX, pedestrianY + 7);
    glVertex2f(pedestrianX, pedestrianY - 3);
    glEnd();

    glBegin(GL_LINES); // Legs
    glVertex2f(pedestrianX, pedestrianY - 3);
    glVertex2f(pedestrianX - 2, pedestrianY - 8);
    glVertex2f(pedestrianX, pedestrianY - 3);
    glVertex2f(pedestrianX + 2, pedestrianY - 8);
    glEnd();

    glBegin(GL_LINES); // Arms
    glVertex2f(pedestrianX, pedestrianY + 5);
    glVertex2f(pedestrianX - 3, pedestrianY);
    glVertex2f(pedestrianX, pedestrianY + 5);
    glVertex2f(pedestrianX + 3, pedestrianY);
    glEnd();
}



// Bus
void drawBus()
{
    float xOffset = busX;
    float wheelY = 70;
    float bodyBottom = wheelY + 6;
    float bodyTop = bodyBottom + 30;

    glColor3f(0.0, 0.8, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(250 + xOffset, bodyBottom);
    glVertex2f(350 + xOffset, bodyBottom);
    glVertex2f(350 + xOffset, bodyTop);
    glVertex2f(250 + xOffset, bodyTop);
    glEnd();

    glColor3f(0.1, 0.1, 0.1);
    for (int i = 0; i < 4; i++)
    {
        float x = 260 + i * 20 + xOffset;
        glBegin(GL_QUADS);
        glVertex2f(x, bodyBottom + 10);
        glVertex2f(x + 15, bodyBottom + 10);
        glVertex2f(x + 15, bodyTop - 3);
        glVertex2f(x, bodyTop - 3);
        glEnd();
    }

    glColor3f(0.0, 0.0, 0.0);
    drawCircle(265 + xOffset, wheelY, 6);
    drawCircle(335 + xOffset, wheelY, 6);
}

// Car
void drawCar()
{
    float xOffset = carX;
    float wheelY = 70;
    float bodyBottom = wheelY + 6;
    float bodyTop = bodyBottom + 18;

    glColor3f(1.0, 0.9, 0.9);
    glBegin(GL_POLYGON);
    glVertex2f(150 + xOffset, bodyBottom);
    glVertex2f(230 + xOffset, bodyBottom);
    glVertex2f(230 + xOffset, bodyTop - 5);
    glVertex2f(220 + xOffset, bodyTop);
    glVertex2f(160 + xOffset, bodyTop);
    glVertex2f(150 + xOffset, bodyTop - 5);
    glEnd();

    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(165 + xOffset, bodyBottom + 5);
    glVertex2f(185 + xOffset, bodyBottom + 5);
    glVertex2f(185 + xOffset, bodyTop - 3);
    glVertex2f(165 + xOffset, bodyTop - 3);

    glVertex2f(190 + xOffset, bodyBottom + 5);
    glVertex2f(210 + xOffset, bodyBottom + 5);
    glVertex2f(210 + xOffset, bodyTop - 3);
    glVertex2f(190 + xOffset, bodyTop - 3);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    drawCircle(165 + xOffset, wheelY, 5);
    drawCircle(215 + xOffset, wheelY, 5);
}

// Street Light
void drawStreetLight(float x, float y)
{
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(x - 2, y);
    glVertex2f(x + 2, y);
    glVertex2f(x + 2, y + 50);
    glVertex2f(x - 2, y + 50);
    glEnd();

    glColor3f(1.0, 1.0, 0.6);
    drawCircle(x, y + 55, 5);
}

// Bird
void drawBird(float x, float y)
{
    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + 5, y + 5);
    glVertex2f(x + 5, y + 5);
    glVertex2f(x + 10, y);
    glEnd();
}

//Flat Building
void drawUniqueFlatBuilding()
{
    // Main structure
    glColor3f(0.85, 0.85, 0.95); // Light purple-gray
    glBegin(GL_QUADS);
    glVertex2f(20, 100);
    glVertex2f(140, 100);
    glVertex2f(140, 320);
    glVertex2f(20, 320);
    glEnd();

    // Roof
    glColor3f(0.3, 0.3, 0.4); // Dark roof
    glBegin(GL_QUADS);
    glVertex2f(15, 320);
    glVertex2f(145, 320);
    glVertex2f(145, 330);
    glVertex2f(15, 330);
    glEnd();

    // 4 Vertical Glass Windows (Window Columns)
    glColor3f(0.3, 0.6, 0.8); // Blueish glass
    for (int i = 0; i < 4; i++)
    {
        float x = 30 + i * 25;
        glBegin(GL_QUADS);
        glVertex2f(x, 120);
        glVertex2f(x + 15, 120);
        glVertex2f(x + 15, 300);
        glVertex2f(x, 300);
        glEnd();
    }

    // 4 columns × 5 floors = 20 Balconies
    glColor3f(0.7, 0.7, 0.8); // Balcony color
    for (int floor = 0; floor < 5; floor++)
    {
        for (int col = 0; col < 4; col++)
        {
            float x = 28 + col * 25;     // Horizontal
            float y = 120 + floor * 35;  // Vertical
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 19, y);
            glVertex2f(x + 19, y + 7);
            glVertex2f(x, y + 7);
            glEnd();
        }
    }
}

//A filled circle
void drawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * (float)i / (float)num_segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

//The sun with a cloud partially covering it
void drawSunWithClouds()
{
    //Sun top-left
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    drawCircle(100, 480, 20, 100); // Sun with radius 20

    // Cloud partially covering the sun (first cloud)
    glColor3f(1.0f, 1.0f, 1.0f); // White
    //A smaller cloud to partially cover the sun
    drawCircle(90, 510, 30, 100); // Left cloud, radius 30
    drawCircle(120, 510, 30, 100); // Right cloud, radius 30

    //Another smaller cloud to cover the sun more
    drawCircle(140, 500, 35, 100); // Cloud on the right, radius 35
}

//Basic Tamarix tree
void drawTamarixTree(float x, float y)
{
    //Draw the trunk
    glColor3f(0.55f, 0.27f, 0.07f); // Brown trunk
    glBegin(GL_QUADS);
    glVertex2f(x - 3, y);        // Bottom left
    glVertex2f(x + 3, y);        // Bottom right
    glVertex2f(x + 3, y + 20);   // Top right
    glVertex2f(x - 3, y + 20);   // Top left
    glEnd();

    //The foliage as a triangle (green)
    glColor3f(0.0f, 0.6f, 0.0f); // Dark green foliage
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 60);        // Top point
    glVertex2f(x - 20, y + 20);   // Bottom left
    glVertex2f(x + 20, y + 20);   // Bottom right
    glEnd();
}


//multiple Tamarix trees
void drawRoadsideTrees()
{
    for (int i = 0; i < 13; i++)
    {
        float x = 10 + i * 50; // Horizontal spacing
        float y = -10;          // Vertical base level
        drawTamarixTree(x, y);
    }
}

//Walking Way
void drawWalkingWay()
{
    glColor3f(0.6f, 0.6f, 0.6f); // Grayish path color
    glBegin(GL_QUADS);
    glVertex2f(0, 50);     // Bottom left
    glVertex2f(1000, 50);  // Bottom right
    glVertex2f(1000, 20);  // Top right (reduced height)
    glVertex2f(0, 20);     // Top left
    glEnd();
}

//A man with shirt color
void drawMan(float x, float y, float shirtColor[])
{
    // Head
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    drawCircle(x, y + 20, 5, 50); // Head

    // Body
    glColor3f(shirtColor[0], shirtColor[1], shirtColor[2]); // Shirt color
    glBegin(GL_LINES);
    glVertex2f(x, y + 15);
    glVertex2f(x, y); // Body
    glVertex2f(x, y + 10);
    glVertex2f(x - 5, y + 5); // Left arm
    glVertex2f(x, y + 10);
    glVertex2f(x + 5, y + 5); // Right arm
    glVertex2f(x, y);
    glVertex2f(x - 5, y - 5); // Left leg
    glVertex2f(x, y);
    glVertex2f(x + 5, y - 5); // Right leg
    glEnd();
}
//Animate the positions of the men
void animateMen()
{
    man1X += manSpeed; // Move first man
    if (man1X > 600) man1X = 0; // Reset to start position after moving off screen

    man2X += manSpeed; // Move second man
    if (man2X > 600) man2X = 0; // Reset to start position after moving off screen
}


//Bench
void drawBench(float x, float y)
{
    // Seat of the bench
    glColor3f(0.5f, 0.3f, 0.1f); // Brown
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 40, y);
    glVertex2f(x + 40, y + 5);
    glVertex2f(x, y + 5);
    glEnd();

    // Backrest
    glBegin(GL_QUADS);
    glVertex2f(x, y + 5);
    glVertex2f(x + 40, y + 5);
    glVertex2f(x + 40, y + 15);
    glVertex2f(x, y + 15);
    glEnd();

    // Legs
    glBegin(GL_QUADS);
    glVertex2f(x + 5, y);
    glVertex2f(x + 7, y);
    glVertex2f(x + 7, y - 10);
    glVertex2f(x + 5, y - 10);

    glVertex2f(x + 33, y);
    glVertex2f(x + 35, y);
    glVertex2f(x + 35, y - 10);
    glVertex2f(x + 33, y - 10);
    glEnd();
}



// Display
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawGradientSky();
    drawSunWithClouds();
    drawCloud(100 + cloudX, 420);
    drawCloud(300 + cloudX, 460);
    drawCloud(450 + cloudX, 410);
    drawGrass();
    drawRoad();
    drawUniqueFlatBuilding();
    drawTrafficLampPost();
    drawPedestrian();
    drawBuilding();
    drawStreetLight(10, 100);
    drawStreetLight(150, 100);
    drawStreetLight(305, 100);
    drawStreetLight(485, 100);
    drawStreetLight(595, 100);
    drawBus();
    drawCar();
    drawStreetLight(5, 60);
    drawStreetLight(145, 60);
    drawStreetLight(300, 60);
    drawStreetLight(480, 60);
    drawStreetLight(590, 60);
    drawWalkingWay();
    drawBench(150, 55);
    drawBench(380, 55);
    float man1ShirtColor[] = {1.0f, 0.0f, 0.0f};
    float man2ShirtColor[] = {0.0f, 0.0f, 1.0f};
    drawMan(man1X, 30, man1ShirtColor);
    drawMan(man2X, 50, man2ShirtColor);
    glutSwapBuffers();
    animateMen();
    drawRoadsideTrees();
    // Birds
    drawBird(100 + cloudX, 450);
    drawBird(150 + cloudX, 460);
    drawBird(200 + cloudX, 445);
    glFlush();
}


// Animation
void update(int value)
{
    busX += 2.0f;
    if (busX > 600 + 150) busX = -150.0f;


    carX += 1.5f;
    if (carX > 600 + 150) carX = -100.0f;


    cloudX += 0.2f;
    if (cloudX > 600) cloudX = -100;


    lightTimer++;
    if (lightTimer >= 60)   // Change light every ~2 seconds
    {
        lightState = (lightState + 1) % 3;
        lightTimer = 0;
    }


    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}


// Init
void init()
{
    glClearColor(0.6, 0.9, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("City Multi-storied building with scenario");
    glutInitWindowSize(600, 500);
    glutDisplayFunc(display);
    init();
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
