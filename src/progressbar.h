#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

class ProgressBar;

namespace {
	static ProgressBar *_instance;
}

class ProgressBar {
	int currProc = -1;
	public:
	static void printProgBar(int percent) {
		if (getInstance()->currProc == percent) return;
		else getInstance()->currProc = percent;
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
		std::cout << percent << "%    " << std::flush;
	};
	private:
		ProgressBar(){};
		
		static ProgressBar *getInstance() {
			if (_instance == 0) {
				_instance = new ProgressBar();
			}
			return _instance;
		}
};

#endif
