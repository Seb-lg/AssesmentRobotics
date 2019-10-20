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

/**
	Class of Arm interface
	Author: Sébastien LE GALL
**/

/// interface of the icub arm
class Arm {
public:
	/// Define the arm
	/// \param arm define the side of the arm
	Arm(std::string const &arm);

	/// Give a name to every muscles (except the fingers)
	void moveArmFrontToBack(double angle);
	void moveArmAwayFromBody(double angle);
	void rotateArmCloserInOrOut(double angle);
	void elbowAngle(double angle);
	void rotateForearm(double angle);
	void rotateHandUpDown(double angle);
	void rotateHandLeftRight(double angle);

private:
	/// save map of vector representing every muscles of the arm
	/// used to save arm configurations
	void setSavedArm();

private:
	/// yarp api to control an arm
	std::shared_ptr<PolyDriver>		robotArm;

	/// used to update POSITION of the arm muscle
	std::shared_ptr<IPositionControl>	pos;

	/// UNSUSED
	/// used to update Velocity of the arm muscle
	std::shared_ptr<IEncoders>		enc;

	/// vecor containing every arm's joint
	Vector muscles;

	/// map containing every saved arm configurations
	std::map<std::string, Vector>		savedArmPosition;
};


#endif //YARPY_ARM_HPP
