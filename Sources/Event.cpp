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

ChoiceEvent::ChoiceEvent()
{
	std::cout << "Creating choice event" << std::endl;
	for (int i = 0; i < ChoiceScreen::idButton::count; i++)
		this->_infoPlayer.push_back(false);
}

BoardEvent::BoardEvent()
{
	std::cout << "Creating game event" << std::endl;
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

		for (auto it : mscreen->getButtons())
		{
			int status = mscreen->getIndex();

			for (auto it2 : it->getColliders())
				if (it2->getShape().getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					if ((status = it->triggerEvent()) != screen.getIndex())
						return (status);
		}
	default:
		break;
	}

	return (screen.getIndex());
}

void		MenuEvent::draw(IScreen& screen)
{
	MenuScreen*	mscreen = static_cast<MenuScreen *>(&screen);

	for (auto it : mscreen->getButtons())
	{
		mscreen->draw(it->getText());
		if (this->_toggle_options[0])
		{
			std::vector<sf::VertexArray>	boxes;

			this->getBoundingBoxes(*it, boxes);
			for (std::vector<sf::VertexArray>::const_iterator it2 = boxes.begin(); it2 != boxes.end(); ++it2)
				mscreen->draw(*it2);
		}
	}
}

int ChoiceEvent::update(IScreen & screen, sf::Event & event)
{
	ChoiceScreen* cscreen = static_cast<ChoiceScreen *>(&screen);

	switch (event.type)
	{
	case sf::Event::MouseMoved:
		for (unsigned int i = 0; i < cscreen->getButtons().size(); i++)
		{
			int status = cscreen->getIndex();
			sf::Text	swap = cscreen->getButtons()[i]->getText();

			for (auto it2 : cscreen->getButtons()[i]->getColliders())
				if (it2->getShape().getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)))
				{
					swap.setFillColor(sf::Color(88, 224, 242));
					cscreen->getButtons()[i]->setText(swap);
					return (status);
				}
			if (i < ChoiceScreen::idButton::count && this->_infoPlayer[i])
			{
				swap.setFillColor(sf::Color(101, 242, 88));
				cscreen->getButtons()[i]->setText(swap);
			}
			else if (cscreen->getButtons()[i]->getText().getFillColor() != sf::Color(255, 255, 255))
			{
				swap.setFillColor(sf::Color(255, 255, 255));
				cscreen->getButtons()[i]->setText(swap);
			}
		}
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
	{
		cscreen->draw(it->getText());
		if (this->_toggle_options[0])
		{
			std::vector<sf::VertexArray>	boxes;

			this->getBoundingBoxes(*it, boxes);
			for (std::vector<sf::VertexArray>::const_iterator it2 = boxes.begin(); it2 != boxes.end(); ++it2)
				cscreen->draw(*it2);
		}
	}
}

int		BoardEvent::update(IScreen& screen, sf::Event& event)
{
	BoardScreen*	gscreen = static_cast<BoardScreen *>(&screen);

	switch (event.type)
	{
	case sf::Event::KeyPressed:
		std::cout << "BoardEvent : Key pressed !" << std::endl;
		switch (event.key.code)
		{
		case sf::Keyboard::Z:
			gscreen->getEntities()[0]->moveOnMap(sf::Vector2f(0.f, -0.1f));
			break;
		case sf::Keyboard::Q:
			gscreen->getEntities()[0]->moveOnMap(sf::Vector2f(-0.1f, 0.f));
			break;
		case sf::Keyboard::S:
			gscreen->getEntities()[0]->moveOnMap(sf::Vector2f(0.f, 0.1f));
			break;
		case sf::Keyboard::D:
			gscreen->getEntities()[0]->moveOnMap(sf::Vector2f(0.1f, 0.f));
			break;
		default:
			break;
		}
	case sf::Event::MouseMoved:
		break;
	case sf::Event::MouseButtonReleased:
		std::cout << "BoardEvent : Click !" << std::endl;
		
		for (auto it : gscreen->getButtons())
		{
			int status = gscreen->getIndex();

			for (auto it2 : it->getColliders())
				if (it2->getShape().getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					if ((status = it->triggerEvent()) != screen.getIndex())
						return (status);
		}
	default:
		break;
	}

	return (screen.getIndex());
}

void		BoardEvent::draw(IScreen& screen)
{
	BoardScreen*	bscreen = static_cast<BoardScreen *>(&screen);
	sf::Color		env = bscreen->getEntities()[0]->getShape().getFillColor();
	
	env.r /= 2.f;
	env.g /= 2.f;
	env.b /= 2.f;

	sf::Color	colors[2] = { sf::Color::Black, env };
	float y = 0, x = 0;
	for (std::vector<std::string>::const_iterator it = bscreen->getMap().begin(); it != bscreen->getMap().end(); ++it)
	{
		for (std::string::const_iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			sf::RectangleShape	rect;

			rect.setSize(sf::Vector2f(
				(float)bscreen->getWindow().getSize().x / (float)((*it).size()),
				(float)bscreen->getWindow().getSize().y / (float)bscreen->getMap().size()));
			rect.setPosition(sf::Vector2f(
				(float)x * rect.getGlobalBounds().width,
				(float)y * rect.getGlobalBounds().height));
			rect.setFillColor(colors[*it2 - '0']);
			bscreen->draw(rect);
			x++;
		}
		x = 0;
		y++;
	}

	for (auto it : bscreen->getEntities())
		bscreen->draw(it->getShape());

	for (auto it : bscreen->getButtons())
	{
		bscreen->draw(it->getText());
		if (this->_toggle_options[0])
		{
			std::vector<sf::VertexArray>	boxes;

			this->getBoundingBoxes(*it, boxes);
			for (std::vector<sf::VertexArray>::const_iterator it2 = boxes.begin(); it2 != boxes.end(); ++it2)
				bscreen->draw(*it2);
		}
	}
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

const std::vector<bool>&	ChoiceEvent::getInfosPlayer() const
{
	return (this->_infoPlayer);
}


//SETTERS
void	IEvent::setToggleOptions(const std::vector<bool>& toggle_options)
{
	this->_toggle_options = toggle_options;
}

int		ChoiceEvent::setInfoPlayer(ChoiceScreen* screen, int idCara)
{
	this->_infoPlayer[idCara] = !this->_infoPlayer[idCara];
	std::cout << std::endl << this->_infoPlayer[idCara] << std::endl;

	return (screen->getIndex());
}

void	BoardEvent::setInfosPlayer(const std::vector<bool>& infos, BoardScreen* bscreen)
{
	Player*		player = static_cast<Player *>(bscreen->getEntities()[0]);
	int			mult[ChoiceScreen::idButton::count][3] = {
		{ -64, 128, -128 }, { -128, -128, 128 }, { 128, -128, -64 },
		{ 128, -128, 0 }, { 255, -192, -64 }, { -64, -64, 64 }};
	sf::RectangleShape	shape = player->getShape();
	sf::Color			final = shape.getFillColor();

	for (unsigned int i = 0; i < ChoiceScreen::idButton::count; i++)
		if (infos[i])
		{
			if (final.r + mult[i][0] >= 0 && final.r + mult[i][0] <= 255)
				final.r += mult[i][0];
			if (final.g + mult[i][1] >= 0 && final.g + mult[i][1] <= 255)
				final.g += mult[i][1];
			if (final.b + mult[i][2] >= 0 && final.b + mult[i][2] <= 255)
				final.b += mult[i][2];
		}

	shape.setFillColor(final);
	player->setShape(shape);
}


//OTHER METHODS
int	IEvent::changeScreen(eGamestate gamestate, IScreen* screen)
{
	std::string	type[4] = { "menu", "options", "game" };
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
	std::string	type[12] = { "menu", "options", "choice",
		"board begin", "board one", "board two", "board three", "board four", "board five", "board six", "board seven", "board end" };
	IScreen*	new_screen;

	std::cout << std::endl << "Creating new " << type[(int)gamestate] << "." << std::endl;

	if (gamestate == MENU)
		new_screen = new MenuScreen(screen->getWindow());
	else if (gamestate == OPTIONS)
		new_screen = NULL;//new OptionsScreen(*window);
	else
		new_screen = new BoardScreen(screen->getWindow(), (eGamestate)gamestate);
	new_screen->getEvents()[1]->setToggleOptions(this->_toggle_options);
	if (screen->getState() == CHOICE)
		static_cast<BoardEvent *>(new_screen->getEvents()[1])->setInfosPlayer(
			static_cast<ChoiceEvent *>(screen->getEvents()[1])->getInfosPlayer(),
			static_cast<BoardScreen *>(new_screen));
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