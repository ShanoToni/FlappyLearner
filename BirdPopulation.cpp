#include "BirdPopulation.h"
#include <chrono>
#include <iostream>

BirdPopulation::BirdPopulation(int numOfBirds)
{
	distribution = std::uniform_real_distribution<float>(0.0f, 720.f);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	float RandomHeight = distribution(generator);

	for (int i = 0; i < numOfBirds; i++)
	{
		RandomHeight = distribution(generator);
		std::shared_ptr<Bird> newBird(new Bird(sf::Vector2f(200, RandomHeight)));
		CurrentBirdPop.push_back(std::move(newBird));
	}
}

void BirdPopulation::Crossover(std::shared_ptr<Bird> & bird1, std::shared_ptr<Bird> & bird2)
{

}

//std::unique_ptr<Bird> & BirdPopulation::GetParent()
//{
//	return std::unique_ptr<Bird>();
//}

void BirdPopulation::AddToPopulation(std::shared_ptr<Bird> bird)
{

}

void BirdPopulation::TrainGeneration()
{

}

void BirdPopulation::addToSavedBirds(std::shared_ptr<Bird> DeadBird)
{

}

void BirdPopulation::UpdateBirds(double DeltaTime)
{
	if (CurrentBirdPop.size() > 0)
	{
		for (auto & b : CurrentBirdPop)
		{
			b->Velocity.x = b->Velocity.x + b->Acceleration.x * DeltaTime;
			b->Velocity.y = b->Velocity.y + b->Acceleration.y * DeltaTime;
			b->Velocity.y = b->Velocity.y + b->Lift;
			b->Velocity = b->Velocity * 0.99999f;

			b->Position.x = b->Position.x + b->Velocity.x * DeltaTime;
			b->Position.y = b->Position.y + b->Velocity.y * DeltaTime;

			b->BirdShape.setPosition(b->Position);

			b->Acceleration = sf::Vector2f(0.0f, 600.f);
			b->Lift = 0.f;
		}
	}
}

void BirdPopulation::RemoveDeadBirds()
{
	if (CurrentBirdPop.size() > 0)
	{
		auto i = CurrentBirdPop.begin();
		while (i != CurrentBirdPop.end())
		{
			if (!(*i)->getAlive())
			{
				SavedBirdPop.push_back(*i);
				CurrentBirdPop.erase(i++);
			}
			else
			{
				++i;
			}
		}
	}
}

void BirdPopulation::ConstrainBirds(float screenHeight)
{
	if (CurrentBirdPop.size() > 0)
	{
		for (auto& p : CurrentBirdPop)
		{
			if (p->GetBirdShape().getPosition().y > screenHeight - 50.f)
			{	
				p->getAlive() = false;
			}
			else if (p->GetBirdShape().getPosition().y < 0.f)
			{
				p->getAlive() = false;
			}
		
		}
	}
}

void BirdPopulation::Think(Pipe& p, float screenW, float screenH)
{
	if (CurrentBirdPop.size() > 0)
	{
		for (auto& b : CurrentBirdPop)
		{
			std::vector<float> inputs;
			inputs.push_back(b->GetBirdShape().getPosition().y / screenH);
			inputs.push_back(b->GetVelocity().y);
			inputs.push_back(p.GetPipeTop().getPosition().x / screenW);
			inputs.push_back(p.GetPipeTop().getPosition().y + p.GetPipeTop().getSize().y / screenH);
			inputs.push_back(p.GetPipeBottom().getPosition().y / screenH);

			std::vector<float> outputs = b->getBrain()->feedForward(inputs);
			if (outputs[0] > 0.5f && b->GetVelocity().y > -500)
			{
				b->Jump();
			}
		}
	}
}

BirdPopulation::~BirdPopulation()
{

}

void UpdateBirds(double DeltaTime)
{
}
