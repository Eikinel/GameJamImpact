#include "Event.h"
#include "Screen.h"
#include "Button.h"
#include "Collider.h"


//CONSTRUCTORS
IEvent::IEvent()
{
	std::cout << "Creating default options" << std::endl;
	this->_toggle_options.push_back(true); // On/off bounding boxes draw
}

WindowDefaultEvent::WindowDefaultEvent()
{
	std::cout << "Creating window default event" << std::endl;
}

MenuEvent::MenuEvent()
{
	std::cout << "Creating menu event" << std::endl;
}

GameEvent::GameEvent()
{
	std::cout << "Creating game event" << std::endl;
}

ChoiceEvent::ChoiceEvent()
{
	std::cout << "Creating choice event" << std::endl;
	for (int i = 0; i < ChoiceScreen::idButton::count; i++)
		_infoPlayer.push_back(0);
}

//UPDATE & DRAW METHODS
int		WindowDefaultEvent::update(IScreen& screen, sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		return (EXIT);
		break;
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			return (EXIT);
			break;
		default:
			break;
		}
	default:
		break;
	}

	return (screen.getIndex());
}

int		MenuEvent::update(IScreen& screen, sf::Event& event)
{
	MenuScreen*	mscreen = static_cast<MenuScreen *>(&screen);

	switch (event.type)
	{
	case sf::Event::MouseButtonReleased:
		std::cout << "MenuEvent : Click !" << std::endl;
	default:
		break;
	}

	return (screen.getIndex());
}

void		MenuEvent::draw(IScreen& screen)
{
	MenuScreen*	mscreen = static_cast<MenuScreen *>(&screen);
}

int		GameEvent::update(IScreen& screen, sf::Event& event)
{
	GameScreen*	gscreen = static_cast<GameScreen *>(&screen);

	switch (event.type)
	{
	case sf::Event::MouseMoved:
		break;
	case sf::Event::MouseButtonReleased:
		std::cout << "GameEvent : Click !" << std::endl;
	default:
		break;
	}

	return (screen.getIndex());
}

void		GameEvent::draw(IScreen& screen)
{
	GameScreen*	gscreen = static_cast<GameScreen *>(&screen);

	for (std::vector<Button *>::const_iterator it = gscreen->getButtons().begin(); it != gscreen->getButtons().end(); ++it)
	{
		gscreen->getWindow().draw((*it)->getText());
		if (this->_toggle_options[0])
		{
			std::vector<sf::VertexArray>	boxes;

			this->getBoundingBoxes(**it, boxes);
			for (std::vector<sf::VertexArray>::const_iterator it2 = boxes.begin(); it2 != boxes.end(); ++it2)
				gscreen->draw(*it2);
		}
	}
	for (auto it : gscreen->getEntities())
	{
		gscreen->draw(it->getShape());
	}
}

int ChoiceEvent::update(IScreen & screen, sf::Event & event)
{
	ChoiceScreen* cscreen = static_cast<ChoiceScreen *>(&screen);

	switch (event.type)
	{
	case sf::Event::MouseMoved:
		break;
	case sf::Event::MouseButtonReleased:
		for (auto it : cscreen->getButtons())
		{
			int status = cscreen->getIndex();
			for (auto it2 : it->getColliders())
				if (it2->getShape().getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					if ((status = it->triggerEvent()) != screen.getIndex())
						return (status);
		}
		std::cout << "GameEvent : Click !" << std::endl;
		break;
	case sf::Event::KeyPressed:

	default:
		break;
	}

	return (screen.getIndex());
}

void ChoiceEvent::draw(IScreen & screen)
{
	ChoiceScreen *cscreen = static_cast<ChoiceScreen*>(&screen);
	

	cscreen->draw(cscreen->getQuestion());
	for (auto it : cscreen->getButtons())
		cscreen->draw(it->getText());
}


//GETTERS
bool	IEvent::getToggleBoundingBoxes()
{
	return (this->_toggle_options[0]);
}

std::vector<bool>&	IEvent::getToggleOptions()
{
	return (this->_toggle_options);
}


//SETTERS
void	IEvent::setToggleOptions(const std::vector<bool>& toggle_options)
{
	this->_toggle_options = toggle_options;
}


//OTHER METHODS
int	IEvent::changeScreen(eGamestate gamestate, IScreen* screen)
{
	std::string	type[4] = { "menu", "options", "game", "choice" };
	int	index;

	if (gamestate == EXIT)
		return (gamestate);
	
	// Check if a screen with the gamestate passed as parameter already exist.
	// If true, copy options and return the gamestate.
	// Else, create the screen before performing operations.
	for (std::vector<IScreen *>::const_iterator it = all_screens.begin(); it != all_screens.end(); ++it)
	{
		if ((*it)->getState() == gamestate)
		{
			if ((*it)->getEvents()[1]->getToggleOptions() != this->_toggle_options)
			{
				std::cout << "Replacing old options" << std::endl;
				(*it)->getEvents()[1]->setToggleOptions(this->_toggle_options);
			}
			return ((*it)->getIndex());
		}
	}
	index = this->createScreen(gamestate, screen);
	std::cout << "Switching screen to " << type[(int)gamestate] << " at index " << index << "." << std::endl;
	return (index);
}

int	IEvent::createScreen(eGamestate gamestate, IScreen* screen)
{
	std::string	type[4] = { "menu", "options", "game" };
	IScreen*	new_screen;

	std::cout << std::endl << "Creating new " << type[(int)gamestate] << "." << std::endl;

	if (gamestate == MENU)
		new_screen = new MenuScreen(screen->getWindow());
	else if (gamestate == OPTIONS)
		new_screen = NULL;//new OptionsScreen(*window);
	else if (gamestate == GAME)
		new_screen = new GameScreen(screen->getWindow());
	new_screen->getEvents()[1]->setToggleOptions(this->_toggle_options);
	all_screens.push_back(new_screen);
	return (new_screen->getIndex());
}

int		IEvent::toggleBoundingBoxes(int index)
{
	this->_toggle_options[0] = !this->_toggle_options[0];
	if (this->_toggle_options[0])
		std::cout << "Showing bounding boxes" << std::endl;
	else
		std::cout << "Hiding bounding boxes" << std::endl;
	return (index);
}
//START
int ChoiceEvent::setInfoPlayer(ChoiceScreen * screen, int idCara, int add)
{
	_infoPlayer[idCara] += add;
	std::cout << std::endl << _infoPlayer[idCara] << std::endl;
	return screen->getIndex();
}
//END