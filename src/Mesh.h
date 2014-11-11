/*
 * Mesh.h

 *
 *  Created on: Feb 15, 2014
 *      Author: zeno
 */
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Coordinate.h"
#include "Face.h"
using namespace std;

#ifndef MESH_H_
#define MESH_H_

class Mesh {
public:
	Mesh();
	virtual ~Mesh();
	int loadObject(const char *filename);
	void draw();
private:
	vector<string*>coord;
	vector<Coordinate*> vertex;
	vector<glm::vec3> normals;
	vector<Face*> faces;
};

#endif /* MESH_H_ */
