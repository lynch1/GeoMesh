#define _CRT_SECURE_NO_WARNINGS

/* Created by Daniel Lynch
This iterations of the project was started on 11/3/2017.
project: GeoMesh
filename: geomesh.cw2
associated files: geomesh.h, geomesh_fcns.c
An edit log is contained at the end of the file.
*/

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "geomesh_fcns.h"
#include "facet.h"

int main() {

	// float is a 2D array that holds elevation values
	float emap[ROW][COLUMN] = { 0 };

	// For mapping / normalizing
	float min = 0;
	float max = 0;
	float range = 0;
	float zrange = 0;
	float tempE = 0;
	float ratio = 0;
	// i, j ,t are for looping
	int i, j, m;

	// used to store some row and column stuff if I get dynamic arrray size working
	int r, c;

	// Initialize a Facet type
	Facet facet1 = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };
	Facet facet2 = { { 0.0, 0.0, 0.0 },{ 0.0, 0.0, 0.0 },{ 0.0, 0.0, 0.0 },{ 0.0, 0.0, 0.0 } };

	// fp is a FILE pointer used to reference the text file that will be used to build the 2D array 
	FILE * fp;
	// fp2 is a file pointer to the file that will be the STL file
	FILE * fp2;

	// Read in elevaation data from a text file called "eldata.txt"
	// Get an output file ready to output the STL file
	fp = fopen("tfiles/utah.txt", "r");
	fp2 = fopen("meshes/utah.stl", "w");
	// Check that the file was accessible
	if (fp == NULL) {
		printf("File Input Error. fp = NULL");
		return 0;
	}
	 // Read in rows and columns
	fscanf(fp, "%d %d", &r, &c);
	// Populate 2D array with floats in the text file
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COLUMN; j++) {
			fscanf(fp, "%f", &emap[i][j]);
		}
	}
	fclose(fp);

	// Map range of elevations onto printable z-heights
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COLUMN; j++) {
			if (emap[i][j] > max) {
				max = emap[i][j];
			}
			if (emap[i][j] < min) {
				min = emap[i][j];
			}
		}
	}
	range = max - min;
	zrange = ZMAX - ZMIN;
	ratio = zrange / range;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COLUMN; j++) {
			tempE = emap[i][j];
			emap[i][j] = ZMIN + ((tempE - min) * ratio);
		}
	}

	// Print the test STL to a file
	// Check that the file was accessible
	if (fp2 == NULL) {
		printf("File Input Error. fp2 = NULL");
		return 0;
	}

	fprintf(fp2, "solid ASCII\n");

	//// This loop builds the vertical walls at the top and bottom (along x-axis) of the wall
	for (m = 0; m < (COLUMN - 1); m++) {

		// Make and print the top wall
		makeWalls(emap, &facet1, &facet2, 't', m);
		printFacet(fp2, &facet1);
		printFacet(fp2, &facet2);

		// Make and print the bottom wall
		makeWalls(emap, &facet1, &facet2, 'b', m);
		printFacet(fp2, &facet1);
		printFacet(fp2, &facet2);

	}

	// This loop builds the horizontal walls at the left and right (along y-axis) of the wall
	for (m = 0; m < (ROW - 1); m++) {
		
		// Make and print the left wall
		makeWalls(emap, &facet1, &facet2, 'L', m);
		printFacet(fp2, &facet1);
		printFacet(fp2, &facet2);

		// Make and print the right wall
		makeWalls(emap, &facet1, &facet2, 'r', m);
		printFacet(fp2, &facet1);
		printFacet(fp2, &facet2);
	}

	// This loop builds the top surface
	for (i = 1; i < ROW; i++) {
		for (j = 0; j < (COLUMN - 1); j++) {
			/* For each [i][j] visit, I want to go into the the 2D array of elevations and return:
			--- The vertex of the cursor
			--- the vertex on the surface to the right [i][j + 1]
			--- the vertex on the surface and one down [i +1][j]
			--- the vertex on the surface down one and to the right [i + 1][j + 1]
			*/

			//makeFacet() will update the facet1 which is of Facet datatype
			makeFacets(emap, &facet1, &facet2, i, j);

			// Print the facets for this cursor position
			printFacet(fp2, &facet1);
			printFacet(fp2, &facet2);

		}
	}

	// Close the bottom of the solid
	makeBottom(emap, &facet1, &facet2);
	printFacet(fp2, &facet1);
	printFacet(fp2, &facet2);

	fprintf(fp2, "endsolid\n");
	fclose(fp2);

	return(0);
}

/* EDIT LOG
11/3/2017-- Started a new project in Visual Studio because the last project organization was getting messy.
11/13/2017-- Came up with a pseudo-code algorithm for generating facets in my notebook. I'm going to try to 
implement it today. 
11/15/2017-- Wrote a bit to generate a 10 by 10 matrix of random floats and then write them to a text file. 
I was going try to make this all functions but for now I'm just doing it all in main() because then when I 
want to eventualy get rid of it, I only have to clean up main() and not all three files in the project. 
For today I am just going to try to define the surface, i.e. not make it water-tight.  
11/16/2017-- The code I wrote yesterday tried to open the stl file in main, then pass the file pointer to the 
printFacet() function and then close the file again in main. I think this isn't possible. Right now I have it 
so that it will open and close the file everytime the function runs. I think it will be better to have some single
datastructure of all the facet-things that need to get written and just have main() do all the writing. Then I'll 
just have a function that keeps calculating and re-writing the facet object (data structure) and main will just 
keep printing the stuff in that object. 
11/17/17-- Made a Facet structure, because opening and closing files to write in main() and the functions was 
getting hard to debug. VS would keep throwing memory allocation errors and stuff. 
11/19/2017-- I put the datastructure stuff (type definition and the related fcns) in their own file: "facet.c"
I'm getting close. I got it to print text to standard output that's almost right. At 6:40PM, it worked.
There's an edge-case thing happening with the bottom right corner. 
11/29/17-- Making a bit in main happen so that it reads in a text file containing elevations. I used a custom 
text file that I created myself that had repeated values so that I could easily see in the result if the 
program read it in correctly.
12/03/19-- Working on getting everything ready for Tuesday presentation. Goal is to have this program be able to read
in essentially csv data, build a 2D matrix out of it, and then generate/write the STL file. Then I will have a seperate
JS program that will be used to access the Google Maps API to get the data and build the texc CSV file used earlier.
That's my goal for today. 
I got it to read in data fine. Then I realized it would be good to be able to map the elevation data into a range of
height values that are more suitable for printing maybe 10 to 70mm is a good range.
I put all the necessary constants in the "geomesh_fcns.h" header file. They had been spread out across the project 
files. Something is happening with the normal vector calculations when I change the dx and dy values.
I made a very particular CSV file so that I could look at the output and find out where the. It looked like steps 
it was cool. It was a problem with the indexing.
I funcition-ified-the printing commands becaus ethe code was getting repetitive and long.
12/4/17-- I cleaned up some cases. Of course, the day before the presentation I realized I was building the mirror
image of my bitmaps. I had to go back and fix the looping of the top surface build and the algorithm for that part 
too.
I finally got it so that it wasn't mirror-oriented and so that you can vary ROW, COLUMN, DX, DY freely (as positive
integers) without it affecting the normals and throwing errors. The problem was that I was defining the vertices of
some of the facets in the wrong order. In addition to the normal vector, the vertices that make up a facet have to
be listed in counter-clockwise order as looking down at the outside. 
Once I fixed my orientation problems, making the side walls was a lot easier. I found a bug in the makeBottom() 
function where instead of writing both facets, I kept re-writing the same one, which caused the solid to not be
water-tight and throw errors in the slicer software. 
12/5/17-- I presented today. It went well. I wrote an image processing script in MATLAB that takes in .jpg's,
processes them (i.e. thresholds and smooths them by removing high frequencies), and then prints out a text file
that this (geomesh.c) program can read. it works well so far. You can put in aerial photographs and get some 
accurate looking meshes. I'm printing one of Lowell, based on the thumbnail .jpg of the USGS LiDAR point cloud data
I downloaded last month. I need to think about what my report is going to look like and what I want the program to 
look like by the time I turn it in.
12/6/17- I tried to make a [400][800] array of floats and I got a stack overflow error. 
For now I just changed the default stack reserv size to 8 MB. In the future I should probably think about working 
with smaller arrays. Maybe building the STL based on pieces of the data not all of it. 
The Boston Harbor STL looked prettty cool. 
*/