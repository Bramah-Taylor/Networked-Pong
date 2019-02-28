#include "Ball.h"

Ball::Ball()
{

	// Initialise the SFML rectangle
	ballShape = new sf::RectangleShape(sf::Vector2f(4.0f, 4.0f));
	ballShape->setFillColor(sf::Color::White);

}

void Ball::Init(int posX, int posY, int speedX, int speedY)
{

	ballShape->setPosition(posX, posY);

	// Output those variables to a struct
	position.x = posX;
	position.y = posY;

	speed.x = speedX;
	speed.y = speedY;

}

void Ball::Update()
{

	// Update from the simulation if it's the server
	if (isClient == false)
	{

		position.x += speed.x;
		position.y += speed.y;

	}

	// Set the SFML object position to be the game object's position
	ballShape->setPosition(position.x, position.y);

}

void Ball::Render(sf::RenderWindow* window)
{

	window->draw(*ballShape);

}

void Ball::InterpolatePositions(int oldTime, int newTime, int xPosOld, int yPosOld, int xPosNew, int yPosNew)
{

	int timeDifference = (newTime - oldTime);

	// Make sure we don't divide by zero - this can happen if a message from the server isn't received
	if (timeDifference != 0)
	{

		// Velocity - difference in positions over time
		int xVelocity = (xPosNew - xPosOld) / timeDifference;
		int yVelocity = (yPosNew - yPosOld) / timeDifference;

		// Linear model - uses velocity * time and most recent values
		int xPosition = (xVelocity * timeDifference) + xPosOld;
		int yPosition = (yVelocity * timeDifference) + yPosOld;

		// Update the position values of the ball
		position.x = xPosition;
		position.y = yPosition;

	}

}