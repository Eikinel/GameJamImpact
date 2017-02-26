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
	Entity(const eEntityType& type, const sf::RectangleShape& shape, const sf::Vector2f& pos = sf::Vector2f(), const sf::Color color = sf::Color())
	{
		this->_type = type;
		this->_shape = shape;
		this->_shape.setFillColor(color);
		this->_shape.setPosition(pos);
	}

	Entity(const Entity& other)
	{
		this->_type = other._type;
		this->_shape = other._shape;
	}

	virtual ~Entity()
	{
		const std::string type[5] = { "button", "player", "enemy", "object" };

		std::cout << "Deleting entity of type " << type[this->_type] << std::endl;
		for (std::vector<T*>::const_iterator it = this->_colliders.begin(); it != this->_colliders.end(); ++it)
			delete(*it);
	}

	//GETTERS
	virtual const sf::RectangleShape&		getShape() const
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

	virtual const sf::Vector2f&		getPositionOnMap() const
	{
		return (this->_pos);
	}


	//SETTERS
	virtual void	setShape(const sf::RectangleShape& shape)
	{
		this->_shape = shape;
	}

	virtual void	setPositionOnMap(const sf::Vector2f& pos)
	{
		this->_pos = pos;
		this->_shape.setPosition(sf::Vector2f(
			this->_pos.x * this->_shape.getGlobalBounds().width,
			this->_pos.y * this->_shape.getGlobalBounds().height));
	}

	virtual void	moveOnMap(const sf::Vector2f& offset)
	{
		this->_pos += offset;
		this->_shape.setPosition(sf::Vector2f(
			this->_pos.x * this->_shape.getGlobalBounds().width,
			this->_pos.y * this->_shape.getGlobalBounds().height));
	}

	//METHODS
	virtual void	addCollider(T * collider)
	{
		this->_colliders.push_back(collider);
	}

protected:
	std::vector<T*>		_colliders;
	sf::Vector2f		_pos;
	sf::RectangleShape	_shape;
	eEntityType			_type;
};