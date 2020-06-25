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

    Bird a = Bird(sf::Vector2f(200, (height / 2)));
    BirdPopulation pop = BirdPopulation(350);

    PipeController Controller = PipeController(width, height, 2.f);
    
 
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
      /*          for (auto b : pop.GetBirdPop())
                {
                    b->Jump();
                }*/
                if (event.key.code == sf::Keyboard::Space)
                {
                    a.Jump();
                }
            }
        }

        pop.ConstrainBirds(height);
        pop.UpdateBirds();
        //Update Game
        a.ConstrainBird(height);
        a.UpdateBird();
        Controller.Update();

        
        //pop.RemoveDeadBirds();
        
    
        Pipe closest = Controller.getClosestPipe(a);
        pop.Think(closest, width, height);
        
        Controller.CollideWithBird(a);
        for (auto& b : pop.GetBirdPop())
        {
            Controller.CollideWithBird(*b);
        }
        
        pop.RemoveDeadBirds();
        window.clear();
        
        window.draw(a.GetBirdShape());
        for (auto& b : pop.GetBirdPop())
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