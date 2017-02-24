#include "Player.h"


//CONSTRUCTORS
Player::Player(const sf::Vector2i& pos, const sf::FloatRect& square_side, const sf::Texture& texture) : Entity<BoxCollider>(PLAYER, pos, texture)
{
	std::cout << "Creating new player" << std::endl;

	this->_pos = pos;
	this->_sprite.setScale(sf::Vector2f(
		square_side.height / this->_sprite.getGlobalBounds().height,
		square_side.height / this->_sprite.getGlobalBounds().height));

	sf::FloatRect	box = this->_sprite.getGlobalBounds();

	this->addCollider(new BoxCollider(
		sf::Vector2f(box.left + box.width / 2.f, box.top),
		sf::Vector2f(box.width / 2.5f, box.height / 4.f),
		HEAD));
}

Player::Player(const sf::Vector2i& pos, const sf::FloatRect& square_side, const std::string& path_to_texture) : Entity<BoxCollider>(PLAYER, pos, path_to_texture)
{
	std::cout << "Creating new player" << std::endl;

	this->_pos = pos;
	this->_sprite.setScale(sf::Vector2f(
		square_side.height / this->_sprite.getGlobalBounds().height,
		square_side.height / this->_sprite.getGlobalBounds().height));

	sf::FloatRect	box = this->_sprite.getGlobalBounds();
	sf::FloatRect	head, body, leg, foot;

	head.left = box.left + box.width / 2.1f;
	head.top = box.top + box.height / 100.f;
	head.width = box.width / 2.3f;
	head.height = box.height / 3.8f;

	body.left = box.left + box.width / 3.15f;
	body.top = head.top + (head.height / 1.5f);
	body.width = box.width / 1.64f;
	body.height = box.height / 2.9f;

	leg.left = box.left + box.width / 3.2f;
	leg.top = body.top + body.height;
	leg.width = box.width / 1.65f;
	leg.height = box.height / 2.9f;

	foot.left = box.left + box.width / 3.9f;
	foot.top = leg.top + leg.height;
	foot.width = box.width / 1.35f;
	foot.height = box.height / 8.f;

	//this->addCollider(new BoxCollider(head, HEAD));

	// HEAD PART
	// Center
	this->addCollider(new BoxCollider(
		sf::Vector2f(head.left + head.width / 4.f, head.top + head.height / 5.5f),
		sf::Vector2f(head.width / 2.2f, head.height / 1.57f),
		HEAD));

	// Left
	this->addCollider(new BoxCollider(
		sf::Vector2f(head.left + head.width / 12.f, head.top + head.height / 3.5f),
		sf::Vector2f((2.f * head.width) / 12.f, head.height / 2.5f),
		HEAD));
	this->addCollider(new BoxCollider(
		sf::Vector2f(head.left + head.width / 5.5f, head.top + head.height / 4.5f),
		sf::Vector2f(head.width / 4.f - head.width / 5.5f, head.height / 3.5f - head.height / 4.5f),
		HEAD));

	this->addCollider(new BoxCollider(
		sf::Vector2f(head.left + head.width / 6.5f, head.top + head.height / 3.5f + head.height / 2.5f),
		sf::Vector2f(head.width / 4.f - head.width / 6.5f, head.height / 11.f),
		HEAD));

	// Right
	this->addCollider(new BoxCollider(
		sf::Vector2f(head.left + head.width / 4.f + head.width / 2.2f, head.top + head.height / 3.7f),
		sf::Vector2f(head.width / 4.5f, head.height / 2.5f),
		HEAD));

	// Top
	this->addCollider(new BoxCollider(
		sf::Vector2f(head.left + head.width / 2.5f, head.top + head.height / 12.f),
		sf::Vector2f(head.width / 4.8f, head.height / 5.5f - head.height / 12.f),
		HEAD));

	// Bottom
	this->addCollider(new BoxCollider(
		sf::Vector2f(head.left + head.width / 1.9f, head.top + head.height / 5.5f + head.height / 1.57f),
		sf::Vector2f(head.width / 5.6f, head.height / 8.5f),
		HEAD));


	// BODY PART
	// Center
	// Left
	// Right
	// Top
	// Bottom
	this->addCollider(new BoxCollider(body, BODY));


	// LEGS PART
	// Center
	// Left
	// Right
	// Top
	// Bottom
	this->addCollider(new BoxCollider(leg, LEG));


	//FEET PART
	// Center
	// Left
	// Right
	// Top
	// Bottom
	this->addCollider(new BoxCollider(foot, FOOT));
}