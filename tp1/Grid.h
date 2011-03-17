//
//  Grid.h
//  tp1
//
//  Created by pierr on 16/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//
#ifndef GRID_H
#define GRID_H
#include <vector>
#include <iostream>
#include "Vec3D.h"
#include "Zone.h"
#include "Vertex.h"

//This class corresponds to the grid 
class Grid {
public:
	Vec3Df firstPoint;//Le vertex associé au point de départ du site.
	float length;//Longeur d'arrête du cube.
	unsigned int res;//Résolution de la grille
	float pas;
    std::vector<Zone> cubeRegions;
	static const int DEB = 1;
    inline Grid(float l, unsigned int r){
		firstPoint = Vec3Df(-l,-l,-l);
		length = l;
		res = r;
		pas = 2*l/res;
        std::cout << "We construct a grid-cube of length "<< l << std::endl;
		constructGridRegion();//On construit l'ensemble des zones du cube
	}
	//Donne la région du cube pour un vertex donné.
	unsigned int vertexRegion(const Vertex & v);
	void constructGridRegion();
};
#endif