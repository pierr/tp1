//
//  Mesh.cpp
//  tp1
//
//  Created by pierr on 16/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Mesh.h"
using namespace std;
// -------------------------------------------
// Basic Mesh class
// -------------------------------------------

void Mesh::loadOFF (const string & filename) {
    ifstream in (filename.c_str ());
    if (!in) 
        exit (EXIT_FAILURE);
    string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    V.resize (sizeV);
    T.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> V[i].p;
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> T[i].v[j];
    }
    in.close ();
    centerAndScaleToUnit ();
    recomputeNormals ();
}

void Mesh::reloadOFF(){
	if(fileName == "" ){
		cerr << "There is no OFF file to reload" << endl;		
	}else{
		((Mesh*)this)->Mesh::loadOFF(fileName);
	}
}

void Mesh::recomputeNormals () {
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n = Vec3Df (0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < T.size (); i++) {
        Vec3Df e01 = V[T[i].v[1]].p -  V[T[i].v[0]].p;
        Vec3Df e02 = V[T[i].v[2]].p -  V[T[i].v[0]].p;
        Vec3Df n = Vec3Df::crossProduct (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            V[T[i].v[j]].n += n;
    }
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n.normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3Df c;
    for  (unsigned int i = 0; i < V.size (); i++)
        c += V[i].p;
    c /= V.size ();
    float maxD = Vec3Df::distance (V[0].p, c);
    for (unsigned int i = 0; i < V.size (); i++){
        float m = Vec3Df::distance (V[i].p, c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < V.size (); i++)
        V[i].p = (V[i].p - c) / maxD;
}


void Mesh::makeCube(){
	//On crée les sommets un par un avec une normale nulle
	V.push_back(
                Vertex(
                       Vec3Df(0.,0.,0.), Vec3Df(0.,0.,0.)
                       )
                );
	V.push_back(Vertex(Vec3Df(1.,0.,0.), Vec3Df(0.,0.,0.)));
	V.push_back(Vertex(Vec3Df(0.,1.,0.), Vec3Df(0.,0.,0.)));
	V.push_back(Vertex(Vec3Df(0.,0.,1.), Vec3Df(0.,0.,0.)));
	V.push_back(Vertex(Vec3Df(1.,1.,0.), Vec3Df(0.,0.,0.)));
	V.push_back(Vertex(Vec3Df(1.,0.,1.), Vec3Df(0.,0.,0.)));
	V.push_back(Vertex(Vec3Df(0.,1.,1.), Vec3Df(0.,0.,0.)));
	V.push_back(Vertex(Vec3Df(1.,1.,1.), Vec3Df(0.,0.,0.)));
	
	//On recrée les normales
	recomputeNormals();
	
	//On ajoute les triangles (attention il faut que les triangles soient dans le sens trigo quand la camera regarde)
	T.push_back(Triangle(0,1,5));
	T.push_back(Triangle(0,5,3));
	T.push_back(Triangle(3,5,7));
	T.push_back(Triangle(3,7,6));
	T.push_back(Triangle(1,4,7));
	T.push_back(Triangle(1,7,5));
	T.push_back(Triangle(2,0,6));
	T.push_back(Triangle(2,6,3));
	T.push_back(Triangle(1,4,7));
	T.push_back(Triangle(1,7,5));
	T.push_back(Triangle(2,4,1));
	T.push_back(Triangle(2,1,0));
}

void Mesh::makeSphere(unsigned int resU, unsigned int resV){
	//n1 et n2 sont les nb de méridiens et de 
	float du = 2*PI/resU;
	float dv = -PI/resV;
	float u = 0;
	float v = PI;
	int k =0;
    //	V.push_back(Vertex(Vec3Df(0.,0.,-1.), Vec3Df(0.,0.,0.)));
	for(unsigned int j = 0; j< resV; j++){
		for(unsigned int i =0 ; i< resU ; i++){
			//On calcule les points avec les coordonnées sphériques.
			V.push_back(Vertex(Vec3Df(cos(u)*sin(v),sin(u)*sin(v),cos(v)), Vec3Df(0.,0.,0.)));
			//On incrémente u 
			//cout << "k " << k << " " << "1 " << cos(u)*sin(v) << " 2 " << sin(u)*sin(v) << " 3 " << cos(v) << endl; 
			u = u + du;
			k++;
			
		}
		//On incrémente v
		v = v + dv;
	}
	V.push_back(Vertex(Vec3Df(0.,0.,1.), Vec3Df(0.,0.,0.)));
	
	recomputeNormals();
	//On dessine les triangles.
    
	for(unsigned int i =0 ; i< resU*(resV)-resU ; i++){
		if(i < resU*resV-resU){
			T.push_back(Triangle(i,i+1,i+resU));
			T.push_back(Triangle(i+1,i+resU+1,i+resU));
		}else{
			T.push_back(Triangle(i,i+1,resV*resU));
		}
	}
}

void Mesh::smooth(double a){
    
	//Vérification de la cohérence du paramêtre de lissage a
	if(a >1 || a <0){
		cerr << "The value of the coefficient alpha must be between  0 et 1" << endl; 
	}else {
		cerr << "We aply the smooth fonction to the mesh with a parameter alpha = " << a << endl;
	}
	
	//On récupère la taille des listes
	unsigned int sizeT =  (unsigned int) T.size();
	unsigned int sizeV = (unsigned int) V.size();
	//On vérifie la taille de la liste de sommets
	if( Grid::DEB ==1){
		cout<< "taille V" << sizeV << endl;
	}
	vector<Vertex> Vp;
	//On crée un vecteur qui contient tous les index
	vector<int> index(sizeV);
	//On crée un vecteur qui contiendra les coordonnées des nouveaux sommets.
	vector<Vec3Df> nV(sizeV);
	
	//Grid::DEB
	if( Grid::DEB == 1){
		for(unsigned int i =0; i< 7 ; i++){
			cout  << "ancien vecteur  " << i<< " coord 0 " << V[i].p[0] <<" coord 1 " << V[i].p[1] << "coord 2" << V[i].p[2] << endl;		
		}
		for(unsigned int i=0; i< 7; i++){
			cout << "nV "<< i  <<" " << nV[i][0] << " " << nV[i][1] << " " << nV[i][2] <<  endl;
			cout << "index "<< i  <<" " << index[i] <<  endl;
		}
		
		//On affiche les coordonnées des anciens vecteurs
		for(unsigned int i =0; i< 7 ; i++){
			cout  << "ancien vecteur  " << i<< " coord 0 " << V[i].p[0] <<" coord 1 " << V[i].p[1] << "coord 2" << V[i].p[2] << endl;		
		}
		for(unsigned int i=0; i< 7; i++){
			//index[i] = 0;
			//On regarde les indices des sommets composants chaque triangle
			cout << "-----------" << endl;
			cout << "les trois  Vertex du triangle d'indice " << i << " valent " << T[i].v[0] << " " << T[i].v[1] << " " <<T[i].v[2] << endl; 
			cout << "les valeurs des coordonées du premier sommet"  << " 0 " << V[T[i].v[1]].p[0] << " 1 " << V[T[i].v[1]].p[1] << " 2 " << V[T[i].v[1]].p[2] << " 1+2+3 " <<  V[T[i].v[1]].p[0] + V[T[i].v[1]].p[1] + V[T[i].v[1]].p[2]<< endl;
			Vec3Df test = V[T[i].v[1]].p + V[T[i].v[0]].p;
			cout << "pour le triangle 1 le barycentre a pour coordonnées 1+2 " << test[0] << endl;
		} 
	}
    
	
	//On parcours tous les triangles
	for(unsigned int i =0; i < sizeT ; i++){
		
		//On parcourt chaque sommet du triangle
		nV[T[i].v[0]] = nV[T[i].v[0]] + ( V[T[i].v[1]].p + V[T[i].v[2]].p ); //On ajoute au vecteur position du sommet de sortie la position du nouveau barycentre calculé
		nV[T[i].v[1]] = nV[T[i].v[1]] + ( V[T[i].v[0]].p + V[T[i].v[2]].p );//On ajoute au vecteur position du sommet de sortie la position du nouveau barycentre calculé 
		nV[T[i].v[2]] = nV[T[i].v[2]] + ( V[T[i].v[1]].p + V[T[i].v[0]].p ); //On ajoute au vecteur position du sommet de sortie la position du nouveau barycentre calculé
		
		//On augmente la cardinalité de deux pour chaque sommet parcourue
		for(int j =0; j<3; j++){
			index[T[i].v[j]]+=2;
		} 
        
	}
	
	//On remplace tous les sommets par les sommets déplaçés
	for(unsigned int i =0 ; i< sizeV ; i++){
		V[i].p = (1-a)*V[i].p + a*nV[i]/index[i];
	}
	//On recalcule les normales puisque les sommets ont bougés.
	recomputeNormals();
	
	//Grid::DEB
	if(Grid::DEB ==1){
		for(unsigned int i =0; i< 7 ; i++){
			cout  << "nouveau  vecteur " << i<< " coord 0 " << V[i].p[0] <<" coord 1 " << V[i].p[1] << "coord 2" << V[i].p[2] << endl;		
		}
		for(unsigned int i =0; i< 7 ; i++){
			cout  << "cardinal numero " << i  <<  " valeur  "<< index[i] << endl;		
		}
	}
}

/**
 * This methode is done in order to reduce the number of vertices and so the number of triangles
 * To do so we use a grid whit a definition which has the value of the parameter
 */
void Mesh::simplifyMesh (unsigned int r){
	unsigned int sizeV = (unsigned int) V.size();
	cout << "La mesh a " << sizeV << "sommets " << endl;
	//On va parcourir tous les sommets et calculer la distance aux autres, à chaque fois qu'on a une distance 
	unsigned int v1 = 0;// indice du sommet 1
	unsigned int v2 = 0; //indice du sommet 2
	float dMax =0; // La distance max entre deux sommets de la mesh
    
	for( unsigned int i = 0; i < sizeV; i++ ){
		if(Grid::DEB ==1){
			cout << "------"<< endl;
			cout << "i " << i << endl;
		}
		for(unsigned int j = i+1; j< sizeV; j++){
			float d = Vec3Df::distance(V[i].p, V[j].p);
			if(Grid::DEB == 1){
				cout << "j " << j << endl;
				cout << "dMax "<< dMax <<  " d " << d << endl;
				cout << "sommet v1 " << v1 << " sommet v2 " << v2 << " distance " << dMax << endl;
			}
			if( d > dMax){
				dMax = d;
				v1 = i;
				v2 = j;
			}
		}
	}
	cout << "°°°°°°°°°°°°°°°°°°" << endl;
	cout << "sommet v1 " << v1 << " sommet v2 " << v2 << " distance " << dMax << endl;
	
	cout << "Les coordonnées du sommet v1 sont : x " << V[v1].p[0] << " y " << V[v1].p[1] << " z "<< V[v1].p[2] << endl; 
	cout << "Les coordonnées du sommet v2 sont : x " << V[v2].p[0] << " y " << V[v2].p[1] << " z "<< V[v2].p[2] << endl;
	
	//On crée la grille
	float lmax = sqrt(2)*dMax;
	Grid g =  Grid(lmax,r);
	int nZone = (unsigned int) g.cubeRegions.size();
	//On crée les récepteurs 
	vector< vector<unsigned int> > vertecesOfZone(nZone);//Un vecteur qui est indicé par la zone et qui contient la liste des indices sommets d'une zone.
	vector<unsigned int> zoneOfVertex(V.size());//Un vecteur indicé par les sommets et qui contient l'indice de la zone de ce sommet.
	vector<Vertex> vertexOfZone(nZone); 
	// faire une méthode qui remplit les vecteurs au dessus
	//faire une méthode qui crée le barycentre pour chaque zone
	//faire une méthode qui calcule les triangles restant pertinant
	
	//On parcours la liste des sommets et on les classe dans les régions du cube
	cout << "$$$$$$$$$$" << endl;
	for(unsigned int i =0; i< sizeV ; i++){
		//cout << "sommet " << i << " zone de la grille " << g.vertexRegion(V[i]) << endl;
		unsigned int nRegion = g.vertexRegion(V[i]);
		vertecesOfZone[nRegion].push_back(i);	
		zoneOfVertex[i] = nRegion;
		//Grid::DEB
		if(i < 5){
			cout << "sommet  " << i << " région " << nRegion << endl; 
		}
	}
	
	for(unsigned int i=0; i< vertecesOfZone.size(); i++){
		unsigned int taille = (unsigned int) vertecesOfZone[i].size();
		for (unsigned int j =0; j<taille ; i++ ){
		//	cout <<"vertex " <<j << "zone" << i	 << vertecesOfZone[i][j].p[0] << endl;
		}
	}
	/*
     //Calcul du barycentre de chaque zone
     
     for(unsigned int i=0; i< vertecesOfZone.size(); i++){
     Vec3Df vTemp;
     unsigned int taille = vertecesOfZone[i].size();
     for (unsigned int j =0; j<taille ; i++ ){
     vTemp += V[vertecesOfZone[i][j]].p;//A chaque zone on ajoute les coordonnées de tous les certex de cette zone
     //Grid::DEB
     if(i < 5){
     cout << "zone " << i << "vertex " << j  << " x " << V[vertecesOfZone[i][j]].p[0] << " y " << V[vertecesOfZone[i][j]].p[1] << " z "  << V[vertecesOfZone[i][j]].p[2]<< endl; 
     }
     }
     if(taille >0 ){
     cout << "Zone non vide" << endl;
     //vertexOfZone[i] = Vertex(vTemp/vertecesOfZone[i].size(), Vec3Df(0.,0.,0.));//On divise par le nombre de vertex dans chaque zone
     }
     if(vertexOfZone[i].p[0] != 0){
     cout << "test "<<vertexOfZone[i].p[0] << endl;
     }
     }
     
     //Calcul des nouveaux triangles 
	 vector<Triangle> Ttemp;
     for(unsigned int i=0; i< T.size(); i++){
     unsigned int reg0 = zoneOfVertex[T[i].v[0]];
     unsigned int reg1 = zoneOfVertex[T[i].v[1]];
     unsigned int reg2 = zoneOfVertex[T[i].v[2]];
     //Grid::DEB
     //cout << "reg0" << reg0 << " reg1 " << reg1 << " reg2 " << reg2 << endl;
     if(reg0 != reg1 && reg1 != reg2 && reg2!= reg0){//Si les trois vertex appartiennent à une zone diférente on conserve le triangle.
     Ttemp.push_back(Triangle(reg0, reg1, reg2));//On construit la nouvelle liste de triangle intermédiaire
     }
     }
     //cout << "taille de T "<< T.size() << " taille du nouveau T " << Ttemp.size() << endl;
     //Création des nouveaux vecteurs T et V
     cout << "taille de V" << V.size() << " taille de T " << T.size() << endl; 
     V.clear();
     T.clear();
     V = vertexOfZone;
     T = Ttemp;
     cout << "après traitements" << endl;
     cout << "taille de V" << V.size() << " taille de T " << T.size() << endl;
     recomputeNormals();
     */
}