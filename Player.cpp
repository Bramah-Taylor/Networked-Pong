#include "Player.h"
#include <iostream>

Player::Player()
{

	// Set up the circle representing the player
	paddle = new sf::RectangleShape(sf::Vector2f(4.0f, 40.0f));
	paddle->setFillColor(sf::Color::White);

	isCurrentPlayer = false;
	isClient = false;

}

void Player::Init(int PosX, int PosY)
{

	// Set the player to their initial position (25.0f, 25.0f by default)
	paddle->setPosition(PosX, PosY);

	// Output those variables to a struct
	position.x = PosX;
	position.y = PosY;

}

void Player::Update()
{
	// If we're updating the current player, get input
	if (isCurrentPlayer == true && isClient == false)
	{

		// Get input from the keyboard
		GetInput();

	}
	// Else we'll get input from the client

	paddle->setPosition(position.x, position.y);

}

void Player::Render(sf::RenderWindow* window)
{

	// Render the shape
	window->draw(*paddle);

}

void Player::GetInput()
{
	
	// When W is pressed, move up a bit
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{

		position.y -= 4.0f;

	}
	// Else if S is pressed, move down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{

		position.y += 4.0f;

	}

}

int Player::ReturnInput()
{

	// When W is pressed, move up a bit
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
	{

		return 1;

	}
	// Else if S is pressed, move down a bit
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{

		return -1;

	}
	// Else there was no input
	else
	{

		return 0;

	}

}

void Player::InterpolatePositions(int oldTime, int newTime, int yPosOld, int yPosNew)
{

	int timeDifference = (newTime - oldTime);

	// Make sure we don't divide by zero - this can happen if a message from the server isn't received
	if (timeDifference != 0)
	{

		// Velocity - difference in positions over time
		int yVelocity = (yPosNew - yPosOld) / timeDifference;

		// Linear model - uses velocity * time and most recent values
		int yPosition = (yVelocity * timeDifference) + yPosOld;

		// Update the position value of the player object
		position.y = yPosition;

	}

}