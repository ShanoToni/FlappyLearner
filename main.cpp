#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bird.h"
#include "PipeController.h"

const int width = 1280;
const int height = 720;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Flapper");

    Bird a = Bird(sf::Vector2f(200, (height / 2)));
    PipeController Controller = PipeController(width, height, 2.f);

    sf::Clock clock;
    const double dt = 0.005f;
    double accumulator = 0.0f;

    double currentTime = static_cast<double>(clock.getElapsedTime().asSeconds());
    while (window.isOpen())
    {
        //handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    a.Jump();
                }
            }
        }

        double newTime = clock.getElapsedTime().asSeconds();
        double frameTime = newTime - currentTime;
        //speed control
        frameTime *= 1.0;
        currentTime = newTime;
        accumulator += frameTime;
        //Update Game
        while (accumulator >= dt)
        {
            a.ConstrainBird(height);
            //movement updates and all 
            a.UpdateBird(dt);

            Controller.Update(dt);

            accumulator -= dt;
        }
        Controller.CollideWithBird(a);
        window.clear();
        window.draw(a.GetBirdShape());
        for (auto& p : Controller.GetPipes())
        {
            window.draw(p->GetPipeTop());
            window.draw(p->GetPipeBottom());
        }
        window.display();
    }

    return 0;
}