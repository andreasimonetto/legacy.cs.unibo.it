/* sys.cc - Operating system facilities
Copyright (C) 2008  Andrea Simonetto

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "sys/Clock"
#include "sys/ClockViewer"
#include "sys/System"
#include "sys/Timer"

#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

namespace sys {

System *System::instance = 0;

static void __sys_exit(int status, void *arg)
{
System **sys = (System**)arg;

	if(*sys) 
		(*sys)->exit(status);
}

void Clock::sync() 
{
	gettimeofday(&tod, 0);
}

Clock operator -(const Clock &clock1, const Clock &clock2)
{
Clock result = clock1;

	result -= clock2;
	return result;
}

Clock operator +(const Clock &clock1, const Clock &clock2)
{
Clock result = clock1;

	result += clock2;
	return result;
}

void Clock::operator -=(const Clock &clock)
{
	tod.tv_sec -= clock.tod.tv_sec;
	if((tod.tv_usec = tod.tv_usec - clock.tod.tv_usec) < 0) {
		tod.tv_sec--;
		tod.tv_usec += 1000000;
	}
}

void Clock::operator +=(const Clock &clock)
{
	tod.tv_sec += clock.tod.tv_sec;
	if((tod.tv_usec = tod.tv_usec + clock.tod.tv_usec) >= 1000000) {
		tod.tv_sec++;
		tod.tv_usec -= 1000000;
	}
}

ClockViewer::ClockViewer(const Clock &clock)
{
	localtime_r(&(clock.tod.tv_sec), &tm);
	tv_usec = clock.tod.tv_usec;
}

System::System()
{
	on_exit(__sys_exit, &instance);
	srand(time(0));
}

void System::exit(int status)
{
	while(!exitHandlers.empty()) {
		exitHandlers.top()(status);
		exitHandlers.pop();
	}

	instance = 0;
	delete this;
	_exit(status);
}

void Timer::restart() 
{
	gettimeofday(&start, 0);
}

void Timer::restart(unsigned long microseconds) 
{
	interval = microseconds;
	gettimeofday(&start, 0);
}

bool Timer::hasExpired() const
{ 
unsigned long diff;
struct timeval now;

	gettimeofday(&now, 0);
	diff = (unsigned long)(difftime(now.tv_sec, start.tv_sec) * 1000000.0);
	diff += now.tv_usec - start.tv_usec;
	return (diff >= interval);
}

};

