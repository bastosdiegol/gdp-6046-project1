#include <iostream>

#include "TicTacToeGame.h"

TicTacToeGame::TicTacToeGame() {
	newGame();
}

void TicTacToeGame::newGame() {
	for (int iLine = 0; iLine < 3; iLine++) {
		for (int iColumn = 0; iColumn < 3; iColumn++) {
			m_board[iLine][iColumn] = ' ';
		}
	}
	m_currentTurnPlayer = 'X';
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

bool TicTacToeGame::makeAMove(char player, char position) {
	// We grab the position and insert in a case
	// If the position equivalent to the position input is empty fill it with the player
	// Else we return false to inform its not a valid move
	switch (position) {
	case 'q': 
		if (m_board[0][0] == ' ')
			m_board[0][0] = player;
		else
			return false;
	case 'w' :
		if (m_board[0][1] == ' ')
			m_board[0][1] = player;
		else
			return false;
	case 'e' :
		if (m_board[0][2] == ' ')
			m_board[0][2] = player;
		else
			return false;
	case 'a' :
		if (m_board[1][0] == ' ')
			m_board[1][0] = player;
		else
			return false;
	case 's' :
		if (m_board[1][1] == ' ')
			m_board[1][1] = player;
		else
			return false;
	case 'd' :
		if (m_board[1][2] == ' ')
			m_board[1][2] = player;
		else
			return false;
	case 'z' :
		if (m_board[2][0] == ' ')
			m_board[2][0] = player;
		else
			return false;
	case 'x' :
		if (m_board[2][1] == ' ')
			m_board[2][1] = player;
		else
			return false;
	case 'c':
		if (m_board[2][2] == ' ')
			m_board[2][2] = player;
		else
			return false;
	}

	// Switch the current player
	m_currentTurnPlayer == 'X' ? 'O' : 'X';
	return true;
}
