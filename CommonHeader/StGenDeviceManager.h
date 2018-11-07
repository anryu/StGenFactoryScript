#pragma once
#include "StGenICamLib.h"
//#include "StGenTL.h"
//class StGenTL

class ST_GENICAMLIB_API StGenDeviceManager
{
public:
	StGenDeviceManager(void);
	~StGenDeviceManager(void);
	bool UpdateList(void);
	int GetGenTLSize(void);
	//StGenTL* GetGenTL(int index);
	void * GetGenTL(int index);

private:
	void *mhThis;

};

