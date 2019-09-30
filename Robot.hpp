//
// Created by seb on 27/09/2019.
//

#ifndef YARPY_ROBOT_HPP
#define YARPY_ROBOT_HPP

#include <Arm.hpp>
#include "Head.hpp"

class Robot {
public:
	Robot();

public:
	Head head;
	Arm leftArm;
	Arm rightArm;
};


#endif //YARPY_ROBOT_HPP
