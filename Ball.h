#pragma once

#include <SFML/Graphics.hpp>

class Ball
{
private:

	// Position struct used for the position and speed of the ball
	struct Position {

		int x;
		int y;

	};

public:

	Ball();

	// Initialise the ball with a position and speed - should be heading to the server player by default
	void Init(int posX, int posY, int speedX, int speedY);
	// Update the ball's position based on relevant data (will vary based on client/server)
	void Update();
	// Render the object
	void Render(sf::RenderWindow* window);

	// Interpolate the ball's position on the client based on messages from the server
	void InterpolatePositions(int oldTime, int newTime, int xPosOld, int yPosOld, int xPosNew, int yPosNew);

	// Getters
	inline sf::RectangleShape* GetShape() { return ballShape; };
	inline int GetXPosition() { return position.x; };
	inline int GetYPosition() { return position.y; };
	inline int GetXSpeed() { return speed.x; };
	inline int GetYSpeed() { return speed.y; };

	// Setters
	inline void SetPosition(int x, int y) { position.x = x; position.y = y; };
	inline void SetSpeed(int x, int y) { speed.x = x; speed.y = y; };
	inline void SetToClient() { isClient = true; };

private:

	sf::RectangleShape* ballShape;

	Position position;
	Position speed;

	// Boolean for determining what state the application is in
	bool isClient;

};

