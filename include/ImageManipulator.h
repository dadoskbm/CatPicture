/**
Class that is responsible for directly manipulating the pixel array of the image.
@author Brandon Dadosky
*/
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageManipulator
{
	
public:
	//Creates an image manipulator for the given Surface
	ImageManipulator(Surface* surface);
};