#pragma once

class Actor;

class Component {

public:
	virtual void start() = 0;
	virtual void update(float deltaTime, const Actor& actor) = 0;
};