#pragma once
#include "Bird.h"
#include <list>
#include <memory>
#include "PipeController.h"

class BirdPopulation
{
public:
	BirdPopulation(int numOfBirds);
private:
	std::list<std::shared_ptr<Bird>> CurrentBirdPop;
	std::list<std::shared_ptr<Bird>> SavedBirdPop;

	void Crossover(std::shared_ptr<Bird>& bird1, std::vector<float>& DNA);
	std::shared_ptr<Bird> & GetParent();
	void AddToPopulation(std::shared_ptr<Bird> bird);

	std::uniform_real_distribution<float> distribution;
	std::uniform_int_distribution<int> intDistribution;

	PipeController Controller;

public:
	PipeController& getController() { return Controller; }
	inline std::list<std::shared_ptr<Bird>>& GetBirdPop() { return CurrentBirdPop; }

	void UpdateBirds();
	void ConstrainBirds(float screenHeight);
	void Think(Pipe& p, float screenW, float screenH);
	void RemoveDeadBirds();

	void addToSavedBirds(std::shared_ptr<Bird> DeadBird);

	void TrainGeneration(int iterations);
	void CreateNewGeneration();

	void ResetPipes();


	~BirdPopulation();
};

