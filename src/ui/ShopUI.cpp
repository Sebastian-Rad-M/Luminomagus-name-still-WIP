#include "ui/ShopUI.h"
#include "ui/UIHelpers.h"
#include "GameManager.h"
#include "backend/ActiveRun.h"
#include "backend/Shop.h"
#include "backend/PlayerInfo.h"
#include "backend/RelicZone.h"
#include "backend/Relic.h"

void ShopListener::startShop(Rml::ElementDocument* doc) {
    shopDoc = doc;
    
    // Load the template ONLY ONCE when the shop starts
    if (cardTemplateRML.empty()) {
        cardTemplateRML = UIHelpers::LoadFileToString("assets/card.rml");
    }

    refreshUI();
}

void ShopListener::refreshUI() {
    if (!shopDoc) return;
    auto& activeRun = GameManager::instance().getActiveRun();
    auto& shop = activeRun.getShop();
    auto& player = activeRun.getPlayer();
    
    if (auto el = shopDoc->GetElementById("player-gold")) {
        el->SetInnerRML("Gold: " + std::to_string(player.getGold()) + "G");
    }

    // Render Card Stock
    auto cardContainer = shopDoc->GetElementById("card-stock");
    if (cardContainer) {
        std::string cardHtml;
        const auto& cards = shop.getCards();
        
        for (size_t i = 0; i < cards.size(); i++) {
            if (cards[i].isSold) {
                cardHtml += "<div class=\"shop-item sold\">[SOLD OUT]</div>";
            } else {
                // Dereference .item because cards[i] is a ShopItem struct containing the Card pointer
                std::string cardVisuals = UIHelpers::GenerateCardRML(cardTemplateRML, *cards[i].item);
                
                // Use 'buy-card-' so the ProcessEvent handler catches the click
                cardHtml += "<div class=\"card-wrapper\" id=\"buy-card-" + std::to_string(i) + "\">\n"
                            + cardVisuals 
                            + "\n<div style=\"text-align: center; margin-top: 5px; color: gold;\">" 
                            + std::to_string(cards[i].price) + "G</div>\n"
                            + "</div>";
            }
        }
        cardContainer->SetInnerRML(cardHtml);
    }

    // Render Relic Stock
    auto relicContainer = shopDoc->GetElementById("relic-stock");
    if (relicContainer) {
        std::string relicHtml;
        const auto& relics = shop.getRelics();
        for (size_t i = 0; i < relics.size(); i++) {
            if (relics[i].isSold) {
                relicHtml += "<div class=\"shop-item sold\">[SOLD OUT]</div>";
            } else {
                relicHtml += "<button class=\"shop-item\" id=\"buy-relic-" + std::to_string(i) + "\">" 
                         + relics[i].item->getName() + " (" + std::to_string(relics[i].price) + "G)</button>";
            }
        }
        relicContainer->SetInnerRML(relicHtml);
    }

    // Render Player Relics for Selling
    auto sellContainer = shopDoc->GetElementById("player-relics-sell");
    if (sellContainer) {
        std::string sellHtml;
        const auto& pRelics = player.getRelicZone().getRelicZone();
        for (size_t i = 0; i < pRelics.size(); i++) {
            int price = Shop::calculatePrice(pRelics[i]->getRarity());
            sellHtml += "<button class=\"sell-btn\" id=\"sell-relic-" + std::to_string(i) + "\">" 
                     + pRelics[i]->getName() + " (+" + std::to_string(price) + "G)</button>";
        }
        sellContainer->SetInnerRML(sellHtml);
    }

    // Re-attach listeners to all generated buttons
    attachItemListeners("buy-card-", shop.getCards().size());
    attachItemListeners("buy-relic-", shop.getRelics().size());
    attachItemListeners("sell-relic-", player.getRelicZone().getRelicZone().size());
}

void ShopListener::attachItemListeners(const std::string& prefix, size_t count) {
    for (size_t i = 0; i < count; i++) {
        auto btn = shopDoc->GetElementById(prefix + std::to_string(i));
        if (btn) btn->AddEventListener(Rml::EventId::Click, this);
    }
}

void ShopListener::ProcessEvent(Rml::Event& event) {
    std::string id = event.GetCurrentElement()->GetId();
    auto& activeRun = GameManager::instance().getActiveRun();
    auto& shop = activeRun.getShop();
    auto& player = activeRun.getPlayer();

    if (id == "btn-leave-shop") {
        GameManager::instance().setState(GameState::COMBAT);
    }
    else if (id.find("buy-card-") == 0) {
        shop.buyCard(std::stoi(id.substr(9)), player);
        refreshUI();
    }
    else if (id.find("buy-relic-") == 0) {
        shop.buyRelic(std::stoi(id.substr(10)), player);
        refreshUI();
    }
    else if (id.find("sell-relic-") == 0) {
        int idx = std::stoi(id.substr(11));
        const auto& pRelics = player.getRelicZone().getRelicZone();
        player.addGold(Shop::calculatePrice(pRelics[idx]->getRarity()));
        player.getRelicZone().removeRelic(idx);
        refreshUI();
    }
}