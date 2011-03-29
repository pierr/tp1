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
#include "Edge.h"
#include <map>

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
    void subdivideLoop();
    
protected:
    int farVertex();
    void correspGridVertex(std::vector< std::vector<unsigned int> >& vertecesOfZone, std::vector<unsigned int> & zoneOfVertex , Grid & g);
    std::vector<Vec3Df> eachZoneBarycentre(std::vector< std::vector<unsigned int> >& vertecesOfZone,std::vector<unsigned int> & correspZoneNVertex);
    void recalculateTriangleAndVerteces(std::vector<unsigned int> & zoneOfVertex, std::vector<unsigned int> & correspZoneNVertex, std::vector<Vec3Df> baryVector);
    unsigned int insertEdge( std::map<Edge, unsigned int>& mapEdge, Edge & e);
};
#endif