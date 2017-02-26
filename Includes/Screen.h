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
	CHOICE,
	BOARD_BEGIN,
	BOARD_ONE,
	BOARD_TWO,
	BOARD_THREE,
	BOARD_FOUR,
	BOARD_FIVE,
	BOARD_SIX,
	BOARD_SEVEN,
	BOARD_END
};

class				IScreen
{
public:
	IScreen(sf::RenderWindow& window, eGamestate state);
	virtual ~IScreen();

	//GETTERS
	virtual sf::RenderWindow&				getWindow();
	virtual const std::vector<IEvent *>&	getEvents() const;
	virtual const eGamestate				getState() const;
	virtual const unsigned int				getIndex() const;
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

	virtual const std::vector<Button *>&	getButtons() const;

protected:
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
	const sf::Text&					getQuestion() const;

private:
	std::vector<Button *>	_buttons;
	std::vector<int>		_infoPlayer;
	sf::Font				_fQuestion;
	sf::Text				_txQuestion;
};

class				BoardScreen : public IScreen
{
public:
	BoardScreen(sf::RenderWindow& window, const eGamestate& gamestate);
	virtual ~BoardScreen();

	//GETTERS
	virtual const std::vector<Button *>&				getButtons() const;
	virtual const std::vector<Entity<BoxCollider> *>&	getEntities() const;
	virtual const std::vector<std::string>&				getMap() const;
	virtual const std::vector<sf::Rect<float> * >&		getBlock() const;
	virtual const int&									getValueField(const int & code) const;

	//SETTERS
	virtual void										setBitField(const int &code, const bool &value);
protected:
	std::vector<Entity<BoxCollider> *>	_entities;
	std::vector<Button *>				_buttons;
	std::vector<std::string>			_map;
	std::vector<sf::Rect<float> *>		_block;
	bool								_bitField[sf::Keyboard::Key::KeyCount] = { false };
};

