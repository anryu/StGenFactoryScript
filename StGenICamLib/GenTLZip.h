#pragma once

#include <string>
class GenTLZip
{
public:
	GenTLZip(void);
	~GenTLZip(void);
    //static bool UncompressGenICamXML( std::string &aData );
    static bool UncompressGenICamXML( std::string &aData, char *aZipData );
};
