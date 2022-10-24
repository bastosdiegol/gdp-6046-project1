#pragma once
#include <vector>
#include "FModManager.h"
#include "Localization.h"

class TicTacToeGame {
public:
	// Game Board
	char m_board[3][3];
	char m_currentTurnPlayer;
	unsigned short m_currentRound; // We gonna use this variable to check last round drawn condition
	bool m_isGameOver;
	bool m_isItADrawn;
	FModManager* m_fModManager;
	Localization* m_localization;

	// Default Constructor
	TicTacToeGame(FModManager* fModManager, Localization* localization);
	// Starts a new game - reset the board
	void newGame();
	// Console Output for the board and its pieces
	void printBoard();
	// Saves a move made by a player
	// Accepts char player and char position intended to move
	// Q W E A S D Z X C will be used as input for each position of the board
	// Returns true is move was made false if not
	bool makeAMove(char position);
	// Method that initiates another turn IF possible
	void nextTurn();
	// Method that checks if the game is over
	bool isGameOver();
	// Method that exports the current state of the game to a XML file
	void saveGame();
	// Method that imports an existing state of the game and let you continue the gameplay
	void loadGame();

private:
	const std::string	SAVE_FILE = "save.xml";
};

// Utility function for a random range of two ints
int randInt(int min, int max);

