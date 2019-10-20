//
// Created by seb on 27/09/2019.
//

#ifndef YARPY_HEAD_HPP
#define YARPY_HEAD_HPP

#include "Eye.hpp"

#include <memory>

#include <yarp/dev/all.h>

using namespace yarp::dev;

/**
	Class of Head interface
	Author: Sébastien LE GALL
**/

/// interface of the icub head
class Head {
public:
	Head();

	/// Update function
	void update();

	/// Give a simple name to every muscles
	void moveHeadX(double angle);
	void moveHeadY(double angle);
	void moveHeadZ(double angle);
	void squint(double angle);

public:
	/// Left eye
	Eye left;

	/// Right eye
	Eye right;

private:
	/// yarp api to control the head
	std::shared_ptr<PolyDriver>		robotHead;

	/// used to update POSITION of the arm muscle
	std::shared_ptr<IPositionControl>	pos;

	/// UNSUSED
	/// used to update Velocity of the arm muscle
	std::shared_ptr<IEncoders>		enc;

	/// vecor containing every head's joint
	Vector muscles;
};


#endif //YARPY_HEAD_HPP
