#include "Player.h"


//CONSTRUCTORS
Player::Player(const sf::Vector2f& pos, const sf::RectangleShape& shape) : Entity<BoxCollider>(PLAYER, shape, pos)
{
	std::cout << "Creating new player" << std::endl;
}