/*
 * Mesh.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: zeno
 */

#include <stdio.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Coordinate.h"
#include "Face.h"
#include "Mesh.h"
using namespace std;

Mesh::Mesh() {
	// TODO Auto-generated constructor stub

}

int Mesh::loadObject(const char* filename){

	 ifstream in(filename);

	    char buf[256];
	    //read in every line to coord
	    while(!in.eof()){
	        in.getline(buf,256);
	        coord.push_back(new std::string(buf));
	    }
	    printf("Coord size is %i\n", coord.size());
	    for(int i = 0; i < coord.size(); i++){
	        if(coord[i]->c_str()[0] == '#')
	            printf("Comment\n");
	        else if((coord[i]->c_str()[0] == 'v') && (coord[i]->c_str()[1] != 'n') && ((coord[i]->c_str()[1] != 't'))){
	            float tmpx, tmpy, tmpz;
	            sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);
	            vertex.push_back(new Coordinate(tmpx, tmpy, tmpz));
	        }
	        else if(coord[i]->c_str()[0] == 'f'){
	            float a, b, c;
	            sscanf(coord[i]->c_str(), "f  %f %f %f", &a, &b, &c);
	            faces.push_back(new Face(a, b, c));
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

	    return 0;

	}


	Mesh::~Mesh() {
		// TODO Auto-generated destructor stub
	}

	void Mesh::draw(){
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

