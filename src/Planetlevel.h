#pragma once
#include "Level.h"
#include "Meteor.h"
#include <vector>

class Planetlevel : public Level
{
public:
	std::vector<Meteor> meteors;

	void generate() override {

	}

	void draw(glm::mat4 projection, glm::mat4 view, glm::vec4 metColor) override {

	}

	void update() override {


	}

private:


};
#pragma once