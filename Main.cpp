#include <stdio.h>
#include <iostream>
/* Get all OS and signal processing YARP classes */
#include <yarp/os/all.h>
#include <yarp/sig/all.h>

#include "EventLibrary.hpp"
#include "Robot.hpp"
#include "face_detection/face_detection.hpp"

using namespace yarp::os;
using namespace yarp::sig;

/// initiat event library manager
EventLibrary event;

/*void moveEye(ImageOf<PixelRgb> *image) {
	//printf("We got an image of size %dx%d\n", image->width(), image->height());
	ImageOf<PixelRgb> grey(*image);
	for (int x=0; x<grey.width(); x++) {
		for (int y=0; y<grey.height(); y++) {

		}
	}

	double xMean = 0;
	double yMean = 0;
	int ct = 0;
	for (int x=0; x<image->width(); x++) {
		for (int y=0; y<image->height(); y++) {
			PixelRgb& pixel = image->pixs	el(x,y);
			// very simple test for blueishness
			// make sure blue level exceeds red and green by a factor of 2
			if (pixel.r>pixel.b*1.2+10 && pixel.r>pixel.g*1.2+10) {
				// there's a blueish pixel at (x,y)!
				// let's find the average location of these pixels
				xMean += x;
				yMean += y;
				ct++;
			}
		}
	}
	if (ct>0) {
		xMean /= ct;
		yMean /= ct;
	}
	if (ct>(image->width()/20)*(image->height()/20)) {
		//exit(0);
		double x = xMean;
		double y = yMean;
		double conf = ct;
		x -= (double)image->width()/2.0;
		y -= (double)image->height()/2.0;
		double vx = x*0.1;
		double vy = -y*0.1;
		if (conf > 0.5) {
			event.fire("move eye x", vx);
			event.fire("move eye y", vy);
			if (vx > 0.0)
				event.fire("add head x", -1.0);
			else if (vx < 0.0)
				event.fire("add head x", 1.0);
			if (-vy > 0.0)
				event.fire("add head y", -1.0);
			else if (-vy < 0.0)
				event.fire("add head y", 1.0);
		}
	} else {

	}
}*/

int main() {
	Network yarp; // set up yarp
	Robot icub;	// set up the robot interface
	FaceDetection detect; // set up detecion

	/// set the event listener that will look at a given position
	event.addEvent<int, int>("look at position", [](int xMean, int yMean){
		std::cout << "I look at position : " << xMean << " " << yMean << std::endl;
		double x = xMean;
		double y = yMean;
		/// change the abscissa and ordinate to the center of the image
		/// 320 is the width of the image and 240 is the height
		x -= (double)320/2.0;
		y -= (double)240/2.0;
		double vx = x*0.1;
		double vy = -y*0.1;

		/// Move the eye to look at the position
		event.fire("move eye x", vx);
		event.fire("move eye y", vy);

		/// Slowly move the eye set the eye position to the center of the image
		if (vx > 0)
			event.fire("add head x", -0.1);
		else if (vx < 0)
			event.fire("add head x", 0.1);
		if (-vy > 0)
			event.fire("add head y", -0.1);
		else if (-vy < 0)
			event.fire("add head y", 0.1);
	});

	/// main loop
	while (true) {
		event.fire("update");
	}
}