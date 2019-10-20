//
// Created by seb on 27/09/2019.
//

#include "Head.hpp"


//TODO delete
/**
 * _setPoints[0]	Head Pitch
 * _setPoints[1]	Head Roll
 * _setPoints[2]	Head Yaw
 * _setPoints[3]	Eye pitch
 * _setPoints[4]	Eye yaw
 * _setPoints[5]	Eye focus distance
 * */

Head::Head():left("left"), right("right") {
	event.addEvent("update", [this](){this->update();});

	event.addEvent<double>("move eye x", [this](double angle){this->muscles[4] = angle;});
	event.addEvent<double>("move eye y", [this](double angle){this->muscles[3] = angle;});
	event.addEvent<double>("move head x", [this](double angle){this->moveHeadX(angle);});
	event.addEvent<double>("move head y", [this](double angle){this->moveHeadY(angle);});
	event.addEvent<double>("add head x", [this](double angle){this->moveHeadX(muscles[2] + angle);});
	event.addEvent<double>("add head y", [this](double angle){this->moveHeadY(muscles[0] + angle);});
	Property options;
	options.put("device", "remote_controlboard");
	options.put("local", "/tutorial/motor/head");
	options.put("remote", "/icubSim/head");

	robotHead = std::make_shared<PolyDriver>(options);

	IPositionControl* tmpPositionControl;
	IEncoders* tmpEncoders;
	robotHead->view(tmpPositionControl);
	robotHead->view(tmpEncoders);

	pos = std::shared_ptr<IPositionControl>(tmpPositionControl);
	enc = std::shared_ptr<IEncoders>(tmpEncoders);

	int jnts = 0;
	pos->getAxes(&jnts);
	muscles.resize(jnts);
	for (int i = 0; i < jnts; ++i)
		muscles[i] = 0;


	event.addEvent("clear head", [this, jnts](){
		for (int i = 0; i < jnts; ++i)
			muscles[i] = 0;
	});
}

void Head::update() {
	pos->positionMove(muscles.data());
}

void Head::moveHeadX(double angle) {
	muscles[2] = angle;
}

void Head::moveHeadY(double angle) {
	muscles[0] = angle;
}

void Head::moveHeadZ(double angle) {
	muscles[1] = angle;
}

void Head::squint(double angle) {
	muscles[5] = angle;
}
