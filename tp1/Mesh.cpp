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
#include "Edge.h"
#include <map>

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

int Mesh::farVertex(){
    int farIndex = 0;
    float dMax = 0;
    Vec3Df center = Vec3Df(0.,0.,0.);
    for (unsigned int i =0; i< this->V.size() ; i++){
        float d = Vec3Df::distance(center, this->V[i].p);
        if(d > dMax){
            dMax = d;
            farIndex =i;
        } 
    }
    return farIndex;
}


/**
 * This methode is done in order to reduce the number of vertices and so the number of triangles
 * To do so we use a grid whit a definition which has the value of the parameter
 */
void Mesh::correspGridVertex(vector< vector<unsigned int> >& vertecesOfZone, vector<unsigned int> & zoneOfVertex , Grid & g){
    for(unsigned int i =0; i< this->V.size() ; i++){
		//cout << "sommet " << i << " zone de la grille " << g.vertexRegion(V[i]) << endl;
		unsigned int nRegion = g.vertexRegion(V[i]);
		vertecesOfZone[nRegion].push_back(i);	
		zoneOfVertex[i] = nRegion;
    }
}

 std::vector<Vec3Df> Mesh::eachZoneBarycentre(std::vector< std::vector<unsigned int> >& vertecesOfZone,std::vector<unsigned int> & correspZoneNVertex/*,std::vector<Vec3Df> & baryVect*/){
     vector<Vec3Df> baryVect;
    //Pour toutes les zones on va calculer le barycentre des sommets la composant
    for(unsigned int i =0; i< vertecesOfZone.size(); i++){
        unsigned int nbSommetInZone = vertecesOfZone[i].size();
        //La notion de barycentre d'une zone n'a de sens que si il y a au moins un sommet dans cette zone.
        if(nbSommetInZone > 0){
            Vec3Df tempBary;
            for(unsigned k =0; k< nbSommetInZone; k++){
                tempBary += V[vertecesOfZone[i][k]].p;
            }
            tempBary/=nbSommetInZone;
            baryVect.push_back(tempBary);
            correspZoneNVertex[i] = baryVect.size()-1;
        }        
    }
     return baryVect;
}

void Mesh::recalculateTriangleAndVerteces(std::vector<unsigned int> & zoneOfVertex, std::vector<unsigned int> & correspZoneNVertex, std::vector<Vec3Df> baryVector){

    //    On crée un nouveau vecteur de Triangle
    std::vector<Vertex> nV;
    std::vector<Triangle> nT; 
    //Pour chaque triangle
    for(unsigned int i =0; i< T.size(); i++){
        bool isv1v2different = (zoneOfVertex[T[i].v[1]] != zoneOfVertex[T[i].v[2]]);
        bool isv0v1different = (zoneOfVertex[T[i].v[0]] != zoneOfVertex[T[i].v[1]]);
        bool isv0v2different = (zoneOfVertex[T[i].v[0]] != zoneOfVertex[T[i].v[2]]);
        
        //Si les trois triangles sont dans une zone séparée alors on le triangle existe toujours
        if(isv0v1different && isv0v2different && isv1v2different){
            nT.push_back(Triangle(
                                  correspZoneNVertex[zoneOfVertex[T[i].v[0]]] ,
                                  correspZoneNVertex[zoneOfVertex[T[i].v[1]]],
                                  correspZoneNVertex[zoneOfVertex[T[i].v[2]]]
                                  )
                         );
        }
    }
    //On remplace la liste d'ancien triangles par les nouveaux
    T = nT;
    
    //On recréer un vecteur de vertex avec les sommets barycentryques précédemment calculés.
    Vec3Df iNormal;
    for(unsigned int i =0; i<baryVector.size();i++){
        nV.push_back(Vertex(baryVector[i], iNormal));
    } 
    //On remplace les anciens vertex par les nouveaux
    V =nV;
    recomputeNormals();
    
}


/**
 *Here we implement an operator of Mesh simplification. We use a grid
 */
void Mesh::simplifyMesh (unsigned int r){
    cout << "°°°°°°We apply a simplification algo with a grid of resolution " << r << " °°°°°°" << endl;
    int v = farVertex();//On cherche le sommet le plus éloigné du centre.
    float dMax = Vec3Df::distance(Vec3Df(0.,0.,0.),this->V[v].p); //On calcule la distance entre ces points
	//On crée la grille
	float lmax = sqrt(2)*dMax;
	Grid g =  Grid(lmax,r);
    
    //On appelle une fonction qui permet de partir d'une grille et de lister les sommets correspondants
	int nZone = (unsigned int) g.cubeRegions.size();
	
	vector< vector<unsigned int> > vertecesOfZone(nZone);//Un vecteur qui est indicé par la zone et qui contient la liste des indices sommets d'une zone.
	
    vector<unsigned int> zoneOfVertex(V.size());//Un vecteur indicé par les sommets et qui contient l'indice de la zone de ce sommet.
	//vector<Vertex> vertexOfZone(nZone); 
    
    //Méthode qui permet d'avoir des index de sommets par zone de grille et l'index réciproque
    correspGridVertex(vertecesOfZone, zoneOfVertex, g);
    
    //Vecteur qui contient les indices des nouveaux sommés ordonnés par zones de grille.
    vector<unsigned int> nVertexOfZone(nZone);
    
    //Méthode qui permeet de récupérer un vecteur contenant les barycentres de chaque zones.
    vector<Vec3Df> barycentreVector = eachZoneBarycentre(vertecesOfZone, nVertexOfZone);
 
    //Cette méthode permet de redéfinir les triangles à partir de ceux qui ont été calculés.
    recalculateTriangleAndVerteces(zoneOfVertex, nVertexOfZone, barycentreVector);
    
}

int getHashKey(int k, int l){
    if(k>l){
        return (k+l)*(k-l);
    }return (k+l)*(l-k);
} 

unsigned int Mesh::insertEdge( std::map<Edge, unsigned int>& mapEdge, Edge & e){
    std::pair<std::map<Edge, unsigned int>::iterator,bool> ret = mapEdge.insert(std::pair<Edge,int>(e,V.size()));
   /* cout << mapEdge.size() << endl;
    cout << "Is it existing " << ret.second << endl;
    cout << "The corresponding vertex is " << ret.first->second << endl;*/
    if(ret.second == true){
        V.push_back(Vertex((Vec3Df(V[e.v1].p)+Vec3Df(V[e.v2].p))/2, Vec3Df()));  
    }
    return ret.first->second;
}
/**
 *This fonction is made in order to add vertices and triangles with a subdivision algorthm (loop subdiv algo.)
 *You can look to the following paper:« A factored Approach to Subdivision Surfaces », Warren and Schaefer, 2004 (http://faculty.cs.tamu.edu/schaefer/research/tutorial.pdf)
 */
void Mesh::subdivideLoop(){
    //On crée tous les conteneurs utiles/
    vector<Triangle> nT;
    std::map<Edge, unsigned int> mapEdge;
    /*
    int indexV0=T[0].v[0];
    int indexV1=T[0].v[1];
   // int indexV2=T[0].v[2];
    Edge e0(indexV0,indexV1);
        Edge e1(indexV1,indexV0);
    insertEdge(mapEdge, e0);
    insertEdge(mapEdge, e1);
    */
    for(unsigned int i =0; i< T.size(); i++){
        unsigned int indexV0=T[i].v[0];
        unsigned int indexV1=T[i].v[1];
        unsigned int indexV2=T[i].v[2];
        //On crée les 3 arrêtes du triangle 
        Edge e0(indexV0,indexV1);
        Edge e1(indexV1,indexV2);
        Edge e2(indexV0,indexV2);
        
        //traitement arrete e0
        unsigned int indexE0 = insertEdge(mapEdge, e0);
        unsigned int indexE1 = insertEdge(mapEdge, e1);
        unsigned int indexE2 = insertEdge(mapEdge, e2);
       //Construction des triangles
        nT.push_back(Triangle(indexV0, indexE0, indexE2));
        nT.push_back(Triangle(indexE0, indexE1, indexE2));
        nT.push_back(Triangle(indexE0, indexV1, indexE1));
        nT.push_back(Triangle(indexE2, indexE1, indexV2));
       
    }
    recomputeNormals();
    T = nT;
}

