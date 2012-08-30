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
	bool CatPictureApp::modify(uint8_t r, uint8_t g, uint8_t b, int x, int y, uint8_t* dataArr);

private:
	Surface* surface;
};

void CatPictureApp::setup()
{
	setWindowSize(WIDTH, HEIGHT);
	surface = new Surface(WIDTH, HEIGHT, false);
}



void CatPictureApp::mouseDown( MouseEvent event )
{
}

void CatPictureApp::update()
{
	uint8_t* dataArr = surface->getData();

	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			if(!modify(0,255,0,x,y,dataArr))
				console() << "Draw failed: (" << x << "," << y << ") is out of bounds." << endl; 
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
bool CatPictureApp::modify(uint8_t r, uint8_t g, uint8_t b, int x, int y, uint8_t* dataArr)
{
	if(x < 0 || y < 0 || x > WIDTH || y > HEIGHT)
		return false;
	int index = 3 * (x + y * WIDTH);
	dataArr[index] = r;
	dataArr[index + 1] = g;
	dataArr[index + 2] = b;
	return true;
}


void CatPictureApp::draw()
{
	//Draw the surface
	gl::draw(*surface);
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
