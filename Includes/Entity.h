#pragma once

/* Everything is an entity, from living monster to background element.
** Entities are objects that are drawn onto the screen and can collide. */

# include "Constants.h"

enum			eEntityType
{
	BUTTON,
	PLAYER,
	ENEMY,
	GRID,
	OBJECT
};

template <typename T>
class			Entity
{
public:
	//CONSTRUCTORS
	Entity(const eEntityType& type, const sf::Vector2i& pos = sf::Vector2i(), const sf::Texture& texture = sf::Texture())
	{
		this->_type = type;
		this->_texture = texture;
		this->_sprite.setTexture(this->_texture);
		this->_pos = pos;
		this->_hover = false;
	}

	Entity(const eEntityType& type, const sf::Vector2i& pos = sf::Vector2i(), const std::string& path_to_texture = "")
	{
		this->_type = type;
		if (this->_texture.loadFromFile(path_to_texture) == NULL)
			std::cout << "Failed to load " << path_to_texture << std::endl;
		this->_sprite.setTexture(this->_texture);
		this->_pos = pos;
		this->_hover = false;
	}

	Entity(const Entity& other)
	{
		this->_colliders = other._colliders;
		this->_type = other._type;
		this->_texture = sf::Texture(other._texture);
		this->_sprite = sf::Sprite(other._sprite);
		this->_pos = other._pos;
		this->_hover = other._hover;
	}

	virtual void	setPositionOnGrid(const sf::Vector2i& pos, const sf::FloatRect& square_side)
	{
		sf::Vector2f	old_sprite_pos = this->_sprite.getPosition();

		std::cout << "Setting entity position to [" << pos.x << " ; " << pos.y << "]" << std::endl;
		this->_pos = pos;
		this->_sprite.setPosition(sf::Vector2f(
			square_side.left + square_side.width / 4.f,
			square_side.top - square_side.height / 2.f));
		for (std::vector<BoxCollider *>::const_iterator it = this->_colliders.begin(); it != this->_colliders.end(); ++it)
		{
			sf::Vector2f offset = (*it)->getShape().getPosition() - old_sprite_pos;

			(*it)->getShape().setPosition(this->_sprite.getPosition() + offset);
		}
	}

	virtual void	moveOnGrid(const sf::Vector2i& offset, const sf::FloatRect& square_side)
	{
		this->setPositionOnGrid(this->_pos + offset, square_side);
	}

	virtual ~Entity()
	{
		const std::string type[5] = { "button", "hero", "enemy", "grid", "object" };

		std::cout << "Deleting entity of type " << type[this->_type] << std::endl;
		for (std::vector<T *>::const_iterator it = this->_colliders.begin(); it != this->_colliders.end(); ++it)
			delete(*it);
	}


	//GETTERS
	virtual sf::Sprite&				getSprite()
	{
		return (this->_sprite);
	}

	virtual const sf::Vector2i&		getPositionOnGrid() const
	{
		return (this->_pos);
	}

	virtual const std::vector<T *>&	getColliders() const
	{
		return (this->_colliders);
	}

	virtual const eEntityType&		getType() const
	{
		return (this->_type);
	}

	virtual const bool				getHover() const
	{
		return (this->_hover);
	}


	//SETTERS
	virtual void	setHover(const bool hover)
	{
		if (hover)
			this->_sprite.setColor(sf::Color(255, 255, 255, 142));
		else
			this->_sprite.setColor(sf::Color(255, 255, 255, 255));
		this->_hover = hover;
	}


	//METHODS
	virtual void	addCollider(T * collider)
	{
		this->_colliders.push_back(collider);
	}

protected:
	sf::Texture			_texture;
	sf::Sprite			_sprite;
	sf::Vector2i		_pos;
	std::vector<T *>	_colliders;
	eEntityType			_type;
	bool				_hover;
};