//
// Created by seb on 27/09/2019.
//

#include "Eye.hpp"

/// Made by Sébastien
Eye::Eye(std::string eye): eye(eye) {
	/// create an image buffer as a port
	imagePort.open(std::string("/project/image/") + eye + std::string("/in")); // give the port a name

	/// Connect the buffer to the given eye
	Network::connect(std::string("/icubSim/cam/") + eye, std::string("/project/image/") + eye + std::string("/in"));

	/// Create the Update event
	event.addEvent("update", [this](){this->update();});
}

/// Made by Sébastien
void Eye::update() {
	this->getImage();
}

/// Made by Sébastien
void Eye::getImage() {
	/// Get image from the buffer
	image = imagePort.read();
	if (image != nullptr) {
		/// Send it to every event that are connected to this event
		event.fire(std::string("image ") + eye + " eye", image);
	}
}

/// Made by Sébastien
void Eye::moveX(double angle) {
	event.fire("move eye x", angle);
}

/// Made by Sébastien
void Eye::moveY(double angle) {
	event.fire("move eye y", angle);
}
