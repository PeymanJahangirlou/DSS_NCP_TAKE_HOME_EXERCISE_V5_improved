


/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Main function.
 */

//#define DEBUG

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <unistd.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include  "sdb_image.h"
#include "JsonParser.h"
#include "Movie.h"
#include "Utility.h"


// Global variables
int					       WIDTH	        = 1020;
int						   HEIGHT		    = 768;
int						   LastXCoordinate  = -1;
int						   LastYCoordinate  = -1;
int						   CurrentRow       = -1;
int						   CurrentCol       = -1;
const float                MovieGridWidth   = 120;
const float				   MovieGridHeigth  = 200;
GLdouble			       NCP			    = 1.0f;      // near clipping planes distance.
GLdouble				   FCP			    = 20000.0f;  // far clipping planes distance.
bool					   DisplayMovieInfo = false;

pair<int, int>			   selectedMoviePos;
JsonParser::MovieContainer moviesMap{};
vector <int>		       allTextures{};

/** @brief sets up texture **/
GLuint getTexture(std::shared_ptr<MovieImage> movieImage_ptr)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
				 movieImage_ptr->width(),
				 movieImage_ptr->height(),
				 0, GL_RGB, GL_UNSIGNED_BYTE,
				 movieImage_ptr->image());

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   movieImage_ptr.reset();
   return texture;
}


/** @brief draw text on the screen **/
void renderBitmapString( int row, const string & msg, void * font )
//float x, float y, void* font, const string& text)
{
	const float y_start = MovieGridHeigth * 0.5f;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	float calculatedYPos = y_start - moviesMap.size() + ((row + 1) * MovieGridHeigth);
	glRasterPos2i(10, calculatedYPos);

	glColor3f(1.0f, 1.0f, 1.0f);
	for (auto ch : msg)
		glutBitmapCharacter(font, ch);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}


/*@ draws movie grids */
void drawMovieGrid()
{
	bool drawTextOnce{ false };
	const float yMargin = MovieGridHeigth * 0.5f;
	for (int row = 0; row < moviesMap.size(); row++) {
		int   x   = 0;
		float y   = row * MovieGridHeigth;
	    drawTextOnce = true;
		glColor3f(1, 1, 1);
		renderBitmapString(row, moviesMap[row].categoryName(), GLUT_BITMAP_HELVETICA_12);
		drawTextOnce = false;
		for (int col = 0; col < moviesMap[row].movieCounts(); col++) {
			auto movieImage_ptr = moviesMap[row][col]->image();
			if (!movieImage_ptr)
				continue;
			auto movieTexture = getTexture(movieImage_ptr);
			float newX =  x - allTextures[row] + (MovieGridWidth / 2);
			float newY =  y + yMargin + (MovieGridHeigth / 2);
			glBindTexture(GL_TEXTURE_2D, movieTexture);
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glTranslatef(newX, newY, 0.0f); // multiply the current matrix by a translation one
			int currText = allTextures[row] / MovieGridWidth;
			// shrink down the selected movie
			if (CurrentRow == row && CurrentCol == col)
				glScalef(0.50f, 0.50f, 0.0f); 
			else 
				glScalef(0.9f, 0.7f, 1.0f); 
			glTranslatef(-newX, -newY, 0.0f);
			glBindTexture(GL_TEXTURE_2D, movieTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 1);
				glVertex3f(x-allTextures[row], yMargin+y, 0.0f);		// left bottom
				glTexCoord2f(1, 1);
				glVertex3f(x+MovieGridWidth-allTextures[row], yMargin+y,0.0f); // right bottom
				glTexCoord2f(1, 0);
				glVertex3f(x+MovieGridWidth-allTextures[row] , yMargin+y+MovieGridHeigth, 0.0f); // right top
				glTexCoord2f(0, 0);
				glVertex3f(x-allTextures[row] , yMargin+y+MovieGridHeigth, 0.0f); // left top
			glEnd();
			glDeleteTextures(1, &movieTexture);
			x += MovieGridWidth;
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			glDeleteTextures(1, &movieTexture);
		}// end inner for loop
	}// end outter for loop

}

/* @ when enter key pressed this function gets called,
 *   and displays selected movie
 */
void drawDisplayMovieWindow()
{
		int x = 0;
		float y = CurrentRow * MovieGridHeigth;
		int J = 0;
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		auto movieObj_ptr = moviesMap[CurrentRow][CurrentCol];
		if (!movieObj_ptr)
			return;
		auto movieTexture = getTexture(movieObj_ptr->image());
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, movieTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3f(MovieGridWidth*2, MovieGridHeigth, 0.0f);
			glTexCoord2f(1, 1);
			glVertex3f(MovieGridWidth*5 - (MovieGridWidth*1/2), MovieGridHeigth, 0.0f);
			glTexCoord2f(1, 0);
			glVertex3f(MovieGridWidth*5 - (MovieGridWidth*1/2), (MovieGridHeigth*4), 0.0f);
			glTexCoord2f(0, 0);
			glVertex3f(MovieGridWidth*2, (MovieGridHeigth * 4), 0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDeleteTextures(1, &movieTexture);
		glColor3f(1, 1, 1);
		renderBitmapString(3,movieObj_ptr->title() , GLUT_BITMAP_TIMES_ROMAN_24);
}

/** @brief glut display callback function  **/
void display(void)
{
    glClearColor(0.0f, 0.05f, 0.13f,1.0f);         // disney+  background
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// passing movie grid width and height for scaling up/down
	// define a 2D orthographic projection matrix
	gluOrtho2D(0.0, MovieGridWidth * 5, 0.0, MovieGridHeigth * 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// start drawing objects on screen
	glEnable(GL_TEXTURE_2D);

	// display Selected Movie info
	if (DisplayMovieInfo) {
		drawDisplayMovieWindow();
	}
	else {
		drawMovieGrid();
	}

	glutSwapBuffers();
	Sleep(1000 / 60);
}

/** @brief resize callback  funciton **/
void onReshape(int newWidth, int newHeight)
{
	if (newHeight == 0)
		return;

	glViewport(0, 0, newWidth, newHeight); // sets a new viewport rectangle
	WIDTH = newWidth;
	HEIGHT = newHeight;
	glMatrixMode(GL_PROJECTION);	// specifies which matrix is current matrix
	glLoadIdentity();				// reset matrix
	// sets correct perspective
	gluPerspective(60.0f, (float)WIDTH/(float)HEIGHT, NCP, FCP);
	// get back to modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/* @ Keyboard input processing routine. work in progress **/
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27: // esc Key
         exit(0);
         break;
      case 13: // Enter Key
		  if (CurrentRow != -1)
			  DisplayMovieInfo = true;
         break;
      case 8: // BackSpace Key
		  if (CurrentRow != -1)
			  DisplayMovieInfo = false;
         break;
      default:
         break;
   }
}


/*@brief processes arrow keys */
void onSpecialKey(int key, int x, int y)
{
	if (CurrentCol == -1 || CurrentRow == -1) {
		CurrentCol = 0;
		CurrentRow = 0;
		return;
	}

	switch (key) {
	case GLUT_KEY_UP:
	{
		CurrentRow++;
		if (CurrentRow >= moviesMap.size())
			CurrentRow = moviesMap.size() - 1;

		CurrentCol = (moviesMap[CurrentRow].lastSelected() == -1) ? 0 : moviesMap[CurrentRow].lastSelected();
	}
		break;
	case GLUT_KEY_DOWN:
	{
		CurrentRow--;
		if (CurrentRow < 0)
			CurrentRow = 0;

		CurrentCol = (moviesMap[CurrentRow].lastSelected() == -1) ? 0 : moviesMap[CurrentRow].lastSelected();
	}
		break;
	case GLUT_KEY_RIGHT:
	{
		if ( CurrentCol < moviesMap[CurrentRow].movieCounts() - 1) {
			if (CurrentCol < moviesMap[CurrentRow].movieCounts() - 5) {
				CurrentCol++;
				moviesMap[CurrentRow].setLastSelected(CurrentCol);
				allTextures[CurrentRow] += MovieGridWidth;
			}
			else {
				moviesMap[CurrentRow].setLastSelected(CurrentCol);
				CurrentCol++;
			}
		}
	}
		break;
	case GLUT_KEY_LEFT:
		if (CurrentCol > 0 ) {
			CurrentCol--;
			allTextures[CurrentRow] -= MovieGridWidth;
			moviesMap[CurrentRow].setLastSelected(CurrentCol);
		}
		// make it stop at the first column in each row
		if (allTextures[CurrentRow]  < 0 )
			allTextures[CurrentRow]  = 0;
		break;
	}
}

/* @brief init opengl flags */
void initGlutAttrib()
{
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON, GL_NICEST);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // select a polygon rasteriztion mode
	glEnable(GL_SMOOTH);

}
/** @brief main function **/
int main(int argc, char** argv)
{
	Utility::displayMessage("Downloading Reference Ids... Please Wait!");
	JsonParser json_parser;
	// get all refIds from home url
	auto refIdsSet = json_parser.getRefIds();

	// for simpliciy, we are only displaying 4 rows of movies
	Utility::displayMessage("Downloading Movies... Please Wait!");
	int index{ 0 };
	for (auto refId : refIdsSet) {
		moviesMap[index] = json_parser.readByRefId(refId);
		for (int i = 0; i < moviesMap[index].movieCounts(); i++)
		   allTextures.push_back(0);
		if (index == 3)
			break;
		index++;

	}// end for loop
	if (moviesMap.empty()) {
		// for simpliciy, we are only displaying 4 rows of movies
		Utility::displayMessage("Failed Downloading Movies... Terminating Application!");
		return EXIT_FAILURE;

	}
	Utility::clearTerminalScreen();

	glutInit(&argc, argv);	// initializes GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);		// enbale double buffered mode.
	glutInitWindowSize(WIDTH, HEIGHT)						;		// specifies window size
	glutInitWindowPosition(0, 0);						    	// position the window's initial top-left corner.
	glutCreateWindow("Disney Plus");								// create main window
	initGlutAttrib();
	glutKeyboardFunc(keyInput);								        // registers keyboard event
	glutSpecialFunc(onSpecialKey);
	glutDisplayFunc(display);								    	// registers display callback function.
	glutIdleFunc(display);
    glutReshapeFunc(onReshape);									// registers reshape callback function.
	glutMainLoop();									         		// infinite main loop

	return EXIT_SUCCESS;
}

//////////////////////////////////// End Main Function /////////////////////////////////////
