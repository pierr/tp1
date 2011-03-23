//
//  Zone.h
//  tp1
//
//  Created by pierr on 16/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//

//This class is made in order to be able to identif a zone of the grid.

#ifndef ZONE_H
#define ZONE_H
class Zone{
public:
	Vec3Df p;
	Vec3Df q ;
	unsigned int num;
	inline Zone( Vec3Df const &p1, Vec3Df const &p2 ,unsigned int numero){
		p = p1;
		q = p2;
		num = numero;
	}
};
#endif