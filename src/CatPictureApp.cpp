/**
* Project by Brandon Dadosky for CSE 274 homework assignment 1.
*/
/**
* @file CatPicture.cpp
* 
* @author Brandon Dadosky
* @date September 2012
* 
* @note This file is (c) 2012.  It is licensed under the
* CC gv 3.0 license (http://creativecommons.org/licenses/by/3.0/),
* which means you are free to use, share, and remix it as long as you
* give attribution.  Commercial uses are allowed.
* 
* This project fulfills the following requirements for homework 1:
* 
* 
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include <stdlib.h>


#define WIDTH 800
#define HEIGHT 600

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void drawLine(int xI, int yI, int xF, int yF, Color8u* color);
	void drawRectangle(int xA, int yA, int xB, int yB, Color8u* line, Color8u* fill);
	void drawCircle(int xC, int yC, int r, Color8u* line, Color8u* fill);
	//void copy(int x, int y, int l, int w, int xF, int yF);
	void drawTriangle(int x1, int y1, int x2, int y2,int x3,int y3, Color8u* line);
	void blur();
	void prepareSettings(Settings* settings);

  private:
	bool modify(Color8u* color, int x, int y);
	uint8_t* get(int x, int y);

	Surface* surface;
	uint8_t* dataArr;
	math<int>* intMath;
};

void CatPictureApp::setup()
{
	setWindowSize(WIDTH, HEIGHT);
	surface = new Surface(WIDTH, HEIGHT, false);
	// not sure what intMath is doing, might help other users to understand
	// if you document this somehow in the comments
	intMath = new math<int>();

	dataArr = surface->getData();


	//Begin test cases
	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			modify(new Color8u(255,255,255),x,y);
		}
	}

	

	drawLine(0,0,800,600,new Color8u(0,0,0));
	drawRectangle(200, 210, 48, 92, new Color8u(0,0,0),new Color8u(255,0,0));
	drawCircle(200, 300, 100, new Color8u(0,0,0), new Color8u(0, 0, 255));
	drawTriangle(35, 135, 210, 411, 510, 500, new Color8u(0,0,0));
	blur();
	// Saves Image
	writeImage("blah.png",*surface);
}


/**
* @brief Draw Circle
* 
* Draws a circle.
* 
* @param xC the X-coordinate of the center of the circle
* @param yC the Y-coordinate of the center of the circle
* @param r the radius of the circle
* @param line the color object containing color information for the outside line
* @param fill the color of the fill. If this value is 0, the circle won't be filled.
*/
void CatPictureApp::drawCircle(int xC, int yC, int r, Color8u* line, Color8u* fill)
{
	double dist;
	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			//Calculate the distance from the center, if it equals (or is close to)
			// the radius, the pixel becomes part of the circle.
			dist = math<double>().sqrt((xC - x) * (xC - x) + (yC - y) * (yC - y));
			if(math<double>().abs(r - dist) < 0.5)
				modify(line, x, y);
			else if(dist < r && fill != 0) //TODO: Fill isn't exactly right, and stays toward the right of the circle. Might be a rounding issue.
				modify(fill, x, y);
		}
	}
			
}

/**
* @brief Mouse Click Circle
* 
* Every time the mouse is clicked, draw a circle with a random radius and colors 
* in that position. Every time the right button is clicked, blur the image.
*/
void CatPictureApp::mouseDown( MouseEvent event )
{
	drawCircle(event.getX(),event.getY(),rand() % 50 + 5, 
		new Color8u(rand() % 256, rand() % 256, rand() % 256), 
		new Color8u(rand() % 256, rand() % 256, rand() % 256));
	if(event.isRight())
		blur();
	// took out save here since you are already saving in the setup
	// seems like it could cause issues to keep saving to the same file
	// each time the user clicks
	// writeImage("blah.png",*surface);
}

/**
* @brief Blur
* 
* Blurs the image using a kernel
*/
void CatPictureApp::blur() //TODO: Very slow.
{
	uint8_t* newData = new uint8_t[WIDTH * HEIGHT * 3];
	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			
			uint8_t* kernel = new uint8_t[27];
			int sumR = 0, sumG = 0, sumB = 0;
			for(int j = y - 1; j <= y + 1; j++)
			{
				for(int i = x - 1; i <= x + 1; i++)
				{
					if(i < 0 || i >= WIDTH || j < 0 || j >= HEIGHT)
						continue;
					uint8_t* arr = get(i, j);
					sumR += arr[0];
					sumG += arr[1];
					sumB += arr[2];
				}
			}
			int index = 3 * (x + y * WIDTH);
			newData[index] = math<double>().floor(sumR / 9.0);
			newData[index + 1] = math<double>().floor(sumG / 9.0);
			newData[index + 2] = math<double>().floor(sumB / 9.0);
		}
	}
	for(int i = 0; i < WIDTH * HEIGHT * 3; i++)
		dataArr[i] = newData[i];

}

// Reactivated the prepSettings method so that now you could come back and change the
// settings for the window if you wanted to.  Liked the idea for WIDTH and HEIGHT
void CatPictureApp::prepareSettings(Settings* settings)
{
	settings->setWindowSize(WIDTH, HEIGHT);
	settings->setResizable(false);
}

void CatPictureApp::update()
{
	dataArr = surface->getData();
}
/**
* @brief Draw a line
* 
* Draws a straight line between the two points.
* 
* @param x1 the x coordinate of the first point
* @param y2 the y coordinate of the first point
* @param x1 the x coordinate of the second point
* @param y2 the y coordinate of the second point
* @param color the Color to of the line
*/
void CatPictureApp::drawLine(int x1, int y1, int x2, int y2, Color8u* color)
// changed the names of the parametets, thought x1 would make more sense
// to the user than xF, fairly arbitrary though.. might just be personal preference
{
	//Implementation of Bresenham's line algorithm, derived from pseudocode
	//from Wikipedia.

	
	int dx = intMath->abs(x2 - x1);
	int dy = intMath->abs(y2 - y1);

	int sx = intMath->signum(x2 - x1);
	int sy = intMath->signum(y2 - y1);
	int err = dx - dy;
	int x = x1;
	int y = y1;

	while(true)
	{
		modify(color,x,y);
		if(x == x2 && y == y2)
			break;
		int e2 = 2 * err;
		if(e2 > -dy)
		{
			err -= dy;
			x += sx;
		}
		if(e2 < dx)
		{
			err += dx;
			y += sy;
		}
	}
}

/**
* @brief Get Color Array
* 
* Retrieves an array of color values at a given pixel using row major order
* based on the x and y coordinate of the pixel in the picture
* 
* @param x the X-coordinate to get pixel data from
* @param y the Y-coordinate to get pixel data from
* 
* @return A pointer to a uint8_t array containing, in order, the red, green, and
* blue values for the pixel.
*/
uint8_t* CatPictureApp::get(int x, int y)
{
	// Calculate row major order
	int start = 3 * (x + y * WIDTH);
	uint8_t* arr = new uint8_t[3];
	arr[0] = dataArr[start];
	arr[1] = dataArr[start + 1];
	arr[2] = dataArr[start + 2];
	return arr;
}

//TODO: Doesn't work...
///**
//* copies a rectangular area from one area to another.
//* params:
//	xa= x-coordinate to start at
//	ya = y-coordinate to start at
//	l = length of the area to copy
//	w = width of the area to copy
//	xf = x-coordinate of the upper-left corner of where to paste to.
//	yf = y-coordinate of above.
//*/
//void catpictureapp::copy(int x, int y, int l, int w, int xf, int yf)
//{
//	uint8_t* arr = new uint8_t[l * w * 3];
//	int count = 0;
//	for(int i = y; i < y + l; i++)
//	{
//		for(int j = x; j < x + w; j++, count += 3)
//		{
//			uint8_t* arrpixel = get(j, i);
//			arr[count] = arrpixel[0];
//			arr[count + 1] = arrpixel[1];
//			arr[count + 2] = arrpixel[2];
//			
//		}
//	}
//	count = 0;
//	for(int i = yf; i < y + l; i++)
//	{
//		for(int j = xf; y < x + w; j++, count += 3)
//		{
//			dataarr[j + i * 3] = arr[count];
//			dataarr[j + i * 3 + 1] = arr[count + 1];
//			dataarr[j + i * 3 + 2] = arr[count + 2];
//		}
//	}
//}

/**
* @brief Draw Triangle
* 
* Draws only the edges of a triangle based on the 
* three points of each of the vertices
* of the triangle as well as the color of the triangle
*  
* @param x1 the x coordinate of the first vertex
* @param y1 the y coordinate of the first vertex
* @param x2 the x coordinate of the second vertex
* @param y2 the y coordinate of the second vertex
* @param x3 the x coordinate of the third vertex
* @param y3 the y coordinate of the third vertex
* @param line the color of the edges of each of the triangle
*/
void CatPictureApp::drawTriangle(int x1, int y1, int x2, int y2,int x3,int y3, Color8u* line)
{
	drawLine(x1, y1, x2, y2, line);
	drawLine(x1, y1, x3, y3, line);
	drawLine(x2, y2, x3, y3, line);

}

/**
* @brief Draw Rectangle
* 
* Draws a rectangle from point (x1,y1) to point (x2,y2).
* 
* @param xA the X-coordinate of the first point
* @param yA the Y-coordinate of the first point
* @param xB the X-coordinate of the second point
* @param yB the Y-coordinate of the secondpoint
* @param line the color for the line
* @param fill the color for the fill. If this is 0, the rectangle will not be filled.
*/

void CatPictureApp::drawRectangle(int xA, int yA, int xB, int yB, Color8u* line, Color8u* fill)
{
	int x1, x2, y1, y2;

	//Swap values so x1 < x2 and y1 < y2
	if(xB < xA && yB < yA)
	{
		x1 = xB;
		y1 = yB;
		x2 = xA;
		y2 = yA;
	}
	else if(xB < xA && yB > yA)
	{
		x1 = xB;
		y1 = yA;
		x2 = xA;
		y2 = yB;
	}
	else if(xB > xA && yB < yA)
	{
		x1 = xA;
		y1 = xB;
		x2 = xB;
		y2 = yA;
	}
	else
	{
		x1 = xA;
		x2 = xB;
		y1 = yA;
		y2 = yB;
	}


	drawLine(x1, y1, x1, y2, line);
	drawLine(x1, y1, x2, y1, line);
	drawLine(x1, y2, x2, y2, line);
	drawLine(x2, y1, x2, y2, line);

	if(fill != 0)
	{
		for(int x = x1 + 1; x < x2; x++)
		{
			for(int y = y1 + 1; y < y2; y++)
			{
				modify(fill, x, y);
			}
		}
	}
}


/**
* @brief Modify Pixel
* 
* Modifies a single pixel to a given color at a given 
* coordinate on the image. This method serves mainly to
* abstract the row-major order of the data array, provide
* one specific place to modify it, and perform bounds
* checking
* 
* @param color the Color8u object that contains of the desired color.
* @param x the X-coordinate
* @param y the Y-coordinate
* 
* @returns false if and only if the X or Y coordinates are out of bounds.
*/
bool CatPictureApp::modify(Color8u* color, int x, int y)
{
	if(x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
	{
		console() << "Draw failed: (" << x << "," << y << ") is out of bounds." << endl;
		return false;
	}
	int index = 3 * (x + y * WIDTH);
	dataArr[index] = color->r;
	dataArr[index + 1] = color->g;
	dataArr[index + 2] = color->b;
	return true;
}


void CatPictureApp::draw()
{
	//Draw the surface
	gl::draw(*surface);
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
