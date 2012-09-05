/**
* Project by Brandon Dadosky for CSE 274 homework assignment 1.
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"


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
	intMath = new math<int>();

	dataArr = surface->getData();


	//Begin test cases
	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			modify(new Color8u(0,255,0),x,y);
		}
	}
	
	drawLine(0,0,800,600,new Color8u(0,0,0));
	drawRectangle(200, 210, 48, 92, new Color8u(0,0,0),new Color8u(255,0,0));
	drawCircle(200, 300, 100, new Color8u(0,0,0), new Color8u(0, 0, 255));
	drawTriangle(35, 135, 210, 411, 510, 500, new Color8u(0,0,0));
	blur();
}


/**
* Draws a circle.
* Params:
	xC = X-coordinate of the center of the circle
	yC = Y-coordinate of the center of the circle
	r = Radius of the circle
	line = Color object containing color information for the outside line
	fill = Color of the fill. If this value is 0, the circle won't be filled.
*/
void CatPictureApp::drawCircle(int xC, int yC, int r, Color8u* line, Color8u* fill)
{
	double dist;
	for(int x = 0; x <= WIDTH; x++)
	{
		for(int y = 0; y <= HEIGHT; y++)
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

void CatPictureApp::mouseDown( MouseEvent event )
{
}

/**
* Blurs the image
*/
void CatPictureApp::blur()
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

void CatPictureApp::update()
{
	dataArr = surface->getData();
}
/**
* Draws a straight line between the two points.
* Params:
	xI = Initial X
	yI = Initial Y
	xF = Final X
	yF = Final Y
	color = Color to draw.
*/
void CatPictureApp::drawLine(int xI, int yI, int xF, int yF, Color8u* color)
{
	//Implementation of Bresenham's line algorithm, derived from pseudocode
	//from Wikipedia.

	
	int dx = intMath->abs(xF - xI);
	int dy = intMath->abs(yF - yI);

	int sx = intMath->signum(xF - xI);
	int sy = intMath->signum(yF - yI);
	int err = dx - dy;
	int x = xI;
	int y = yI;

	while(true)
	{
		modify(color,x,y);
		if(x == xF && y == yF)
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
* Retrieves an array of color values at a given pixel
* Params:
	x = X-coordinate to get pixel data from
	y = Y-coordinate to get pixel data from
* Returns:
	A pointer to a uint8_t array containing, in order, the red, green, and
	blue values for the pixel.
*/
uint8_t* CatPictureApp::get(int x, int y)
{
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
* Draws a triangle.
* Params: 
	x1, y1, x2, y2, x3, y3 = Points of the triangle
	line = Color of the line
*/
void CatPictureApp::drawTriangle(int x1, int y1, int x2, int y2,int x3,int y3, Color8u* line)
{
	drawLine(x1, y1, x2, y2, line);
	drawLine(x1, y1, x3, y3, line);
	drawLine(x2, y2, x3, y3, line);

}

/**
* Draws a rectangle from point (x1,y1) to point (x2,y2).
* Params:
	xA = X-coordinate to start at
	yA = Y-coordinate to start at
	xB = X-coordinate to end at
	yB = Y-coordinate to end at
	line = Color for the line
	fill = Color for the fill. If this is 0, the rectangle will not be filled.
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
* Modifies a single pixel to a given color at a given coordinate on the image. This method serves mainly to
* abstract the row-major order of the data array, provide one specific place to modify it, and perform bounds
* checking
* Params: 
	color = A Color8u object that contains the desired color.
	x = X-coordinate
	y = Y-coordinate
* Returns:
	false if and only if the X or Y coordinates are out of bounds.
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
