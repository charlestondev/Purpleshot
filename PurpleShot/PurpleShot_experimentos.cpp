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

	cvarManager->registerNotifier("bt", [this](std::vector<std::string> args) {
		ballOnTop();
		}, "", PERMISSION_ALL);


	cvarManager->registerNotifier("sh", [this](std::vector<std::string> args) {
		shot();
		}, "", PERMISSION_ALL);

	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&PurpleShot::onTick, this));

	//cvarManager->log("Plugin loaded!");

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	cvarManager->log("the cvar with name: " + cvarName + " changed");
	//	cvarManager->log("the new value is:" + newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&PurpleShot::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&PurpleShot::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	cvarManager->log("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&PurpleShot::YourPluginMethod, this);
	
}

void PurpleShot::onUnload()
{
}



void PurpleShot::onTick() {
	CarWrapper car = gameWrapper->GetLocalCar();
	if (!car) { return; }
	//LOG("Pitch: " + std::to_string(car.GetRotation().Pitch));
	//LOG("Wheel on ball" + std::to_string(car.GetNumWheelContacts()));
	//LOG("touch ground " + std::to_string(car.AnyWheelTouchingGround()));
	//LOG("Key pressed 01 " + std::to_string(gameWrapper->IsKeyPressed(1)));
	//LOG("jump " + std::to_string(car.GetInput().Jump));
	
	if (car.GetInput().Jump == 1) {
		shot();
	}
	counter++;
}

void PurpleShot::ballOnTop() {
	if (!gameWrapper->IsInFreeplay()) { return; }
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) { return; }
	
	BallWrapper ball = server.GetBall();
	if (!ball) { return; }
	CarWrapper car = gameWrapper->GetLocalCar();
	if (!car) { return; }

	Vector carVelocity = car.GetVelocity();
	ball.SetVelocity(carVelocity);

	Vector carLocation = car.GetLocation();
	float ballRadius = ball.GetRadius();
	ball.SetLocation(carLocation + Vector{ 0, 0, ballRadius * 2 });

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


	//Vector shotVelocity = Vector(0, 0, 1000) + ball.GetVelocity();

	//Vector shotVelocity = (carLocation).getNormalized() * 1500 * -1;
	//Vector shotVelocity = carLocation * 1500;
	// Vector shotVelocity = car.GetVelocity().getNormalized() * 1000;
	if (car.GetNumWheelContacts() < 2) {return;}
	if (car.AnyWheelTouchingGround()) { return; }
	if (counter < 500) { return; }
	// todo: check wheels on car

	Vector shotVelocity = (ballLocation - carLocation).getNormalized() * 1000;
	ball.SetVelocity(ball.GetVelocity() + shotVelocity);
	LOG("shot ");
	
	counter = 0;
	
	//Vector shotVelocity = Vector(rot.X * cos(90) + rot.Z * sin(90), rot.Y, -rot.X * sin(90) + rot.Z * cos(90));

	//32536
	// r += 1000;
	// car.SetRotation(Rotator(r, 16268, 0));
	// car.HandleWheelBallHit();
	//car.HasFlip();
	
	
	//Rotator rot = car.GetRotation();
	//Rotator rot = car.GetRelativeRotation();
	
	//rot.Pitch += 48000;
	//Vector vet = RotatorToVector(rot);
	//ball.SetVelocity(vet * 1000);
	
	//ball.SetVelocity(rot * 1000);
	//LOG(std::to_string(RotatorToVector(car.GetRotation()).X));
	//LOG(std::to_string(RotatorToVector(car.GetRotation()).Y));
	//LOG(std::to_string(RotatorToVector(car.GetRotation()).Z));

	//LOG("Pitch: " + std::to_string(car.GetRotation().Pitch));
	//LOG("Yaw: " + std::to_string(car.GetRotation().Yaw));
	//LOG("Roll: " + std::to_string(car.GetRotation().Roll));
	
	

	// 
	//LOG(std::to_string(car.GetVelocity().X));
	//LOG(std::to_string(car.GetVelocity().Y));
	//LOG(std::to_string(car.GetVelocity().Z));
	// gameWrapper->IsKeyPressed()
	//car.GetInput().Jump;
	
}
