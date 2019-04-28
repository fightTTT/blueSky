#pragma once
#include "Timer.h"

template<class T>

class Fader {
public:
	Fader() :current(T()), goal(T()), velocity(T())
	{}
	Fader(const T& value) :current(value), goal(value), velocity(T())
	{}

	const T& getCurrent() const { return current; }
	const T& getGoal() const { return goal; }

	void update() {
		if (timer.isPassed()) {
			current = goal;
		}
		else {
			timer.update();
			current += velocity;
		}
	}

	operator T() const {
		return getCurrent();
	}

	const T& operator=(const T& value) {
		set(value);
		return value;
	}
	T& operator=(T& value) {
		set(value);
		return value;
	}
	void set(const T& value) {
		current = goal = value;
		velocity = T();
		timer.setTimeAndReset(0);
	}
	void set(const T& value, int duration) {
		if (value != goal) {
			if (duration <= 0) {
				return set(value);
			}
			goal = value;
			velocity = (goal - current) / duration;
			timer.setTimeAndReset(duration);
		}
	}
	bool isCompleted() const { return timer.isPassed(); }
private:
	T current;
	T goal;
	T velocity;
	Timer timer;
};
