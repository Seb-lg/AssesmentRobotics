//
// Created by seb on 28/09/2019.
//

#include "Arm.hpp"

Arm::Arm(std::string const &arm) {
	Property options;
	options.put("device","remote_controlboard");
	options.put("local",std::string("/client/")+ arm + "_arm");
	options.put("remote",std::string("/icubSim/") + arm + "_arm");

	robotArm = std::make_shared<PolyDriver>(options);

	IPositionControl* tmpPositionControl;
	IEncoders* tmpEncoders;
	robotArm->view(tmpPositionControl);
	robotArm->view(tmpEncoders);

	pos = std::shared_ptr<IPositionControl>(tmpPositionControl);
	enc = std::shared_ptr<IEncoders>(tmpEncoders);

	int jnts = 0;
	pos->getAxes(&jnts);
	muscles.resize(jnts);
	for (int i = 0; i < jnts; ++i)
		muscles[i] = 0;

	event.addEvent("update", [this](){
		pos->positionMove(muscles.data());
	});
	event.addEvent("random " + arm, [this, jnts](){
		for (int i = 0; i < jnts; ++i)
			muscles[i] = rand() % 360 - 180;
	});
	event.addEvent<std::string>("saved arm " + arm, [this](std::string saved){
		if (savedArmPosition.find(saved) != savedArmPosition.end())
			muscles = savedArmPosition[saved];
	});

	setSavedArm();
}

void Arm::moveArmFrontToBack(double angle) {
	muscles[0] = angle;
}

void Arm::moveArmAwayFromBody(double angle) {
	muscles[1] = angle;
}

void Arm::rotateArmCloserInOrOut(double angle) {
	muscles[2] = angle;
}

void Arm::elbowAngle(double angle) {
	muscles[3] = angle;
}

void Arm::rotateForearm(double angle) {
	muscles[4] = angle;
}

void Arm::rotateHandUpDown(double angle) {
	muscles[5] = angle;
}
void Arm::rotateHandLeftRight(double angle) {
	muscles[6] = angle;
}

void Arm::setSavedArm() {
	int jnts = 0;
	pos->getAxes(&jnts);

	auto &flip = savedArmPosition["flip"];
	flip.resize(jnts);
	int i = 0;
	flip[i++] = -74;
	flip[i++] = 11;
	flip[i++] = 11;
	flip[i++] = 105;
	flip[i++] = -88;
	flip[i++] = 0;
	flip[i++] = 0;
	flip[i++] = 60;
	flip[i++] = 90;
	flip[i++] = 0;
	flip[i++] = 180;
	flip[i++] = 90;
	flip[i++] = 180;
	flip[i++] = 0;
	flip[i++] = 0;
	flip[i++] = 270;

	auto &bronchade = savedArmPosition["bronchade"];
	bronchade.resize(jnts);
	i = 0;
	bronchade[i++] = -15;
	bronchade[i++] = 11;
	bronchade[i++] = 0;
	bronchade[i++] = 16;
	bronchade[i++] = 0;
	bronchade[i++] = 0;
	bronchade[i++] = 0;
	bronchade[i++] = 60;
	bronchade[i++] = 59;
	bronchade[i++] = 0;
	bronchade[i++] = 99;
	bronchade[i++] = 57;
	bronchade[i++] = 115;
	bronchade[i++] = 0;
	bronchade[i++] = 52;
	bronchade[i++] = 21;
}
