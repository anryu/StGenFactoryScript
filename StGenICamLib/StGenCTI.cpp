#include "StdAfx.h"

//#include <iostream>
#include <string>
#include "StGenCTI.h"
#include "StGenTL.h"

using namespace std;

#pragma warning (disable : 4996)

StGenCTI::StGenCTI(void)
{
}


StGenCTI::~StGenCTI(void)
{
}

BOOL Is64bitOS()
{
   BOOL b64BitOS = TRUE;
//#ifndef _WIN64
  ::IsWow64Process(GetCurrentProcess(), &b64BitOS);
//#endif
  return b64BitOS; 
}

//すでに登録済のディレクトリははじく
//EuresysCXPでインストールアンインストールを繰り返すと環境変数に同じディレクトリが複数記述されていたため
BOOL IsExistDirectory(std::vector< CString > ctiDirectoryList, CString aDirectory)
{
	BOOL bExist = FALSE;
	for( int i=0; i<ctiDirectoryList.size() && bExist==FALSE; i++ )
	{
		if( aDirectory.Compare(ctiDirectoryList[i])==0 )
		{
			bExist = TRUE;
		}
	}
	return bExist;
}

bool StGenCTI::UpdateCTIList( bool *pbChanged )
{
#if 1
    std::vector< std::string > NewTLList;
    std::vector< CString > ctiDirectryList;

    std::string directory;
	BOOL b64BitApp=FALSE;
#ifdef _WIN64
	b64BitApp=TRUE;
#endif
	//if( Is64bitOS() )
	if( b64BitApp )
	{
		try
		{
			directory = std::getenv("GENICAM_GENTL64_PATH");
		}
		catch (std::string* e)
		{
			//AfxMessageBox(_T("GENICAM_GENTL64_PATH nothing"));
		}
	}
	else
	{
		try
		{
			directory = std::getenv("GENICAM_GENTL32_PATH");
		}
		catch (std::string* e)
		{
			//AfxMessageBox(_T("GENICAM_GENTL32_PATH nothing"));
		}
	}

	CString szDirectory(directory.c_str());
	int dirLength = szDirectory.GetLength();
	int iPos=0, iStart=0, length=0;
	do{

		//bType = directory.find_first_of(";",start);
		iPos = szDirectory.Find(';',iStart);
		if( iPos>0 )
		{
			//std::string tmp(directory,start,bType-start);

			CString tmp = szDirectory.Mid(iStart,iPos-iStart);
			if( tmp.Right(1)!="\\" )
			{
				tmp+="\\";
			}
			if( IsExistDirectory(ctiDirectryList,tmp)==FALSE )
			{
				ctiDirectryList.push_back(tmp);
			}
			iStart = iPos + 1;
			if( iStart>=dirLength ) break;
		}
		else
		{
			//";"がないとき
			if( iStart>=dirLength ) break;
			length = dirLength - iStart;
			CString tmp = szDirectory.Right(length);
			if( tmp.Right(1)!="\\" )
			{
				tmp+="\\";
			}
			if( IsExistDirectory(ctiDirectryList,tmp)==FALSE )
			{
				ctiDirectryList.push_back(tmp);
			}
			break;
		}
	}while(1);

	TCHAR szTmp[256];
    WIN32_FIND_DATA find_data;
	NewTLList.clear();
	for( unsigned int i=0; i<ctiDirectryList.size(); i++ )
	{

//{TCHAR text[256];
//swprintf_s( text,256,TEXT("ctiDirectryList[%d] ==> [%s]  \n"),i,ctiDirectryList[i]);
//OutputDebugString(text);}

		StString fullPath = ctiDirectryList[i];
		fullPath += "*.cti";
		MultiByteToWideChar(CP_ACP, 0, fullPath.GetAscii(), -1, szTmp, _countof(szTmp) );
		HANDLE hFileSearch = FindFirstFile(szTmp, &find_data);
		if (INVALID_HANDLE_VALUE == hFileSearch)
		{
			continue;
		} 

        do
        {
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
  //              std::cout<< "found directory "<< find_data.cFileName << std::endl;
            }
            else
            {
  //              std::cout << "found file " << find_data.cFileName << std::endl;
                StString ctiPath = ctiDirectryList[i];
				char cTmp[256];
				WideCharToMultiByte(CP_ACP, 0, find_data.cFileName, -1, cTmp, _countof(cTmp), NULL, NULL);
				ctiPath += cTmp;

                //ctiPath.append(find_data.cFileName);
                //SimpleTL* element = new SimpleTL(ctiPath);
				//CString szTmp(ctiPath.GetAscii());
				std::string szTLPath = ctiPath.GetAscii();
                NewTLList.push_back(szTLPath);
            }
        }
        while (FindNextFile(hFileSearch, &find_data) != 0);

        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) 
        {
  //          std::cerr << "FindFirstFile reported error" << std::endl;
        }

        FindClose(hFileSearch);
	}

    // create a copy of the previous known TL
    std::vector< std::string > OldTLlist;
    OldTLlist.insert(OldTLlist.end(), m_vGCLibList.begin(), m_vGCLibList.end());

    // empty the member list and fill it with current
    m_vGCLibList.clear();

    // now check the list of new TL against the already existing list 
    // and set HasChanged Flag in case there is a difference
    bool bChanged = false;
    bool bMatch = false;
    for (std::vector < std::string >::iterator Iter1 = NewTLList.begin(); Iter1 != NewTLList.end(); )
    {
        for (std::vector < std::string >::iterator Iter2 = OldTLlist.begin(); Iter2 != OldTLlist.end(); )
        {
			//std::string tmp1 = *Iter1;
			//std::string tmp2 = *Iter2;
            //if (tmp1 == tmp2)
            if ((*Iter1) == (*Iter2))
            {
                m_vGCLibList.insert(m_vGCLibList.end(), *Iter2);
                //delete *Iter1;
                NewTLList.erase(Iter1);
                OldTLlist.erase(Iter2);
                Iter1 = NewTLList.begin();
                Iter2 = OldTLlist.begin();
                bMatch = true;
                break;
            }
            else
                Iter2++;
        }

        if (!bMatch)
            Iter1++;
        else
            bMatch = false;
    }

    // if there are still new TLs or any old TLs left, a change happened
    bChanged = (NewTLList.size() > 0) || (OldTLlist.size() > 0);

    if (NewTLList.size())
    {
        m_vGCLibList.insert(m_vGCLibList.end(), NewTLList.begin(), NewTLList.end());
    }

    while (OldTLlist.size())
    {
        //delete *(OldTLlist.end());
        OldTLlist.erase(OldTLlist.end());
    }


#else
    std::vector< std::string > NewTLList;

    std::string directory = std::getenv("GENICAM_GENTL32_PATH");

    directory.append("\\");

    std::string ctiPath = directory;
    ctiPath.append("*.cti");

    WIN32_FIND_DATA find_data;

	TCHAR szTmp[256];
	MultiByteToWideChar(CP_ACP, 0, ctiPath.c_str(), -1, szTmp, _countof(szTmp) );
    HANDLE hFileSearch = FindFirstFile(szTmp, &find_data);
    //HANDLE hFileSearch = FindFirstFile(ctiPath.c_str(), &find_data);

    if (INVALID_HANDLE_VALUE == hFileSearch)
    {
        NewTLList.clear();
    } 
    else 
    {
        // List all the files in the directory with some info about them.
        do
        {
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
  //              std::cout<< "found directory "<< find_data.cFileName << std::endl;
            }
            else
            {
  //              std::cout << "found file " << find_data.cFileName << std::endl;
                ctiPath = directory;
				char cTmp[256];
				WideCharToMultiByte(CP_ACP, 0, find_data.cFileName, -1, cTmp, _countof(cTmp), NULL, NULL);
				ctiPath.append(cTmp);

                //ctiPath.append(find_data.cFileName);
                //SimpleTL* element = new SimpleTL(ctiPath);
                NewTLList.push_back(ctiPath);
            }
        }
        while (FindNextFile(hFileSearch, &find_data) != 0);

        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) 
        {
  //          std::cerr << "FindFirstFile reported error" << std::endl;
        }

        FindClose(hFileSearch);
    }

    // create a copy of the previous known TL
    std::vector< std::string > OldTLlist;
    OldTLlist.insert(OldTLlist.end(), m_vGCLibList.begin(), m_vGCLibList.end());

    // empty the member list and fill it with current
    m_vGCLibList.clear();

    // now check the list of new TL against the already existing list 
    // and set HasChanged Flag in case there is a difference
    bool bChanged = false;
    bool bMatch = false;
    for (std::vector < std::string >::iterator Iter1 = NewTLList.begin(); Iter1 != NewTLList.end(); )
    {
        for (std::vector < std::string >::iterator Iter2 = OldTLlist.begin(); Iter2 != OldTLlist.end(); )
        {
			//std::string tmp1 = *Iter1;
			//std::string tmp2 = *Iter2;
            //if (tmp1 == tmp2)
            if ((*Iter1) == (*Iter2))
            {
                m_vGCLibList.insert(m_vGCLibList.end(), *Iter2);
                //delete *Iter1;
                NewTLList.erase(Iter1);
                OldTLlist.erase(Iter2);
                Iter1 = NewTLList.begin();
                Iter2 = OldTLlist.begin();
                bMatch = true;
                break;
            }
            else
                Iter2++;
        }

        if (!bMatch)
            Iter1++;
        else
            bMatch = false;
    }

    // if there are still new TLs or any old TLs left, a change happened
    bChanged = (NewTLList.size() > 0) || (OldTLlist.size() > 0);

    if (NewTLList.size())
    {
        m_vGCLibList.insert(m_vGCLibList.end(), NewTLList.begin(), NewTLList.end());
    }

    while (OldTLlist.size())
    {
        //delete *(OldTLlist.end());
        OldTLlist.erase(OldTLlist.end());
    }
#endif    
    if (pbChanged)
        *pbChanged = bChanged;

    return true;
}

StString StGenCTI::GetGCLibPathName( uint32_t index )
{
    if (index < m_vGCLibList.size())
    {
        return m_vGCLibList[index].c_str();
    }
    else
    {
        return "";
    }        
}

#pragma warning (default : 4996)
