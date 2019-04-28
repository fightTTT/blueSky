#pragma once

class Timer {
public:
	Timer() :time(0), count(0) {};
	Timer(int time) :time(time), count(0) {};
	Timer(int time, int defaultCount) :time(time), count(defaultCount) {};
	~Timer() {};
	void update() { ++count; };
	void reset() { count = 0; }
	void setTime(int time) { this->time = time; };
	void setTimeAndReset(int time) { this->time = time; count = 0; };
	int getCount() const { return count; };
	bool isPassed() const { return count >= time; };
private:
	int count;
	int time;
};
