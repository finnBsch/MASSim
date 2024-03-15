//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_VIZ_H
#define MASSIM_VIZ_H
#include <SFML/Graphics.hpp>

struct InputEvent {
    bool newInput = false;
    std::string keyInput;
};

class DrawableObject: public sf::Drawable, public sf::Transformable {
private:
public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

class Viz {
private:
    InputEvent in_ev;
    sf::RenderWindow window;
    sf::ContextSettings settings;
    sf::View defaultView;
    std::vector<DrawableObject*> drawables;

public:
    void addDrawable(DrawableObject* drawable);
    Viz();
    void draw(float i);
    InputEvent& readEvent();
};

#endif //MASSIM_VIZ_H
