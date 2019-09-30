//
// Created by seb on 28/09/2019.
//

#ifndef YARPY_ARM_HPP
#define YARPY_ARM_HPP

#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <memory>

#include "EventLibrary.hpp"

using namespace yarp::os;
using namespace yarp::sig;

using namespace yarp::dev;

class Arm {
public:
	Arm(std::string const &arm);

	void moveArmFrontToBack(double angle);
	void moveArmAwayFromBody(double angle);
	void rotateArmCloserInOrOut(double angle);
	void elbowAngle(double angle);
	void rotateForearm(double angle);
	void rotateHandUpDown(double angle);
	void rotateHandLeftRight(double angle);

private:
	void setSavedArm();

private:
	std::shared_ptr<PolyDriver>		robotArm;

	std::shared_ptr<IPositionControl>	pos;
	std::shared_ptr<IEncoders>		enc;

	Vector muscles;
	std::map<std::string, Vector>		savedArmPosition;
};


#endif //YARPY_ARM_HPP
