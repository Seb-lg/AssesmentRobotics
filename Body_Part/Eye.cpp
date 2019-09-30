//
// Created by seb on 27/09/2019.
//

#include "Eye.hpp"

Eye::Eye(std::string eye): eye(eye) {
	imagePort.open(std::string("/tutorial/image/") + eye + std::string("/in")); // give the port a name
	Network::connect(std::string("/icubSim/cam/") + eye, std::string("/tutorial/image/") + eye + std::string("/in"));

	event.addEvent("update", [this](){this->update();});
}

void Eye::update() {
	this->getImage();
}

void Eye::getImage() {
	image = imagePort.read();
	if (image != nullptr) {
		event.fire(std::string("image ") + eye + " eye", image);
	}
}

void Eye::moveX(double angle) {
	event.fire("move eye x", angle);
}

void Eye::moveY(double angle) {
	event.fire("move eye y", angle);
}
