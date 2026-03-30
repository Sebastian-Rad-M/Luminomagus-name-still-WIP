#pragma once
#include <memory>
class RoundTracker;
class IEffect {
   public:
	virtual ~IEffect() = default;
	virtual void resolve(RoundTracker& state) = 0;

	virtual std::unique_ptr<IEffect> clone() const = 0;
};

