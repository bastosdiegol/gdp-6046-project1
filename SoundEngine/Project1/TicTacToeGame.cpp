#include <iostream>
#include <fstream> 
#include <pugixml/pugixml.hpp>

#include "TicTacToeGame.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

TicTacToeGame::TicTacToeGame(FModManager* fModManager, Localization* localization) {
	DEBUG_PRINT("TicTacToeGame::TicTacToeGame()\n");
	m_fModManager = fModManager;
	m_localization = localization;
	m_currentTurnPlayer = 'X';
	m_currentRound = 1;
	m_isGameOver = false;
	m_isItADrawn = false;
}

void TicTacToeGame::newGame() {
	DEBUG_PRINT("TicTacToeGame::newGame()\n");
	m_fModManager->stopSound("ch0 master");
	m_fModManager->stopSound("ch1 music");
	m_fModManager->stopSound("ch2 fx");
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
	system("cls"); // Clear Screen
	std::cout << m_localization->m_newGame << std::endl;
	std::cout << m_localization->m_instruction << std::endl;
	std::cout << " Q | W | E" << std::endl;
	std::cout << "----------" << std::endl;
	std::cout << " A | S | D" << std::endl;
	std::cout << "----------" << std::endl;
	std::cout << " Z | X | C" << std::endl;
	std::cout << m_localization->m_gameBoard << std::endl;
	printBoard();
	std::cout << m_localization->m_firstPlayerMove << std::endl;
	int randomInt = randInt(1,3);
	switch (randomInt) {
	case 1:
		//m_fModManager->playSound("Moonlight Sonata", "ch1 music");
		break;
	case 2:
		//m_fModManager->playSound("Dance of the Sugar Plum Fairy", "ch1 music");
		break;
	case 3:
		//m_fModManager->playSound("Winter Movement 1", "ch1 music");
		break;
	}
}

void TicTacToeGame::printBoard() {
	DEBUG_PRINT("TicTacToeGame::printBoard()\n");
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
	DEBUG_PRINT("TicTacToeGame::makeAMove(%c)\n", position);
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
	// We play a random Move Piece sound (1 to 12)
	m_fModManager->playSound("Move Piece "+std::to_string(randInt(1, 12)),"ch2 fx");
	// Switch the current player for next turn
	m_currentTurnPlayer == 'X' ? m_currentTurnPlayer = 'O' : m_currentTurnPlayer = 'X';
	// Increments turn counter for draw condition
	m_currentRound++;
	return true;
}

void TicTacToeGame::nextTurn() {
	DEBUG_PRINT("TicTacToeGame::nextTurn()\n");
	system("cls"); // Clear Screen
	// Checks if the game is over
	if (isGameOver()) {
		// Checks if it was a draw
		if (m_isItADrawn) {
			// Print the Winning/Last Board
			std::cout << m_localization->m_gameOver << std::endl;
			printBoard();
			std::cout << m_localization->m_gameOverDrawn << std::endl;
			m_fModManager->playSound("Draw Game", "ch2 fx");
			return;
		}
		std::cout << m_localization->m_gameOver << std::endl;
		// Print the Winning/Last Board
		printBoard();
		// Grabs last turn player
		m_currentTurnPlayer == 'X' ? m_currentTurnPlayer = 'O' : m_currentTurnPlayer = 'X';
		// Winning Game Message
		printf(m_localization->m_congratulations.c_str(), m_currentTurnPlayer);
		std::cout << std::endl;
		m_fModManager->playSound("Victory Melody","ch2 fx");
		std::cout << m_localization->m_newGameInfo << std::endl;
		return;
	}
	std::cout << m_localization->m_newTurn << std::endl;
	printBoard();
	printf(m_localization->m_newTurnPlayer.c_str(), m_currentTurnPlayer);
	std::cout << std::endl;
}

bool TicTacToeGame::isGameOver() {
	DEBUG_PRINT("TicTacToeGame::isGameOver()\n");
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

	// Found winning condition?
	if (m_isGameOver)
		return m_isGameOver;
	// No? Lets check for draw now
	else if (m_isGameOver == false && m_currentRound == 10) {
		m_isItADrawn = true;
		m_isGameOver = true;
	}

	// Since I have the round number no need to loop up for empty slots
	/* First looks for empty slot
	bool foundEmptySpot = false;
	for (int iLine = 0; iLine < 3; iLine++) {
		for (int iColumn = 0; iColumn < 3; iColumn++) {
			if (m_board[iLine][iColumn] == ' ')
				foundEmptySpot = true;
		}
	}
	// If empty slot wasn't found - it's a draw
	if (!foundEmptySpot) {
		m_isGameOver = true;
		m_isItADrawn = true;
	}*/

	return m_isGameOver;
}

/// <summary>
/// To save a new game I'm creating a new document everytime and overwriting it
/// So I won't have to worry about checking its file integrity
/// </summary>
void TicTacToeGame::saveGame() {
	DEBUG_PRINT("TicTacToeGame::saveGame()\n");
	pugi::xml_document saveFile;
	pugi::xml_node saveSlot;
	std::string value;
	//pugi::xml_parse_result result = saveFile.load_file(SOUND_FILE.c_str());
	// File not found - We won't throw an error, we are just going to create a new XML
	
	saveSlot = saveFile.append_child("tttsavefile");
	// For each board position we grab its values
	value = m_board[0][0];
	// And create a new Child on the XML and also Set its Value on the same line
	saveSlot.append_child("q").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[0][1];
	saveSlot.append_child("w").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[0][2];
	saveSlot.append_child("e").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[1][0];
	saveSlot.append_child("a").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[1][1];
	saveSlot.append_child("s").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[1][2];
	saveSlot.append_child("d").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[2][0];
	saveSlot.append_child("z").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[2][1];
	saveSlot.append_child("x").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_board[2][2];
	saveSlot.append_child("c").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = m_currentTurnPlayer;
	saveSlot.append_child("currentTurnPlayer").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = std::to_string(m_currentRound);
	saveSlot.append_child("currentRound").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = std::to_string(m_isGameOver);
	saveSlot.append_child("isGameOver").append_child(pugi::node_pcdata).set_value(value.c_str());
	value = std::to_string(m_isItADrawn);
	saveSlot.append_child("isItADrawn").append_child(pugi::node_pcdata).set_value(value.c_str());
	// Then we finally save the XML file overwriting any if created previously
	saveFile.save_file(SAVE_FILE.c_str());
	// Play Save Game Sound
	m_fModManager->playSound("Save Game", "ch2 fx");
}

void TicTacToeGame::loadGame() {
	DEBUG_PRINT("TicTacToeGame::loadGame()\n");
	pugi::xml_document saveFile;
	pugi::xml_node saveSlot;

	// Load the XML Save File
	pugi::xml_parse_result result = saveFile.load_file(SAVE_FILE.c_str());
	if (result.status == pugi::status_file_not_found) {
		std::cout << m_localization->m_noSaveFile << std::endl;
	}else if (!result) {
		std::cout << "pugi::xml Error description: " << result.description() << std::endl;
		std::cout << "pugi::xml Error offset: " << result.offset << " (error at [..." << (SAVE_FILE.c_str() + result.offset) << std::endl;
	}
	// Checks if there's elements to the XML
	pugi::xml_object_range<pugi::xml_node_iterator> saveInfo = saveFile.child("tttsavefile").children();
	if (saveInfo.empty()) {
		std::cout << m_localization->m_noSaveInformation << std::endl;
		return;
	}
	// Iteration through the childs - save information
	pugi::xml_node_iterator saveIt;
	std::string value;
	std::vector<std::string> saveData; // stores each info on this array
	for (saveIt = saveInfo.begin(); saveIt != saveInfo.end(); saveIt++) {
		pugi::xml_node saveNode = *saveIt;
		value = saveNode.child_value();
		if (!value.empty()) { // If Child is not empty
			saveData.push_back(value);
			DEBUG_PRINT("#%s#\n", saveNode.child_value());
		} else { // Child is empty - Store ' ' instead
			saveData.push_back(" ");
			DEBUG_PRINT("empty\n");
		}
	}
	// Loading TicTacToeGame Member Variables
	m_board[0][0]		= saveData[0].c_str()[0];
	m_board[0][1]		= saveData[1].c_str()[0];
	m_board[0][2]		= saveData[2].c_str()[0];
	m_board[1][0]		= saveData[3].c_str()[0];
	m_board[1][1]		= saveData[4].c_str()[0];
	m_board[1][2]		= saveData[5].c_str()[0];
	m_board[2][0]		= saveData[6].c_str()[0];
	m_board[2][1]		= saveData[7].c_str()[0];
	m_board[2][2]		= saveData[8].c_str()[0];
	m_currentTurnPlayer = saveData[9].c_str()[0];
	m_currentRound		= std::stoi(saveData[10]);
	m_isGameOver		= std::stoi(saveData[11]);
	m_isItADrawn		= std::stoi(saveData[12]);

	// Load Game Sound
	m_fModManager->playSound("Load Game", "ch2 fx");

	// Since we loaded all variables we just call next turn to continue the game
	nextTurn();
}

int randInt(int min, int max) {
	if (max == min)
		return 0;

	int diff = (max - min) + 1;
	return min + (rand() % diff);
}
