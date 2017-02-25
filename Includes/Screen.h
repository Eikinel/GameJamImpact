#pragma once

# include <vector>
# include "Constants.h"
# include "Event.h"
# include "Collider.h"
# include "Player.h"

class IEvent;
class Button;

enum				eGamestate
{
	EXIT = -1,
	MENU,
	OPTIONS,
	GAME,
	CHOICE
};

class				IScreen
{
public:
	IScreen(sf::RenderWindow& window, eGamestate state);
	virtual ~IScreen();

	//GETTERS
	virtual sf::RenderWindow&		getWindow();
	virtual std::vector<IEvent *>&	getEvents();
	virtual eGamestate				getState();
	virtual const unsigned int		getIndex();
	void draw(const sf::Drawable& object)
	{
		this->_window.draw(object);
	}

	//METHODS
	virtual int		run();

protected:
	sf::RenderWindow&		_window;
	std::vector<IEvent *>	_events;
	eGamestate				_state;
	unsigned int			_index;
};

class				MenuScreen : public IScreen
{
public:
	MenuScreen(sf::RenderWindow& window);
	virtual ~MenuScreen();

	virtual std::vector<Button *>&	getButtons();

protected:
	std::vector<Button *>	_buttons;
};

class				GameScreen : public IScreen
{
public:
	GameScreen(sf::RenderWindow& window);
	virtual ~GameScreen();

	//GETTERS
	virtual std::vector<Button *>&	getButtons();
	std::vector<Entity<BoxCollider> *>	getEntities();

protected:
	std::vector<Entity<BoxCollider> *> _entities;
	std::vector<Button *>	_buttons;
};

class ChoiceScreen : public IScreen
{
public:
	ChoiceScreen(sf::RenderWindow& window);
	virtual ~ChoiceScreen();

	enum idButton
	{
		CLEVER,
		SHY,
		CHOLERIC,
		SELFISH,
		GROSS,
		LAZY,
		count
	};
	//GETTERS
	const std::vector<Button *>&	getButtons() const;
	const sf::Text&				getQuestion() const;
private:
	std::vector<Button *> _buttons;
	std::vector<int> _infoPlayer;
	sf::Font		_fQuestion;
	sf::Text		_txQuestion;
};