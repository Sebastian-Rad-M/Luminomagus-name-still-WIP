#include "GameManager.h"
#ifdef _WIN32		  // useless
#include <windows.h>  //useless
#endif				  // useless

int main() {
#ifdef _WIN32					  // useless
	SetConsoleOutputCP(CP_UTF8);  // useless
#endif							  // useless
	GameManager::instance().run();
	return 0;
}
// i could have a 4 line main, but nooo, windows console encoding needs babysiting
