#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bird.h"
#include "PipeController.h"
#include "BirdPopulation.h"

const int width = 1280;
const int height = 720;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Flapper");

    //Bird a = Bird(sf::Vector2f(200, (height / 2)));
    BirdPopulation pop = BirdPopulation(50);

    PipeController Controller = PipeController(width, height, 2.f);

    sf::Clock clock;
    const double dt = 0.0005f;
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
                for (auto b : pop.GetBirdPop())
                {
                    b->Jump();
                }
                /*if (event.key.code == sf::Keyboard::Space)
                {
                    a.Jump();
                }*/
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
            //movement updates and all 
            pop.ConstrainBirds(height);
            pop.UpdateBirds(dt);
            Controller.Update(dt);

            accumulator -= dt;
        }
        Pipe closest = Controller.getClosestPipe(*pop.GetBirdPop().front());
        pop.Think(closest, width, height);
        for (auto b : pop.GetBirdPop())
        {
            Controller.CollideWithBird(*b);
        }

        window.clear();
        for (auto b : pop.GetBirdPop())
        {
            window.draw(b->GetBirdShape());
        }
        for (auto& p : Controller.GetPipes())
        {
            window.draw(p->GetPipeTop());
            window.draw(p->GetPipeBottom());
        }

        window.display();
    }

    return 0;
}