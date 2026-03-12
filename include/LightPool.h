#include <iostream>
#include <stdexcept>
#pragma once
class ManaPool {
   private:
	int red;
	int blue;
	int green;

   public:
	ManaPool(const int r = 0, const int b = 0, const int g = 0) : red(r), blue(b), green(g) {}

	void addMana(const int r, const int b, const int g);
	void addManaByColor(char color, int amount = 1);
	void clearPool();

	bool canAfford(const int costR, const int costB, const int costG, const int costGeneric) const;

	void spendMana(const int costR, const int costB, const int costG, const int costGeneric);
	// check canAfford before
	// for generic We drain Green first, then Blue, then Red (arbitrary priority).

	friend std::ostream& operator<<(std::ostream& os, const ManaPool& pool) {
		os << "[Mana: " << pool.red << "R, " << pool.blue << "B, " << pool.green << "G]";
		return os;
	}
};
