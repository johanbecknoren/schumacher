#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <map>
class Times;
typedef std::map<std::string, std::vector<Times>> TimerList;

class Times {
	public:
		Times(double start, int id) {
			_start = start;
			_elapsed = 0;
			_id = id;
			_active = true;
		}
		void start(double current) {
			stop(current);
			_active = true;
			_start = current;
		}
		void stop(double current) {
			_elapsed += _start - current;
			_active = false;
		}
		bool isActive() { return _active; };
		int getId() const { return _id; };
	private:
		double _start;
		double _elapsed;
		int _id;
		bool _active;
};

class Timer {
	public:
		static Timer& getInstance() {
			static Timer instance;
			return instance;
		}
		
		void startTimer(std::string name, int threadId = 0) {
			double v = getCurrentTime();
			std::vector<Times> vec = timers[name];
			if (vec != timers.end()) { 
				for (int i = 0; i < vec.size(); ++i) {
					if (threadId == vec[i].getId()) {
						if (vec[i].isActive())
							std::cout << "timer.h l40 - ERROR: Id already added for this name. Stop it or add with other ID.";
						else
							vec[i].start(v);
						return;
					}
				}
			}
			Times t = Times(v, threadId);
			vec.push_back(t);
		}
		void stopTimer(std::string name, int threadId = 0) {
			double v = getCurrentTime();
			std::vector<Times> vec = timers[name];
			if (vec != timers.end()) {
				for (int i = 0; i < vec.size(); ++i) {
					if (threadId == vec[i].getId()) {
						vec[i].stop(v);
					}
				}
			}
		}
		void resetTimer(std::string name, int threadId = -1) {
			if (threadId == -1) { 
				timers.erase(name);
				return;
			}
			std::vector<Times> vec = timers[name];
			if (vec != timers.end()) {
				for (int i = 0; i < vec.size(); ++i) {
					if (threadId == vec[i].getId()) {
						vec[i].stop(v);
					}
				}
			}

		}
		double getElapsedTime(std::string name) const {
			return 0;
		}
		double getElapsedTime(std::string name, int threadId) const {
			
		}
		void printAllTimers() const {

		}
	private:
		TimerList timers;
		Timer() {

		};
		Timer(Timer const&);
		void operator=(Timer const&);
		double getCurrentTime() { return 0; };
};

#endif
