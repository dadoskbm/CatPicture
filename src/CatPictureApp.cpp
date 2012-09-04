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
	bool modify(Color8u* color, int x, int y);
	void drawLine(int xI, int yI, int xF, int yF, Color8u* color);
	void drawRectangle(int xA, int yA, int xB, int yB, Color8u* line, Color8u* fill);
	void drawCircle(int xC, int yC, int r, Color8u* line, Color8u* fill);
	void copy(int x1, int x2, int y1, int y2, int xF, int yF);

  private:
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

	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			modify(new Color8u(0,255,0),x,y);
		}
	}
	
	drawLine(0,0,800,600,new Color8u(0,0,0));
	drawRectangle(200, 210, 48, 92, new Color8u(0,0,0),new Color8u(255,0,0));
}



void CatPictureApp::mouseDown( MouseEvent event )
{
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
