#include <SFML\Graphics.hpp>
#include "Screen.h"
#include "Event.h"
#include "Button.h"

std::vector<IScreen *>	all_screens;

int		main()
{
	sf::RenderWindow	window(sf::VideoMode(
		sf::VideoMode::getDesktopMode().width / 1.25f, sf::VideoMode::getDesktopMode().height / 1.25f),
		"Game Jam Impact 2017",
		sf::Style::Close);

	// Pushing screens to the stack
	all_screens.push_back(new ChoiceScreen(window));//new BoardScreen(window, BOARD_BEGIN));

	// Run every elements in the stack. If there's no more screen left or a close request is done, break the loop.
	int status = MENU;
	while ((status = all_screens[status]->run()) != EXIT && status < all_screens.size());

	// On exit request, delete all screens properly
	for (std::vector<IScreen *>::const_iterator it = all_screens.begin(); it != all_screens.end(); ++it)
	{
		std::cout << "Delete screen n°" << (*it)->getIndex() << " : " << std::endl;
		delete(*it);
	}
	return (status);
}