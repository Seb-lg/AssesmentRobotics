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

/**
	Class of Eye interface
	Author: Sébastien LE GALL
**/

/// interface of the icub eye
class Eye {
public:
	/// Define the eye
	/// \param eye
	Eye(std::string eye);

	/// Update function of the eye
	void update();

	/// Get Image from Left or Right eye
	void getImage();

	/// Move BOTH eye of given angle horizontally
	/// \param angle
	void moveX(double angle);

	/// Move BOTH eye of given angle vertically
	/// \param angle
	void moveY(double angle);

private:
	/// eye's side
	std::string eye;

	/// Image buffer for the eye
	BufferedPort<ImageOf<PixelRgb> > imagePort;

	/// Image read from the eye
	ImageOf<PixelRgb> *image;

};


#endif //YARPY_EYE_HPP
