#include "Bird.h"
#include <iostream>
#include <chrono>

Bird::Bird() :Acceleration(sf::Vector2f(0.f, 0.f)), Velocity(sf::Vector2f(0.f, 0.f))
{
	std::uniform_real_distribution<float>distribution = std::uniform_real_distribution<float>(100.0f, 620.f);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	float RandomHeight = distribution(generator);

	Position = sf::Vector2f(200.f, 702/2);
	BirdShape = sf::CircleShape(25.f);
	BirdShape.setFillColor(sf::Color(255, 255, 255, 50));
	Lift = 0;
	brain = std::shared_ptr<NeuralNetwork>(new NeuralNetwork(5, 8, 2));
	canJump = true;
	std::vector<float> brainDNA = brain->getWeightsAsDNA();
	DNA.insert(DNA.begin(), brainDNA.begin(), brainDNA.end());
	fitness = 0.f;
	fitnessNormalized = 0.f;
	Alive = true;
}

Bird::Bird(sf::Vector2<float> position) : Position(position), Acceleration(sf::Vector2f(0, 0)), Velocity(sf::Vector2f(0, 0))
{
	BirdShape = sf::CircleShape(25.f);
	BirdShape.setFillColor(sf::Color(255, 255, 255, 50));
	Lift = 0;
	brain = std::unique_ptr<NeuralNetwork>(new NeuralNetwork(5, 8, 2));
	canJump = true;
	std::vector<float> brainDNA = brain->getWeightsAsDNA();
	DNA.insert(DNA.begin(), brainDNA.begin(), brainDNA.end());
	fitness = 0.f;
	fitnessNormalized = 0.f;
	Alive = true;
}



void Bird::Copy(Bird& other)
{
	Lift = 0;
	brain = other.brain;
	canJump = true;
	std::vector<float> brainDNA = brain->getWeightsAsDNA();
	DNA.insert(DNA.begin(), brainDNA.begin(), brainDNA.end());
	fitness = 0.f;
	fitnessNormalized = 0.f;
	Alive = true;
}

void Bird::UpdateBird()
{
	Velocity.x = Velocity.x + Acceleration.x;
	Velocity.y = Velocity.y + Acceleration.y;
	Velocity.y = Velocity.y + Lift;
	Velocity = Velocity * 0.99999f;

	Position.x = Position.x + Velocity.x;
	Position.y = Position.y + Velocity.y;

	BirdShape.setPosition(Position);

	Acceleration = sf::Vector2f(0.0f, 0.2f);
	Lift = 0.f;
}

void Bird::ConstrainBird( float screenHeight)
{
	if (BirdShape.getPosition().y > screenHeight-50.f )
	{
		Acceleration = sf::Vector2f(0,0);
		Velocity = sf::Vector2f(0, 0);
		canJump = true;
		Alive = false;
		return;
	}
	else if (BirdShape.getPosition().y < 0.f)
	{
		Velocity = sf::Vector2f(0, 0);
		canJump = false;
		Alive = false;
		return;
	}
	canJump = true;
}

void Bird::Think(Pipe& p, float screenW, float screenH)
{
	std::vector<float> inputs;
	inputs.push_back(BirdShape.getPosition().y/ screenH);
	inputs.push_back(Velocity.y);
	inputs.push_back(p.GetPipeTop().getPosition().x / screenW);
	inputs.push_back(p.GetPipeTop().getPosition().y + p.GetPipeTop().getSize().y / screenH);
	inputs.push_back(p.GetPipeBottom().getPosition().y / screenH);

	std::vector<float> outputs = brain->feedForward(inputs);
	if (outputs[0] > 0.5f)
	{
		Jump();
	}
}

void Bird::Jump()
{
	if (canJump)
	{
		Lift = -9.f;
	}
}

void Bird::CalculateFitnessNormalized(float maxFitness)
{
	fitnessNormalized = fitness / maxFitness;
}



Bird::~Bird()
{
	
	brain = nullptr;
}
