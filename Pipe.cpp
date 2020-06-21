#include "Pipe.h"
#include <iostream>

Pipe::Pipe(float sceenWidth, float screenHeight, float gapMod):Pipe(0,0, sceenWidth,screenHeight, gapMod)
{
}

Pipe::Pipe(float w, float gap, float sceenWidth, float screenHeight, float gapMod):
	Width(w), Gap(gap), GapModifier(gapMod)
{
	PipeTop.setSize(sf::Vector2f(Width, screenHeight / (4.f-GapModifier)));
	PipeTop.setFillColor(sf::Color(255, 255, 255, 150));
	PipeTop.setPosition(sf::Vector2f(sceenWidth + (Width* 1), 0));

	PipeBottom.setSize(sf::Vector2f(Width, (screenHeight -PipeTop.getSize().y) - Gap ));
	PipeBottom.setFillColor(sf::Color(255, 255, 255, 150));
	PipeBottom.setPosition(sf::Vector2f(sceenWidth + (Width * 1), screenHeight-PipeBottom.getSize().y));
	
	ShouldDelete = false;

}

void Pipe::UpdatePipe(float DeltaTime,float Speed)
{
	PipeTop.setPosition(sf::Vector2f(PipeTop.getPosition().x - DeltaTime * Speed, PipeTop.getPosition().y ));
	PipeBottom.setPosition(sf::Vector2f(PipeBottom.getPosition().x - DeltaTime * Speed, PipeBottom.getPosition().y));
	
	if (PipeTop.getPosition().x + PipeTop.getSize().x < 0.0f)
	{
		ShouldDelete = true;
	}
}

void Pipe::CollideWithBird(Bird& bird)
{
	float pipeTopY = PipeTop.getPosition().y + PipeTop.getSize().y;
	float pipeTopX = PipeTop.getPosition().x ;
	float pipeTopXMax = PipeTop.getPosition().x + PipeTop.getSize().x;

	float PipeBottomY = PipeBottom.getPosition().y;

	float birdPosX = bird.GetBirdShape().getPosition().x;
	float birdPosY = bird.GetBirdShape().getPosition().y;

	//if the bird and pipe have a similar X
	if (pipeTopX <= birdPosX + 50 && pipeTopXMax >= birdPosX )
	{
		
		if (pipeTopY >= birdPosY || PipeBottomY <= birdPosY+50)
		{
			std::cout << "HIT!" << std::endl;
		}
	}
	

}

Pipe::~Pipe()
{
}