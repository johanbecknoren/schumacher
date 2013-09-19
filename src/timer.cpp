#include "timer.h"

namespace TimeTypes {
Times::Times() {
	_active = false;
	_id = 0;
}
Times::Times(TimePoint start, int id) {
	_start = start;
	_id = id;
	_active = true;
}

void Times::start(TimePoint current) {
	stop(current);
	_active = true;
	_start = current;
}

void Times::stop(TimePoint current) {
	_elapsed += std::chrono::duration_cast<Millisecs>(_start - current);		
	_active = false;
}

Millisecs Times::timeElapsed(TimePoint current) const {
	Millisecs totalElapsed = _elapsed;
	if (_active)
		totalElapsed += std::chrono::duration_cast<Millisecs>(_start - current);
	return totalElapsed;
}

TimeTracker::TimeTracker() {
}

double TimeTracker::getRealtime(TimePoint current) const {
	return _realTime.timeElapsed(current).count();
}

void TimeTracker::push_back(Times t) {
	if (t.getId() != -1) {
		std::cout << "Adding thread value\n";
		_threadTimes.push_back(t);
	}
	else {
		std::cout << "Adding realtime value\n";
		_realTime = t;	
	}
}

} // namespace TimeTypes

void Timer::start(std::string name, int threadId) {
	TimeTypes::TimePoint v = getCurrentTime();
	TimeTypes::TimerList::iterator it = timers.find(name);
	if (it != timers.end()) { 
		for (int i = 0; i < it->second.size(); ++i) {
			if (threadId == it->second[i].getId()) {
				if (it->second[i].isActive()) {
					std::cout << "timer.h l40 - ERROR: Id already added "
					  << "for this name. Stop it or add with other ID.\n";
				}
				else {
					it->second[i].start(v);
					return;
				}
			}
		}
	}
	else {
		TimeTypes::Times t = TimeTypes::Times(v, threadId);
		std::cout << "Adding new timer!";
		it->second.push_back(t); // FEEEL
	}
	std::cout << timers.size() << std::endl;
}

void Timer::stop(std::string name, int threadId) {
	TimeTypes::TimePoint v = getCurrentTime();
	TimeTypes::TimerList::iterator it = timers.find(name);

	if (it != timers.end()) {
		for (int i = 0; i < it->second.size(); ++i) {
			if (threadId == it->second[i].getId()) {
				it->second[i].stop(v);
				return;
			}
		}
	}
}

void Timer::reset(std::string name, int threadId) {
	if (threadId == -1) { 
		timers.erase(name);
		return;
	}
	TimeTypes::TimerList::iterator it = timers.find(name);

	if (it != timers.end()) {
		for (int i = 0; i < it->second.size(); ++i) {
			if (threadId == it->second[i].getId()) {
				it->second.erase(it->second.begin() + i);
				return;
			}
		}
	}
}

double Timer::getElapsedTime(std::string name) const {
	return 0;
}


double Timer::getElapsedTime(std::string name, int threadId) const {
	return 0;
}	

void Timer::printRealTime(std::string name, TIME_FORMAT format) const {
	std::cout << timers.size() << std::endl;
	for(int i = 0; i < timers.size(); ++i) {
		std::cout << "a";
	}
	TimeTypes::TimerList::const_iterator it = timers.find(name);
	std::cout << "Printing realtime value: ";
	if (it != timers.end()) {
		std::cout << "Hej!";	
		std::cout << name << " " << it->second.getRealtime(getCurrentTime());	
	}
	std::cout << std::endl;
}
void Timer::printThreadTime(std::string name, TIME_FORMAT format) const {
	TimeTypes::TimerList::const_iterator it = timers.find(name);

	if (it != timers.end()) {
		for (int i = 0; i < it->second.size(); ++i) {
				
				return;
		}
	}
}

Timer *Timer::getInstance() {
	if (_instance == 0) {
		// Lock here
		if (_instance == 0) {
			std::cout << "Constructed instance" << std::endl;
			_instance = new Timer();	
		}
	}
	return _instance; 
}

TimeTypes::TimePoint Timer::getCurrentTime() { 
	TimeTypes::TimePoint p = std::chrono::steady_clock::now() ;
	return p; 
}

