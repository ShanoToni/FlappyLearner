#include "PipeController.h"
#include <iostream>

PipeController::PipeController(float ScreenX, float ScreenY):PipeController(ScreenX,ScreenY,100)
{
	
}

PipeController::PipeController(float ScreenX, float ScreenY, float d):
	delay(d),height(ScreenY),width(ScreenX)
{
	timer = delay;
	speed = 6.f;
	dt = 0.03f;
	AddPipe();
}

void PipeController::AddPipe()
{
	distribution = std::uniform_real_distribution<float>(0.5f, 2.5f);
	float GapModifier = distribution(generator);
	std::unique_ptr<Pipe> NewPipe(new Pipe(100.f, 200.f, width, height, GapModifier));
	PipeList.push_back(std::move(NewPipe));
}

void PipeController::RemovePipe()
{
	 PipeList.pop_front();
}

void PipeController::Update()
{
	timer = timer - dt * speed * 0.1f;
	if (timer <= 0.f)
	{
		timer = delay;
		AddPipe();
	}
	if (PipeList.size() > 0)
	{
		for (auto& p : PipeList)
		{
			p->UpdatePipe(speed);
			//p->GetPipeTop().setFillColor(sf::Color(255, 255, 255, 150));
		}
		if (PipeList[0]->GetShouldDelete())
		{
			RemovePipe();
		}
	}
}

void PipeController::CollideWithBird(Bird& b)
{
	if (PipeList.size() > 0)
	{
		for (auto& p : PipeList)
		{
			//CollideWithBird(bird);
			float pipeTopY = p->GetPipeTop().getPosition().y + p->GetPipeTop().getSize().y;
			float pipeTopX = p->GetPipeTop().getPosition().x;
			float pipeTopXMax = p->GetPipeTop().getPosition().x + p->GetPipeTop().getSize().x;

			float PipeBottomY = p->GetPipeBottom().getPosition().y;

			float birdPosX = b.GetBirdShape().getPosition().x;
			float birdPosY = b.GetBirdShape().getPosition().y;

			//if the bird and pipe have a similar X
			if (pipeTopX <= birdPosX + 50 && pipeTopXMax >= birdPosX)
			{

				if (pipeTopY >= birdPosY || PipeBottomY <= birdPosY + 50)
				{
					std::cout << "HIT!" << std::endl;
					b.getAlive() = false;
				}
			}
		}
	}
}

void PipeController::ResetPipes()
{
	PipeList.clear();
	timer = delay;
	AddPipe();
}

Pipe PipeController::getClosestPipe(Bird& bird)
{
	std::shared_ptr<Pipe> closest = PipeList[0];
	float closestDistance = closest->GetPipeTop().getPosition().x + (closest->GetPipeTop().getSize().x / 2) - (bird.GetBirdShape().getPosition().x + 25.f);
	for (auto& p : PipeList)
	{
		p->GetPipeBottom().setFillColor(sf::Color(255, 255, 255, 150));
		p->GetPipeTop().setFillColor(sf::Color(255, 255, 255, 150));
		float distance = p->GetPipeTop().getPosition().x + (p->GetPipeTop().getSize().x / 2) - (bird.GetBirdShape().getPosition().x + 25.f);

		if (closestDistance < 0)
		{
			closestDistance = 1000.f;
		}

		if (distance < closestDistance && distance + p->GetPipeTop().getSize().x > 0)
		{
			closest = p;
		}
	}
	closest->GetPipeBottom().setFillColor(sf::Color(255, 0, 0, 250));
	closest->GetPipeTop().setFillColor(sf::Color(255, 0, 0, 250));

	return *closest;
}

PipeController::~PipeController()
{
	PipeList.clear();
}
