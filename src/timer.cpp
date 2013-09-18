#include "timer.h"

Times::Times(double start, int id) {
	_start = start;
	_elapsed = 0;
	_id = id;
	_active = true;
}

void Times::start(double current) {
	stop(current);
	_active = true;
	_start = current;
}

void Times::stop(double current) {
	_elapsed += _start - current;		
	_active = false;
}

