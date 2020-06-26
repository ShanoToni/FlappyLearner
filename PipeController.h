#pragma once

#include "Pipe.h"
#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Bird.h"
#include <random>

class PipeController
{
public:
	PipeController();
	PipeController(float ScreenX, float ScreenY);
	PipeController(float ScreenX, float ScreenY, float d);

	//vars
private:
	std::deque<std::shared_ptr<Pipe>> PipeList;
	float delay;
	float timer;
	float width;
	float height;
	float speed;
	float dt;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

public:
	//get&set
	inline std::deque<std::shared_ptr<Pipe>>& GetPipes() { return PipeList; }
	//functions
	void resetTimer() { timer = delay; }
	void AddPipe();
	void RemovePipe();
	void Update();
	void CollideWithBird(Bird& b);
	Pipe getClosestPipe(Bird& bird);

	~PipeController();
};

