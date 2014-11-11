#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

float position[] = {-10.0f, -10.0f, 0.0f};
float velocity = 0, acceleration = 0, maxAcceleration = 5;
float upVector[] = {0.0f, 0.0f, 1.0f};
float target[] = {10.0f, 10.0f, 0.0f};
float maxGearSpeed[] = {25, 50, 70, 100, 140, 170};
int fps = 30;
int gear = 1, rpm = 0;


//struct to hold the coordinates for each vertex
struct coordinate{
	float x, y, z;
	coordinate(float a, float b, float c) : x(a), y(b), z(c) {};
};

//struct that will store the indices for each face
struct face{
	int faces[3];
	face(int f1, int f2, int f3){
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
	}
};

vector<string*>coord;
vector<coordinate*> vertex;
vector<glm::vec3> normals;
vector<face*> faces;

int loadObject(const char* filename){

	 ifstream in(filename);

	    char buf[512];
	    //read in every line to coord
	    while(!in.eof()){
	        in.getline(buf,512);
	        coord.push_back(new string(buf));
	    }
	    printf("Coord size is %i\n", coord.size());
	    for(int i = 0; i < coord.size(); i++){
	        if(coord[i]->c_str()[0] == '#'){
	        }
	        else if((coord[i]->c_str()[0] == 'v') && (coord[i]->c_str()[1] != 'n') && ((coord[i]->c_str()[1] != 't'))){
	            float tmpx, tmpy, tmpz;
	            sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);
	            vertex.push_back(new coordinate(tmpx, tmpy, tmpz));
	        }
	        else if(coord[i]->c_str()[0] == 'f'){
	            float a, b, c, a1, a2, b1, b2, c1, c2;
	            sscanf(coord[i]->c_str(), "f  %f/%f/%f %f/%f/%f %f/%f/%f", &a, &a1, &a2, &b, &b1, &b2, &c, &c1, &c2);
	            faces.push_back(new face(a, b, c));
	        }
	        else if((coord[i]->c_str()[0] == 'v') && (coord[i]->c_str()[1] == 'n')){
	        	float a, b, c;
	        	sscanf(coord[i]->c_str(), "f  %f %f %f", &a, &b, &c);
	        	normals.push_back(glm::vec3(a, b, c));
	        }
	        else{
	            //printf("Garbage\n");
	        }
	    }
	    printf("vertices: %d\nnormals: %d\n", vertex.size(), normals.size());
	    printf("faces: %d\n", faces.size());
	    return 0;
}

void draw(){
	for(int i = 0; i < faces.size(); i++){
        glBegin(GL_TRIANGLES);
            glNormal3f(normals[faces[i]->faces[0]-1].x,normals[faces[i]->faces[0]-1].y,normals[faces[i]->faces[0]-1].z);
            glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
            glNormal3f(normals[faces[i]->faces[1]-1].x,normals[faces[i]->faces[1]-1].y,normals[faces[i]->faces[1]-1].z);
            glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
            glNormal3f(normals[faces[i]->faces[2]-1].x,normals[faces[i]->faces[2]-1].y,normals[faces[i]->faces[2]-1].z);
            glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
        glEnd();
    }
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

	glClearColor (0.5, 0.5, 1.0, 0.0);	// sky
	glEnable(GL_DEPTH_TEST);


}

void display(void)
{
	GLfloat seaamb[] = {0.0,0.0,0.2,1.0};
	GLfloat seadiff[] = {0.0,0.0,0.8,1.0};
	GLfloat seaspec[] = {0.5,0.5,1.0,1.0};	// Single polygon, will only have highlight if light hits a vertex just right

	GLfloat lpos[] = {0.0,0.0,10.0,0.0};	// sun, high noon


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();             /* clear the matrix */



	/* viewing transformation, look at the origin  */
	gluLookAt (-10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// send the light position down as if it was a vertex in world coordinates
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);



	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);


	glColor3f(1.0, 0.0, 0.0);//color = red
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
		glVertex3f(-5,-5,0);
		glVertex3f(5,-5,0);
		glVertex3f(5,5,0);
		glVertex3f(-5,5,0);
	glEnd();

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

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
		case 27:
			exit(0);
			break;
   }
}


void gearUp(){
	if(gear < 6)
		gear++;
	acceleration = maxAcceleration;
}

void gearDown(){
	if(gear > 1)
		gear--;
}

void speedUp(){
	acceleration--;
	velocity += acceleration;
	position[0] += velocity;
}

void slowDown(){
	if(velocity > 1)
		velocity--;
	else
		velocity = 0.0;
	position[0] += velocity;
}

void moveLeft(){
	position[2]--;
}

void moveRight(){
	position[2]++;
}

void move(int key, int x, int y){
	switch(key){
		case 97:
			gearUp();
			break;
		case 122:
			gearDown();
			break;
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

void timer(int v){
	position[0] += velocity * target[0];
	position[1] += velocity * target[1];
	position[2] += velocity * target[2];

	glutTimerFunc(1000/fps,timer,v);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(move);
   glutTimerFunc(100,timer,fps);
   glutMainLoop();
   return 0;
}
