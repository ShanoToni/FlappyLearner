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

	Controller = PipeController(1280, 720, 2.f);
}

void BirdPopulation::Crossover(std::shared_ptr<Bird> & bird1, std::vector<float>& DNA)
{

	/*intDistribution = std::uniform_int_distribution<int>(0, bird1->DNA.size());
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	int RandomGene = intDistribution(generator);

	for (int i = 0; i < bird1->DNA.size(); i++)
	{
		if (RandomGene < i)
		{
			DNA.push_back(bird1->DNA[i]);
		}
		else
		{
			DNA.push_back(bird2->DNA[i]);
		}
	}*/

	//distribution = std::uniform_real_distribution<float>(0.0f, 1.f);
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	//std::default_random_engine generator(seed);
	//for (int i = 0; i < bird1->DNA.size(); i++)
	//{

	//	float RandomGene = distribution(generator);

	//	if (RandomGene < 0.5)
	//	{
	//		DNA.push_back(bird1->DNA[i]);	
	//	}
	//	else
	//	{
	//		DNA.push_back(bird2->DNA[i]);
	//	}
	//}
	distribution = std::uniform_real_distribution<float>(0.0f, 0.3f);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	for (int i = 0; i < bird1->DNA.size(); i++)
	{
		float chance = distribution(generator);
		if (chance > 1.5f)
		{
			float RandomGene = distribution(generator);
			DNA.push_back(bird1->DNA[i] + RandomGene - 0.15f);
		}
		else
		{
			DNA.push_back(bird1->DNA[i]);
		}

	}


}

std::shared_ptr<Bird>& BirdPopulation::GetParent()
{	
	float maxFitness{ 0 };
	for (auto& b : SavedBirdPop)
	{
		if (b->getFitnes() > maxFitness)
		{
			maxFitness = b->getFitnes();
		}
	}
	distribution = std::uniform_real_distribution<float>(0.0f, maxFitness-1.f);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	float RandomFitness = distribution(generator);
	
	std::shared_ptr<Bird> SelectedBird;

	for (auto& b : SavedBirdPop)
	{
		if (b->getFitnes() > RandomFitness)
		{
			SelectedBird = b;
		}

	}
	if (SelectedBird == nullptr)
	{
		std::cout << "Nope" << std::endl;
	}
	return SelectedBird;

}

void BirdPopulation::AddToPopulation(std::shared_ptr<Bird> bird)
{
	float minFit{ 999999 };
	for (auto& b : SavedBirdPop)
	{
		if (b->getFitnes() < minFit && b->getFitnes()!= 0.f)
		{
			minFit = b->getFitnes();
		}
	}

	for (auto& b : CurrentBirdPop)
	{
		if (b->getFitnes() == minFit)
		{
			bird->GetBirdShape().setFillColor(sf::Color(255, 0, 0, 150));
			b = bird;

			return;
		}
	}
}

void BirdPopulation::TrainGeneration(int iterations)
{
	for (int i = 0; i < iterations;)
	{
		if (GetBirdPop().size() == 0)
		{
			float maxFitness{ 0 };
			for (auto& b : SavedBirdPop)
			{
				if (b->getFitnes() > maxFitness)
				{
					maxFitness = b->getFitnes();
				}
			}
			std::cout << "Max Fitness :" << maxFitness << std::endl;
			CreateNewGeneration();
			ResetPipes();
			i++;
		}


		//Update Game
		auto closest = Controller.getClosestPipe(*GetBirdPop().front());
		Think(closest, 1280, 720);

		UpdateBirds();
		ConstrainBirds(720);

		RemoveDeadBirds();
	}
}

void BirdPopulation::CreateNewGeneration()
{

	distribution = std::uniform_real_distribution<float>(0.0f, 720.f);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	float RandomHeight = distribution(generator);

	for (auto & b : SavedBirdPop)
	{
		RandomHeight = distribution(generator);
		b->Position.y = RandomHeight;
		b->Acceleration = sf::Vector2f(0, 0);
		b->Velocity = sf::Vector2f(0, 0);
		b->Alive = true;
	}

	if (SavedBirdPop.size() > 0)
	{
		std::vector <std::shared_ptr<Bird>> NewBirds;
		float numOfReproduction = 10;
		for (int i = 0; i < numOfReproduction; i++)
		{	
			std::vector<float> newDNA;

			auto b1 = GetParent();

			Crossover(b1, newDNA);

			std::shared_ptr<Bird> newBird(new Bird(b1->Position));
			newBird->brain->setWeightsFromDNA(newDNA);
			NewBirds.push_back(newBird);
		}

		auto iterator = SavedBirdPop.begin();
		while (SavedBirdPop.size() > CurrentBirdPop.size())
		{
			CurrentBirdPop.push_back(*iterator++);
		}
		for (auto & b : NewBirds)
		{
			AddToPopulation(b);
		}
		for (auto& b : CurrentBirdPop)
		{
			b->fitness = 0.f;
		}
		SavedBirdPop.clear();
	}
}

void BirdPopulation::addToSavedBirds(std::shared_ptr<Bird> DeadBird)
{

}

void BirdPopulation::UpdateBirds()
{
	Controller.Update();
	if (CurrentBirdPop.size() > 0)
	{
		for (auto & b : CurrentBirdPop)
		{
			b->UpdateBird();
			b->setFitnes(b->getFitnes() + 1);
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
		
			Controller.CollideWithBird(*p);
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
			inputs.push_back(b->GetBirdShape().getPosition().y /screenH);
			inputs.push_back(b->GetVelocity().y);
			inputs.push_back(p.GetPipeTop().getPosition().x /screenW );
			inputs.push_back(p.GetPipeTop().getPosition().y + p.GetPipeTop().getSize().y / screenH);
			inputs.push_back(p.GetPipeBottom().getPosition().y/screenH);

			std::vector<float> outputs = b->getBrain()->feedForward(inputs);
			if (outputs[0] > 0.5f)
			{
				b->Jump();
			}
		}
	}
}

void BirdPopulation::ResetPipes()
{
	Controller.GetPipes().clear();
	Controller.resetTimer();
	Controller.AddPipe();
	//Controller.AddPipe();
}

BirdPopulation::~BirdPopulation()
{

}

void UpdateBirds(double DeltaTime)
{
}
