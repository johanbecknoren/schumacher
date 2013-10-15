#include "timer.h"
#include <sstream>
#include <iomanip>
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

void Times::reset() {
	_elapsed = std::chrono::seconds(0);
	_active = false;
}

Millisecs Times::timeElapsed(TimePoint current) const {
	Millisecs totalElapsed = _elapsed;
	if (_active) {
		totalElapsed += std::chrono::duration_cast<Millisecs>(current - _start );
	}
	return totalElapsed;
}

double Times::millisecsElapsed(TimePoint current) const {
	Millisecs elapsed = timeElapsed(current);
	return elapsed.count();
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

void TimeTracker::stop(TimePoint v, int threadId) {
	if (threadId == -1) {
		_realTime.stop(v);
		return;
	}
	bool allOff = true;
	for (size_t i = 0; i < _threadTimes.size(); ++i) {
		if (threadId == _threadTimes[i].getId()) {
			_threadTimes[i].stop(v);
			if (!allOff) return;
		}
		if (_threadTimes[i].isActive()) allOff = false;
	}
	if (allOff)
		_realTime.stop(v);

}

void TimeTracker::reset(int threadId) {
	if (threadId == -1) {
		_realTime.reset();
		return;
	}
	for (size_t i = 0; i < _threadTimes.size(); ++i) {
		if (threadId == _threadTimes[i].getId()) {
			_threadTimes[i].reset();
			return;
		}
	}
}

void TimeTracker::start(TimePoint v, int threadId) {
	_realTime.start(v);
	if (threadId == -1) {	
		return;
	}
	for (size_t i = 0; i < _threadTimes.size(); ++i) {
		if (threadId == _threadTimes[i].getId()) {
			_threadTimes[i].start(v);
			return;
		}
	}
	TimeTypes::Times t = TimeTypes::Times(v, threadId);
	_threadTimes.push_back(t);
}

} // namespace TimeTypes

void Timer::start(std::string name, int threadId) {
	TimeTypes::TimePoint v = getCurrentTime();
	TimeTypes::TimerList::iterator it = timers.find(name);
	if (it != timers.end()) { 
		it->second.start(v, threadId);		
	}
	else {
		TimeTypes::Times t = TimeTypes::Times(v, threadId);
		TimeTypes::TimeTracker tr = TimeTypes::TimeTracker();
		tr.push_back(t);
		timers.insert(std::pair<std::string, TimeTypes::TimeTracker>(name, tr));
	}
}

void Timer::stop(std::string name, int threadId) {
	TimeTypes::TimePoint v = getCurrentTime();
	TimeTypes::TimerList::iterator it = timers.find(name);

	if (it != timers.end()) {
		it->second.stop(v, threadId);
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

double Timer::getThreadTime(std::string name, int threadId) const {
	TimeTypes::TimerList::const_iterator it = timers.find(name);
	double time = 0;
	if (it != timers.end()) {
		for (int i = 0; i < it->second.size(); ++i) {
			if (threadId == it->second.cget(i).getId()) {
					return it->second.cget(i).millisecsElapsed(getCurrentTime());
			}
			time += it->second.cget(i).millisecsElapsed(getCurrentTime());
		}
		std::cout << "n threads" << it->second.size() << std::endl;	
		if (threadId == -1) {
			return time;
		}
	}
	return 0;
}	

void Timer::printRealTime(std::string name, TIME_FORMAT format) const {
	TimeTypes::TimerList::const_iterator it = timers.find(name);

	if (it != timers.end()) {
		std::cout << "Timer: ";	
		double time = it->second.getRealtime(getCurrentTime());
		
		std::cout << name << " ";	
		printLine(time, format);
	}
	else {
		std::cout << "No timer called " << name << " found.";
	}
	std::cout << std::endl;
}

std::string Timer::printable(double time, TIME_FORMAT format) const {
	std::ostringstream ss;
	std::string v = "ms";

	if (format == HIGHEST) {
		time = convertToHighest(format, time);
	}
	if (format == SEC) {
		v = "s";
	}
	else if (format == MIN) {
		v = "m";
	}
	else if (format == HRS) {
		v = "h";
	}
	ss << (int) time << v;
	return ss.str();
}

void Timer::printLine(double time, TIME_FORMAT format) const {
	std::string v = "ms";

	if (format == HIGHEST) {
		time = convertToHighest(format, time);
	}
	if (format == SEC) {
		v = "s";
	}
	else if (format == MIN) {
		v = "m";
	}
	else if (format == HRS) {
		v = "h";
	}
	std::cout << time << v;

}

void Timer::printAllTimers(TIME_FORMAT format) const {

}


void Timer::printThreadTime(std::string name, TIME_FORMAT format) const {
	double time = getThreadTime(name);
	std::cout << "Timer:";
	std::cout << name << " ";
	printLine(time,format);
	std::cout << std::endl;
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

std::string Timer::approximateTimeLeft(std::string name, double percentage) const {
	TimeTypes::TimerList::const_iterator it = timers.find(name);
	std::ostringstream ss;
	ss.precision(3);

	if (it != timers.end()) {
		double time = it->second.getRealtime(getCurrentTime());
		TIME_FORMAT f1;
		double ctime = convertToHighest(f1, time);

		ss << printable(ctime, f1) << " ";
		//percentage = 0, 100
		if (percentage > 0.0001) {
			double approx = time / percentage * (100.0 - percentage);	
			double capprox = convertToHighest(f1, approx);
			ss << std::setprecision(1);
			ss << printable(capprox, f1);
		}
		return ss.str();
	}
	else {
		std::cout << "No timer called " << name << " found.";
	}
	std::cout << std::endl;
	return "";
}

double Timer::convertToHighest(TIME_FORMAT &format, double t) const {
	if (t < 1000.0) {
		format = TIME_FORMAT::MILLISEC;
		return t;
	}
	else t = msToS(t);
	if (t < 60.0) {
		format = TIME_FORMAT::SEC;
		return t;
	}
	else t /= 60.0; // Convert to minutes
	if (t < 60) {
		format = TIME_FORMAT::MIN;
		return t;
	}
	else t /= 60.0;
	format = TIME_FORMAT::HRS;
	return t;
	
}
Timer *Timer::_instance;
