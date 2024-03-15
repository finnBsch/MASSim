//
// Created by finn on 3/11/24.
//
#include "viz.h"

Viz::Viz():
        window(),
        settings(24, 0, 0, 4, 5, sf::ContextSettings::Default, false)
{
    defaultView = sf::View(sf::Vector2f(5, 5), sf::Vector2f(10, -10));
    defaultView.setViewport(sf::FloatRect(0, 0, 1, 1));
    window.create(sf::VideoMode(1000, 1000), "PF", sf::Style::None, settings);
    window.setView(defaultView);
}

void Viz::draw(float i) {
    window.clear(sf::Color::Black);
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::KeyPressed) {
            switch (ev.key.code) {
                case sf::Keyboard::D:
                    in_ev.newInput = true;
                    in_ev.keyInput = "D";
                    break;
                case sf::Keyboard::A:
                    in_ev.newInput = true;
                    in_ev.keyInput = "A";
                    break;
                case sf::Keyboard::W:
                    in_ev.newInput = true;
                    in_ev.keyInput = "W";
                    break;
                case sf::Keyboard::S:
                    in_ev.newInput = true;
                    in_ev.keyInput = "S";
                    break;
                case sf::Keyboard::Q:
                    in_ev.newInput = true;
                    in_ev.keyInput = "Q";
                    break;
                case sf::Keyboard::E:
                    in_ev.newInput = true;
                    in_ev.keyInput = "E";
                    break;
            }
        }
    }
    for (auto drawable:drawables){
        window.draw(*drawable);
    }
    window.display();
}

void Viz::addDrawable(DrawableObject *drawable) {
    drawables.push_back(drawable);
}

InputEvent &Viz::readEvent() {
    return in_ev;
}


