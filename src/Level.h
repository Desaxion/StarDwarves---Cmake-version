#pragma once
class Level
{
public:

	virtual void generate() = 0;

	virtual void draw() = 0;

	virtual void update() = 0;

	float speed = 5.0f; //this should go faster if the player has played good

private:


};