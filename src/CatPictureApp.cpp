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

  private:
	Surface* surface;
	uint8_t* dataArr;
	
};

void CatPictureApp::setup()
{
	setWindowSize(WIDTH, HEIGHT);
	surface = new Surface(WIDTH, HEIGHT, false);

	dataArr = surface->getData();

	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			modify(new Color8u(0,255,0),x,y);
		}
	}
	
	drawLine(10,10,409,259,new Color8u(76,11,21));
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

/*
Bresenham's line algorithm:
function line(x0, y0, x1, y1)
   dx := abs(x1-x0)
   dy := abs(y1-y0) 
   if x0 < x1 then sx := 1 else sx := -1
   if y0 < y1 then sy := 1 else sy := -1
   err := dx-dy
 
   loop
     setPixel(x0,y0)
     if x0 = x1 and y0 = y1 exit loop
     e2 := 2*err
     if e2 > -dy then 
       err := err - dy
       x0 := x0 + sx
     end if
     if e2 <  dx then 
       err := err + dx
       y0 := y0 + sy 
     end if
   end loop
*/
void CatPictureApp::drawLine(int xI, int yI, int xF, int yF, Color8u* color)
{
	//Implementation of Bresenham's line algorithm

	math<int>* intMath = new math<int>();
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
* Modifies a single pixel to a given color at a given coordinate on the image. This method serves mainly to
* abstract the row-major order of the data array, provide one specific place to modify it, and perform bounds
* checking
* Params: 
	r = Red value
	g = Green value
	b = Blue value
	x = X-coordinate
	y = Y-coordinate
	dataArr = Data array of the image.
* Returns:
	false if and only if the X or Y coordinates are out of bounds.
*/
bool CatPictureApp::modify(Color8u* color, int x, int y)
{
	if(x < 0 || y < 0 || x > WIDTH || y > HEIGHT)
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
