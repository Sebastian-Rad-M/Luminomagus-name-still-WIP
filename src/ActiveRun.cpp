#include "ActiveRun.h"

void ActiveRun::finishRoundSequence() {
	player.addGold(calcTargetScore() / 10);
	roundsWon++;
	currentRound++;
	currentShop.generateRandomStock();
}

int ActiveRun::calcTargetScore() const { return 10 + (currentRound - 1) * 10; }	 // formula
void ActiveRun::resetRun(const PlayerInfo& p) {
	currentRound = 1;
	roundsWon = 0;
	player = p;
}

void ActiveRun::printStatus() const {
	std::cout << "  Run   : Round " << currentRound << "  (won: " << roundsWon << ")\n";
}
int ActiveRun::getCurrentRound() const { return currentRound; }

PlayerInfo& ActiveRun::getPlayer() { return player; }
Shop& ActiveRun::getShop() { return currentShop; }
