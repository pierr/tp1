//
//  Grid.cpp
//  tp1
//
//  Created by pierr on 16/03/11.
//  Copyright 2011 Télécom ParisTech. All rights reserved.
//

#include "Grid.h"
//Cette méthode permet de trouver l'indice de région dans le cube entourant
unsigned int Grid::vertexRegion(const Vertex & v){
	for(unsigned int i=0; i < cubeRegions.size() ; i++){
        
		//on teste si la région en x est bonne 
		//(peut être améliorable en ne faisant qu'une comparaison sachant qu'on en a fait une avant)
		if(cubeRegions[i].p[0] < v.p[0] && cubeRegions[i].q[0] > v.p[0]){
            
			//On teste maintanant la région en y
			if(cubeRegions[i].p[1] < v.p[1] && cubeRegions[i].q[1] > v.p[1]){
                
				//On teste maintanant la région en z
				if(cubeRegions[i].p[2] < v.p[2] && cubeRegions[i].q[2] > v.p[2]){
					return cubeRegions[i].num;
				}	
			}	
		}		
	}return NULL; 
}


//Permet de construire les régions du cube et de les mettre dans un vecteur
void Grid::constructGridRegion(){
	unsigned int nZone =0;//Le numéro de la zone
	//parcours en x
	for(unsigned int i =0; i < res; i++){
		//parcours y 
		for(unsigned int j = 0; j <res; j ++){
			//parcours en z
			for(unsigned int k = 0; k < res ; k++){
				cubeRegions.push_back(
                                      Zone(
                                           Vec3Df(firstPoint[0]+i*pas, firstPoint[1]+j*pas, firstPoint[2]+k*pas),//On ajoute toutes les coordonnées minimales
                                           Vec3Df(firstPoint[0]+(i+1)*pas, firstPoint[1]+(j+1)*pas, firstPoint[2]+(k+1)*pas),//toutes les max
                                           nZone
                                           )
                                      );
                nZone++;//We increment the number of the zone
			}
		}
	}
	
    
    //Juste pour vérifier qu'on a bien couper le cube en zone
	if(DEB ==1){
		for(int i =0; i < 10 ; i++ ){
            std::cout << "---------------------------" << std::endl;
			std::cout << "région " << cubeRegions[i].num << std::endl;
			std::cout << " coord xmin " << cubeRegions[i].p[0] << " coord xmax " << cubeRegions[i].q[0] << std::endl;		
			std::cout << " coord ymin " << cubeRegions[i].p[1] << " coord ymax " << cubeRegions[i].q[1] << std::endl;
			std::cout << " coord zmin " << cubeRegions[i].p[2] << " coord ymax " << cubeRegions[i].q[2] << std::endl;
		}
	}
    
}
