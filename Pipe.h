#pragma once
#include<SFML/Graphics.hpp>

class Pipe
{
public:
	Pipe() = delete;
	Pipe(float sceenWidth, float screenHeight, float gapMod);
	Pipe(float w, float gap, float sceenWidth, float screenHeight, float gapMod);

	//vars
private:
	float Width;
	float Gap;
	float GapModifier;
	sf::RectangleShape PipeBottom;
	sf::RectangleShape PipeTop;
	bool ShouldDelete;

public:
	//get and sets
	inline sf::RectangleShape GetPipeTop() { return PipeTop; }
	inline sf::RectangleShape GetPipeBottom() { return PipeBottom; }
	inline float GetWidth() { return Width; }

	inline void SetPipeTop(sf::RectangleShape pipe) { PipeTop = pipe; }
	inline void SetPipeBottom(sf::RectangleShape pipe) { PipeBottom = pipe; }

	inline bool GetShouldDelete() { return ShouldDelete; }

	void UpdatePipe(float Speed);

	~Pipe();
};

