#pragma once
#include "afxwin.h"
//#include "CheckItemID.h"


class CCheckListBoxForCheck :
	public CCheckListBox
{
public:
	CCheckListBoxForCheck(void);
	~CCheckListBoxForCheck(void);
	//void AddItem(LPCTSTR szName, E_CHECK_ITEM_ID nCheckItemID)
	void AddItem(LPCTSTR szName, INT nCheckItemID)
	{
		int lbErr = this->InsertString(-1, szName);
		if (lbErr == LB_ERR || lbErr == LB_ERRSPACE)
		{
		}
		else
		{
			int nIndex = this->GetCount() - 1;
			this->SetItemData(nIndex, (DWORD_PTR)nCheckItemID);
		}
	};

	void AddItem(LPCTSTR szName, INT nCheckItemID, INT nCheckMode)
	{
		int lbErr = this->InsertString(-1, szName);
		if (lbErr == LB_ERR || lbErr == LB_ERRSPACE)
		{
		}
		else
		{
			int nIndex = this->GetCount() - 1;
			nCheckItemID &= 0xFFFF;
			nCheckItemID |= (nCheckMode<<16);
			this->SetItemData(nIndex, (DWORD_PTR)nCheckItemID);
		}
	}

	//BOOL IsChecked(E_CHECK_ITEM_ID nCheckItemID)
	BOOL IsChecked(INT nCheckItemID)
	{
		BOOL isChecked = FALSE;
		for(int nIndex = 0; nIndex < GetCount(); nIndex++)
		{
			//if(nCheckItemID == (E_CHECK_ITEM_ID)GetItemData(nIndex))
			//Å•1.0.0.1072
			//if(nCheckItemID == GetItemData(nIndex))
			UINT unItemData = GetItemData(nIndex)&0xFFFF;
			if(nCheckItemID == unItemData)
			//Å£1.0.0.1072
			{
				isChecked = (BST_CHECKED == GetCheck(nIndex));
				break;
			}
		}
		return(isChecked);

	};
	//void SetCheck(E_CHECK_ITEM_ID nMaxID)
	void SetCheck(INT nMaxID)
	{
		for(int nIndex = 0; nIndex < GetCount(); nIndex++)
		{
			//CCheckListBox::SetCheck(nIndex, ((E_CHECK_ITEM_ID)GetItemData(nIndex) <= nMaxID) ? BST_CHECKED : BST_UNCHECKED);
			//Å•1.0.0.1072
			//CCheckListBox::SetCheck(nIndex, (GetItemData(nIndex) <= nMaxID) ? BST_CHECKED : BST_UNCHECKED);
			UINT unItemData = GetItemData(nIndex)&0xFFFF;
			CCheckListBox::SetCheck(nIndex, (unItemData <= nMaxID) ? BST_CHECKED : BST_UNCHECKED);
			//Å£1.0.0.1072
		}
	};

	//Å•1.0.0.1072
	void SetCheck(INT nMaxID, INT nCheckMode,BOOL bChecked=TRUE)
	{
		for(int nIndex = 0; nIndex < GetCount(); nIndex++)
		{
			UINT unItemData = GetItemData(nIndex);
			INT nCheckModeBit = unItemData>>16;
			unItemData &= 0xFFFF;
			BOOL bCheck = ((unItemData <= nMaxID) && ((nCheckModeBit>>nCheckMode)&1));
			bCheck &= bChecked;
			CCheckListBox::SetCheck(nIndex, bCheck ? BST_CHECKED : BST_UNCHECKED);
		}
	}
	//Å£1.0.0.1072

	void UncheckAll()
	{
		//SetCheck((E_CHECK_ITEM_ID)-1);
		SetCheck(-1);
	};

	//void SetCheckItem( E_CHECK_ITEM_ID nID, INT nCheck )
	void SetCheckItem( INT nID, INT nCheck )
	{
		for(int nIndex = 0; nIndex < GetCount(); nIndex++)
		{
			//if( (E_CHECK_ITEM_ID)GetItemData(nIndex) == nID )
			//Å•1.0.0.1072
			//if( GetItemData(nIndex) == nID )
			UINT unItemData = GetItemData(nIndex)&0xFFFF;
			if( unItemData == nID )
			//Å£1.0.0.1072
			{
				CCheckListBox::SetCheck(nIndex, nCheck );
				break;
			}
		}
	};

};
