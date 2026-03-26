#include "GameManager.h"
#include <RmlUi/Core.h>
#include <raylib.h>
#include "RaylibRenderInterface.h"
#include "RaylibSystemInterface.h"
#include "RaylibInput.h"
#include "CardDatabase.h"
#include "BossFactory.h"
#include "RelicFactory.h"
#include "MainMenuUI.h"
#include "DraftMenuUI.h"
#include "ShopUI.h"
#include "CombatUI.h"
#include "UIHelpers.h"
//debug
#include <RmlUi/Debugger.h>

GameManager& GameManager::instance() {static GameManager inst;return inst;}

void GameManager::run() {

	// RAYLIB
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
	InitWindow(1280, 720, "Luminomagus");
	SetTargetFPS(60);

	//SHADRES 
	Shader voidShader = LoadShader(0, "assets/Void.fs");
	int timeLoc = GetShaderLocation(voidShader, "u_time");
	int resLoc = GetShaderLocation(voidShader, "u_resolution");

	// RMLUI
	RaylibRenderInterface renderInterface;
	RaylibSystemInterface systemInterface;
	Rml::SetRenderInterface(renderInterface.GetAdaptedInterface());
	Rml::SetSystemInterface(&systemInterface);
	Rml::Initialise();
	Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(GetScreenWidth(), GetScreenHeight()));
	if (!context) { CloseWindow(); return; }
	Rml::LoadFontFace("assets/ARIAL.ttf");
	Rml::LoadFontFace("assets/CinzelDecorative-Regular.ttf");
	//debug
	Rml::Debugger::Initialise(context);
	Rml::Debugger::SetVisible(true);

	Rml::ElementDocument* docMainMenu = context->LoadDocument("assets/mainMenu.rml");
	Rml::ElementDocument* docDraft    = context->LoadDocument("assets/draft.rml");
	Rml::ElementDocument* docCombat   = context->LoadDocument("assets/combat.rml");
	Rml::ElementDocument* docShop     = context->LoadDocument("assets/shop.rml");
	Rml::ElementDocument* docPostRun  = context->LoadDocument("assets/postRun.rml");

	MainMenuListener menuListener;
	DraftListener    draftListener;
	CombatListener   combatListener;
	ShopListener     shopListener;

	if (docMainMenu) {
		if (auto el = docMainMenu->GetElementById("btn-new-run")) el->AddEventListener(Rml::EventId::Click, &menuListener);
		if (auto el = docMainMenu->GetElementById("btn-quit")) el->AddEventListener(Rml::EventId::Click, &menuListener);
		docMainMenu->Show();
	}

	if (docPostRun) {
		if (auto el = docPostRun->GetElementById("btn-post-restart")) el->AddEventListener(Rml::EventId::Click, &menuListener);
		if (auto el = docPostRun->GetElementById("btn-post-menu")) el->AddEventListener(Rml::EventId::Click, &menuListener);
	}

	if (docShop) if (auto el = docShop->GetElementById("btn-leave-shop"))el->AddEventListener(Rml::EventId::Click, &shopListener);
	GameState previousState = state;

	// ===== MAIN FRAME LOOP =====
	while (!WindowShouldClose()) {
		GameState currentState = state;

		if (currentState != previousState) {
			if (previousState == GameState::MAIN_MENU && docMainMenu) docMainMenu->Hide();
			if (previousState == GameState::DRAFT    && docDraft)    docDraft->Hide();
			if (previousState == GameState::COMBAT   && docCombat)   docCombat->Hide();
			if (previousState == GameState::SHOP     && docShop)     docShop->Hide();
			if (previousState == GameState::POST_RUN && docPostRun)  docPostRun->Hide();

			if (previousState == GameState::DRAFT && currentState == GameState::COMBAT) {
				// debug relic
				if (activeRun.getPlayer().getRelicZone().getRelicZone().empty()) activeRun.getPlayer().getRelicZone().addRelic(RelicDatabase::getInstance().getRandomRareOrLegendaryRelic());
				round.emplace(activeRun);
				round->setupDeck(activeRun.getPlayer().getDeck(), activeRun.getPlayer().getRelicZone());
				round->startNewRound();
			}
			if (previousState == GameState::SHOP && currentState == GameState::COMBAT) {
				round.emplace(activeRun);
				round->setupDeck(activeRun.getPlayer().getDeck(), activeRun.getPlayer().getRelicZone());
				int cr = activeRun.getCurrentRound();
				if (cr == 3 || cr == 6 || cr == 9) round->addStatus(BossFactory::getRandomBoss('R'));
				if (cr == 10) round->addStatus(BossFactory::getRandomBoss('E'));
				round->startNewRound();
			}
			if (currentState == GameState::MAIN_MENU && docMainMenu)docMainMenu->Show();
			else if (currentState == GameState::DRAFT && docDraft) {
				draftListener.startDraft(docDraft);
				docDraft->Show();
			}
			else if (currentState == GameState::COMBAT && docCombat) {
				combatListener.startCombat(docCombat);
				docCombat->Show();
			}
			else if (currentState == GameState::SHOP && docShop) {
				shopListener.startShop(docShop);
				docShop->Show();
			}
			else if (currentState == GameState::POST_RUN && docPostRun) {
				if (auto title = docPostRun->GetElementById("post-run-title")) {
					if (playerWon) {
						title->SetInnerRML("Victory!");
						title->SetClass("win", true);
						title->SetClass("lose", false);
					} else {
						title->SetInnerRML("Game Over!");
						title->SetClass("lose", true);
						title->SetClass("win", false);
					}
				}
				docPostRun->Show();
			}
			//Q should this not be a break?
			else if (currentState == GameState::GAME_OVER) break;

			previousState = currentState;
		}

		UpdateRmlInput(context);
		context->Update();

		BeginDrawing();
		ClearBackground(BLACK); // Keep black as a fallback

		// --- RENDER THE VOID SHADER ---
		float time = (float)GetTime();
		float resolution[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
		
		SetShaderValue(voidShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
		SetShaderValue(voidShader, resLoc, resolution, SHADER_UNIFORM_VEC2);

		BeginShaderMode(voidShader);
		// Draw a blank white rectangle over the whole screen; the shader will paint the fog onto it
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE); 
		EndShaderMode();
		// ------------------------------

		context->Render(); // RmlUi (your main menu) draws on top of the void
		EndDrawing();
		if (IsWindowResized()) context->SetDimensions(Rml::Vector2i(GetScreenWidth(), GetScreenHeight()));
	}

	Rml::Shutdown();
	CloseWindow();
}
