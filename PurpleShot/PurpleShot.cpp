#include "pch.h"
#include "PurpleShot.h"
#include<string>
#include<math.h>

BAKKESMOD_PLUGIN(PurpleShot, "Purpleshot", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
float r = 0;
bool canShot = true;
int counter = 0;
void PurpleShot::onLoad()
{
	_globalCvarManager = cvarManager;

	LOG("Point your belly to the ball and Jump!");

	cvarManager->registerNotifier("purpleshot", [this](std::vector<std::string> args) {
		shot();
		}, "", PERMISSION_ALL);

	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&PurpleShot::onTick, this));

	
	
}

void PurpleShot::onUnload()
{
}



void PurpleShot::onTick() {
	CarWrapper car = gameWrapper->GetLocalCar();
	if (!car) { return; }
	
	
	if (car.GetInput().Jump == 1) {
		shot();
	}
	counter++;
}

void PurpleShot::shot() {
	if (!gameWrapper->IsInFreeplay()) { return; }
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) { return; }

	BallWrapper ball = server.GetBall();
	if (!ball) { return; }

	CarWrapper car = gameWrapper->GetLocalCar();
	if (!car) { return; }

	Vector carLocation = car.GetLocation();
	Vector ballLocation = ball.GetLocation();


	
	if (car.GetNumWheelContacts() < 3) {return;}
	if (car.AnyWheelTouchingGround()) { return; }
	if (counter < 500) { return; }
	// todo: check wheels on car

	Vector shotVelocity = (ballLocation - carLocation).getNormalized() * 1000;
	ball.SetVelocity(ball.GetVelocity() + shotVelocity);
	LOG("I'm shooting!");
	
	counter = 0;
	
}
