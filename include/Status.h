#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "Card.h"
class Card;
class RoundTracker;

class IStatus {
   protected:
	int charges;

   public:
	IStatus(int startingCharges) : charges(startingCharges) {}
	virtual ~IStatus() = default;
	virtual std::string getName() const = 0;
	bool isExpired() const { return charges <= 0; }
	virtual void modifyCost(int& r, int& b, int& g, int& generic) {}
	virtual void onCardPlayed(Card& card, RoundTracker& state) {}
	virtual std::unique_ptr<IStatus> clone() const = 0;
};

class OverchargeStatus : public IStatus {
   public:
	OverchargeStatus(int c = 1) : IStatus(c) {}
	std::string getName() const override { return "Overcharged"; }

	std::unique_ptr<IStatus> clone() const override {
		return std::make_unique<OverchargeStatus>(charges);
	}

	void onCardPlayed(Card& card, RoundTracker& state) override {
		if (charges > 0) {
			std::cout << " ![Overcharge] Copy spell!\n";
			card.play(state);
			charges--;
		}
	}
};
