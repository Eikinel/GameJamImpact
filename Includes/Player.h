#pragma once

# include "Entity.h"
# include "Collider.h"

class			Player : public Entity<BoxCollider>
{
public:
	Player(const sf::Vector2f& pos, const sf::RectangleShape& shape, const sf::Color color);
	~Player() {	std::cout << "Deleting player" << std::endl; }

protected:

	//std::vector<ISkill *>	_skills;
};