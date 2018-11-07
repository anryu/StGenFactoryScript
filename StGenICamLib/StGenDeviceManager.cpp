#include <stdlib.h>

#include "../CommonHeader/StGenDeviceManager.h"
#include <vector>
#include "StGenCTI.h"
#include "StGenTL.h"
#include "StTL.h"

#include <windows.h>

class StDeviceManager
{
public:
	StDeviceManager(void);
	~StDeviceManager(void);

	bool InitializeCTI(void);
	bool UpdateCTI(void);
	int GetGenTLSize(void);
	StGenTL* GetGenTL(int index);

private:
//	StGenCTI m_StCTI;
	//std::vector<StGenTL*> m_pstTL;
	std::vector<StTL*> m_pstTL;
	void DeleteTL(void);

};

StDeviceManager::StDeviceManager(void)
{
}


StDeviceManager::~StDeviceManager(void)
{
	DeleteTL();
}


bool StDeviceManager::InitializeCTI(void)
{
    bool bChanged = false;

	StGenCTI stCTI;
    stCTI.UpdateCTIList(&bChanged);

    if (bChanged)
    {
		DeleteTL();

        uint32_t num = stCTI.GetNumGCLibs();
        for (unsigned int i = 0; i < num; i++)
        {
//TRACE(TEXT("GetGCLibPathName (%s)\n"), stCTI.GetGCLibPathName(i).GetUnicode());

			StGenGCLib *pGCLib = new StGenGCLib(stCTI.GetGCLibPathName(i));
			//m_pstGCLib.push_back(pGCLib);

			StTL *pTL = new StTL(pGCLib);
			m_pstTL.push_back(pTL);

			//SimpleTL* pTL = new SimpleTL(m_sCTI.GetTLPathName(i));
			//m_psTL.push_back(pTL);
        }
    }

	//TL�ȉ����X�V
	for( unsigned int i=0; i<m_pstTL.size(); i++ )
	{
		bool bChangedTL=false;
		m_pstTL[i]->UpdateAllList(&bChangedTL);
		if( bChangedTL ) bChanged = true;
		//bChangedTL=false;
		//m_pstTL[i]->InvalidateInterfaceList(&bChangedTL);
		//if( bChangedTL ) bChanged = true;
	}



	return bChanged;
}

//StGenTL�̎擾
//Bug����H�H
bool StDeviceManager::UpdateCTI(void)
{

	if( m_pstTL.size()==0 )
		return InitializeCTI();

	//����ɉ��L���Ƃ����ƂȂ����o�O�Ȃ̂ŁAInitializeCTI�������Ȃ�

    bool bChanged = false;

	StGenCTI stCTI;
    stCTI.UpdateCTIList(&bChanged);

	//UpdateCTIList��Change�ԈႢ
	bChanged = false;
	//1 Delete
	//StGenTL�ɂ�����stCTI�ɂȂ����̂��폜
	do
	{
		bool bDelete=false;
		for( std::vector<StTL*>::iterator it = m_pstTL.begin(); it != m_pstTL.end(); it++ )
		{
			bool bExist=false;
			StGenGCLib *pGCLib = (*it)->GetGenGCLib();
			for( unsigned int i=0; i<stCTI.GetNumGCLibs(); i++ )
			{
				if( stCTI.GetGCLibPathName(i)==(*it)->GetPathName() )
				{
					bExist = true;
					break;
				}
			}
			if( bExist==false )
			{
				delete (*it);
				m_pstTL.erase(it);
				bChanged = true;
				//�폜������ŏ����炨���Ȃ�
				bDelete = true;
				break;
			}
		}
		if( !bDelete ) break;
	}while(1);

	//2 Create
	//stCTI�ɂ�����StGenTL�ɂȂ����̂��쐬
	for( unsigned int i=0; i<stCTI.GetNumGCLibs(); i++ )
	{
		bool bExist=false;
		for( std::vector<StTL*>::iterator it = m_pstTL.begin(); it != m_pstTL.end(); it++ )
		{
			StGenGCLib *pGCLib = (*it)->GetGenGCLib();
			if( stCTI.GetGCLibPathName(i)==(*it)->GetPathName() )
			{
				bExist = true;
				break;
			}
		}
		if( bExist==false )
		{
			
			StGenGCLib *pGCLib = new StGenGCLib(stCTI.GetGCLibPathName(i));
			//m_pstGCLib.push_back(pGCLib);
			StTL *pTL = new StTL(pGCLib);
			m_pstTL.push_back(pTL);
			//pTL->UpdateAllList(NULL);
			bChanged = true;
		}
	}

	//TL�ȉ����X�V
	for( unsigned int i=0; i<m_pstTL.size(); i++ )
	{
		bool bChangedTL=false;
		//m_pstTL[i]->UpdateAllList(&bChangedTL);
		//if( bChangedTL ) bChanged = true;
		//bChangedTL=false;
		m_pstTL[i]->InvalidateInterfaceList(&bChangedTL);
		if( bChangedTL ) bChanged = true;
	}
	//psTL->UpdateAllList(&bChangedTL);�ō\�z
	
	return bChanged;
}

int StDeviceManager::GetGenTLSize(void)
{
	return m_pstTL.size();
}

StGenTL* StDeviceManager::GetGenTL(int index)
{
	StGenTL *pGenTL=NULL;

	if( m_pstTL.size()>(unsigned int)index )
	{
		pGenTL = m_pstTL[index];
	}
	return pGenTL;
}

void StDeviceManager::DeleteTL(void)
{
	//if( m_pstTL.size()>0 )
	//{
	//	for( unsigned int i=0; i<m_pstTL.size(); i++ )
	//		delete m_pstTL[i];
	//	m_pstTL.clear();
	//}

    for (unsigned int i = 0; i < m_pstTL.size(); i++)
	{
		if( m_pstTL[i] )
		{
			StGenGCLib* pstGCLib = m_pstTL[i]->GetGenGCLib();
			delete m_pstTL[i];
			delete pstGCLib;
		}
	}
	m_pstTL.clear();

}



//---------------------------------------------------------------------
StGenDeviceManager::StGenDeviceManager(void)
{
	mhThis = (void *)new StDeviceManager();
	if( mhThis )
	{
		//UpdateList();
	}
}
StGenDeviceManager::~StGenDeviceManager(void)
{
	if( mhThis )
	{
		StDeviceManager *pDeviceManager = (StDeviceManager *)mhThis;
		delete pDeviceManager;
	}
}
bool StGenDeviceManager::UpdateList(void)
{
	bool bReval = FALSE;
	if( mhThis )
	{
		StDeviceManager *pDeviceManager = (StDeviceManager *)mhThis;
		bReval = pDeviceManager->UpdateCTI();
	}
	return bReval;
}
int StGenDeviceManager::GetGenTLSize(void)
{
	int iSize=0;
	if( mhThis )
	{
		StDeviceManager *pDeviceManager = (StDeviceManager *)mhThis;
		iSize = pDeviceManager->GetGenTLSize();
	}
	return iSize;
}
//StGenTL* StGenDeviceManager::GetGenTL(int index)
void* StGenDeviceManager::GetGenTL(int index)
{
	StGenTL* aGenTL = NULL;
	if( mhThis )
	{
		StDeviceManager *pDeviceManager = (StDeviceManager *)mhThis;
		aGenTL = pDeviceManager->GetGenTL(index);
	}
	return (void *)aGenTL;
}