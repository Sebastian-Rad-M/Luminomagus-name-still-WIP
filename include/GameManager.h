#pragma once
#include <optional>

#include "ActiveRun.h"
#include "RoundTracker.h"
#include "View.h"

class GameManager {
   private:
	GameManager() = default;
	ActiveRun activeRun;
	std::optional<RoundTracker> round;		 // if not in round, it no exist
	GameState state = GameState::MAIN_MENU;	 // we start on main menu
	bool playerWon = false;
	View view;

   public:
	static GameManager& instance();
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	// i :heart_emoji: singletons

	void run();
};
