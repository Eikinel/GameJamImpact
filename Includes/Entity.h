#pragma once

/* Everything is an entity, from living monster to background element.
** Entities are objects that are drawn onto the screen and can collide. */

# include "Constants.h"

enum			eEntityType
{
	BUTTON,
	PLAYER,
	ENEMY,
	OBJECT
};

template <typename T>
class			Entity
{
public:
	//CONSTRUCTORS
	Entity(const eEntityType& type, const sf::Vector2f& pos = sf::Vector2f(), const sf::ConvexShape& shape = sf::ConvexShape(), const sf::Color color = sf::Color())
	{
		this->_type = type;
		this->_shape = shape;
		this->_shape.setFillColor(color);
		this->_pos = pos;
	}

	Entity(const Entity& other)
	{
		this->_type = other._type;
		this->_shape = other._shape;
		this->_pos = other._pos;
	}

	virtual ~Entity()
	{
		const std::string type[5] = { "button", "player", "enemy", "object" };

		std::cout << "Deleting entity of type " << type[this->_type] << std::endl;
		for (std::vector<T*>::const_iterator it = this->_colliders.begin(); it != this->_colliders.end(); ++it)
			delete(*it);
	}

	//GETTERS
	virtual const sf::ConvexShape&	getShape() const
	{
		return (this->_shape);
	}

	virtual const eEntityType&		getType() const
	{
		return (this->_type);
	}

	virtual const std::vector<T*>&	getColliders() const
	{
		return (this->_colliders);
	}


	//METHODS
	virtual void	addCollider(T * collider)
	{
		this->_colliders.push_back(collider);
	}

protected:
	std::vector<T*>		_colliders;
	sf::Vector2f		_pos;
	sf::ConvexShape		_shape;
	eEntityType			_type;
};