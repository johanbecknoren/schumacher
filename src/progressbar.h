#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <mutex>
class ProgressBar {	
	private:
        ProgressBar() : currProc(-1){};
 		static ProgressBar *_instance;
		static std::mutex progMutex;
		static ProgressBar *getInstance() {
			if (_instance == 0) {
				progMutex.lock();
				if (_instance == 0) {
					_instance = new ProgressBar();
				}
				progMutex.unlock();
			}
			return _instance;
		}
		int currProc;
		
	public:
	static void printProgBar(int count, int total);
	static void printTimedProgBar(int count, int total, std::string timerName);

};

#endif
