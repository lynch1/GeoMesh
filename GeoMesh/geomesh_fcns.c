/* Created by Daniel Lynch
This iterations of the project was started on 11/3/2017.
project: GeoMesh
filename: geomesh_fcns.c
associated files: geomesh.h, geomesh.c
An edit log is contained at the end of the file.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include "geomesh_fcns.h"

// Used to clear line if input formatting error occurs
void badInput() {
	//Keep scanning until scanf() gets to the new line
	char junk;
	printf("Error: Incorrectly formatted input\n");
	do {
		scanf("%c", &junk);
	} while (junk != '\n');

}


// clearHist(int hist[], int n) clears the counts of all the letters
void clearHist(int hist[], int n) {
	int i = 0;
	for (i = 0; i < n; i++) {
		hist[i] = 0;
	}
}

void clearLine() {
	//Keep scanning until scanf() gets to the new line
	char junk;
	do {
		scanf("%c", &junk);
	} while (junk != '\n');
}

/* EDIT LOG
11/13/2017-- Created a basic template for this function definition file. I based it on the function definition file I used
in Program 6.
11/15/2017-- Started writing the functions updateCursor() and printFacet(). I should probably make a Cursor, facet, or 
vertex a data structure at some point, but I'm just trying to make shit work for now. 
I don't know if just passing the file pointer into the function is going to work. Right now I have it so that the file
is only opened and closed in main(). I don't think I have to open and close the file everytime the function runs but I'm 
not sure. I ran the project and something is going on with the file pointer for the second file and the printFacet function.
11/19/2017-- I'm going to move the Facet datastructure functions to their own function definition file: "facet.c". 
I got rid of the updateCursor function. 
*/