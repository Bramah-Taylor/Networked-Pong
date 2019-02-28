#pragma once

#include <SFML/Graphics.hpp>

class Player
{
private:

	// Position struct used for the player's position
	struct Position {

		int x;
		int y;

	};

public:

	Player();

	// Initialise the player with a position
	void Init(int posX, int posY);
	// Update the player based on relevant data (will differ based on server/client application)
	void Update();
	// Render the SFML object
	void Render(sf::RenderWindow* window);

	// Interpolate the positions on the client based on data from the server
	void InterpolatePositions(int oldTime, int newTime, int yPosOld, int yPosNew);

	// Return the input if we're on the client - this will be used to send data to the server
	int ReturnInput();

	// Getters
	inline sf::RectangleShape* GetShape() { return paddle; };
	inline int GetXPos() { return position.x; };
	inline int GetYPos() { return position.y; };

	// Setters
	inline void ChangePosition(int y) { position.y -= y; };
	inline void SetPosition(int y) { position.y = y; };
	inline void SetCurrentPlayer() { isCurrentPlayer = true; };
	inline void SetToClient() { isClient = true; };

private:

	void GetInput();

	// Visual representation of the player
	sf::RectangleShape* paddle;

	Position position;

	// Booleans necessary to determine whether the player should move based on input or not
	bool isCurrentPlayer;
	bool isClient;

};

