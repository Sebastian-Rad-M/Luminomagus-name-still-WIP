#include "LightPool.h"
#include <cctype>
void ManaPool::addMana(int r, int b, int g) {
	red += r;
	blue += b;
	green += g;
}
void ManaPool::addManaByColor(char color, int amount) {
    color = std::toupper(color);
    switch (color) {
        case 'R': 
            this->red += amount; break;
        case 'B': this->blue += amount; break;
        case 'G': this->green += amount; break;
        default:std::cout << "  [!] System Error: Tried to add unknown mana color '" << color << "'\n";break;
    }//TODO: error handling
}
void ManaPool::clearPool() {
	red = 0;
	blue = 0;
	green = 0;
}

bool ManaPool::canAfford(const int costR, const int costB, const int costG,
						 const int costGeneric) const {
	if (red < costR || blue < costB || green < costG) {
		return false;
	}
	const int leftoverR = red - costR;
	const int leftoverB = blue - costB;
	const int leftoverG = green - costG;
	const int totalLeftover = leftoverR + leftoverB + leftoverG;
	return totalLeftover >= costGeneric;
}

// TODO: check if can afford first
void ManaPool::spendMana(const int costR, const int costB, const int costG, const int costGeneric) {
	red -= costR;
	blue -= costB;
	green -= costG;
	/// We drain Green first, then Blue, then Red (arbitrary priority).
	/// TODO: player chosen
	int genericRemaining = costGeneric;
	// is this overkill? yes
	int* const vars[3] = {&green, &blue, &red};
	for (int i = 0; i < 3; i++) {
		while (genericRemaining > 0 && *vars[i] > 0) {
			(*vars[i])--;
			genericRemaining--;
		}
	}
}
