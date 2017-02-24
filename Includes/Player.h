#pragma once

# include "Entity.h"
# include "Collider.h"

class			Player : public Entity<BoxCollider>
{
public:
	Player(const sf::Vector2i& pos, const sf::FloatRect& square_side, const sf::Texture& texture);
	Player(const sf::Vector2i& pos, const sf::FloatRect& square_side, const std::string& path_to_texture);
	~Player() {	std::cout << "Deleting player" << std::endl; }
};