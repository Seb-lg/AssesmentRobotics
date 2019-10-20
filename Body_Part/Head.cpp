//
// Created by seb on 27/09/2019.
//

#include "Head.hpp"

Head::Head():left("left"), right("right") {
	/// Set Update Event to update the muscles position
	event.addEvent("update", [this](){this->update();});

	/// Add Event to move the eye to given Y or X position
	event.addEvent<double>("move eye x", [this](double angle){this->muscles[4] = angle;});
	event.addEvent<double>("move eye y", [this](double angle){this->muscles[3] = angle;});
	/// Add Event to move the eye to given Y or X position
	event.addEvent<double>("move head x", [this](double angle){this->muscles[2] = angle;});
	event.addEvent<double>("move head y", [this](double angle){this->muscles[0] = angle;});
	/// Add Event to add to the eye the given angle to Y or X position
	event.addEvent<double>("add head x", [this](double angle){this->muscles[2] += angle;});
	event.addEvent<double>("add head y", [this](double angle){this->muscles[0] += angle;});

	/// Setup YARP api options to get control over Head
	Property options;
	options.put("device", "remote_controlboard");
	options.put("local", "/tutorial/motor/head");
	options.put("remote", "/icubSim/head");

	/// Create YARP api with set options
	robotHead = std::make_shared<PolyDriver>(options);

	/// Create POSITION and VELOCITY control and bind them to the arm API
	IPositionControl* tmpPositionControl;
	IEncoders* tmpEncoders;
	robotHead->view(tmpPositionControl);
	robotHead->view(tmpEncoders);

	pos = std::shared_ptr<IPositionControl>(tmpPositionControl);
	enc = std::shared_ptr<IEncoders>(tmpEncoders);

	/// Set the right number of Arm muscles
	int jnts = 0;
	pos->getAxes(&jnts);
	muscles.resize(jnts);
	for (int i = 0; i < jnts; ++i)
		muscles[i] = 0;

	/// Set an Event to clear the head muscles to DEFAULT
	event.addEvent("clear head", [this, jnts](){
		for (int i = 0; i < jnts; ++i)
			muscles[i] = 0;
	});
}

/// Made by Sébastien
void Head::update() {
	pos->positionMove(muscles.data());
}

/// Made by Sébastien
void Head::moveHeadX(double angle) {
	muscles[2] = angle;
}

/// Made by Sébastien
void Head::moveHeadY(double angle) {
	muscles[0] = angle;
}

/// Made by Sébastien
void Head::moveHeadZ(double angle) {
	muscles[1] = angle;
}

/// Made by Sébastien
void Head::squint(double angle) {
	muscles[5] = angle;
}
