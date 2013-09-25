#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

class ProgressBar {	
	private:
		ProgressBar(){};
 		static ProgressBar *_instance;

		static ProgressBar *getInstance() {
			if (_instance == 0) {
				_instance = new ProgressBar();
			}
			return _instance;
		}
		int currProc = -1;

	public:
	static void printProgBar(int count, int total);

};

#endif
