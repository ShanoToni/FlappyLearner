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
}

Bird::Bird(Bird& other)
{
	//copy constructor if mem needs alocating
	//this.var = new var(*other.var);
}

Bird::Bird(Bird&& other)
{
	//move mem constructor
	//this.var = other.var;
	//other.var = nullptr;
}

Bird& Bird::operator=(const Bird& other)
{
	// TODO: move semantics if mem is allocated
	if (this != &other)
	{
		//delete this->var;
		//this->var = new var(*other.var);
	}
	return *this;
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
	if (BirdShape.getPosition().y > screenHeight-100.f )
	{
		Acceleration = sf::Vector2f(0,0);
		Velocity = sf::Vector2f(0, 0);
		

		
	}
	else if (BirdShape.getPosition().y < 0)
	{
		Velocity = sf::Vector2f(0, 0);
	}
}

void Bird::Jump()
{
	Lift = -500;
}



Bird::~Bird()
{
	//copy constructor if mem needs transfered
	//this.var = new var(*other.var);
	//other.var = nullptr;
}
