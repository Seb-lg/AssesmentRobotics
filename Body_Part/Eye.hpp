//
// Created by seb on 27/09/2019.
//

#ifndef YARPY_EYE_HPP
#define YARPY_EYE_HPP

#include <yarp/os/all.h>
#include <yarp/sig/all.h>

#include "EventLibrary.hpp"

using namespace yarp::os;
using namespace yarp::sig;

class Eye {
public:
	Eye(std::string eye);

	void update();

	void getImage();

	void moveX(double angle);
	void moveY(double angle);

private:
void test();
	std::string eye;
	BufferedPort<ImageOf<PixelRgb> > imagePort;
	ImageOf<PixelRgb> *image;

};


#endif //YARPY_EYE_HPP
