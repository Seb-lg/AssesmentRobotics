//
// Created by seb on 27/09/2019.
//

#ifndef YARPY_HEAD_HPP
#define YARPY_HEAD_HPP

#include "Eye.hpp"

#include <memory>

#include <yarp/dev/all.h>

using namespace yarp::dev;

class Head {
public:
	Head();

	void update();

	void moveHeadX(double angle);
	void moveHeadY(double angle);
	void moveHeadZ(double angle);

	void squint(double angle);

public:
	Eye left;
	Eye right;

private:
	std::shared_ptr<PolyDriver>		robotHead;

	std::shared_ptr<IPositionControl>	pos;
	std::shared_ptr<IEncoders>		enc;

	Vector muscles;
};


#endif //YARPY_HEAD_HPP
