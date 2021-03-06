#ifndef PLAYER_H
#define PLAYER_H


#include "Entity.h"
#include "EntityComponents.h"


class Player : public Entity
{
private:
	Toxicology toxic;
	Psychology psych;
	Wallet wallet;

	Demographic demographic;

	std::map<std::string, int>* keyBinds;
	std::map<std::string, bool>* keyBindPressed;
public:
	Player(std::map<std::string, int>* keyBinds, std::map<std::string, bool>* keyBindPressed);
	Player(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, std::map<std::string, int>* keyBinds, std::map<std::string, bool>* keyBindPressed);
	virtual ~Player();

	void setDemographic(Demographic demographic);

	Toxicology& getToxicology();
	Psychology& getPsychology();
	Wallet& getWallet();

	//===Update===//
	void updateMovement(const float& dt) override;
	void updateCollision(const sf::FloatRect& bounds) override;
	void update(const float& dt) override;
	//---Update---//
};


#endif
