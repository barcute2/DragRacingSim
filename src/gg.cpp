#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

float sealevel;


int nFPS = 30;
float velocity = 0.0020;
float fRotateAngle1 = 0.0f, fRotateAngle2 = 0.0f;
float pos[3] = {-50.0f, 1.0f, 1.0f};
float target[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float up[4] = {0.0f, 0.0f, 1.0f, 1.0f};
float v[3], u[3], r[3];
float rotation[4][4];
float result[4][4];
float R[4][4];
float RT[4][4];
float oneof[4][4];
float acceleration = 0, maxAcceleration = 5;
float maxGearSpeed[] = {25, 50, 70, 100, 140, 170};
float actualSpeed[] = {0.0145, 0.034, 0.044, 0.0555, 0.07, 0.094};
int fps = 30;
int gear = 1;
unsigned int tex;
float rpm = 0, pi = 3.141529;

void showSpeed (int posx, int posy, int posz, int spaceBetweenChars, int veloc){
        int j=0,p,k;
        GLvoid *font_style1 = GLUT_BITMAP_TIMES_ROMAN_24;

        p = veloc;
        j = 0;
        k = 0;
        while(p > 9)
        {
            k = p % 10;
            glRasterPos3f ((posx-(j*spaceBetweenChars)),posy, posz);
            glutBitmapCharacter(font_style1,48+k);
            j++;
            p /= 10;
        }
            glRasterPos3f ((posx-(j*spaceBetweenChars)), posy, posz);
            glutBitmapCharacter(font_style1,48+p);

}

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		int veloc) {

	int k = veloc, p;
	char *c;
	glRasterPos3f(x, y,z);
	while(k > 9){
		p = k % 10;
		k = k / 10;
		glutBitmapCharacter(font, p + 48);
	}
	glutBitmapCharacter(font, k + 48);
}

unsigned int loadTexture(const char* filename)
{
    unsigned int num;
    glGenTextures(1,&num);
    SDL_Surface* img=IMG_Load(filename);

    SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
    SDL_Surface* img2=SDL_ConvertSurface(img,&form,SDL_SWSURFACE);

    glBindTexture(GL_TEXTURE_2D,num);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);
    SDL_FreeSurface(img);
    SDL_FreeSurface(img2);
    return num;
}

void init(void)
{
	GLfloat amb[] = {0.2,0.2,0.2};
	GLfloat diff[] = {1.0,1.0,1.0};
	GLfloat spec[] = {1.0,1.0,1.0};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	glClearColor (0.5, 0.5, 1.0, 0.0);	//blue background
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	tex = loadTexture("texture.bmp");
}


void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	GLfloat seaamb[] = {0.37,0.62,0.62,1.0}; //brown
	GLfloat seadiff[] = {0.5,0.35,0.05,1.0};
	GLfloat seaspec[] = {0.37,0.62,0.62,1.0};

	//load single light
	GLfloat lpos[] = {0.0,0.0,10.0,0.0};


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();
	glBindTexture(GL_TEXTURE_2D, tex);

	/* viewing transformation, look at the origin  */
	gluLookAt (pos[0], pos[1], pos[2], pos[0] + target[0], pos[1] + target[1], pos[2] + target[2], up[0], up[1], up[2]);


	// send the light position down as if it was a vertex in world coordinates
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);



	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);


	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,2.0);
		glVertex3f(-50,-50,0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(100,-50,0);
		glTexCoord2f(2.0,0.0);
		glVertex3f(100,50,0);
		glTexCoord2f(2.0,2.0);
		glVertex3f(-50,50,0);
	glEnd();

	glPushMatrix();
	glLoadIdentity();
	showSpeed(30, 100, 0, 1, velocity);
	glPopMatrix();

	glutSwapBuffers();
	glFlush ();

	glutPostRedisplay();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,(float)w/h,0.01,10.0);
	glMatrixMode (GL_MODELVIEW);
}

void gearUp(){
	if(gear < 6){
		rpm = (pi/2) * (velocity/actualSpeed[gear - 1]);
		gear++;
	}
}

void gearDown(){
	if(gear > 1){
		rpm = (pi/2) * (velocity/actualSpeed[gear]);
		gear--;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 97:
			gearUp();
			break;
		case 122:
			gearDown();
			break;
		case 27:
			exit(0);
			break;
   }
}

void speedUp(){
	if(acceleration < 0)
		acceleration = 0 - acceleration;
	if(rpm < pi){
		rpm += 100/(maxGearSpeed[gear-1]*maxGearSpeed[gear-1]);
		acceleration = sin(rpm)/gear;
	}
	else
		acceleration = 0;
	velocity += acceleration / 1000;
}

void slowDown(){
	if(velocity > 0)
		velocity -= 0.001;
	else
		velocity = 0;
}

void moveLeft(){
	pos[1]++;
}

void moveRight(){
	pos[1]--;
}


/**
 * This function will determine the movement of the plane depending on which key is pressed
 */
void movement(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP:
			speedUp();
			break;
		case GLUT_KEY_DOWN:
			slowDown();
			break;
		case GLUT_KEY_LEFT:
			moveLeft();
			break;
		case GLUT_KEY_RIGHT:
			moveRight();
			break;
	}
}

/**
 * Function provides a timer for the animation. This is how the velocity of the plane is implemented
 */
void timer(int v)
{
	pos[0] += velocity * target[0];
	pos[1] += velocity * target[1];
	pos[2] += velocity * target[2];

	glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Drag Racer Unlimited");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(movement);
   glutTimerFunc(100,timer,nFPS);
   glutMainLoop();
   return 0;
}
