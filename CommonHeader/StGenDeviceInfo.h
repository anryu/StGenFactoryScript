#pragma once

#include "StGenICamLib.h"
//��1.0.0.1051
#include "StGenInterface.h"
//��1.0.0.1051

#include "StString.h"
#include "StType.h"

class ST_GENICAMLIB_API StGenDeviceInfo
{
public:
	StGenDeviceInfo(void);
	virtual ~StGenDeviceInfo(void);

	const StString GetID() const;
	const StString GetVendor() const;
	const StString GetModel() const;
	const StString GetDisplayName() const;
	//��1.0.0.1051
	StGenInterface *GetInterface(void);
	//��1.0.0.1051

protected:
	void *mhData;

};

