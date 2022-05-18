#include "HUD.h"


void PlayerFace::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

    // apply the texture
    states.texture = texture;

    // you may also override states.shader or states.blendMode if you want

    // draw the vertex array
    target.draw(vertices, states);
}

PlayerFace::PlayerFace()
{
}

PlayerFace::PlayerFace(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale)
{
    this->texture = texture;

    this->vertices = sf::VertexArray(sf::Quads, 4);

    float textSizeX = static_cast<float>(this->texture->getSize().x);
    float textSizeY = static_cast<float>(this->texture->getSize().y);

    // Defines it as a rectangle, located at (0, 0) and with size (texture.size.x*scaleX)x(texture.size.y*scaleY)
    this->vertices[0].position = sf::Vector2f(0, 0);
    this->vertices[1].position = sf::Vector2f(textSizeX * scale.x, 0);
    this->vertices[2].position = sf::Vector2f(textSizeX * scale.x, textSizeY * scale.y);
    this->vertices[3].position = sf::Vector2f(0, textSizeY * scale.y);

    this->setPosition(position);

    // define its texture area to be a (texture.size.x)x(texture.size.y) rectangle starting at (0, 0)
    this->vertices[0].texCoords = sf::Vector2f(0, 0);
    this->vertices[1].texCoords = sf::Vector2f(textSizeX, 0);
    this->vertices[2].texCoords = sf::Vector2f(textSizeX, textSizeY);
    this->vertices[3].texCoords = sf::Vector2f(0, textSizeY);
}



HUD::HUD()
{
    
    // Use pointer
}

HUD::HUD(sf::RenderWindow* window, sf::Font& fontConnectionII)
{
    this->window = window;

    // 1280.f x 720.f
    //as long as we're dealing with a window use getDefaultView
    float scale = (this->window->getDefaultView().getSize().x + this->window->getDefaultView().getSize().y) / 2000.f;
    float scaleX = this->window->getDefaultView().getSize().x / 1280.f;
    float scaleY = this->window->getDefaultView().getSize().y / 720.f;
    sf::Vector2f defSize = this->window->getDefaultView().getSize();

    //set size to view (window is the window, get default view is the size you get when you open the window, get size searches the current size of the window, scale is the defaultview().getsize/ by 1280 or 720 and finally that number is multiplied by .25 

    this->view = this->window->getDefaultView();
    this->view.setViewport(sf::FloatRect(0.25f, 0.25, 0.5f, 0.5f));

    this->vertices = sf::VertexArray(sf::Quads, 4);
    this->vertices[0].position = sf::Vector2f(0.f, 0.f);
    this->vertices[1].position = sf::Vector2f(1280.f * scaleX, 0.f);
    this->vertices[2].position = sf::Vector2f(1280.f * scaleX, 720.f * scaleY);
    this->vertices[3].position = sf::Vector2f(0.f, 720.f * scaleY);


    this->vertices[0].color = sf::Color::White;
    this->vertices[1].color = sf::Color::White;
    this->vertices[2].color = sf::Color::White;
    this->vertices[3].color = sf::Color::White;

    this->view.setViewport(sf::FloatRect(.6f, .5f, .4f, .5f));


    this->texturePlayer = new sf::Texture;
    this->texturePlayer->loadFromFile("Assets/SPRITES/CHARCTER.png");

    sf::Vector2f position = sf::Vector2f(1280.f * scaleX / 2.f - this->texturePlayer->getSize().x * scaleX / 2.f, 720.f * scaleY / 2.f - this->texturePlayer->getSize().y * scaleY / 2.f);
    
    this->playerFace = PlayerFace(this->texturePlayer, position, sf::Vector2f(scaleX, scaleY));

    // BRIAN:: SET THE POSITIONS OF THE NAMES FIRST, AND MAKE THE STATS RELATIVE OT THE NAME
    // TEXT ME ON DISCORD FOR MORE
    this->textNames[0].setFont(fontConnectionII);
    this->textNames[0].setCharacterSize(50.f * scale);
    this->textNames[0].setString("0");
    this->textNames[0].setFillColor(sf::Color::Red);

    this->textStats[0] = this->textNames[0];

    for (int i = 1; i < 6; i++)
    {
        this->textNames[i] = this->textNames[0];
        this->textStats[i] = this->textNames[0];
    }


    this->textNames[0].setString("Intelligence");
    this->textNames[0].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textNames[0].getGlobalBounds().width / 2.f, defSize.y * 1.f / 6.f - this->textNames[0].getGlobalBounds().height));
    this->textNames[1].setString("Joy");
    this->textNames[1].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textNames[1].getGlobalBounds().width / 2.f, defSize.y * 3.f / 6.f - this->textNames[1].getGlobalBounds().height));
    this->textNames[2].setString("Sobriety");
    this->textNames[2].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textNames[2].getGlobalBounds().width / 2.f, defSize.y * 5.f / 6.f - this->textNames[2].getGlobalBounds().height));
    this->textNames[3].setString("GPA");
    this->textNames[3].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textNames[3].getGlobalBounds().width / 2.f, defSize.y * 1.f / 6.f - this->textNames[3].getGlobalBounds().height));
    this->textNames[4].setString("Sadness");
    this->textNames[4].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textNames[4].getGlobalBounds().width / 2.f, defSize.y * 3.f / 6.f - this->textNames[4].getGlobalBounds().height));
    this->textNames[5].setString("Fatigue");
    this->textNames[5].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textNames[5].getGlobalBounds().width / 2.f, defSize.y * 5.f / 6.f - this->textNames[5].getGlobalBounds().height));

    this->textStats[0].setString("0");
    this->textStats[0].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textStats[0].getGlobalBounds().width / 2.f, defSize.y * 1.f / 6.f + this->textStats[0].getGlobalBounds().height));
    this->textStats[1].setString("0");
    this->textStats[1].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textStats[1].getGlobalBounds().width / 2.f, defSize.y * 3.f / 6.f + this->textStats[1].getGlobalBounds().height));
    this->textStats[2].setString("0");
    this->textStats[2].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textStats[2].getGlobalBounds().width / 2.f, defSize.y * 5.f / 6.f + this->textStats[2].getGlobalBounds().height));
    this->textStats[3].setString("0");
    this->textStats[3].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textStats[3].getGlobalBounds().width / 2.f, defSize.y * 1.f / 6.f + this->textStats[3].getGlobalBounds().height));
    this->textStats[4].setString("0");
    this->textStats[4].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textStats[4].getGlobalBounds().width / 2.f, defSize.y * 3.f / 6.f + this->textStats[4].getGlobalBounds().height));
    this->textStats[5].setString("0");
    this->textStats[5].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textStats[5].getGlobalBounds().width / 2.f, defSize.y * 5.f / 6.f + this->textStats[5].getGlobalBounds().height));

}

HUD::~HUD()
{
}

void HUD::update(const float& dt, std::string stats[5])
{
    sf::Vector2f defSize = this->window->getDefaultView().getSize();

    for (int i = 0; i < 6; i++)
    {
        this->textStats[i].setString(stats[i]);
    }

    this->textStats[0].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textStats[0].getGlobalBounds().width / 2.f, defSize.y * 1.f / 6.f + this->textStats[0].getGlobalBounds().height));
    this->textStats[1].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textStats[1].getGlobalBounds().width / 2.f, defSize.y * 3.f / 6.f + this->textStats[1].getGlobalBounds().height));
    this->textStats[2].setPosition(sf::Vector2f(defSize.x * 1.f / 8.f - this->textStats[2].getGlobalBounds().width / 2.f, defSize.y * 5.f / 6.f + this->textStats[2].getGlobalBounds().height));
    this->textStats[3].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textStats[3].getGlobalBounds().width / 2.f, defSize.y * 1.f / 6.f + this->textStats[3].getGlobalBounds().height));
    this->textStats[4].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textStats[4].getGlobalBounds().width / 2.f, defSize.y * 3.f / 6.f + this->textStats[4].getGlobalBounds().height));
    this->textStats[5].setPosition(sf::Vector2f(defSize.x * 7.f / 8.f - this->textStats[5].getGlobalBounds().width / 2.f, defSize.y * 5.f / 6.f + this->textStats[5].getGlobalBounds().height));
}

void HUD::render(sf::RenderTarget* target)
{
    target->setView(view);
    target->draw(this->vertices);

    for (int i = 0; i < 6; i++) 
    {
        target->draw(this->textNames[i]);
    }

    for (int i = 0; i < 6; i++) 
    {
        target->draw(this->textStats[i]);
    }

    target->draw(this->playerFace);

    target->setView(this->window->getDefaultView());
}
