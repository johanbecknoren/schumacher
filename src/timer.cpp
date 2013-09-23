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
	if(!_active) return;
	_elapsed += std::chrono::duration_cast<Millisecs>(current - _start );		
	_active = false;
}

Millisecs Times::timeElapsed(TimePoint current) const {
	Millisecs totalElapsed = _elapsed;
	if (_active)
	{
		totalElapsed += std::chrono::duration_cast<Millisecs>(current - _start );
	}
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
		TimeTypes::TimeTracker tr = TimeTypes::TimeTracker();
		tr.push_back(t);
		timers.insert(std::pair<std::string, TimeTypes::TimeTracker>(name, tr));
	}
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
	TimeTypes::TimerList::const_iterator it = timers.find(name);

	if (it != timers.end()) {
		std::cout << "Timer: ";	
		std::string v = "ms";
		double time = it->second.getRealtime(getCurrentTime());
		if (format == SEC) {
			time = msToS(time);
			v = "s";
		}
		std::cout.precision(2);
		std::cout << name << " " << time << v;	
	}
	else {
		std::cout << "No timer called " << name << " found.";
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
double Timer::msToS(double d) const {
	return d * 0.001;
}
Timer *Timer::getInstance() {
	if (_instance == 0) {
		// Lock here
		if (_instance == 0) {
			_instance = new Timer();	
		}
	}
	return _instance; 
}

TimeTypes::TimePoint Timer::getCurrentTime() { 
	TimeTypes::TimePoint p = std::chrono::steady_clock::now() ;
	return p; 
}

