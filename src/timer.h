#ifndef TIMER_H
#define TIMER_H
#include <iostream>
#include <chrono>
#include <map>
#include <string>
#include <vector>
enum TIME_FORMAT { MILLISEC, SEC };
namespace TimeTypes {
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::duration<int, std::milli> Millisecs;

class Times {	
	public:
		Times();
		Times(TimePoint start, int id);
		void start(TimeTypes::TimePoint current);		
		void stop(TimePoint current);
		bool isActive() { return _active; };
		int getId() const { return _id; };
		Millisecs timeElapsed(TimePoint current) const;
	private:
		TimePoint _start;
		Millisecs _elapsed;
		int _id;
		bool _active;
};

class TimeTracker {
	public:
		TimeTracker();
		int size() const { return _threadTimes.size(); };
		Times& operator [](int i) { return _threadTimes[i]; }
		void push_back(Times t); 
		void erase(std::vector<Times>::iterator it) { _threadTimes.erase(it); }
		std::vector<Times>::iterator begin() { return _threadTimes.begin(); }
		double getRealtime(TimePoint current) const;
	private:
		Times _realTime;
		std::vector<Times> _threadTimes;
};

typedef std::map<std::string, TimeTracker> TimerList;
};// namespace TimeTypes

class Timer {
	public:
			
		static Timer *getInstance(); 		
		void start(std::string name, int threadId = -1);
		void stop(std::string name, int threadId = -1);		
		void reset(std::string name, int threadId = -1);
		double getElapsedTime(std::string name) const;
		double getElapsedTime(std::string name, int threadId) const;
		void printAllTimers(TIME_FORMAT format = TIME_FORMAT::SEC) const;	
		void printRealTime(std::string name, TIME_FORMAT format = TIME_FORMAT::SEC) const;
		void printThreadTime(std::string name, TIME_FORMAT format = TIME_FORMAT::SEC) const;

		static TimeTypes::TimePoint getCurrentTime();
	private:
		TimeTypes::TimerList timers;
		Timer() {

		};
		double msToS(double d) const;
		Timer(Timer const&);
		void operator=(Timer const&);
		
};
namespace {
	static Timer *_instance;
};
#endif
