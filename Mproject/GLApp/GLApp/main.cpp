

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

#include  "sdb_image.h"
#include "JsonParser.h"
#include "Movie.h"
#include "Utility.h"

using namespace std;

// Global variables
int WIDTH = 1020;
int HEIGHT = 800;
pair<int, int> SelectedMovie; // sets to moviesGrid[0][0];

unsigned int texture[1];
JsonParser::MovieContainer moviesGrid;

typedef unordered_set<std::shared_ptr<Movie>>::iterator MovieSetIterator;
JsonParser::MovieContainer trendingMovieMap{};
JsonParser::MovieContainer personalizedCuratedMap{};
JsonParser::MovieContainer becauseYouSetMap{};
MovieSetIterator trendingMovieIter;
MovieSetIterator personalizeCuratedMoviesIter;
MovieSetIterator  becauseYouSetMoviesIter;

JsonParser::MovieContainer moviesMap{};

/** @brief sets up texture **/
void setUpTexture(std::shared_ptr<MovieImage> movieImage_ptr)
{
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			   movieImage_ptr->width(),
			   movieImage_ptr->height(), 0,
	           GL_RGB, GL_UNSIGNED_BYTE,
		       movieImage_ptr->image());
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

/** @brief download image from web then load texture **/
void loadTexture(const std::string & imgUrl)
{
	if (!Utility::downloadMovieImage(imgUrl)) {
		cout << "LOASTEXTURE::Utility::downloadMovieImage() Failed\n" << endl;
		return;
	}
   const string imgPath{ "movieImage.jpg" };
   auto movieImage_ptr = Utility::loadImageFromDirectory(imgPath);
   if (!movieImage_ptr) {
		cout << "LOASTEXTURE::Utility::loadImageFromDirectory() Failed\n" << endl;
		return;
   }
   setUpTexture(movieImage_ptr);
   movieImage_ptr.reset();
}

/** @brief draw text on the screen **/
void renderBitmapString(float x, float y, void* font, const string& text)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(x, y);
	for (auto ch : text)
		glutBitmapCharacter(font, ch);
}

const pair<float, float> SquareSize = std::make_pair(200.0, 170.0);
void drawMovieBoxes()
{
	const int colMargin{ 10 };
	const int rowMargin{ 20 };
	int col_start = colMargin;
	int row_start = rowMargin;
	float col_space{ 50 };
	float rowExtraSpace{ 40 };
	int row = 0;
	int col = 0;
	for (; col_start < WIDTH; col_start += SquareSize.first) {
		if ((col_start + SquareSize.first) >= WIDTH) {
			moviesMap[row][col-1]->setIsLastItemOnViewPort(true);
			row_start += SquareSize.second;
			row++;
			col_start = colMargin;
			col = 0;

		}
		if ((row_start + SquareSize.second) >= HEIGHT)
			break;
		if (col_start == colMargin) {
			renderBitmapString( col_start,
								row_start + SquareSize.second + 20,
								GLUT_BITMAP_TIMES_ROMAN_24,
								moviesMap[row].categoryName());
		}
		if (col == 0)
			moviesMap[row][col]->setIsFirstItemOnViewPort(true);

		loadTexture(moviesMap[row][col]->imgUrl());
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0);
		glVertex2f((float)col_start + colMargin, (float)row_start + rowMargin + rowExtraSpace );
		glTexCoord2f(1.0, 0.0);
		glVertex2f((float)col_start + SquareSize.first, (float)row_start + rowMargin + rowExtraSpace);
		glTexCoord2f(1.0, 1.0);
		glVertex2f((float)col_start + SquareSize.first, (float)row_start + SquareSize.second );
		glTexCoord2f(0.0, 1.0);
		glVertex2f((float)col_start + colMargin, (float)row_start + SquareSize.second );
		glEnd();
		col++;
	}
}



static bool draw{ true }; // draw function gets called once
/** @brief display function for GLUT  **/
void display(void)
{
	if (!draw)
		return;

	draw = false;
    glClearColor(0.0f, 0.05f, 0.13f,1.0f);         // disney plus background
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glGenTextures(1,texture);
	drawMovieBoxes();
	glutSwapBuffers(); // double buffered - swap the front and back buffers.
	glDeleteTextures(1, texture);
}

/** @brief resize callback  funciton **/
void onReshape(int newWidth, int newHeight)
{
	// ignore new width and height
	//glutReshapeWindow(WIDTH, HEIGHT);
	WIDTH = newWidth;
	HEIGHT = newHeight;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	draw = true;
	display();

}

/*@brief Mouse input processing routin. for debug */
void onMouse(int buttonId, int buttonState, int x, int y) 
{
	if(buttonState != GLUT_DOWN)
		return;
	cout << "mouse coordinates -- (x,y): " << "(" << x << ", " << y << ")" << endl;
}

/* @ Keyboard input processing routine. work in progress **/
void keyInput(unsigned char key, int x, int y)
{
   std::cout << "x: " << x << "y: " << y << std::endl;
   switch(key) 
   {
      case 27: // esc key
         exit(0);
         break;
      default:
         break;
   }
}

void printSelectedMovieInfo()
{
	cout << "You selected:" << endl;
	cout << moviesMap[SelectedMovie.first].categoryName() << endl;
	auto moviesList_ptr = moviesMap[SelectedMovie.first];
	cout << "\t" << moviesList_ptr.at(SelectedMovie.second)->title() << endl;
	cout << endl;

}

/*
/** @brief Callback routine for non-ASCII key entry
 ** processing arrow keys
 **/
void specialKeyInput(int key, int x, int y)
{
	static bool first(true);
	if (first) {
		first = false;
		SelectedMovie = std::make_pair(0, 0);
		printSelectedMovieInfo();
		return;
	}
	int row = SelectedMovie.first;
	int col = SelectedMovie.second;
	switch (key) {
	case GLUT_KEY_UP:
		if (row >= 0 && row+1 < moviesMap.size())
			row++;
		break;

	case GLUT_KEY_DOWN:
		if (row-1 < moviesMap.size() && row-1 >= 0)
			row--;
		break;

	case GLUT_KEY_LEFT:
		if (moviesMap[row].isIndexValid(col - 1) && !moviesMap[row][col]->isFirstItemOnViewPort()) {
			col--;

		}
		break;

	case GLUT_KEY_RIGHT:
		if (moviesMap[row].isIndexValid(col + 1)) {
			if (moviesMap[row][col]->isLastItemOnViewPort()) {
				moviesMap[row][col]->setIsFirstItemOnViewPort(false);
				moviesMap[row][col+3]->setIsFirstItemOnViewPort(true);
				moviesMap[row][col+3]->setIsFirstItemOnViewPort(true);

			}
			else {
			col++;
			}
		}
		break;
	}


	if (SelectedMovie.second != col || SelectedMovie.first != row) {
		SelectedMovie = { row,col };
		printSelectedMovieInfo();
	}

}


/** @brief main function **/
int main(int argc, char** argv)
{
	cout << "It is Loading..." << endl;
	JsonParser json_parser;
	// get all refIds from home url
	auto refIdsSet = json_parser.getRefIds();
	int index{ 0 };
	//const int rowMargin{ 20 };
	//int rowStart = rowMargin;
	for (auto refId : refIdsSet) {
	//	if ((rowStart + SquareSize.second) >= HEIGHT)
	//		break;
		moviesMap[index] = json_parser.readByRefId(refId);
		index++;
	//	rowStart += SquareSize.second;
	}

	// clear the window
	cout << string(100, '\n');

#ifdef DEBUG
	// print content of moviesMap into terminal
	for (auto containerPair : moviesMap) {
		cout << "-------------------------------------------" << endl;
		cout << "  " << containerPair.second.categoryName() << endl;
		auto movieList = containerPair.second;
		for (auto col = 0; col < movieList.movieCounts(); col++) {
			cout << "\t" << movieList.at(col)->title() << endl;
		}
	}
#endif // DEBUG

	// load images corresponding to the number of grids in viewport




	glutInit(&argc, argv);	// initializes GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		// enbale double buffered mode.
	glutInitWindowSize(WIDTH, HEIGHT)						;		// specifies window size
	glutInitWindowPosition(100, 100);						    	// position the window's initial top-left corner.
	glutCreateWindow("Disney Plus");								// create main window

	glMatrixMode(GL_PROJECTION);										// setup viewing projection
	glLoadIdentity();												// start with identity matrix
	glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);				    // set clipping area of 2D orthographic view
	//glutMouseFunc(onMouse);										    // registers mouse callback function.
	glutKeyboardFunc(keyInput);								        // registers keyboard event
	glutSpecialFunc(specialKeyInput);							    // registers function keys.
//	glutReshapeFunc(onReshape);										// registers reshape callback function.
	glutDisplayFunc(display);								    	// registers display callback function.
	glutMainLoop();									         		// infinite main loop

	return EXIT_SUCCESS;
}
