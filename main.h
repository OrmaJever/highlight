#ifndef _MAIN_H
#define _MAIN_H

#define VERSION "1.0"

#ifdef DEBUG
#	include <chrono>
#	define DTIME std::chrono::high_resolution_clock::now()
#	define MCSEC(s, e) std::chrono::duration_cast <std::chrono::milliseconds>( e - s ).count()
#endif

#endif // _MAIN_H
