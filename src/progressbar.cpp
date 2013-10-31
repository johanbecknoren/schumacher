#include "progressbar.h"
#include <iostream>
#include "timer.h"
#include <sstream>

ProgressBar *ProgressBar::_instance;
std::mutex ProgressBar::progMutex;

std::string getBar(int percent) {
	std::ostringstream ss;
	std::string bar;
	for (int i = 0; i < 50; ++i) {
		if (i < (percent / 2)) {
			bar.replace(i, 1, "=");
		}
		else if( i == (percent / 2)) {
			bar.replace(i, 1, ">");
		}
		else {
			bar.replace(i, 1, " ");
		}
	}
	ss << "\r" "[" << bar << "] ";
	
	ss << percent;
	return ss.str();
}

void ProgressBar::printTimedProgBar(int count, int total, std::string timerName) {	
	int percent = int(100.0f * float(count) / float(total));
	Timer::getInstance()->start("timeElapsed");
	double t = Timer::getInstance()->getRealTime();
	if (getInstance()->currProc == percent || t < 1000) { 
		return;
	}
	Timer::getInstance()->reset("timeElapsed");
	progMutex.lock();
	getInstance()->currProc = percent;
	std::string bar = getBar(percent);
	std::cout << bar << "% ";
	std::cout.width(3);
	std::cout << Timer::getInstance()->approximateTimeLeft(timerName, percent);
	std::cout << std::flush;
	progMutex.unlock();
}

void ProgressBar::printProgBar(int count, int total) {
	
	int percent = int(100.0f * float(count) / float(total)); 
	if (getInstance()->currProc == percent) { 
		return;
	}
	progMutex.lock();
	
	getInstance()->currProc = percent;
	std::string bar;

	for (int i = 0; i < 50; ++i) {
		if (i < (percent / 2)) {
			bar.replace(i, 1, "=");
		}
		else if( i == (percent / 2)) {
			bar.replace(i, 1, ">");
		}
		else {
			bar.replace(i, 1, " ");
		}
	}
	std::cout << "\r" "[" << bar << "] ";
	std::cout.width(3);
	std::cout << percent;
	if (total > 0)
		std::cout << "% (" << count << "/" << total << ")";
	std::cout << std::flush;

	progMutex.unlock();

}
