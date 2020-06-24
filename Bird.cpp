#include "Bird.h"
#include <iostream>

Bird::Bird():Bird(sf::Vector2f(0.0f,0.0f))
{

}

Bird::Bird(sf::Vector2<float> position) : Position(position), Acceleration(sf::Vector2f(0, 0)), Velocity(sf::Vector2f(0, 0))
{
	BirdShape = sf::CircleShape(25.f);
	BirdShape.setFillColor(sf::Color(255, 255, 255, 50));
	Lift = 0;
	brain = std::unique_ptr<NeuralNetwork>(new NeuralNetwork(5, 8, 1));
	canJump = true;
	std::vector<float> brainDNA = brain->getWeightsAsDNA();
	DNA.insert(DNA.begin(), brainDNA.begin(), brainDNA.end());
	fitness = 0.f;
	fitnessNormalized = 0.f;
	Alive = true;
}

Bird::Bird(Bird& other)
{
	Position = other.GetPostion();
	Acceleration = other.GetAcceleration();
	Velocity = other.GetVelocity();
	BirdShape = sf::CircleShape(25.f);
	BirdShape.setFillColor(sf::Color(255, 255, 255, 50));
	Lift = 0;
	brain = std::move(other.brain);
	canJump = true;
	std::vector<float> brainDNA = brain->getWeightsAsDNA();
	DNA.insert(DNA.begin(), brainDNA.begin(), brainDNA.end());
	fitness = 0.f;
	fitnessNormalized = 0.f;
	Alive = true;
}

Bird::Bird(Bird&& other)
{
	Position = other.GetPostion();
	Acceleration = other.GetAcceleration();
	Velocity = other.GetVelocity();
	BirdShape = sf::CircleShape(25.f);
	BirdShape.setFillColor(sf::Color(255, 255, 255, 50));
	Lift = 0;
	brain = std::move(other.brain);
	canJump = true;
	std::vector<float> brainDNA = brain->getWeightsAsDNA();
	DNA.insert(DNA.begin(), brainDNA.begin(), brainDNA.end());
	fitness = 0.f;
	fitnessNormalized = 0.f;
	Alive = true;
	// TODO: mem constructor
	//this.var = other.var;
	//other.var = nullptr;
}


void Bird::UpdateBird(double DeltaTime)
{
	Velocity.x = Velocity.x + Acceleration.x * DeltaTime;
	Velocity.y = Velocity.y + Acceleration.y * DeltaTime;
	Velocity.y = Velocity.y + Lift;
	Velocity = Velocity * 0.99999f;

	Position.x = Position.x + Velocity.x * DeltaTime;
	Position.y = Position.y + Velocity.y * DeltaTime;

	BirdShape.setPosition(Position);

	Acceleration = sf::Vector2f(0.0f,600.f);
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
		Lift = -500;
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
