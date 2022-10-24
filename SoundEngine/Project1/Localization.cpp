#include "Localization.h"
#include <pugixml/pugixml.hpp>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#include <iostream>
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

Localization::Localization() {
	DEBUG_PRINT("Localization::Localization()\n");
	// Create a document object
	pugi::xml_document localization;
	// Load a xml file into the object
	pugi::xml_parse_result result = localization.load_file(LOCALIZATION_FILE.c_str());
	if (!result) {
		DEBUG_PRINT("error: Failed to load file named #%s", LOCALIZATION_FILE.c_str());
		return;
	}
	DEBUG_PRINT("Successfully loaded file named #%s\n", LOCALIZATION_FILE.c_str());

	// Gets all nodes of sound inside the localization
	pugi::xml_node messages = localization.child("localization");
	// For each message ... iterate
	for (pugi::xml_node message = messages.child("message");
						message;
						message = message.next_sibling("message")) {
		m_vLanguagesAvailable.push_back(message.attribute("lang").value());
		// Checks if there's no current language
		if (m_currentLanguage.empty()) {
			m_currentLanguage = message.attribute("lang").value();

			pugi::xml_node messageNode = *message.children().begin();
			m_title				= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newGame			= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_instruction		= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_gameBoard			= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_firstPlayerMove	= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_gameOver			= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_gameOverDrawn		= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_congratulations	= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newGameInfo		= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newTurn			= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newTurnPlayer		= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_noSaveFile		= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_noSaveInformation = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_saveGame			= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_loadGame			= messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_quit				= messageNode.child_value();
		}
	}
}

void Localization::setLanguage(std::string lang) {
	DEBUG_PRINT("Localization::setLanguage(%s)\n", lang.c_str());

	// Checks if the parameter language passed by is available on the XML
	for (int i = 0; i < m_vLanguagesAvailable.size(); i++) {
		if (m_vLanguagesAvailable[i] == lang) {
			m_currentLanguage = lang;
			break;
		}
	}

	// Create a document object
	pugi::xml_document localization;
	// Load a xml file into the object
	pugi::xml_parse_result result = localization.load_file(LOCALIZATION_FILE.c_str());
	if (!result) {
		DEBUG_PRINT("error: Failed to load file named #%s", LOCALIZATION_FILE.c_str());
		return;
	}
	DEBUG_PRINT("Successfully loaded file named #%s\n", LOCALIZATION_FILE.c_str());

	// Gets all nodes of sound inside the localization
	pugi::xml_node messages = localization.child("localization");

	for (pugi::xml_node message = messages.child("message");
		message;
		message = message.next_sibling("message")) {\
		// Checks if there's no current language
		if (strcmp(m_currentLanguage.c_str(), message.attribute("lang").value()) == 0) {
			pugi::xml_node messageNode = *message.children().begin();
			m_title = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newGame = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_instruction = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_gameBoard = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_firstPlayerMove = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_gameOver = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_gameOverDrawn = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_congratulations = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newGameInfo = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newTurn = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_newTurnPlayer = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_noSaveFile = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_noSaveInformation = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_saveGame = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_loadGame = messageNode.child_value(); messageNode = messageNode.next_sibling();
			m_quit = messageNode.child_value();
		}
	}
}
