/* Created by Daniel Lynch
This iterations of the project was started on 11/3/2017.
project: GeoMesh
filename: geomesh_fcns.h
associated files: geomesh_fcns.c, geomesh.c
An edit log is contained at the end of the file.
*/

// So user-defined header isn't included multiple times
#ifndef geomesh_fcns_h
#define geomesh_fcns_h

// Constants that both main() and the functions need
#define ROW 200
#define	COLUMN 200
// The following parameters effect print size and are in mm
#define DX 0.5
#define DY 0.5
#define ZMIN 6
#define ZMAX 16

// Used to clear line if input formatting error occurs
void badInput();

// Used to clear a line of input in case the next call to scanf() reads it. 
void clearLine();
#endif /* More conditional compilation--end of prog6_functions_h */

/* EDIT LOG
11/13/2017-- Created a basic template for this header file. I based it on the header file I used in Program 6.
11/15/2017-- Basic functions added
11/16/2017-- Added the printFacet() and updateCursor() functions. But this plan isn't working well, so I'm going to
change it soon. 
*/