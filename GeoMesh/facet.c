#define _CRT_SECURE_NO_WARNINGS
/* This is a file for the Facet datatype and related functions for the Geomesh project.
 The Facet type will contain three vertices and a normal vector which all have x, y, z coordinates
 There will be a single facet that keeps getting rewritten since I don't want to or need to save all of the
 information for all of the facets at a given time.
 A function will get the pointer to this facet, the pointer to the 2D array of elevations, and then the current
 i, j coordinates of the cursor and will rewrite the correct facet. Then main will print the facet after every
 re-write.
*/

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "facet.h"

// Functions for facet type

//makeFacet() will get the pointer to the next two facets, the pointer to the 2D array of elevations, and then the current 
// i, j coordinates of the cursor and will write the next facets over the old ones. Then main will print the facet after 
//every re-write.
void makeFacets(float e[ROW][COLUMN], Facet *f1, Facet *f2, int i, int j) {
	// e[][] is the 2D array that holds elevation values
	// I don't want to change the values in it, but I also don't want to have to copy it for every
	// facet calculation
	// Facet f1, f2 are the facets that make the last projected square on the grid that has been printed
	// i and j are the coordinates of the cursor. 

	// cC[] contains the x, y, z coordinates of the cursor
	// i correspond to rows so it indexes the y-axis of grid
	// j corresponds to columns so it indexes the x-axis of grid
	float cC[3] = { (j * DX),  (((ROW - 1) - i) * DY), e[i][j] };
	// cR[] contains the x, y, z coordinates of the vertex to the right of the cursor
	float cR[3] = { ((j + 1) * DX), (((ROW - 1) - i) * DY), e[i][j + 1] };
	// cD[] contains the x, y, z coordinates of the vertex down one from the cursor
	float cU[3] = { (j * DX), ((ROW - i) * DY), e[i - 1][j] };
	// cDR[] contains the x, y, z coordinate of the vertes one down and one to the right of the cursor
	float cUR[3] = { ((j + 1) * DX), (( ROW - i) * DY), e[i - 1][j + 1] };
	// a[] and b[] and c[] are vectors of the edge of the facet that will be used to caluculate the normals
	float a[3] = { 0 };
	float b[3] = { 0 };
	float c[3] = { 0 };
	float norm[3] = { 0 };
	float mag = 0;

	//Find the edge vectors
	a[0] = cUR[0] - cC[0];
	a[1] = cUR[1] - cC[1];
	a[2] = cUR[2] - cC[2];
	b[0] = cU[0] - cC[0];
	b[1] = cU[1] - cC[1];
	b[2] = cU[2] - cC[2];
	c[0] = cR[0] - cC[0];
	c[1] = cR[1] - cC[1];
	c[2] = cR[2] - cC[2];

	// Update f1.v1[] vector
	f1->v1[0] = cC[0];
	f1->v1[1] = cC[1];
	f1->v1[2] = cC[2];
	// Update f1.v2[] vector
	f1->v2[0] = cUR[0];
	f1->v2[1] = cUR[1];
	f1->v2[2] = cUR[2];
	// Update f1.v3[] vector
	f1->v3[0] = cU[0];
	f1->v3[1] = cU[1];
	f1->v3[2] = cU[2];
	// Find normal vector by crossing: a X b
	norm[0] = (a[1] * b[2]) - (b[1] * a[2]);
	norm[1] = (-1) * ((a[0] * b[2]) - (b[0] * a[2]));
	norm[2] = (a[0] * b[1]) - (b[0] * a[1]);
	// Get magnitude of normal
	mag = sqrt((norm[0] * norm[0]) + (norm[1] * norm[1]) + (norm[2] * norm[2]));
	// Update unit vector of Facet
	f1->n[0] = norm[0] / mag;
	f1->n[1] = norm[1] / mag;
	f1->n[2] = norm[2] / mag;


	// Update f1.v1[] vector
	f2->v1[0] = cC[0];
	f2->v1[1] = cC[1];
	f2->v1[2] = cC[2];
	// Update f1.v2[] vector
	f2->v2[0] = cR[0];
	f2->v2[1] = cR[1];
	f2->v2[2] = cR[2];
	// Update f1.v3[] vector
	f2->v3[0] = cUR[0];
	f2->v3[1] = cUR[1];
	f2->v3[2] = cUR[2];
	// Find normal vector by crossing: c X a
	norm[0] = (c[1] * a[2]) - (a[1] * c[2]);
	norm[1] = (-1) * ((c[0] * a[2]) - (a[0] * c[2]));
	norm[2] = (c[0] * a[1]) - (a[0] * c[1]);
	// Get magnitude of normal
	mag = sqrt(norm[0] * norm[0]) + (norm[1] * norm[1]) + (norm[2] * norm[2]);
	// Update unit vector of Facet
	f2->n[0] = norm[0] / mag;
	f2->n[1] = norm[1] / mag;
	f2->n[2] = norm[2] / mag;
}

// This function updates the facets f1 and f2 while printing the vertical walls of the shape
void makeWalls(float e[ROW][COLUMN], Facet *f1, Facet *f2, char c, int m) {

	// e[][] is the 2D array that holds elevation values
	// Facet f1, f2 are the facets that are to be printed next
	// m is the coordinates of the cursor, it's moving in the x-axis direction
	// char is a chacter literal that determines what wall is to be printed

	// This conditional is to build the "top wall" corres to i = 0.
	if (c == 't') {

		// cC[] contains the x, y, z coordinates of the cursor
		float cC[3] = { (m * DX), ((ROW - 1) * DY), 0 };
		// cL[] contains the x, y, z coordinates of the vertex one to the left of the cursor on the x-axis
		float cL[3] = { ((m + 1) * DX), ((ROW - 1) * DY), 0 };
		// cUL[] contains the x, y, z coordinate of the elevation point one to the left of the cursor on the x-axis
		float cUL[3] = { ((m + 1) * DX), ((ROW - 1) * DY), e[0][m + 1] };
		// cU[] contains the x, y, z coordinates of the elevation point above the cursor
		float cU[3] = { (m * DX), ((ROW - 1) * DY), e[0][m] };

		// Build one facet
		// Update f1.v1[] coordinate
		f1->v1[0] = cL[0];
		f1->v1[1] = cL[1];
		f1->v1[2] = cL[2];
		// Update f1.v2[] coordinate
		f1->v2[0] = cC[0];
		f1->v2[1] = cC[1];
		f1->v2[2] = cC[2];
		// Update f1.v3[] coordinate
		f1->v3[0] = cU[0];
		f1->v3[1] = cU[1];
		f1->v3[2] = cU[2];
		// Update f1.n[] vector
		f1->n[0] = 0;
		f1->n[1] = 1;
		f1->n[2] = 0;

		// Build another facet
		// Update f2.v1[] coordinate
		f2->v1[0] = cL[0];
		f2->v1[1] = cL[1];
		f2->v1[2] = cL[2];
		// Update f1.v2[] coordinate
		f2->v2[0] = cU[0];
		f2->v2[1] = cU[1];
		f2->v2[2] = cU[2];
		// Update f1.v3[] coordinate
		f2->v3[0] = cUL[0];
		f2->v3[1] = cUL[1];
		f2->v3[2] = cUL[2];
		// Update f1.n[] vector
		f2->n[0] = 0;
		f2->n[1] = 1;
		f2->n[2] = 0;
	}

	// This conditional is to build the "bottom wall" corres to i = ROW - 1.
	if (c == 'b') {
		// cC[] contains the x, y, z coordinates of the cursor
		float cC[3] = { (m * DX), 0, 0 };
		// cR[] contains the x, y, z coordinates of the vertex one to the right of the cursor on the x-axis
		float cR[3] = { ((m + 1) * DX), 0, 0 };
		// cUR[] contains the x, y, z coordinate of the elevation point one to the right of the cursor on the x-axis
		float cUR[3] = { ((m + 1) * DX), 0, e[ROW - 1][m + 1] };
		// cU[] contains the x, y, z coordinates of the elevation point above the cursor
		float cU[3] = { (m * DX), 0, e[ROW - 1][m] };

		// Build one facet
		// Update f1.v1[] coordinate
		f1->v1[0] = cC[0];
		f1->v1[1] = cC[1];
		f1->v1[2] = cC[2];
		// Update f1.v2[] coordinate
		f1->v2[0] = cR[0];
		f1->v2[1] = cR[1];
		f1->v2[2] = cR[2];
		// Update f1.v3[] coordinate
		f1->v3[0] = cUR[0];
		f1->v3[1] = cUR[1];
		f1->v3[2] = cUR[2];
		// Update f1.n[] vector
		f1->n[0] = 0;
		f1->n[1] = -1;
		f1->n[2] = 0;

		// Build another facet
		// Update f2.v1[] coordinate
		f2->v1[0] = cC[0];
		f2->v1[1] = cC[1];
		f2->v1[2] = cC[2];
		// Update f1.v2[] coordinate
		f2->v2[0] = cUR[0];
		f2->v2[1] = cUR[1];
		f2->v2[2] = cUR[2];
		// Update f1.v3[] coordinate
		f2->v3[0] = cU[0];
		f2->v3[1] = cU[1];
		f2->v3[2] = cU[2];
		// Update f1.n[] vector
		f2->n[0] = 0;
		f2->n[1] = -1;
		f2->n[2] = 0;
	}

	// This conditional is to build the "left wall" corresp to m = 0.
	if (c == 'L') {
		// cC[] contains the x, y, z coordinates of the cursor
		float cC[3] = { 0, (((ROW - 1) - m) * DY), 0 };
		// cR[] contains the x, y, z coordinates of the vertex one down on the y-axis fronm the cursor
		float cR[3] = { 0, (((ROW - 1) - (m + 1)) * DY), 0};
		// cUR[] contains the x, y, z coordinates of the elevation point above the cursor
		float cUR[3] = { 0, (((ROW - 1) - (m + 1)) * DY), e[m + 1][0] };
		// cU[] contains the x, y, z coordinate of the elevation point one down on the y-axis from the cursor
		float cU[3] = { 0, (((ROW - 1) - m) * DY), e[m][0]};

		// Build one facet
		// Update f1.v1[] coordinate
		f1->v1[0] = cC[0];
		f1->v1[1] = cC[1];
		f1->v1[2] = cC[2];
		// Update f1.v2[] coordinate
		f1->v2[0] = cUR[0];
		f1->v2[1] = cUR[1];
		f1->v2[2] = cUR[2];
		// Update f1.v3[] coordinate
		f1->v3[0] = cU[0];
		f1->v3[1] = cU[1];
		f1->v3[2] = cU[2];
		// Update f1.n[] vector
		f1->n[0] = -1;
		f1->n[1] = 0;
		f1->n[2] = 0;

		// Build another facet
		// Update f2.v1[] coordinate
		f2->v1[0] = cC[0];
		f2->v1[1] = cC[1];
		f2->v1[2] = cC[2];
		// Update f1.v2[] coordinate
		f2->v2[0] = cR[0];
		f2->v2[1] = cR[1];
		f2->v2[2] = cR[2];
		// Update f1.v3[] coordinate
		f2->v3[0] = cUR[0];
		f2->v3[1] = cUR[1];
		f2->v3[2] = cUR[2];
		// Update f1.n[] vector
		f2->n[0] = -1;
		f2->n[1] = 0;
		f2->n[2] = 0;
	}

	// This conditional is to build the "right wall" corresp to m = 0.
	if (c == 'r') {
		// cC[] contains the x, y, z coordinates of the cursor
		float cC[3] = { ((COLUMN - 1) * DX), (((ROW - 1) - m) * DY), 0 };
		// cL[] contains the x, y, z coordinates of the vertex one down on the y-axis fronm the cursor
		float cL[3] = { ((COLUMN - 1) * DX), (((ROW - 1) - (m + 1)) * DY), 0 };
		// cUL[] contains the x, y, z coordinate of the elevation point one down on the y-axis from the cursor
		float cUL[3] = { ((COLUMN - 1) * DX), (((ROW - 1) - (m + 1)) * DY), e[m + 1][COLUMN - 1] };
		// cU[] contains the x, y, z coordinates of the elevation point above the cursor
		float cU[3] = { ((COLUMN - 1) * DX), (((ROW - 1) - m) * DY), e[m][COLUMN - 1]};

		// Build one facet
		// Update f1.v1[] coordinate
		f1->v1[0] = cC[0];
		f1->v1[1] = cC[1];
		f1->v1[2] = cC[2];
		// Update f1.v2[] coordinate
		f1->v2[0] = cU[0];
		f1->v2[1] = cU[1];
		f1->v2[2] = cU[2];
		// Update f1.v3[] coordinate
		f1->v3[0] = cL[0];
		f1->v3[1] = cL[1];
		f1->v3[2] = cL[2];
		// Update f1.n[] vector
		f1->n[0] = 1;
		f1->n[1] = 0;
		f1->n[2] = 0;

		// Build another facet
		// Update f2.v1[] coordinate
		f2->v1[0] = cU[0];
		f2->v1[1] = cU[1];
		f2->v1[2] = cU[2];
		// Update f1.v2[] coordinate
		f2->v2[0] = cUL[0];
		f2->v2[1] = cUL[1];
		f2->v2[2] = cUL[2];
		// Update f1.v3[] coordinate
		f2->v3[0] = cL[0];
		f2->v3[1] = cL[1];
		f2->v3[2] = cL[2];
		// Update f1.n[] vector
		f2->n[0] = 1;
		f2->n[1] = 0;
		f2->n[2] = 0;
	}
}

// This function updates the facets f1 and f2 while printing the bottom (floor) of the shape
void makeBottom(float e[ROW][COLUMN], Facet *f1, Facet *f2) {
	
	// Build one facet from origin to top-left-floor to bottom-right-floor
	// Update f1.v1[] coordinate 
	// Origin
	f1->v1[0] = 0;
	f1->v1[1] = 0;
	f1->v1[2] = 0;
	// Update f1.v2[] coordinate
	// Top-Left-Floor
	f1->v2[0] = 0;
	f1->v2[1] = (ROW - 1) * DY;
	f1->v2[2] = 0;
	// Update f1.v3[] coordinate
	// Bottom-Right-Floor
	f1->v3[0] = (COLUMN - 1) * DX;
	f1->v3[1] = 0;
	f1->v3[2] = 0;
	// Update f1.n[] vector
	// Points down
	f1->n[0] = 0;
	f1->n[1] = 0;
	f1->n[2] = -1;

	// Build one facet from top-left-floor to top-right-floor to bottom-right-floor
	// Update f2.v1[] coordinate 
	// Top-Left-Floor
	f2->v1[0] = 0;
	f2->v1[1] = (ROW - 1) * DY;
	f2->v1[2] = 0;
	// Update f2.v2[] coordinate
	// Top-right-floor
	f2->v2[0] = (COLUMN - 1) * DX;
	f2->v2[1] = (ROW - 1) * DY;
	f2->v2[2] = 0;
	// Update f2.v3[] coordinate
	// Bottom-Right-Floor
	f2->v3[0] = (COLUMN - 1) * DX;
	f2->v3[1] = 0;
	f2->v3[2] = 0;
	// Update f2.n[] vector
	// Points down
	f2->n[0] = 0;
	f2->n[1] = 0;
	f2->n[2] = -1;
}

	// Prints Facets
void printFacet(FILE *fa, Facet *f) {

		// Print the a facet in STL ASCII format
		fprintf(fa, "  facet normal %.6e %.6e %.6e\n", f->n[0], f->n[1], f->n[2]);
		fprintf(fa, "    outer loop\n");
		fprintf(fa, "      vertex   %.6e %.6e %.6e\n", f->v1[0], f->v1[1], f->v1[2]);
		fprintf(fa, "      vertex   %.6e %.6e %.6e\n", f->v2[0], f->v2[1], f->v2[2]);
		fprintf(fa, "      vertex   %.6e %.6e %.6e\n", f->v3[0], f->v3[1], f->v3[2]);
		fprintf(fa, "    endloop\n");
		fprintf(fa, "  endfacet\n");
}