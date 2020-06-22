#include "PipeController.h"
#include <iostream>

PipeController::PipeController(float ScreenX, float ScreenY):PipeController(ScreenX,ScreenY,100)
{
	
}

PipeController::PipeController(float ScreenX, float ScreenY, float d):
	delay(d),height(ScreenY),width(ScreenX)
{
	timer = delay;
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

void PipeController::Update(float DeltaTime)
{
	timer = timer - DeltaTime;
	if (timer <= 0.f)
	{
		timer = delay;
		AddPipe();
	}
	if (PipeList.size() > 0)
	{
		for (auto& p : PipeList)
		{
			p->UpdatePipe(DeltaTime, 200);
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
				}
			}
		}
	}
}

Pipe PipeController::getClosestPipe(Bird& bird)
{
	Pipe closest = *PipeList[0];
	float closestDistance = closest.GetPipeTop().getPosition().x - bird.GetBirdShape().getPosition().x;
	for (auto& p : PipeList)
	{
		float distance = p->GetPipeTop().getPosition().x - bird.GetBirdShape().getPosition().x;;
		
		if (closestDistance + closest.GetWidth() < 0)
		{
			closest = *p;
		}
		if (distance < closestDistance && distance + p->GetWidth() >0)
		{
			closest = *p;
		}
	}

	return closest;

}

PipeController::~PipeController()
{
	PipeList.clear();
}
