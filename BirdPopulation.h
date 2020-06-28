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

	std::uniform_real_distribution<float> distributionF;
	std::uniform_int_distribution<int> distributionI;
	std::default_random_engine generator;
	
	std::shared_ptr<PipeController> Controller;

	void Crossover(Bird& bird1, Bird& bird2, std::vector<float>& DNA);
	void Mutate(std::vector<float>& DNA);
	std::shared_ptr<Bird> & GetParent();
	void AddToPopulation(std::shared_ptr<Bird> bird);
	float getRandf(float min, float max);
	int getRandi(int min, int max);


public:
	inline std::shared_ptr<PipeController>& getPipes() { return Controller; }
	inline std::list<std::shared_ptr<Bird>>& GetBirdPop() { return CurrentBirdPop; }
	void TrainGeneration(int iterations);
	void addToSavedBirds(std::shared_ptr<Bird> DeadBird);
	void UpdateBirds();
	void RemoveDeadBirds();
	void ConstrainBirds(float screenHeight);
	void Think(Pipe& p, float screenW, float screenH);
	void CreateNewGeneration();

	~BirdPopulation();
};

