//
//  Edge.h
//  tp1
//
//  Created by pierr on 23/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//
#ifndef MESH_H
#define MESH_H
#include "Vertex.h"
class Edge{

public:
    Vertex v1;
    Vertex v2;
    inline Edge(Vertex & _v1, Vertex & _v2){
        this->v1 = _v1;
        this->v2 = _v2;
    }
    inline Edge (const Edge & e) {
        this->v1 = e.v1;
        this->v2 = e.v1;
    }
    inline Edge& operator= (const Edge & e) {
        this->v1 = e.v1;
        this->v2 = e.v1;
        return (*this);
    };
    inline bool operator==( const Edge& e){
        return e.v1 == v1 && e.v2 == v2;
    };
    inline bool operator!=( const Edge & e){
        return !(e.v1 == v1 && e.v2 == v2);
    };
    inline bool operator<( const Edge & e){
        
    };
    inline bool operator>( const Edge & e ){
    
    };
    inline bool operator<=( const Edge & e){
        
    };
    inline bool operator>=( const Edge & e ){
    
    };

    
};

#endif