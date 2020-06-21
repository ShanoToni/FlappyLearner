#include "PipeController.h"

PipeController::PipeController(float ScreenX, float ScreenY):PipeController(ScreenX,ScreenY,100)
{
	
}

PipeController::PipeController(float ScreenX, float ScreenY, float d):
	delay(d),height(ScreenY),width(ScreenX)
{
	timer = delay;
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

void PipeController::CollideWithBird(Bird& bird)
{
	if (PipeList.size() > 0)
	{
		for (auto& p : PipeList)
		{
			p->CollideWithBird(bird);
		}
	}
}

PipeController::~PipeController()
{
	PipeList.clear();
}
