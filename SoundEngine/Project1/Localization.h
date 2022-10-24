#pragma once
#include <string>
#include <vector>

class Localization {
public:

	std::vector<std::string> m_vLanguagesAvailable;
	const std::string LOCALIZATION_FILE = "locales.xml";
	std::string m_currentLanguage;

	std::string m_title;
	std::string m_newGame;
	std::string m_instruction;
	std::string m_gameBoard;
	std::string m_firstPlayerMove;
	std::string m_gameOver;
	std::string m_gameOverDrawn;
	std::string m_congratulations;
	std::string m_newGameInfo;
	std::string m_newTurn;
	std::string m_newTurnPlayer;
	std::string m_noSaveFile;
	std::string m_noSaveInformation;
	std::string m_saveGame;
	std::string m_loadGame;
	std::string m_quit;

	// Constructor
	Localization();
	// Method to set a language to be used 
	void setLanguage(std::string lang);
};