#ifndef MENU
#define MENU


#include "../Physics.h"


enum class MenuState{MAIN=0};


class Menu
{
private:
    MenuState state;

    sf::FloatRect bounds;

    sf::VertexArray background;
    sf::VertexArray border;

    sf::View view;
    sf::RenderWindow* window;
public:
    Menu();
    Menu(sf::RenderWindow* window, sf::Font& fontConnectionII);
    virtual ~Menu();

    void update(const float& dt);
    void render(sf::RenderTarget* target);
};


#endif

