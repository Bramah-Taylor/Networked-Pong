#include <iostream>					// Used for outputting messages to the console
#include <string>					// Ditto
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"
#include "Ball.h"

struct ServerMessage
{

	int playerOneYPos = 0;
	int playerTwoYPos = 0;
	int ballXPos = 0;
	int ballYPos = 0;
	int playerOneScore = 0;
	int playerTwoScore = 0;
	int time = 0;

};

struct ClientMessage
{

	int movement = 0;

};

void HandleCollisions(Player* player1, Player* player2, Ball* ball, sf::RectangleShape &topBorder, sf::RectangleShape &bottomBorder);

int main()
{

	// Initialise boolean for determining the state of the application
	bool isServer = false;

	// Get a response from the user
	std::string response;

	std::cout << "Would you like to host a game? (Yes/No)\n";

	std::cin >> response;

	// Based on the response, make the application a server or a client
	if (response == "Yes" || response == "yes")
	{

		isServer = true;

	}

	// Initialise the UDP socket
	sf::UdpSocket socket;
	socket.setBlocking(false);

	// Initialise the server address and port - will be used for bind on the server, and send on the client
	sf::IpAddress serverAddress = "127.0.0.1";
	unsigned short serverPort = 4444;

	// Initialise empty address and port for the client - the server will get these later from the receive function
	sf::IpAddress clientAddress;
	unsigned short clientPort;

	std::size_t received;

	// If the application is a server
	if (isServer == true)
	{

		// Initialise a message - the actual message isn't important, it just needs to be big enough to fit the client message
		char welcomeMessage[22];
		memset(welcomeMessage, 0, sizeof(welcomeMessage));

		// Bind the UDP socket to the server address and port
		if (socket.bind(serverPort, serverAddress) != sf::Socket::Done)
		{

			// If bind fails, output an error message to the console
			std::cout << "Bind failed...\n";

		}

		// Initialise a boolean for looping
		bool isWaitingForClient = true;

		std::cout << "Waiting for client to connect...\n";

		// Wait for a client to connect
		while (isWaitingForClient == true)
		{

			// When the client connects, the receive function will be used to get the address and port required later for sending data
			if (socket.receive(welcomeMessage, sizeof(welcomeMessage), received, clientAddress, clientPort) != sf::Socket::Done)
			{

				// Do nothing

			}
			else
			{

				// When receive returns true, stop looping and output the connection welcome message to the console
				isWaitingForClient = false;
				std::cout << welcomeMessage << "\n";

				if (socket.send(welcomeMessage, sizeof(welcomeMessage), clientAddress, clientPort) != sf::Socket::Done)
				{

					// Output error message
					std::cout << "Didn't send message to client\n";

				}

			}

		}

	}
	// If the application is not a server, send a welcome message to the server
	else
	{

		// Initialise messages for communication with the server
		char connectionMessage[] = "Client has connected.";
		char receivedMessage[22];

		// Loop until a connection has been established with the server - we don't want the game to run without it
		bool isWaitingForServer = true;

		while (isWaitingForServer == true)
		{

			// Send a message to the server
			if (socket.send(connectionMessage, sizeof(connectionMessage), serverAddress, serverPort) != sf::Socket::Done)
			{

				std::cout << "Sending welcome message failed...\n";

			}

			// Receive the message back from the server
			if (socket.receive(receivedMessage, sizeof(receivedMessage), received, serverAddress, serverPort) != sf::Socket::Done)
			{

				// Output error message
				std::cout << "Receiving welcome message failed...\n";
				std::cout << "If this happens multiple times, it probably means that a server isn't open.\n";

			}
			else
			{

				// Output the received message - it should be the same as connectionMessage
				std::cout << receivedMessage << "\n";
				// Stop looping
				isWaitingForServer = false;

			}

		}

	}
	
	// Initialise time
	int time = 0;

	// Set up messages
	ServerMessage serverMessage;
	ServerMessage lastMessage;
	ClientMessage clientMessage;

	// Initialise the game scores
	int player1Score = 0;
	int player2Score = 0;

	// Create the game objects
	Player* player = new Player();
	Player* player2 = new Player();

	Ball* ball = new Ball();

	// Initialise the player at 25.0f, 25.0f
	int playerStartY = 180;
	player->Init(25, playerStartY);

	// Set the current player depending on whether the application is a server or client
	if (isServer == true)
	{

		player->SetCurrentPlayer();

	}
	else
	{

		player2->SetCurrentPlayer();

	}

	// Initialise the second player
	player2->Init(771, playerStartY);

	// Initialise the ball
	int ballStartX = 400;
	int ballStartY = 200;
	ball->Init(ballStartX, ballStartY, -1, 1);

	if (isServer == false)
	{

		player->SetToClient();
		player2->SetToClient();
		ball->SetToClient();

	}

	// Set up the window
	sf::RenderWindow window(sf::VideoMode(800, 400), "Networked Pong");
	window.setFramerateLimit(60);

	// Name and position the windows for ease of testing on a single machine
	if (isServer == true)
	{

		window.setTitle("Networked Pong Server");
		window.setPosition(sf::Vector2i(500, 100));

	}
	else
	{

		window.setTitle("Networked Pong Client");
		window.setPosition(sf::Vector2i(500, 600));

	}

	// Create the borders for the top and bottom so that the paddles and ball stay where we want them
	sf::RectangleShape topBorder(sf::Vector2f(750.0f, 4.0f));
	topBorder.setPosition(25.0f, 0.0f);
	topBorder.setFillColor(sf::Color::White);

	sf::RectangleShape bottomBorder(sf::Vector2f(750.0f, 4.0f));
	bottomBorder.setPosition(25.0f, 396.0f);
	bottomBorder.setFillColor(sf::Color::White);

	// Window handling
	while (window.isOpen())
	{

		sf::Event event;

		// Increment time
		time++;

		// Handle events
		while (window.pollEvent(event))
		{

			// Close event is the only event we're interested in for now
			if (event.type == sf::Event::Closed)
			{

				window.close();

			}

		}

		// Receive the relevant message
		if (isServer == true)
		{

			if (socket.receive((char *)&clientMessage, sizeof(ClientMessage), received, clientAddress, clientPort) != sf::Socket::Done)
			{

				// Do nothing

			}

			// Get the data from the client
			if (clientMessage.movement == 1)
			{

				player2->ChangePosition(4);

			}
			else if (clientMessage.movement == -1)
			{

				player2->ChangePosition(-4);

			}
			// No need to change the position if the player isn't moving

		}
		else
		{

			// Set the last message to hold the data in the most recent message just before we expect to receive new data from the server
			lastMessage = serverMessage;

			if (socket.receive((char *)&serverMessage, sizeof(ServerMessage), received, serverAddress, serverPort) != sf::Socket::Done)
			{

				// Do nothing

			}

			// Get data from the server message

			// Interpolate the positions of the objects in the game world based on data from the two most recent messages from the server
			player->InterpolatePositions(lastMessage.time, serverMessage.time, lastMessage.playerOneYPos, serverMessage.playerOneYPos);
			player2->InterpolatePositions(lastMessage.time, serverMessage.time, lastMessage.playerTwoYPos, serverMessage.playerTwoYPos);
			ball->InterpolatePositions(lastMessage.time, serverMessage.time, lastMessage.ballXPos, lastMessage.ballYPos, serverMessage.ballXPos, serverMessage.ballYPos);

			// Update the score if necessary
			player1Score = serverMessage.playerOneScore;
			player2Score = serverMessage.playerTwoScore;

		}

		// Update the players
		player->Update();
		player2->Update();

		// Update the ball
		ball->Update();

		// Handle collisions between objects in a seperate function to keep main clean
		HandleCollisions(player, player2, ball, topBorder, bottomBorder);

		// Increase the player scores according to who won a round
		// If the host player won
		if (ball->GetXPosition() > 800)
		{

			player1Score++;

			// Reset object positions
			ball->SetPosition(ballStartX, ballStartY);
			player->SetPosition(playerStartY);
			player2->SetPosition(playerStartY);

			std::cout << "Score is: " << player1Score << " to " << player2Score << ".\n";

		}

		// Else if the client player won
		if (ball->GetXPosition() < 0)
		{

			player2Score++;

			// Reset object positions
			ball->SetPosition(ballStartX, ballStartY);
			player->SetPosition(playerStartY);
			player2->SetPosition(playerStartY);

			std::cout << "Score is: " << player1Score << " to " << player2Score << ".\n";

		}

		// Clear the window for rendering
		window.clear();

		// Render the players
		player->Render(&window);
		player2->Render(&window);

		// Render the ball
		ball->Render(&window);

		// Render the borders
		window.draw(topBorder);
		window.draw(bottomBorder);

		// Display the render
		window.display();

		// Send the relevant message
		if (isServer == true)
		{

			// Construct the message to send to the client
			serverMessage.playerOneYPos = player->GetYPos();
			serverMessage.playerTwoYPos = player2->GetYPos();
			serverMessage.ballXPos = ball->GetXPosition();
			serverMessage.ballYPos = ball->GetYPosition();
			serverMessage.playerOneScore = player1Score;
			serverMessage.playerTwoScore = player2Score;
			serverMessage.time = time;

			if (socket.send((const char *)&serverMessage, sizeof(ServerMessage), clientAddress, clientPort) != sf::Socket::Done)
			{

				// Output error message
				std::cout << "Didn't send message to client\n";

			}

		}
		else
		{

			// Construct the message to the server
			clientMessage.movement = player2->ReturnInput();

			// Redefine the address and port of the server - SFML will forget them otherwise
			serverAddress = "127.0.0.1";
			serverPort = 4444;

			if (socket.send((const char *)&clientMessage, sizeof(ClientMessage), serverAddress, serverPort) != sf::Socket::Done)
			{

				std::cout << "Sending client message failed...\n";

			}

		}

	}

	return 0;

}

void HandleCollisions(Player* player1, Player* player2, Ball* ball, sf::RectangleShape &topBorder, sf::RectangleShape &bottomBorder)
{

	// Set up bounding boxes for collision detection
	sf::FloatRect topBorderBox = topBorder.getGlobalBounds();
	sf::FloatRect bottomBorderBox = bottomBorder.getGlobalBounds();
	sf::FloatRect playerBox = player1->GetShape()->getGlobalBounds();
	sf::FloatRect player2Box = player2->GetShape()->getGlobalBounds();
	sf::FloatRect ballBox = ball->GetShape()->getGlobalBounds();

	// If the player collides with the top, stop them from moving
	if (playerBox.intersects(topBorderBox))
	{

		player1->SetPosition(4);

	}

	// Do the same for the bottom box
	if (playerBox.intersects(bottomBorderBox))
	{

		player1->SetPosition(356);

	}

	// If the second player collides with the top, stop them from moving
	if (player2Box.intersects(topBorderBox))
	{

		player2->SetPosition(4);

	}

	// Do the same for the bottom box
	if (player2Box.intersects(bottomBorderBox))
	{

		player2->SetPosition(356);

	}

	// If the ball collides with the top, stop them from moving past the border
	if (ballBox.intersects(topBorderBox))
	{

		ball->SetPosition(ball->GetXPosition(), 4);
		ball->SetSpeed(ball->GetXSpeed(), 0 - ball->GetYSpeed());

	}

	// Do the same for the bottom box
	if (ballBox.intersects(bottomBorderBox))
	{

		ball->SetPosition(ball->GetXPosition(), 392);
		ball->SetSpeed(ball->GetXSpeed(), 0 - ball->GetYSpeed());

	}

	// Do the same for the player
	if (ballBox.intersects(playerBox))
	{

		ball->SetPosition(ball->GetXPosition() + 1, ball->GetYPosition());
		ball->SetSpeed(0 - ball->GetXSpeed(), ball->GetYSpeed());

	}

	// Do the same for the second player
	if (ballBox.intersects(player2Box))
	{

		ball->SetPosition(ball->GetXPosition() - 1, ball->GetYPosition());
		ball->SetSpeed(0 - ball->GetXSpeed(), ball->GetYSpeed());

	}

}