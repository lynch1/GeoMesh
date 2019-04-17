#ifndef facet_h
#define facet_h
#include "geomesh_fcns.h"

//Definition of Facet type
typedef struct {
	float v1[3];
	float v2[3];
	float v3[3];
	float n[3];
} Facet;

// Function prototypes for Facet type functions

// Makers the surface facets
void makeFacets(float e[ROW][COLUMN], Facet *f1, Facet *f2, int i, int j);

// Makes the vertical Facets
void makeWalls(float e[ROW][COLUMN], Facet *f1, Facet *f2, char c, int m);

// Makes the bottom surface of the solid
void makeBottom(float e[ROW][COLUMN], Facet *f1, Facet *f2);

// Prints Facets
void printFacet(FILE *fp, Facet *f1);
#endif
