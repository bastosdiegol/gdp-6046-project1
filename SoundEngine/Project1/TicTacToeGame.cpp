#include <iostream>

#include "TicTacToeGame.h"

TicTacToeGame::TicTacToeGame(FModManager* fModManager) {
	m_fModManager = fModManager;
	m_currentTurnPlayer = 'X';
	m_currentRound = 1;
	m_isGameOver = false;
	m_isItADrawn = false;
}

void TicTacToeGame::newGame() {
	m_fModManager->playSound("New Game","ch2 fx");
	// Sets all variables to ' '
	for (int iLine = 0; iLine < 3; iLine++) {
		for (int iColumn = 0; iColumn < 3; iColumn++) {
			m_board[iLine][iColumn] = ' ';
		}
	}
	// TicTacToe first player is always the X
	m_currentTurnPlayer = 'X';
	m_currentRound = 1;
	m_isGameOver = false;
	m_isItADrawn = false;
	std::cout << "It's a new game. Good lucky to both players!" << std::endl;
	std::cout << "Use your keyboard to place pieces on your turn." << std::endl;
	std::cout << " Q | W | E" << std::endl;
	std::cout << "----------" << std::endl;
	std::cout << " A | S | D" << std::endl;
	std::cout << "----------" << std::endl;
	std::cout << " Z | X | C" << std::endl;
	std::cout << "The Game Board:" << std::endl;
	printBoard();
	std::cout << "TicTacToe first player is always the X! Now, make your move!" << std::endl;
	m_fModManager->stopSound("ch1 music");
	int randomInt = randInt(1,3);
	switch (randomInt) {
	case 1:
		m_fModManager->playSound("Moonlight Sonata", "ch1 music");
		break;
	case 2:
		m_fModManager->playSound("Dance of the Sugar Plum Fairy", "ch1 music");
		break;
	case 3:
		m_fModManager->playSound("Winter Movement 1", "ch1 music");
		break;
	}
}

void TicTacToeGame::printBoard() {
	std::cout << "     |     |     " << std::endl;
	std::cout << "  " << m_board[0][0] << "  |  " << m_board[0][1] << "  |  " << m_board[0][2] << "  " << std::endl;
	std::cout << "_ _ _|_ _ _|_ _ _" << std::endl;
	std::cout << "     |     |     " << std::endl;
	std::cout << "  " << m_board[1][0] << "  |  " << m_board[1][1] << "  |  " << m_board[1][2] << "  " << std::endl;
	std::cout << "_ _ _|_ _ _|_ _ _" << std::endl;
	std::cout << "     |     |     " << std::endl;
	std::cout << "  " << m_board[2][0] << "  |  " << m_board[2][1] << "  |  " << m_board[2][2] << "  " << std::endl;
	std::cout << "     |     |     " << std::endl;
}

bool TicTacToeGame::makeAMove(char position) {
	// We grab the position and insert in a case
	// If the position equivalent to the position input is empty fill it with the player
	// Else we return false to inform its not a valid move
	switch (position) {
	case 'q': 
		if (m_board[0][0] == ' ')
			m_board[0][0] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 'w' :
		if (m_board[0][1] == ' ')
			m_board[0][1] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 'e' :
		if (m_board[0][2] == ' ')
			m_board[0][2] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 'a' :
		if (m_board[1][0] == ' ')
			m_board[1][0] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 's' :
		if (m_board[1][1] == ' ')
			m_board[1][1] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 'd' :
		if (m_board[1][2] == ' ')
			m_board[1][2] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 'z' :
		if (m_board[2][0] == ' ')
			m_board[2][0] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 'x' :
		if (m_board[2][1] == ' ')
			m_board[2][1] = m_currentTurnPlayer;
		else
			return false;
		break;
	case 'c':
		if (m_board[2][2] == ' ')
			m_board[2][2] = m_currentTurnPlayer;
		else
			return false;
		break;
	}
	//std::string moveFx = m_currentRound;
	m_fModManager->playSound("Move Piece "+std::to_string(m_currentRound),"ch2 fx");
	// Switch the current player for next turn
	m_currentTurnPlayer == 'X' ? m_currentTurnPlayer = 'O' : m_currentTurnPlayer = 'X';
	// Increments turn counter for FX
	m_currentRound++;
	return true;
}

void TicTacToeGame::nextTurn() {
	// Checks if the game is over
	if (isGameOver()) {
		// Checks if it was a drawn
		if (m_isItADrawn) {
			// Print the Winning/Last Board
			printBoard();
			std::cout << "The game is over! It's a drawn! Press (N) to start a new game." << std::endl;
			m_fModManager->playSound("Drawn Game", "ch2 fx");
			return;
		}
		// Print the Winning/Last Board
		printBoard();
		// Grabs last turn player
		m_currentTurnPlayer == 'X' ? m_currentTurnPlayer = 'O' : m_currentTurnPlayer = 'X';
		// Winning Game Message
		std::cout << "Player " << m_currentTurnPlayer << " won! Congratulations!" << std::endl;
		m_fModManager->playSound("Victory Melody","ch2 fx");
		std::cout << "Press (N) to start a new game." << std::endl;
		return;
	}
	std::cout << "New turn! ... " << std::endl;
	printBoard();
	std::cout << "Now is the time for player " << m_currentTurnPlayer << " to make a move." << std::endl;
}

bool TicTacToeGame::isGameOver() {
	// Checks for game winning condition on lines
	for (int iLine = 0; iLine < 3; iLine++) {
		if ( ( (m_board[iLine][0] & m_board[iLine][1] & m_board[iLine][2]) == 'X'
			|| (m_board[iLine][0] & m_board[iLine][1] & m_board[iLine][2]) == 'O')
			&&  m_board[iLine][0] != ' ')
			m_isGameOver = true;
	}
	// Checks for game winning condition on columns
	for (int iColumn = 0; iColumn < 3; iColumn++) {
		if ( ( (m_board[0][iColumn] & m_board[1][iColumn] & m_board[2][iColumn]) == 'X'
			|| (m_board[0][iColumn] & m_board[1][iColumn] & m_board[2][iColumn]) == 'O')
			&&  m_board[0][iColumn] != ' ')
			m_isGameOver = true;
	}
	// Checks for game winning condition on diagonals
	if ( ( (m_board[0][0] & m_board[1][1] & m_board[2][2]) == 'X'
		|| (m_board[0][0] & m_board[1][1] & m_board[2][2]) == 'O')
		&&  m_board[0][0] != ' ')
		m_isGameOver = true;
	if ( ( (m_board[0][2] & m_board[1][1] & m_board[2][0]) == 'X'
		|| (m_board[0][2] & m_board[1][1] & m_board[2][0]) == 'O')
		&&  m_board[0][0] != ' ')
		m_isGameOver = true;
	// Checks for game ending condition - DRAWN
	// First looks for empty slot
	bool foundEmptySpot = false;
	for (int iLine = 0; iLine < 3; iLine++) {
		for (int iColumn = 0; iColumn < 3; iColumn++) {
			if (m_board[iLine][iColumn] == ' ')
				foundEmptySpot = true;
		}
	}
	// Found winning condition?
	if (m_isGameOver)
		return m_isGameOver;
	// If empty slot wasn't found - it's a drawn
	if (!foundEmptySpot) {
		m_isGameOver = true;
		m_isItADrawn = true;
	}
	return m_isGameOver;
}

int randInt(int min, int max) {
	if (max == min)
		return 0;

	int diff = (max - min) + 1;
	return min + (rand() % diff);
}
