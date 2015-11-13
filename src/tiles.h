#ifndef _TILES_H_
#define _TILES_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_NUM_VARS 20        // Maximum number of variables in a grid-tiling      

void get_tiles0(int tiles[],int num_tilings,double variables[], int num_variables, int memory_size);
void get_tiles1(int tiles[],int num_tilings,double variables[], int num_variables, int memory_size,int hash1);
void get_tiles2(int tiles[],int num_tilings,double variables[], int num_variables, int memory_size,int hash1, int hash2);
void get_tiles3(int tiles[],int num_tilings,double variables[], int num_variables, int memory_size,int hash1, int hash2, int hash3);

//void GetTiles(int tiles[],int num_tilings,double variables[], int num_variables, 
		   //int memory_size,int hash1=-1, int hash2=-1, int hash3=-1);


#endif
