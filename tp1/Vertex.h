//
//  Vertex.h
//  tp1
//
//  Created by pierr on 16/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//
#ifndef VERTEX_H
#define VERTEX_H
#include "Vec3D.h"
class Vertex {
public:
    inline Vertex () {}
    inline Vertex (const Vec3Df & p, const Vec3Df & n) : p (p), n (n) {}
    inline Vertex (const Vertex & v) : p (v.p), n (v.n) {}
    inline virtual ~Vertex () {}
    inline Vertex & operator= (const Vertex & v) {
        p = v.p;
        n = v.n;
        return (*this);
    }
    inline bool operator==( const Vertex& v){
        return p == v.p && v.n == n;
    };
    inline bool operator<( const Vertex& v){
        return p < v.p &&   n < v.n;
    };
    Vec3Df p;
    Vec3Df n;
};
#endif