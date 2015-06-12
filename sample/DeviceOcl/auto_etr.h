#ifndef __AUTO_ETR_H
#define __AUTO_ETR_H

//Auto Execution Time Recorder
#include <iostream>
#include <string>
#include <sys/timeb.h>


//refer to http://www.cplusplus.com/forum/general/43203/

using namespace std;

class AutoETR{
private:
	std::string name;

	int &start_milli;
	int &end_milli;

	inline int getMilliCount(){
		timeb tb;
		ftime(&tb);
		//int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
		int nCount = tb.millitm + (tb.time) * 1000;
		return nCount;
	}

public:
	
	explicit AutoETR(std::string name, int &start_msec, int &end_msec) : start_milli(start_msec), end_milli(end_msec) {
		//save name first
		this->name = name;
		
		start_milli = getMilliCount();
		start_msec = start_milli;
	}

	~AutoETR(){
		end_milli = getMilliCount();
		cout << "Execution time of " << name << " : " << end_milli - start_milli << " milliseconds" << endl;
	}
};



#endif