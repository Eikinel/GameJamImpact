#include <iostream>
#include "Screen.h"
#include "Button.h"


//CONSTRUCTORS
IScreen::IScreen(sf::RenderWindow& window, eGamestate state) : _window(window), _state(state)
{
	this->_index = all_screens.size();
	this->_window.setFramerateLimit(120);
}

MenuScreen::MenuScreen(sf::RenderWindow& window) : IScreen(window, MENU)
{
	std::cout << std::endl << "Creating menu screen" << std::endl;
	this->_events.push_back(new WindowDefaultEvent); // Event handler for options, close window, etc.
	this->_events.push_back(new MenuEvent); // Update menu, draw it and react in terms of user inputs.

	this->_buttons.push_back(new Button("New Game", this->_window.getSize().y / 8.f, sf::Vector2f(
		this->_window.getSize().x / 1.55f,
		this->_window.getSize().y / 4.f), RIGHT));
	this->_buttons.push_back(new Button("Options", this->_window.getSize().y / 8.f, sf::Vector2f(
		this->_window.getSize().x / 1.55f,
		this->_window.getSize().y / 2.f), RIGHT));
	this->_buttons.push_back(new Button("Exit", this->_window.getSize().y / 16.f, sf::Vector2f(
		this->_window.getSize().x / 1.55f,
		this->_window.getSize().y / 1.25f), RIGHT));
	this->_buttons.push_back(new Button("Toggle bounding boxes", this->_window.getSize().y / 25.f, sf::Vector2f(
		this->_window.getSize().x / 9.2f,
		this->_window.getSize().y - this->_window.getSize().y / 25.f), CENTER));
	this->_buttons[0]->onClick(&IEvent::changeScreen, this->_events[1], GAME, static_cast<IScreen *>(this));
	this->_buttons[1]->onClick(&IEvent::changeScreen, this->_events[1], OPTIONS, static_cast<IScreen *>(this));
	this->_buttons[2]->onClick(&IEvent::changeScreen, this->_events[1], EXIT, static_cast<IScreen *>(this));
	this->_buttons[3]->onClick(&IEvent::toggleBoundingBoxes, this->_events[1], this->_index);
}

GameScreen::GameScreen(sf::RenderWindow& window) :  IScreen(window, GAME)
{
	std::cout << std::endl << "Creating game screen" << std::endl;
	this->_events.push_back(new WindowDefaultEvent); // Event handler for options, close window, etc.
	this->_events.push_back(new GameEvent); // Update game, draw it and react in terms of user inputs.

	this->_buttons.push_back(new Button("Toggle bounding boxes", this->_window.getSize().y / 25.f, sf::Vector2f(
		this->_window.getSize().x / 9.2f,
		this->_window.getSize().y - this->_window.getSize().y / 6.f), LEFT));
	this->_buttons.push_back(new Button("Toggle grid", this->_window.getSize().y / 25.f, sf::Vector2f(
		this->_window.getSize().x / 9.2f,
		this->_window.getSize().y - this->_window.getSize().y / 10.f), LEFT));
	this->_buttons.push_back(new Button("Back to menu", this->_window.getSize().y / 25.f, sf::Vector2f(
		this->_window.getSize().x / 9.2f,
		this->_window.getSize().y - this->_window.getSize().y / 25.f), LEFT));
	this->_buttons[0]->onClick(&IEvent::toggleBoundingBoxes, this->_events[1], this->_index);
	this->_buttons[2]->onClick(&IEvent::changeScreen, this->_events[1], MENU, static_cast<IScreen *>(this));

	this->_map_texture.loadFromFile(SPRITES_DIR"/Environment/map_final.png");
	this->_map.setTexture(this->_map_texture);
	this->_map.setScale(sf::Vector2f(
		this->_window.getSize().x / this->_map.getGlobalBounds().width,
		this->_window.getSize().x / this->_map.getGlobalBounds().width));
}

IScreen::~IScreen()
{
	std::cout << "Deleting window events" << std::endl;
	for (std::vector<IEvent *>::const_iterator it = this->_events.begin(); it != this->_events.end(); ++it)
		delete(*it);
}

MenuScreen::~MenuScreen()
{
	std::cout << "Deleting menu screen" << std::endl;
	for (std::vector<Button *>::const_iterator it = this->_buttons.begin(); it != this->_buttons.end(); ++it)
		delete (*it);
}

GameScreen::~GameScreen()
{
	std::cout << "Deleting game screen" << std::endl;
	for (std::vector<Button *>::const_iterator it = this->_buttons.begin(); it != this->_buttons.end(); ++it)
		delete (*it);
}


//GETTERS
sf::RenderWindow&		IScreen::getWindow()
{
	return (this->_window);
}

std::vector<IEvent *>&	IScreen::getEvents()
{
	return (this->_events);
}

eGamestate				IScreen::getState() const
{
	return (this->_state);
}

const unsigned int		IScreen::getIndex() const
{
	return (this->_index);
}

std::vector<Button *>&	MenuScreen::getButtons()
{
	return (this->_buttons);
}

std::vector<Button *>&	GameScreen::getButtons()
{
	return (this->_buttons);
}

const sf::Sprite&		GameScreen::getMap() const
{
	return (this->_map);
}


//METHODS
int		IScreen::run()
{
	int			status;
	sf::Event	event;

	while (this->_window.pollEvent(event))
	{
		for (std::vector<IEvent *>::const_iterator it = this->_events.begin(); it != this->_events.end(); ++it)
		{
			if ((status = (*it)->update(*this, event)) != this->_index)
				return (status);
		}
	}

	this->_window.clear();
	for (std::vector<IEvent *>::const_iterator it = this->_events.begin(); it != this->_events.end(); ++it)
		(*it)->draw(*this);
	this->_window.display();

	return (this->_index);
}