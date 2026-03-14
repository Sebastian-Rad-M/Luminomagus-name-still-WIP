#include "RoundTracker.h"
#include "Status.h"

void EchoStatus::onCardPlayed(Card& card, RoundTracker& state){
        if (charges > 0) {
            int copies = state.getStormCount();        
            if (copies > 0)  for (int i = 0; i < copies; i++) card.play(state);  
            charges--;
        }
    }

void GlobalExileStatus::modifyDestination(CardZone*& destination, RoundTracker& state){
        if (charges > 0) {
			destination = &state.getExile(); 
            charges--;       }
    }