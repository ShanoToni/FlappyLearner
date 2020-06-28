#include "BirdPopulation.h"
#include <chrono>
#include <iostream>

BirdPopulation::BirdPopulation(int numOfBirds)
{
	for (int i = 0; i < numOfBirds; i++)
	{
		std::shared_ptr<Bird> newBird(new Bird());
		CurrentBirdPop.push_back(std::move(newBird));
	}

	Controller = std::shared_ptr<PipeController>(new PipeController(1280, 720, 2));
}

void BirdPopulation::Crossover(Bird & bird1, Bird & bird2,  std::vector<float>& DNA)
{
	//random recombination
	for (int i = 0; i < bird1.DNA.size(); i++)
	{
		float crossoverChance = getRandf(0.f, 1.f);
		if (crossoverChance < 0.5f)
		{
			DNA.push_back(bird1.DNA[i]);
		}
		else
		{
			DNA.push_back(bird2.DNA[i]);
		}
	}
}

void BirdPopulation::Mutate(std::vector<float>& DNA)
{
	for (int i = 0; i < DNA.size(); i++)
	{
		float mutationChance = getRandf(0.f, 1.f);
		if (mutationChance < 0.1)
		{
			float mutationRate = getRandf(-0.1f, 0.1f);
			DNA[i] = DNA[i] + mutationRate;

		}
	}
}

std::shared_ptr<Bird> & BirdPopulation::GetParent()
{
	float selectionBias = getRandf(0.f, 1.f);
	for (auto& b : SavedBirdPop)
	{
		selectionBias = selectionBias - b->getFitnesNormalized();
		if (selectionBias < 0)
		{
			return b;
		}
	}
}

void BirdPopulation::AddToPopulation(std::shared_ptr<Bird> bird)
{
	float minFitness{ 1 };
	
	for (auto& b : CurrentBirdPop)
	{
		if (b->getFitnesNormalized() < minFitness && b->getFitnesNormalized() > 0)
		{
			minFitness = b->getFitnesNormalized();
		}
	}

	for (auto& b : CurrentBirdPop)
	{
		if (b->getFitnesNormalized() == minFitness)
		{
			b->Copy(*bird);
			b->GetBirdShape().setFillColor(sf::Color(255, 0, 0, 50));
			return;
		}
	}

}

float BirdPopulation::getRandf(float min, float max)
{
	distributionF = std::uniform_real_distribution<float>(min, max);
	return distributionF(generator);
}

int BirdPopulation::getRandi(int min, int max)
{
	distributionI = std::uniform_int_distribution<int>(min, max);
	return distributionF(generator);
}

void BirdPopulation::TrainGeneration(int iterations)
{
	for (int i = 0; i < iterations;)
	{
		if (CurrentBirdPop.size() == 0)
		{
			float maxFit{ 0 };
			for (auto& b : SavedBirdPop)
			{
				if (maxFit < b->getFitnes())
				{
					maxFit = b->getFitnes();
				}
			}
			std::cout << "Max Fitness : " << maxFit << std::endl;
			++i;
			CreateNewGeneration();
			Controller->ResetPipes();
		}
		UpdateBirds();
		ConstrainBirds(720);

		auto closest = Controller->getClosestPipe(*GetBirdPop().front());
		Think(closest, 1280, 720);

		RemoveDeadBirds();
	}
}

void BirdPopulation::addToSavedBirds(std::shared_ptr<Bird> DeadBird)
{

}

void BirdPopulation::UpdateBirds()
{
	Controller->Update();

	if (CurrentBirdPop.size() > 0)
	{
		for (auto & b : CurrentBirdPop)
		{
			b->setFitnes(b->getFitnes() + 1);
			b->UpdateBird();
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
			Controller->CollideWithBird(*p);

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
			inputs.push_back(b->GetVelocity().y / 10);
			inputs.push_back((p.GetPipeTop().getSize().y) / screenH);
			inputs.push_back(p.GetPipeBottom().getPosition().y / screenH);
			inputs.push_back(p.GetPipeTop().getPosition().x / screenW);

			std::vector<float> outputs = b->getBrain()->feedForward(inputs);
			if (outputs[0] > outputs[1])
			{
				b->Jump();
			}
		}
	}
}

void BirdPopulation::CreateNewGeneration()
{
	if (SavedBirdPop.size() > 0)
	{
		//format the fitnes for eval
		float fitnessSum{ 0 };

		for (auto& b : SavedBirdPop)
		{
			fitnessSum = fitnessSum + b->getFitnes();
		}

		for (auto& b : SavedBirdPop)
		{
			b->CalculateFitnessNormalized(fitnessSum);
		}

		//create children
		int numReproductions = SavedBirdPop.size();
		std::vector<std::shared_ptr<Bird>> newBirds;

		for (int i = 0; i < numReproductions; i++)
		{
			Bird b1 = Bird() ;
			b1.Copy(*GetParent());
			Bird b2 = Bird();
			b2.Copy(*GetParent());
			std::vector<float> newDna = b1.DNA;

			//Crossover(b1, b2, newDna);
			Mutate(newDna);

			std::shared_ptr<Bird> child(new Bird());
			child->DNA = newDna;
			child->brain->setWeightsFromDNA(newDna);
			newBirds.push_back(child);
		}

		//create a new population from old birds
		for (auto& b : SavedBirdPop)
		{
			std::shared_ptr<Bird> newBird(new Bird());
			newBird->Copy(*b);
			newBird->setFitnesNormalized(b->getFitnesNormalized());
			CurrentBirdPop.push_back(newBird);
		}

		//add the new bird to the population
		for (auto& b : newBirds)
		{
			AddToPopulation(b);
		}
		
		Controller->ResetPipes();
		SavedBirdPop.clear();
		for (auto& b : CurrentBirdPop)
		{
			b->setFitnes(0.f);
			b->setFitnesNormalized(0.f);
		}
	}
}

BirdPopulation::~BirdPopulation()
{

}


