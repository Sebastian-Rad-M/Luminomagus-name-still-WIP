#include "RelicFactory.h"
#include "RelicEffects.h"
#include "RNG.h"
RelicDatabase::RelicDatabase() { 
    loadAllRelics(); 
}

RelicDatabase& RelicDatabase::getInstance() {
    static RelicDatabase inst;
    return inst;
}

void RelicDatabase::loadAllRelics() {
    //here we go again
    registry["r_kinnan"] = std::make_shared<OnManaRelic>("Kinnan, Bonder Prodigy", "+1 to all mana ",'R',2);
    registry["r_birgi"] = std::make_shared<OnCastTriggerRelic>("Birgi, God of Storytelling"," spell -> add 1 Red mana.",'C',std::make_unique<AddManaEffect>(1, 0, 0));
    registry["r_guttersnipe_dagger"] = std::make_shared<OnCastTriggerRelic>("Guttersnipe Dagger","spell -> deal 2 damage.",'R',std::make_unique<Score>(2));
    registry["r_archmages_tome"] = std::make_shared<OnCastTriggerRelic>("Archmage's Tome","spell -> draw 1 card.",'R',std::make_unique<DrawCardEffect>(1));
    registry["r_nivs_scale"] = std::make_shared<OnDrawRelic>("Niv's Scale","draw -> gain 2 Score.",'U',std::make_unique<Score>(2));
    registry["r_bloodstone_whetstone"] = std::make_shared<OnDamageRelic>("Bloodstone Whetstone","damage -> deal 2 additional damage",'C',2);
}


std::shared_ptr<IRelic> RelicDatabase::getRelic(const std::string& id) {
    auto it = registry.find(id);
    if (it != registry.end()) {
        return it->second->clone(); 
    }
    std::cerr << "  [!] Relic Database Error: Unknown ID '" << id << "'\n";
    return nullptr;
}

std::shared_ptr<IRelic> RelicDatabase::getRandomRelic() {
    if (registry.empty()) {
        return nullptr;
    }

    int totalWeight = 0;
    for (const auto& pair : registry) {
        char r = pair.second->getRarity();
        if (r == 'B') {
            continue;  
        } else if (r == 'C') {
            totalWeight += 60;  // Commons = 60%
        } else if (r == 'U') {
            totalWeight += 30;  // Uncommons = 30%
        } else if (r == 'R') {
            totalWeight += 10;  // Rares = 10%
        } else if (r == 'L') {
            totalWeight += 1;   // Legendaries = 1%
        }
    }

    if (totalWeight == 0) {
        std::cerr << "  [!] Error: No draftable relics in the database!\n";
        return nullptr;
    }

    int winningTicket = RNG::range(1, totalWeight);
    
    int currentWeight = 0;
    for (const auto& pair : registry) {
        char r = pair.second->getRarity();

        if (r == 'B') {
            continue;
        } else if (r == 'C') {
            currentWeight += 60;
        } else if (r == 'U') {
            currentWeight += 30;
        } else if (r == 'R') {
            currentWeight += 10;
        } else if (r == 'L') {
            currentWeight += 1;
        }

        if (currentWeight >= winningTicket) {
            return pair.second->clone();
        }
    }

    return nullptr;
}