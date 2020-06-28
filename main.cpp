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
    window.setFramerateLimit(60);

    BirdPopulation pop = BirdPopulation(350);
    //pop.TrainGeneration(50);

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
                }
            }
        }

        //********* HANDLE POPULATION UPDATES
        for (int i = 0; i < 2; i++)
        {
            if (pop.GetBirdPop().size() == 0)
            {
                pop.CreateNewGeneration();
            }

            pop.UpdateBirds();
            pop.ConstrainBirds(height);

            auto closest = pop.getPipes()->getClosestPipe(*pop.GetBirdPop().front());
          
            pop.Think(closest, width, height);

            pop.RemoveDeadBirds();
        }


        window.clear();

        //********* DRAW HERE *********
        for (auto& b : pop.GetBirdPop())
        {
            window.draw(b->GetBirdShape());
        }
        for (auto& p : pop.getPipes()->GetPipes())
        {
            window.draw(p->GetPipeBottom());
            window.draw(p->GetPipeTop());

        }

        window.display();

     
    }

    return 0;
}