#include "Player.h"


//CONSTRUCTORS
Player::Player(const sf::Vector2f& pos, const sf::ConvexShape& shape, const sf::Color color) : Entity<BoxCollider>(PLAYER, pos, shape, color)
{
	std::cout << "Creating new player" << std::endl;
}