//
// Created by seb on 28/09/2019.
//

#include "Arm.hpp"

///Made by Sébastien
Arm::Arm(std::string const &arm) {

	/// Setup YARP api options to get control over arm Left or Right
	Property options;
	options.put("device","remote_controlboard");
	options.put("local",std::string("/client/")+ arm + "_arm");
	options.put("remote",std::string("/icubSim/") + arm + "_arm");

	/// Create YARP api with set options
	robotArm = std::make_shared<PolyDriver>(options);

	/// Create POSITION and VELOCITY control and bind them to the arm API
	IPositionControl* tmpPositionControl;
	IEncoders* tmpEncoders;
	robotArm->view(tmpPositionControl);
	robotArm->view(tmpEncoders);

	pos = std::shared_ptr<IPositionControl>(tmpPositionControl);
	enc = std::shared_ptr<IEncoders>(tmpEncoders);

	/// Set the right number of Arm muscles
	int jnts = 0;
	pos->getAxes(&jnts);
	muscles.resize(jnts);
	for (int i = 0; i < jnts; ++i)
		muscles[i] = 0;

	/// Set Update Event to update the muscles position
	event.addEvent("update", [this](){
		pos->positionMove(muscles.data());
	});

	/// Event added by Morag: Randomise every arm muscles
	event.addEvent("random " + arm, [this, jnts](){
		for (int i = 0; i < jnts; ++i)
			muscles[i] = rand() % 360 - 180;
	});

	/// Add event to change the arm configuration to the given one
	event.addEvent<std::string>("saved arm " + arm, [this](std::string saved){
		if (savedArmPosition.find(saved) != savedArmPosition.end())
			muscles = savedArmPosition[saved];
	});

	setSavedArm();
}

/// Made by Sébastien
void Arm::moveArmFrontToBack(double angle) {
	muscles[0] = angle;
}

/// Made by Sébastien
void Arm::moveArmAwayFromBody(double angle) {
	muscles[1] = angle;
}

/// Made by Sébastien
void Arm::rotateArmCloserInOrOut(double angle) {
	muscles[2] = angle;
}

/// Made by Sébastien
void Arm::elbowAngle(double angle) {
	muscles[3] = angle;
}

/// Made by Sébastien
void Arm::rotateForearm(double angle) {
	muscles[4] = angle;
}

/// Made by Sébastien
void Arm::rotateHandUpDown(double angle) {
	muscles[5] = angle;
}

/// Made by Sébastien
void Arm::rotateHandLeftRight(double angle) {
	muscles[6] = angle;
}

/// Made by Morag
void Arm::setSavedArm() {
	int jnts, i = 0;
	pos->getAxes(&jnts);

	/// set a configuration named "wave"
	auto &momove = savedArmPosition["wave"];
	momove.resize(jnts);
	i = 0;
	momove[i++] = -76;
	momove[i++] = 32;
	momove[i++] = 0;
	momove[i++] = 85;
	momove[i++] = 68;
	momove[i++] = -29;
	momove[i++] = 0;
	momove[i++] = 55;
	momove[i++] = 26;
	momove[i++] = 90;
	momove[i++] = 0;
	momove[i++] = 0;
	momove[i++] = 0;
	momove[i++] = 0;
	momove[i++] = 0;
	momove[i++] = 0;

	/// set a configuration named "wave"
	auto &mocatch = savedArmPosition["catch"];
	mocatch.resize(jnts);
	i = 0;
	mocatch[i++] = -44;
	mocatch[i++] = 160;
	mocatch[i++] = 23;
	mocatch[i++] = 56;
	mocatch[i++] = -90;
	mocatch[i++] = -3;
	mocatch[i++] = 7;
	mocatch[i++] = 53;
	mocatch[i++] = 27;
	mocatch[i++] = 70;
	mocatch[i++] = 0;
	mocatch[i++] = 0;
	mocatch[i++] = 0;
	mocatch[i++] = 0;
	mocatch[i++] = 0;
	mocatch[i++] = 0;

}
