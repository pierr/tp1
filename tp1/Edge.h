//
//  Edge.h
//  tp1
//
//  Created by pierr on 23/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//
#ifndef EDGE_H
#define EDGE_H
#include "Vertex.h"
#include <vector>
#include <iostream>

//bool operator== (const Edge & e1, const Edge & e2) {
 //   return (e1.v == p2[0] && p1[1] == p2[1] && p1[2] == p2[2]);
//}


class Edge{

public:
    //Ce sont les indices des vertex correspondant
    unsigned int v1;
    unsigned int v2;
    inline Edge(){
    
    }
    inline Edge(unsigned int _v1, unsigned int _v2){
       // std::cout << "------ New Edge -- " << std::endl;
        //std::cout << _v1 << " " << _v2 << std::endl;
        if(_v1 < _v2){
            this->v1 = _v1;
            this->v2 = _v2;
        }else{
            this->v1 = _v2;
            this->v2 = _v1;
        }
        
    }
    inline Edge (const Edge & e) {
        this->v1 = e.v1;
        this->v2 = e.v1;
    }
    inline Edge& operator= (const Edge & e) {
        this->v1 = e.v1;
        this->v2 = e.v2;
        return (*this);
    };
    inline bool operator==( const Edge& e){
        return (v1 == e.v1 && v2 == e.v2);
    };
   /* inline bool operator<( const Edge & e){
        return v1 < e.v1; 
    };
     inline bool operator!=( const Edge & e){
        return !(v1 == e.v1 && v2 == e.v2);
    };
    
    inline bool operator>( const Edge & e ){
        return !(v1 < e.v1);
    };
    inline bool operator<=( const Edge & e){
        return (*this)==e || (*this)<e;
    };
    inline bool operator>=( const Edge & e ){
        return (*this)==e || (*this)>e;
    };*/

    
};
inline bool operator<( const Edge & e1, const Edge & e2){
    return e1.v1 < e2.v1; 
};

#endif