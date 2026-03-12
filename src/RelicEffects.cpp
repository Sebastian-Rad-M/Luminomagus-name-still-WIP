#include "RelicEffects.h"

std::string OnManaRelic::getName() const { return name; }
std::string OnCastTriggerRelic::getName() const { return name; }
std::string OnDamageRelic::getName() const { return name; }
std::string OnDrawRelic::getName() const { return name; }

std::string OnManaRelic::getDescription() const { return description; }
std::string OnCastTriggerRelic::getDescription() const { return description; }
std::string OnDamageRelic::getDescription() const { return description; }
std::string OnDrawRelic::getDescription() const { return description; }

char OnManaRelic::getRarity() const { return rarity; }
char OnCastTriggerRelic::getRarity() const { return rarity; }
char OnDamageRelic::getRarity() const { return rarity; }
char OnDrawRelic::getRarity() const { return rarity; }

OnManaRelic::OnManaRelic(const std::string& n, const std::string& desc, const char r,
						 const int mult, std::unique_ptr<IEffect> effect)
	: name(std::move(n)),
	  description(std::move(desc)),
	  rarity(r),
	  multiplier(mult),
	  triggerEffect(std::move(effect)) {}
OnCastTriggerRelic::OnCastTriggerRelic(const std::string& n, const std::string& desc, const char r,
									   std::unique_ptr<IEffect> effect)
	: name(std::move(n)),
	  description(std::move(desc)),
	  rarity(r),
	  triggeredEffect(std::move(effect)) {}
OnDamageRelic::OnDamageRelic(std::string n, std::string desc, char r, int bonus,
							 std::unique_ptr<IEffect> effect)
	: name(n), description(desc), rarity(r), flatBonus(bonus), triggerEffect(std::move(effect)) {}
OnDrawRelic::OnDrawRelic(std::string n, std::string desc, char r, std::unique_ptr<IEffect> effect)
	: name(n), description(desc), rarity(r), triggerEffect(std::move(effect)) {}

void OnManaRelic::onManaAdded(int& r, int& b, int& g, RoundTracker& state) {
	bool triggered = false;
	if (r > 0 || b > 0 || g > 0) {
		r *= multiplier;
		b *= multiplier;
		g *= multiplier;
		triggered = true;
	}
	if (triggered && triggerEffect) {
		std::cout << "! [" << name << "] Triggered its secondary effect!\n";
		triggerEffect->resolve(state);
	}
}

void OnCastTriggerRelic::onCardPlayed(RoundTracker& state) {
	std::cout << " ! [" << name << "] Triggered on cast!\n";
	triggeredEffect->resolve(state);
}

void OnDamageRelic::onDamageDealt(int& damage, RoundTracker& state) {
	if (damage > 0) {
		damage += flatBonus;

		if (triggerEffect) {
			std::cout << " ! [" << name << "] Triggered on damage!\n";
			triggerEffect->resolve(state);
		}
	}
}

void OnDrawRelic::onCardDrawn(RoundTracker& state) {
	if (triggerEffect) {
		std::cout << " ! [" << name << "] Triggered on card draw!\n";
		triggerEffect->resolve(state);
	}
}

std::shared_ptr<IRelic> OnManaRelic::clone() const {
	auto clonedEffect = triggerEffect ? triggerEffect->clone() : nullptr;
	return std::make_shared<OnManaRelic>(name, description, rarity, multiplier,
										 std::move(clonedEffect));
}
std::shared_ptr<IRelic> OnDamageRelic::clone() const {
	auto clonedEffect = triggerEffect ? triggerEffect->clone() : nullptr;
	return std::make_shared<OnDamageRelic>(name, description, rarity, flatBonus,
										   std::move(clonedEffect));
}
std::shared_ptr<IRelic> OnCastTriggerRelic::clone() const {
	return std::make_shared<OnCastTriggerRelic>(name, description, rarity,
												triggeredEffect->clone());
}
std::shared_ptr<IRelic> OnDrawRelic::clone() const {
	return std::make_shared<OnDrawRelic>(name, description, rarity, triggerEffect->clone());
}
