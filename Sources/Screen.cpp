#include <iostream>
#include <fstream>
#include <regex>
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
	this->_buttons[0]->onClick(&IEvent::changeScreen, this->_events[1], BOARD_BEGIN, static_cast<IScreen *>(this));
	this->_buttons[1]->onClick(&IEvent::changeScreen, this->_events[1], OPTIONS, static_cast<IScreen *>(this));
	this->_buttons[2]->onClick(&IEvent::changeScreen, this->_events[1], EXIT, static_cast<IScreen *>(this));
	this->_buttons[3]->onClick(&IEvent::toggleBoundingBoxes, this->_events[1], this->_index);
}

ChoiceScreen::ChoiceScreen(sf::RenderWindow & window) : IScreen(window, eGamestate::CHOICE)
{
	std::cout << std::endl << "Creating choice screen" << std::endl;
	this->_events.push_back(new WindowDefaultEvent);
	this->_events.push_back(new ChoiceEvent);

	sf::Vector2f win_size(this->_window.getSize());

	if (!this->_fQuestion.loadFromFile(FONTS_DIR"/moyko.ttf"))
		std::cout << "Failed to load " << FONTS_DIR"/moyko.ttf" << "." << std::endl;
	this->_txQuestion.setFont(this->_fQuestion);
	this->_txQuestion.setCharacterSize(win_size.y / 12.f);
	this->_txQuestion.setString("Which word(s) describe(s) you better ?");
	this->_txQuestion.setPosition(sf::Vector2f(
		(float)win_size.x / 2.f - this->_txQuestion.getGlobalBounds().width / 2.f,
		(float)win_size.y / 10.f));

	std::string allCara[6] = { "CLEVER", "SHY", "CHOLERIC", "SELFISH", "GROSS", "LAZY" };
	int posY = this->_txQuestion.getGlobalBounds().top + this->_txQuestion.getGlobalBounds().height + (win_size.y / 25.f) * (allCara->size() - 1);

	for (int i = 0; i < idButton::count; i++)
	{
		this->_buttons.push_back(new Button(allCara[i], win_size.y / 24.f,
			sf::Vector2f(win_size.x / 2.f, posY), CENTER));
		this->_buttons[i]->onClick(&ChoiceEvent::setInfoPlayer,
			dynamic_cast<ChoiceEvent*>(this->_events[1]),
			this,
			i);
		posY += this->_buttons[i]->getText().getGlobalBounds().height + win_size.y / 25.f;
	}
	this->_buttons.push_back(new Button("Next", win_size.y / 24.f,
		sf::Vector2f(win_size.x / 1.8f, win_size.y / 1.1f), RIGHT));
	this->_buttons[idButton::count]->onClick(&IEvent::changeScreen, this->_events[1], BOARD_BEGIN, static_cast<IScreen *>(this));
}

BoardScreen::BoardScreen(sf::RenderWindow& window, const eGamestate& gamestate) : IScreen(window, gamestate)
{
	std::cout << std::endl << "Creating board screen" << std::endl;
	this->_events.push_back(new WindowDefaultEvent); // Event handler for options, close window, etc.
	this->_events.push_back(new BoardEvent); // Update game board, draw it and react in terms of user inputs.

	this->_buttons.push_back(new Button("Toggle bounding boxes", this->_window.getSize().y / 25.f, sf::Vector2f(
		this->_window.getSize().x / 9.2f,
		this->_window.getSize().y - this->_window.getSize().y / 6.f), LEFT));
	this->_buttons.push_back(new Button("Back to menu", this->_window.getSize().y / 25.f, sf::Vector2f(
		this->_window.getSize().x / 9.2f,
		this->_window.getSize().y - this->_window.getSize().y / 25.f), LEFT));
	this->_buttons[0]->onClick(&IEvent::toggleBoundingBoxes, this->_events[1], this->_index);
	this->_buttons[1]->onClick(&IEvent::changeScreen, this->_events[1], MENU, static_cast<IScreen *>(this));
	this->_entities.push_back(new Player(sf::Vector2f(), sf::RectangleShape()));

	std::string		board_path = MAPS_DIR"/board" + std::to_string(this->_state - 2) + ".gji";
	std::ifstream	board_file(board_path);
	std::string		line;
	std::regex		re("[^=]+");
	std::vector<std::string> tmp_infos;

	if (board_file.is_open())
	{
		while (std::getline(board_file, line))
		{
			std::sregex_token_iterator
				begin(line.begin(), line.end(), re),
				end;
			std::copy(begin, end, std::back_inserter(tmp_infos));
		}
	}
	else
		std::cout << "Failed to load " << board_path << "." << std::endl;

	sf::RectangleShape	shape;
	Player*				player = static_cast<Player *>(this->_entities[0]);

	for (unsigned int i = 0; i < tmp_infos.size(); i++)
	{
		if (tmp_infos[i] == "Map")
		{
			i++;
			while (i < tmp_infos.size() && tmp_infos[i][0] >= '0' && tmp_infos[i][0] <= '9')
				this->_map.push_back(tmp_infos[i++]);
		}
	}

	shape.setSize(sf::Vector2f(
		(float)this->_window.getSize().x / (float)this->_map[0].size(),
		(float)this->_window.getSize().y / (float)this->_map.size()));

	for (unsigned int y = 0; y < this->_map.size(); y++)
		for (unsigned int x = 0; x < this->_map[y].size(); x++)
			if (this->_map[y][x] == '2')
			{
				this->_entities[0]->setPositionOnMap(sf::Vector2f(x, y));
				shape.setPosition(sf::Vector2f(
					x * shape.getGlobalBounds().width,
					y * shape.getGlobalBounds().height));
				break;
			}

	shape.setFillColor(sf::Color::White); //Player's color changed after quiz
	player->setShape(shape);

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

ChoiceScreen::~ChoiceScreen()
{
	std::cout << "Deleting choice screen" << std::endl;
	for (auto it : this->_buttons)
		delete it;
}

BoardScreen::~BoardScreen()
{
	std::cout << "Deleting game screen" << std::endl;
	for (auto it : this->_buttons)
		delete (it);
	for (auto it : this->_entities)
		delete it;
}


//GETTERS
sf::RenderWindow&			IScreen::getWindow()
{
	return (this->_window);
}

const std::vector<IEvent *>&	IScreen::getEvents() const
{
	return (this->_events);
}

const eGamestate				IScreen::getState() const
{
	return (this->_state);
}

const unsigned int				IScreen::getIndex() const
{
	return (this->_index);
}

const std::vector<Button *>&	MenuScreen::getButtons() const
{
	return (this->_buttons);
}

const std::vector<Button *>&	ChoiceScreen::getButtons() const
{
	return (this->_buttons);
}

const sf::Text&					ChoiceScreen::getQuestion() const
{
	return (this->_txQuestion);
}

const std::vector<Button *>&				BoardScreen::getButtons() const
{
	return (this->_buttons);
}

const std::vector<Entity<BoxCollider> *>&	BoardScreen::getEntities() const
{
	return (this->_entities);
}

const std::vector<std::string>&				BoardScreen::getMap() const
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