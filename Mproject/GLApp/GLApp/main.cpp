

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
#include <optional>

#include  "sdb_image.h"
#include "JsonParser.h"
#include "Movie.h"
#include "Utility.h"

using namespace std;

// Global variables
int					       WIDTH	       = 1020;
int						   HEIGHT		   = 750;
int						   LastXCoordinate = -1;
int						   LastYCoordinate = -1;
int						   CurrentRow      = -1;
const int				   SquareHeight    = 180;
const int				   SquareWidth     = 250;
GLdouble			       NCP			   = 1.0f;      // near clipping planes distance.
GLdouble				   FCP			   = 20000.0f;  // far clipping planes distance.

pair<int, int>			   selectedMoviePos;
JsonParser::MovieContainer moviesMap{};
vector <int>		       allTextures{};

/** @brief sets up texture **/
std::optional<GLuint> getTexture(std::shared_ptr<MovieImage> movieImage_ptr)
{
	if (!movieImage_ptr)
		return std::optional<GLuint>();

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
   return texture;
}


/** @brief draw text on the screen **/
void renderBitmapString(float x, float y, void* font, const string& text)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(x, y);
	for (auto ch : text)
		glutBitmapCharacter(font, ch);
}


/** @brief glut display callback function  **/
void display(void)
{
		
    glClearColor(0.0f, 0.05f, 0.13f,1.0f);         // disney+  background
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// start drawing objects on screen
	const int SquareHeight{ 180 };
	const int SquareWidth { 250 };
	const int ColMargin{ 10 };
	const int RowMargin{ 50 };
	const int ColSpace{ 50 };
	const int RowExtraSpace{ 40 };
	glEnable(GL_TEXTURE_2D);
	for (int row = 0; row < moviesMap.size(); row++) {
		float y =  row * SquareHeight;
		float x = ColMargin;
		renderBitmapString(x+10, y+10+SquareHeight, GLUT_BITMAP_TIMES_ROMAN_24, moviesMap[row].categoryName());
		for (int col = 0; col < moviesMap[row].movieCounts(); col++) {
			auto movieTexture = getTexture(moviesMap[row][col]->image());
			if (movieTexture.has_value()) {
				glBindTexture(GL_TEXTURE_2D, movieTexture.value());
				allTextures.push_back(0);
				glBegin(GL_POLYGON);
				glTexCoord2f(0, 1);
				glVertex2f(x+ColMargin+allTextures[row], y+RowMargin);		// left bottom
				glTexCoord2f(1, 1);
				glVertex2f(x+SquareWidth+allTextures[row], y + RowMargin); // right bottom
				glTexCoord2f(1, 0);
				glVertex2f(x+SquareWidth+allTextures[row] , y+SquareHeight); // right top
				glTexCoord2f(0, 0);
				glVertex2f(x+ColMargin+allTextures[row] , y+SquareHeight); // left top
				glEnd();
			}// end if statement
			glDeleteTextures(1, &movieTexture.value());
			x += SquareWidth;
		}// end inner for loop
	}// end outter for loop

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

void onMouseButton(int button, int state, int x, int y)
{
	LastXCoordinate = x;
	LastYCoordinate = HEIGHT - y;
	CurrentRow = LastYCoordinate / SquareHeight;
	if (CurrentRow >= moviesMap.size())
		CurrentRow = -1;
}

/*@brief glut Mouse Motion callback function **/
void onMouseMotion(int x, int y) 
{
	int XDelta = x - LastXCoordinate;
	if (CurrentRow != -1) {
		allTextures[CurrentRow] += XDelta;
	}
	LastXCoordinate = x;
	LastYCoordinate = HEIGHT - y;
}

/* @ Keyboard input processing routine. work in progress **/
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27: // esc key
         exit(0);
         break;
      default:
         break;
   }
}

void initGlutAttrib()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
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

	Utility::displayMessage("Downloading Movies... Please Wait!");
	int index{ 0 };
	for (auto refId : refIdsSet) {
		moviesMap[index] = json_parser.readByRefId(refId);
		index++;
	}// end for loop
	Utility::clearTerminalScreen();

	glutInit(&argc, argv);	// initializes GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		// enbale double buffered mode.
	glutInitWindowSize(WIDTH, HEIGHT)						;		// specifies window size
	glutInitWindowPosition(0, 0);						    	// position the window's initial top-left corner.
	glutCreateWindow("Disney Plus");								// create main window
	initGlutAttrib();
	glutKeyboardFunc(keyInput);								        // registers keyboard event

	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMotion);

	glutDisplayFunc(display);								    	// registers display callback function.
	glutIdleFunc(display);
    glutReshapeFunc(onReshape);									// registers reshape callback function.
	glutMainLoop();									         		// infinite main loop

	return EXIT_SUCCESS;
}

//////////////////////////////////// End Main Function /////////////////////////////////////
