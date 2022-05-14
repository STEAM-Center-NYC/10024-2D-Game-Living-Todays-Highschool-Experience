﻿#include "GameState.h"


Date::Date()
{
	this->day = Day::MONDAY;
	this->hour = 6;
	this->minute = sf::seconds(0.f);
}

Date::~Date()
{
}

void Date::update(const float& dt)
{
	minute += sf::seconds(dt);

	// 1 Game Hour ≈ 2 Real Minutes
	// 1 Day ≈ 48 Real Minutes
	// If the minutes is divisible by 120, then an hour has passed 
	if (static_cast<int>(minute.asSeconds()) >= 120)
	{
		while (static_cast<int>(minute.asSeconds()) >= 120)
		{
			this->hour++;
			this->minute -= sf::seconds(120.f);
		}
	}

	if (this->hour >= 24)
	{
		this->hour -= 24;
		switch (this->day)
		{
		case Day::SATURDAY:
			this->day = Day::SUNDAY;
			break;
		default:
			this->day = static_cast<Day>(static_cast<int>(this->day) + 1);
			break;
		}
	}
}

const Day& Date::getDay() const
{
	return day;
}

const int& Date::getHour() const
{
	return hour;
}

const int Date::getMinute() const
{
	return static_cast<int>(minute.asSeconds() / 2.f);
}

const float Date::getTrueMinute() const
{
	return minute.asSeconds();
}

const std::string Date::getDayAsString() const
{
	switch (this->day)
	{
	case Day::SUNDAY:
		return "SUNDAY";
	case Day::MONDAY:
		return "MONDAY";
	case Day::TUESDAY:
		return "TUESDAY";
	case Day::WEDNESDAY:
		return "WEDNESDAY";
	case Day::THURSDAY:
		return "THURSDAY";
	case Day::FRIDAY:
		return "FRIDAY";
	case Day::SATURDAY:
		return "SATURDAY";
	default:
		return "NULL";
	}
}

const std::string Date::getHourAsString() const
{
	return std::to_string(this->hour);
}

const std::string Date::getMinuteAsString() const
{
	return std::to_string(this->getMinute());
}

void Date::setDay(Day& day)
{
	this->day = day;
}

void Date::setHour(int& hour)
{
	this->hour = hour;
}

void Date::setMinute(int& minute)
{
	this->minute = sf::seconds(static_cast<float>(minute * 2));
}

void Date::setTrueMinute(float& minute)
{
	this->minute = sf::seconds(minute);
}

//===INITIALIZE FUNCTIONS===//
void GameState::initTextures()
{
	this->texturePlayer = new sf::Texture;
	this->texturePlayer->loadFromFile("Assets/Tiles/Sprite-0003.png");

	this->textureBed = new sf::Texture;
	this->textureBed->loadFromFile("Assets/Tiles/Sprite-0004.png");
}

void GameState::initBackground()
{
}

void GameState::initBools()
{
	this->booleansPlayerFunctions.insert({"Sleep", new bool});
	this->booleansPlayerFunctions.insert({"Wake", new bool});
	this->booleansPlayerFunctions.insert({"High", new bool});
	this->booleansPlayerFunctions.insert({"Study", new bool});
}

void GameState::initButtons()
{
	this->buttonsHidden.insert({"Sleep", new Button()});
}

void GameState::initViews()
{
	this->viewWorld = window->getDefaultView();

	sf::Vector2f scale;
	scale.x = window->getDefaultView().getSize().x / 1280.f;
	scale.y = window->getDefaultView().getSize().y / 720.f;

	this->viewWorld.setViewport(sf::FloatRect(0, 0, .6f, 1.f));
}

//---INITIALIZE FUNCTIONS---//
GameState::GameState(sf::RenderWindow* window, sf::Vector2i* mosPosWindow, sf::Vector2f* mosPosView, std::map<std::string, int>* keyBinds, std::map<std::string, bool>* keyBindPressed, std::map<std::string, bool*> booleans) : State(window, mosPosWindow, mosPosView, keyBinds, keyBindPressed, booleans)
{
	this->initTextures();
	this->initBackground();
	this->initBools();
	this->initButtons();
	this->initViews();

	sf::Vector2f scale;
	scale.x = this->window->getDefaultView().getSize().x / 1280.f;
	scale.y = this->window->getDefaultView().getSize().y / 720.f;

	sf::Vector2f viewWorldScale;
	viewWorldScale.x = scale.x / this->viewWorld.getViewport().width;
	viewWorldScale.y = scale.y / this->viewWorld.getViewport().height;

	//===Init Boolean (Maps)===//
	this->pause = false;
	this->gameOver = false;

	this->booleansPause.insert({ "PauseGameState", &this->pause });
	this->booleansPause.insert({ "QuitGameState", &this->quit });
	//---Init Boolean (Maps)---//

	//===Init Timers===//
	this->cooldownPauseCreationMax = sf::seconds(0.5f);
	this->cooldownPauseCreation = this->cooldownPauseCreationMax;
	//---Init Timers---//

	// define the level with an array of tile indices
	const int level[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	// create the tilemap from the level definition
	if (!this->map.load("Assets/Tiles/floorTileSheet.png", sf::Vector2u(64, 64), level, 16, 8, viewWorldScale))
		std::cout << "IT AINT LOAD" << std::endl;

	this->map.setPosition(sf::Vector2f(this->window->getView().getSize().x / 2.f - this->map.getGlobalBounds().width / 2.f, this->window->getView().getSize().y / 2.f - this->map.getGlobalBounds().height / 2.f));

	this->player = new Player(this->texturePlayer, sf::Vector2f(this->map.getGlobalBounds().left + this->map.getGlobalBounds().width / 2.f, this->map.getGlobalBounds().top + this->map.getGlobalBounds().height / 2.f), viewWorldScale, this->keyBinds, this->keyBindPressed);
	this->player->setPosition(sf::Vector2f(this->player->getGlobalBounds().left - this->player->getGlobalBounds().width / 2.f, this->player->getGlobalBounds().top - this->player->getGlobalBounds().height / 2.f));

	this->bed = WorldItem(this->textureBed, sf::Vector2f(this->map.getGlobalBounds().left + 0.f, this->map.getGlobalBounds().top + this->map.getGlobalBounds().height), viewWorldScale);
	this->bed.setPosition(sf::Vector2f(this->bed.getGlobalBounds().left, this->bed.getGlobalBounds().top - this->bed.getGlobalBounds().height));
	this->bed.getCollisionButton().button = this->buttonsHidden["Sleep"];
	this->buttonsHidden["Sleep"]->setPosition(sf::Vector2f(this->bed.getGlobalBounds().left + this->bed.getGlobalBounds().width / 2.f + 32.f, this->bed.getGlobalBounds().top - 64.f));
	this->buttonsHidden["Sleep"]->setBool(this->booleansPlayerFunctions["Sleep"], true);

}

GameState::~GameState()
{
	while (!this->stateStack.empty())
	{
		delete this->stateStack.top();
		this->stateStack.pop();
	}
}

void GameState::endState()
{
	this->window->setView(this->window->getDefaultView());
}

void GameState::confirmQuit()
{
}

void GameState::resetButton()
{
}

void GameState::updateLevel()
{
}

void GameState::updateTimers(const float& dt)
{
	this->cooldownPauseCreation += sf::seconds(dt);
}

void GameState::updateInput()
{
	if (this->keyBindPressed->at("PAUSE"))
	{
		if (this->cooldownPauseCreation >= this->cooldownPauseCreationMax)
		{
			switch (this->pause)
			{
			case false:
				this->pause = true;
				break;
			default:
				break;
			}
			this->cooldownPauseCreation = sf::seconds(0.f);
		}
	}
}

void GameState::updateDate(const float& dt)
{
	float timeBlend = 0;

	if (this->player->getToxicology().getSobriety<double>() < -20.0)
	{
		timeBlend += 5 + std::abs(this->player->getToxicology().getSobriety<float>()) / 100;
	}
	if (this->player->getPsychology().getIsStudying())
	{
		timeBlend += 2;
	}	
	if (this->player->getPsychology().getIsAsleep())
	{
		timeBlend += 15;
	}
	
	if (static_cast<int>(timeBlend) == 0)
	{
		this->date.update(dt);
	}
	else
	{
		this->date.update(dt * timeBlend);
	}
	
	//std::cout << "Day: " << this->date.getDayAsString() << " Hour: " << this->date.getHourAsString() << " Minute: " << this->date.getMinuteAsString() << std::endl;
}

void GameState::updatePlayerFunctions()
{
	switch (*this->booleansPlayerFunctions["Sleep"])
	{
	case true:
		this->player->getPsychology().sleep();
		std::cout << "Sleep On" << std::endl;
		break;
	default:
		break;
	}	

	switch (*this->booleansPlayerFunctions["Study"])
	{
	case true:
		this->player->getPsychology().study();
		break;
	default:
		break;
	}	

	switch (*this->booleansPlayerFunctions["High"])
	{
	case true:
		this->player->getToxicology().getHigh();
		break;
	default:
		break;
	}	
}

void GameState::update(const float& dt)
{
	if ((this->pause && !this->gameOver) && (this->stateStack.empty())) // Checks if the player has initiated pause AND the stack is already empty
	{
		this->stateStack.push(new PauseState(this->window, this->mosPosWindow, this->mosPosView, this->keyBinds, this->keyBindPressed, this->booleansPause));
	}

	if (!this->stateStack.empty()) // If the stack isn't empty, that state will be updated
	{
		this->stateStack.top()->update(dt);
		if (this->stateStack.top()->getQuit())
		{
			this->stateStack.top()->confirmQuit();
			if (this->stateStack.top()->getQuit()) // Done Twice because the it will first confirm that user still chooses to quit, saving etc
			{
				this->stateStack.top()->endState();
				delete this->stateStack.top();
				this->stateStack.pop();
			}
		}
		return;
	}

	this->updateTimers(dt);

	this->updateDate(dt);

	this->updateInput();

	this->player->update(dt);

	this->player->updateCollision(this->map.getGlobalBounds());

	sf::Vector2f mtv;
	satCollision(this->player->getGlobalBounds(), this->bed.getGlobalBounds(), &mtv);
	this->player->move(mtv);

	if (satCollision(this->player->getGlobalBounds(), this->bed.getCollisionButton().radius))
	{
		if (this->bed.getCollisionButton().button)
		{
			this->bed.getCollisionButton().button->update(dt, *this->mosPosView);
		}
	}

	this->viewWorld.setCenter(sf::Vector2f(this->player->getGlobalBounds().left + this->player->getGlobalBounds().width / 2.f, this->player->getGlobalBounds().top + this->player->getGlobalBounds().height / 2.f));

}

void GameState::renderViewWorld(sf::RenderTarget* target)
{
	target->setView(this->viewWorld);

	target->draw(this->map);

	target->draw(this->bed);
	target->draw(*this->player);

	if (satCollision(this->player->getGlobalBounds(), this->bed.getCollisionButton().radius))
	{
		if (this->bed.getCollisionButton().button)
		{
			target->draw(*this->bed.getCollisionButton().button);
		}
	}

	target->setView(this->window->getDefaultView());
}

void GameState::render(sf::RenderTarget* target)
{
	/*
	Render Hierarchy
		Map
		Player
	*/

	this->renderViewWorld(target);

	if (!this->stateStack.empty()) // As long as the stack is not empty, it will render the top
	{
		this->stateStack.top()->render(target);
	}
}


