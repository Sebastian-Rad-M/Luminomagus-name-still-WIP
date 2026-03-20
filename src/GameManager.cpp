#include "GameManager.h"
#include "BossFactory.h"
#include "RelicFactory.h"//DEBUG

GameManager& GameManager::instance() {
	static GameManager inst;
	return inst;
}

void GameManager::run() {
	state = GameState::MAIN_MENU;
	while (state != GameState::GAME_OVER) {
		switch (state) {
			case GameState::MAIN_MENU:
				view.showMainMenu(state, activeRun);
				break;
			case GameState::DRAFT:

			// --- DEBUG HOOK: START WITH CODEX ---
				if (activeRun.getPlayer().getRelicZone().getRelicZone().empty()) {
					activeRun.getPlayer().getRelicZone().addRelic(
						RelicDatabase::getInstance().getRelic("r_undescifrable_codex")
					);
                   }
				// ------------------------------------

				view.showDraft(state, activeRun);
				round.emplace(activeRun, &view);
				round->setupDeck(activeRun.getPlayer().getDeck(),activeRun.getPlayer().getRelicZone());
				
				round->startNewRound();
				 break;

			case GameState::COMBAT:
				view.showCombat(state, activeRun, round.value(), playerWon);
				break;

			case GameState::SHOP:
				view.showShop(state, activeRun);
				if (state == GameState::COMBAT) { 
					round.emplace(activeRun, &view);
					round->setupDeck(activeRun.getPlayer().getDeck(),activeRun.getPlayer().getRelicZone());
					int cr = activeRun.getCurrentRound();
					if (cr == 3 || cr == 6 || cr == 9) round->addStatus(BossFactory::getRandomBoss('R'));
						else if (cr == 10) round->addStatus(BossFactory::getRandomBoss('E'));
					round->startNewRound();
				}
				break;

			default:
				break;
		}
	}

	view.showGameOver(playerWon, activeRun);
}
