#pragma once
#include <SFML/Graphics.hpp>
#include "NeuralNetwork.h"
#include "Pipe.h"
#include <memory>

class Bird
{
	friend class BirdPopulation;
	//CONSTRUCTOR
public:
	Bird();
	Bird(sf::Vector2<float> position);
	Bird(Bird& other);
	Bird(Bird&& other);


	//VARIABLES
protected:
	sf::Vector2f Velocity;
	sf::Vector2f Acceleration;
	sf::Vector2f Position;
	float Lift;
	sf::CircleShape BirdShape;
	std::unique_ptr<NeuralNetwork> brain;
	bool canJump;
	bool Alive;

	std::vector<float> DNA;
	float fitness;
	float fitnessNormalized;

	//GETTERS AND SETTERS
public:
	inline float& getLift() { return Lift; }

	inline std::unique_ptr<NeuralNetwork>& getBrain() { return brain; }

	inline bool & getAlive() { return Alive; }

	inline float getFitnes() { return fitness; }
	void setFitnes(float addition) { fitness = addition; }
	inline float getFitnesNormalized() { return fitnessNormalized; }

	inline sf::Vector2f & GetVelocity() { return Velocity; }
	inline void SetVelocity(sf::Vector2f velocity) { Velocity = velocity; }

	inline sf::Vector2f & GetPostion() { return Position; }
	inline void SetPosition(sf::Vector2f position) { Position = position; }

	inline sf::Vector2f GetAcceleration() { return Acceleration; }
	inline void SetAcceleration(sf::Vector2f acceleration) { Acceleration = acceleration; }
	inline void AddToAcceleration(sf::Vector2f acceleration) { Acceleration = Acceleration + acceleration; }

	inline sf::CircleShape & GetBirdShape() { return BirdShape; }

	//FUNCTIONS
	void UpdateBird(double DeltaTime);
	void ConstrainBird(float screenHeight);
	void Think(Pipe &p, float screenW, float screenH);
	void Jump();
	void CalculateFitnessNormalized(float maxFitness);

public:
	~Bird();
};

