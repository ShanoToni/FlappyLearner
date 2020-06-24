#pragma once
#include "Bird.h"
#include <list>
#include <memory>

class BirdPopulation
{
public:
	BirdPopulation(int numOfBirds);
private:
	std::list<std::shared_ptr<Bird>> CurrentBirdPop;
	std::list<std::shared_ptr<Bird>> SavedBirdPop;

	void Crossover(std::shared_ptr<Bird> &bird1, std::shared_ptr<Bird>& bird2);
	//std::unique_ptr<Bird> & GetParent();
	void AddToPopulation(std::shared_ptr<Bird> bird);

	std::uniform_real_distribution<float> distribution;

public:
	inline std::list<std::shared_ptr<Bird>>& GetBirdPop() { return CurrentBirdPop; }
	void TrainGeneration();
	void addToSavedBirds(std::shared_ptr<Bird> DeadBird);
	void UpdateBirds(double DeltaTime);
	void RemoveDeadBirds();
	void ConstrainBirds(float screenHeight);
	void Think(Pipe& p, float screenW, float screenH);


	~BirdPopulation();
};

