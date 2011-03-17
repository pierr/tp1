//
//  Mesh.h
//  tp1
//
//  Created by pierr on 16/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//
#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "Vertex.h"
#include "Triangle.h"
#include "Grid.h"
class Mesh {
public:
    std::vector<Vertex> V;
    std::vector<Triangle> T;
    std::string fileName;
    static int const  PI = 3.14159265;
    void loadOFF (const std::string & filename);
    void recomputeNormals ();
    void centerAndScaleToUnit ();
    void scaleUnit ();
	void makeCube();
	void makeSphere(unsigned int n1, unsigned int n2);
	void smooth(double a);
	void reloadOFF();
	void simplifyMesh (unsigned int r);
    //private:
	//void correspVertecesZone(Grid &g, );
};
#endif