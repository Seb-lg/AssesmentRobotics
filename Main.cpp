#include <stdio.h>
#include <iostream>
/* Get all OS and signal processing YARP classes */
#include <yarp/os/all.h>
#include <yarp/sig/all.h>

#include "EventLibrary.hpp"
#include "Robot.hpp"

using namespace yarp::os;
using namespace yarp::sig;

EventLibrary event;

void moveEye(ImageOf<PixelRgb> *image) {
	//printf("We got an image of size %dx%d\n", image->width(), image->height());
	double xMean = 0;
	double yMean = 0;
	int ct = 0;
	for (int x=0; x<image->width(); x++) {
		for (int y=0; y<image->height(); y++) {
			PixelRgb& pixel = image->pixel(x,y);
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
			std::cout << vx << " -- " << vy << std::endl;
			event.fire("move eye x", vx);
			event.fire("move eye y", vy);
			if (vx > 0)
				event.fire("add head x", -1);
			else if (vx < 0)
				event.fire("add head x", 1);
			if (-vy > 0)
				event.fire("add head y", -1);
			else if (-vy < 0)
				event.fire("add head y", 1);
		}
	} else {

	}
}

int main() {
	Network yarp; // set up yarp
	Robot icub;

	//icub.head.moveHeadZ(60);

	event.fire("saved arm left", std::string("bronchade"));
	event.fire("saved arm right", std::string("bronchade"));

	event.addEvent<ImageOf<PixelRgb>*>("image right eye", moveEye);

	while (true) {
		event.fire("update");
	}
}