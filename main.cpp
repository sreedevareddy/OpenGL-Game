#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h> 
#include<GL/glut.h>

#define CIRCLE_RADIUS  0.15f
# define PI 3.14159265358979323846

enum Window { MENU, INSTRUCTIONS, GAME, GAME_OVER };
Window active_window = MENU;
int missile_xc, missile_yc;
// for coordinates of missile
int spaceship_x, spaceship_y;
// for coordinates of spaceship
int a = 1920, b = 650; // for default size of the screen
int s = 0;
int missiles_caught = 0, missed_missiles = 0, level_count = 1, points = 0;
// for menu option
int dropped_missiles = 0;
int speed_1 = 10, speed_2 = 15, speed_3 = 20, speed_4 = 25;
int w = 48, h = 48, t = 10, e = 9, g = 12;
void myinit();
void cloud1();
void missile();
void spaceship(int, int);
void truck(int, int);
void print_score();
void missile_start();
void color();
void score();
void menu(int);
void display(void);
void spaceship_set(int, int);
void myReshape(int, int);
void keys(unsigned char, int, int);


//Used to write text in the opengl window
void drawString(void* font, const char* s, float x, float y) {
    unsigned int i;
    glRasterPos2f(x, y);
    for (i = 0; i < strlen(s); i++)
        glutBitmapCharacter(font, s[i]);
}


void draw_button(int x1, int x2, int y1, int y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void cloud2(int xcl,int ycl)
{
    float theta;
    glLineWidth(1.5);
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        theta = i * PI * i / 180;
        glVertex2f(xcl + 100 * cos(theta) / 2, ycl + 100 * sin(theta) / 2);
    }

    glEnd();
    glFlush();

}

void cloud3(int xcl,int ycl)
{
    float theta;
    glLineWidth(1.5);
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        theta = i * PI * i / 180;
        glVertex2f(xcl + 100 * cos(theta) / 2, ycl + 100 * sin(theta) / 2);
    }

    glEnd();
    glFlush();

}


void missile()
{

    float x, y, z;
    int t;

    glColor3f(0.1, 0.9, 0.6);
    glBegin(GL_POLYGON);
    {
        glVertex2f(missile_xc + 10.0, missile_yc + 50);
        glVertex2f(missile_xc - 10.0, missile_yc + 50);
        glVertex2f(missile_xc - 10.0, missile_yc + 17);
        glVertex2f(missile_xc + 10.0, missile_yc + 17);
    }
    glEnd();
    glColor3f(0.1, 0.9, 0.7);
    glBegin(GL_POLYGON);
    for (t = 0;t <= 360;t += 1)
    {
        x = missile_xc + 9 * (cos(t));
        y = missile_yc + 18 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();
}
void spaceship(int i, int j)
{
    j = 100;
    if (i >= a - 300)i = a - 300;
    if(i <= 300) i = 300;
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (t = 0;t <= 90;t += 1)
    {
        int x = i + 35 * (cos(t));
        int y = j + 60 * (sin(t));
        glVertex2f(x, y);
    }
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POLYGON);
    for (t = 0;t <= 360;t += 1)
    {
        int x = i + 300 * (cos(t));
        int y = j + 35 * (sin(t));

        glVertex2f(x, y);
    }
    glEnd();
}


void truck(int i, int j)
{
    int h;
    cloud3(i+60,j+90); 
    cloud3(i+220,j+90);
    glColor3f(0.0258, 0.43, 0.1134);
    draw_button(i, i+200, j, j+100);
    draw_button(i, i+30, j, j-150);
    draw_button(i+200, i+300, j+100, j-100);
    glFlush();
}

void print_score()
{
    printf("\nLevel reached: %d\n\n", level_count);
    printf("\nNo. of missiles dropped= %d \n", dropped_missiles);
    printf("\nNo. of missiles caught = %d\n", missiles_caught);
    printf("\nNo. of missiles missed = %d\n", missed_missiles);

    getchar();
    exit(0);
}
void missile_start()
{
    missile_yc = 800;
    if (missiles_caught >= 5)
    {
        active_window = GAME_OVER;
    }
    dropped_missiles++;

    switch (rand() % 9)
    {
        case 0:missile_xc = 120; break;
        case 1:missile_xc = 620; break;
        case 2:missile_xc = 1120; break;
        case 5:missile_xc = 1620; break;
        case 3:missile_xc = 120; break;
        case 4:missile_xc = 620; break;
        case 7:missile_xc = 1120; break;
        case 6:missile_xc = 1620; break;
        case 8:missile_xc = 1120; break;
    }
}

void score()
{
    if (missile_yc <= 50 && (missile_xc >= spaceship_x - 300 && missile_xc <= spaceship_x + 300))
    {   
        printf("\a");
        missiles_caught++;
        missile_yc = -100;
    }
    missed_missiles = dropped_missiles - missiles_caught;
}
//This function is called on windows resize
void reshape(int x, int y)
{
    glViewport(0, 0, x, y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, x, y, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}


// Callback function for drawing the instructions window
void draw_instructions()
{

    // Draw the back button
    glColor3f(0.0, 1.0, 0.0);
    draw_button(0, 200, 0, 50);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_18, "BACK", 20, 30);

    glColor3f(1.0, 0.0, 0.0);
    draw_button(1720, 1920, 0, 50);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_18, "EXIT", 1740, 30);

    glColor3f(1.0, 0.0, 0.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "INSTRUCTIONS TO PLAY", 860, 200);


    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "!!!YOUR UNDER ATTACK IN A WARZONE!!!", 760, 300);

    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "CONTROL SPACESHIP TO ESCAPE FROM MISSILES", 720, 375);

    glColor3f(0.0, 1.0, 0.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "HOW TO PLAY", 100, 500);

    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "1. CLICK ON START GAME BUTTON TO PLAY!!!", 100, 550);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "2. MOVE SPACESHIP LEFT OR RIGHT WITH YOUR MOUSE OR PRESS A AND D RESPECTIVELY TO ESCAPE FROM MISSILES", 100, 600);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "3. TO EXIT CLICK ON THE 'EXIT' BUTTON.", 100, 650);

    glColor3f(0.0, 1.0, 0.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "RULES", 100, 750);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "1. AFTER MISSING 10 MISSILES IN EACH LEVEL, THE TRUCK LEVEL AND FIRING RATE WILL INCREASE ACCORDINGLY.", 100, 800);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "2. YOU WILL LOSE THE GAME.IF THE SPACESHIP GET HIT BY 5 MISSILES.", 100, 850);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "3. RESULTS WILL BE DISPLAYED AFTER YOU LOSE", 100, 900);      
 
    glutSwapBuffers();
}

void game_over()
{

    // Draw the back button
    glColor3f(0.0, 1.0, 0.0);
    draw_button(0, 200, 0, 50);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_18, "MENU", 20, 30);

    glColor3f(1.0, 0.0, 0.0);
    draw_button(1720, 1920, 0, 50);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_18, "EXIT", 1740, 30);

    glColor3f(1.0, 0.0, 0.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER", 860, 200);

    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_18, "TOTAL MISSILES LAUNCHED ", 850, 400);
    char myString[32];
    sprintf(myString, "%d", dropped_missiles);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, myString, 1200, 400);

    drawString(GLUT_BITMAP_HELVETICA_18, "NUMBER OF HITS TAKEN", 850, 500);
    char myString1[32];
    sprintf(myString1, "%d", missiles_caught);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, myString1, 1200, 500);

    drawString(GLUT_BITMAP_HELVETICA_18, "MISSED MISSILES", 850, 600);
    char myString2[32];
    sprintf(myString2, "%d", missed_missiles);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, myString2, 1200, 600);


    glutSwapBuffers();
}

// Callback function for drawing the game window
void draw_game()
{
    // Draw the back button
    glColor3f(0.0, 1.0, 0.0);
    draw_button(0, 200, 0, 50);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_18, "BACK", 20, 30);

    //ground
    glColor3f(0.0, 1.0, 0.0);
    draw_button(0, 1920, 780, 1080);

    //back
    glColor3f(0, 0.0, 1.0);
    draw_button(0, 1920, 0, 780);

    cloud2(120,90);
    cloud2(160,50);
    cloud2(200,100);
    cloud2(250,70);

    cloud2(1000,100);
    cloud2(1040,50);
    cloud2(1070,130);
    cloud2(1100+10,90);

    cloud2(500,200);
    cloud2(540,150);
    cloud2(570,230);
    cloud2(600+10,190);

    cloud2(1700,150);
    cloud2(1740,100);
    cloud2(1770,180);
    cloud2(1800+10,140);

    int i;
    char level1[12] = "LEVEL 1";
    char level2[12] = "LEVEL 2";
    char level3[12] = "LEVEL 3";
    char level4[12] = "LEVEL 4";

    if (s >= 1)
    {
        glColor3f(0, 0, 1);

        if (level_count == 1)
        {
            glColor3f(1, 1, 1);
            glRasterPos2i(100, 500);
            for (i = 0;i < 12;i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, level1[i]);
        }
        else if (level_count == 2)
        {
            glColor3f(1, 0, 0);
            glRasterPos2i(100, 500);
            for (i = 0;i < 12;i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, level2[i]);
        }
        else if (level_count == 3)
        {
            glColor3f(0, 1, 1);
            glRasterPos2i(100, 500);
            for (i = 0;i < 12;i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, level3[i]);
        }
        if (level_count == 4)
        {
            glColor3f(1, 0, 1);
            glRasterPos2i(100, 500);
            for (i = 0;i < 12;i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, level4[i]);
        }

        if (missile_yc <= 10)
            missile_start();

        missile();
        spaceship(spaceship_x, spaceship_y);
        if (missed_missiles >= 10)
        {
            missile_yc -= speed_2;
            level_count = 2;
        }
        if (missed_missiles >= 20)
        {
            missile_yc -= speed_3;
            level_count = 3;
        }
        if (missed_missiles >= 30)
        {
            missile_yc -= speed_4;
            level_count = 4;
        }
        else
            missile_yc -= speed_1;
        score();
    }

    truck(100, 900);
    truck(600, 900);
    truck(1100, 900);
    truck(1600, 900);
    
    glColor3f(1.0, 0.0, 0.0);
    draw_button(1720, 1920, 0, 50);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_18, "BACK TO MENU", 1740, 30);

    glFlush();
    glutSwapBuffers();
}


void click(int button, int state, int x, int y)
{
    switch (active_window)
    {
    case MENU:
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            // Check if the Play button was clicked
            if (x >= 760 && x <= 1160 && y >= 600 && y <= 650)
            {
                // Start the game
                active_window = GAME;
                s+=1;
            }

            // Check if the Instructions button was clicked
            else if (x >= 760 && x <= 1160 && y >= 700 && y <= 750)
            {
                // Show the instructions
                active_window = INSTRUCTIONS;
            }
            // Check if the Exit button was clicked
            else if (x >= 760 && x <= 1160 && y >= 800 && y <= 850)
            {
                exit(0);
            }
        }
        break;
    case INSTRUCTIONS:
        // Check if the Back button was clicked
        if (x >= 0 && x <= 200 && y >= 0 && y <= 50)
        {
            // Go back to the main menuDESCRIPTION
            active_window = MENU;
        }
        else if (x >= 1720 && x <= 1920 && y >= 0 && y <= 50) {
            exit(0);

        }
        break;
    case GAME:
        if (x >= 1720 && x <= 1920 && y >= 0 && y <= 50) {
            active_window = MENU;
        }
        break;
    case GAME_OVER:
        if (x >= 0 && x <= 200 && y >= 0 && y <= 50)
        {
            // Go back to the main menu
            missiles_caught = 0;
            missed_missiles = 0;
            level_count = 1;
            points = 0;
            
            active_window = MENU;
            
        }
        else if (x >= 1720 && x <= 1920 && y >= 0 && y <= 50) {
            exit(0);

        }
        break;
    }
}


// Callback function for drawing the buttons
void draw_buttons()
{
    // Title
    glColor3f(0.0578, 0.1848, 0.34);
    draw_button(0, 1920, 0, 300);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "AERIAL ASSUALT", 850, 235);
    
    // Draw the Play button
    glColor3f(0.0578, 0.1848, 0.34);
    draw_button(760, 1160, 600, 650);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "START GAME", 780, 635);

    // Draw the Instructions button
    glColor3f(0.0578, 0.1848, 0.34);
    draw_button(760, 1160, 700, 750);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "INSTRUCTIONS", 780, 735);

    // Draw the Exit button
    glColor3f(0.0578, 0.1848, 0.34);
    draw_button(760, 1160, 800, 850);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "EXIT GAME", 780, 835);

    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "20BCS041 - CHAITANYA DHARMIREDDI", 1300, 900);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "20BCS108 - RANGAM SAI CHARAN", 1300, 950);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "20BCS128 - SREEDEVA KRUPANANDA B REDDY", 1300, 1050);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "20BCS138 - VISHNU KUMAR P", 1300, 1000);
}

void idle() {
    glClearColor(0, 0, 0, 1);
    switch (active_window)
    {
    case MENU:
        draw_buttons();
        break;
    case INSTRUCTIONS:
        draw_instructions();
        break;
    case GAME:
        draw_game();
        break;
    case GAME_OVER:
        game_over();
        break;
    }
    glutPostRedisplay();
}

void spaceship_set(int a, int b)
{
    spaceship_x = a;
    spaceship_y = b;
    glutPostRedisplay();
}

void keys(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
    {
        spaceship_x -= 70;
    }
    if (key == 'd' || key == 'D')
    {
        spaceship_x += 70; 
    }
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
    glColor3f(0, 0, 0);

    switch (active_window)
    {
    case MENU:
        draw_buttons();
        break;
    case INSTRUCTIONS:
        draw_instructions();
        break;
    case GAME:
        draw_game();
        break;
    case GAME_OVER:
        game_over();
        break;
    default:
        break;
    }

    glutSwapBuffers();
}


//main function of the program
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Aireal Assault");
    glutFullScreen();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(click);
    glutKeyboardFunc(keys);
    glutPassiveMotionFunc(spaceship_set);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
                // Exit the game
}