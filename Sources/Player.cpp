#include "Player.h"


//CONSTRUCTORS
Player::Player(const sf::Vector2f& pos, const sf::RectangleShape& shape, const sf::Color color) : Entity<BoxCollider>(PLAYER, shape, pos, color)
{
	std::cout << "Creating new player" << std::endl;
}