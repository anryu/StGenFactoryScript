#include "StdAfx.h"
#include "StValue.h"


StValue::StValue(void)
		: m_ClassName(_T("StValue"))
{
	Initialize();
}

StValue::StValue(LPCTSTR szVarName, LPCTSTR szArgument)
		: m_ClassName(_T("StValue"))
		, m_VarName(szVarName)
		, m_szInit(szArgument)
{
	Initialize();
}

StValue::~StValue(void)
{
}

void StValue::Initialize()
{
	m_szValue = _T("");
	m_dblValue = 0.0;
	m_nMode = -1;
	m_szInit.Trim();
	if( m_szInit.GetLength() > 0 )
	{


		//Å•1.0.0.1024
		//if( m_szInit.Left(1)==_T("\"") )
		//{
		//	m_nMode = 1;
		//	m_szValue = m_szInit;
		//}
		//else
		//{
		//	m_nMode = 0;
		//	m_dblValue = _ttof(m_szInit);
		//}


		CString szInit = m_szInit;


		//Å•1.0.0.1031
		for( int i=0; i<(int)m_vecValue.size(); i++ )
		{
			if( szInit==m_vecValue[i]->GetVarName()->GetUnicode() )
			{
				if( m_vecValue[i]->GetMode()!=0 )
				{
					m_nMode = 1;
					m_szValue = m_vecValue[i]->GetValueString();
				}
				else
				{
					m_nMode = 0;
					m_dblValue = m_vecValue[i]->GetValue();
				}
				return;
			}
		}
		//Å£1.0.0.1031




		double dblValue;
		BOOL bReval = GetValue(m_szInit, dblValue);
		if( bReval )
		{
			m_nMode = 0;
			m_dblValue = dblValue;
		}
		else
		{
			m_nMode = 1;
			m_szValue = m_szInit;
			CString szInit = m_szInit;
			if( szInit.Left(1)==_T("\"") )
			{
				szInit = szInit.Mid(1);
				INT nPos = szInit.Find(_T("\""));
				if( nPos>0)
				{
					szInit = szInit.Left(nPos);
				}
				else
				if( nPos==0)
				{
					szInit.Empty();
				}
				m_szValue = szInit;
			}
		}
		//Å£1.0.0.1024
	}

}


BOOL OperationMain( std::vector <StValue *> &vecValue, CString szString, double &dblValue, BOOL &bVariable );

//Å•1.0.0.1024
BOOL StValue::GetValue(LPCTSTR szFunc, double &dblValue)
{
	BOOL bVariable = FALSE;
	double dblVal;
	OperationMain( m_vecValue, szFunc, dblVal, bVariable );
	if( bVariable )
	{
		dblValue = dblVal;
	}
	else
	{
		//ï∂éöóÒÇ…StValueÇ»ÇµÅAï∂éöÇ©Ç«Ç§Ç©îªíf
		CString szFc(szFunc);
		if( szFc.Left(1)==_T("\"") )
		{
			//szFc = szFc.Mid(1);
			//INT nPos = szFc.Find(_T("\""));
			//if( nPos>0)
			//{
			//	szFc = szFc.Left(nPos);
			//}
			//else
			//if( nPos==0)
			//{
			//	szFc.Empty();
			//}
			//m_szValue = szFc;
		}
		else
		{
			bVariable = TRUE;
			dblValue = _ttof(szFunc);
		}

	}
	return bVariable;
}
//Å£1.0.0.1024

BOOL StValue::function(LPCTSTR szFunc)
{
	BOOL bReval = FALSE;
	CString aFunc(szFunc);
	aFunc.Trim();
	
	INT nVarSize = m_VarName.GetLength();
	if( aFunc.Left(nVarSize).Compare(m_VarName.GetUnicode())==0 )
	{
		aFunc = aFunc.Mid(nVarSize);
		aFunc.Trim();

		if( aFunc.Left(1).Compare(_T("="))==0 )
		{
			m_szInit = aFunc.Mid(1).Trim();
			Initialize();
			bReval = TRUE;
		}
		else
		if( m_nMode==0 )	//Value
		{
			if( aFunc.Compare(_T("++"))==0 )
			{
				m_dblValue += 1.0;
				bReval = TRUE;
			}
			else
			if( aFunc.Compare(_T("--"))==0 )
			{
				m_dblValue -= 1.0;
				bReval = TRUE;
			}
			else
			if( aFunc.Left(2).Compare(_T("+="))==0 )
			{
				//Å•1.0.0.1024
				//m_dblValue += _ttof(aFunc.Mid(2).Trim());
				//bReval = TRUE;
				double dblValue;
				bReval = GetValue(aFunc.Mid(2).Trim(), dblValue);
				if( bReval )
				{
					m_dblValue += dblValue;
				}
				//Å£1.0.0.1024
			}
			else
			if( aFunc.Left(2).Compare(_T("-="))==0 )
			{
				//Å•1.0.0.1024
				//m_dblValue -= _ttof(aFunc.Mid(2).Trim());
				//bReval = TRUE;
				double dblValue;
				bReval = GetValue(aFunc.Mid(2).Trim(), dblValue);
				if( bReval )
				{
					m_dblValue -= dblValue;
				}
				//Å£1.0.0.1024
			}
		}
		else
		if( m_nMode==1 )	//String
		{
			if( aFunc.Left(2).Compare(_T("+="))==0 )
			{

				//Å•1.0.0.1031
				//m_szValue += aFunc.Mid(2).Trim();

				CString szValue = aFunc.Mid(2).Trim();
				for( int i=0; i<(int)m_vecValue.size(); i++ )
				{
					if( szValue==m_vecValue[i]->GetVarName()->GetUnicode() )
					{
						if( m_vecValue[i]->GetMode()!=0 )
						{
							szValue = m_vecValue[i]->GetValueString();
						}
						else
						{
							//Å•1.0.0.1072c
							//szValue.Format(_T("%i"),(INT)m_vecValue[i]->GetValue());
							double dblData = m_vecValue[i]->GetValue();
							if( dblData-(double)(INT)dblData>0.0 )
							{
								szValue.Format(_T("%f"),dblData);
							}
							else
							{
								szValue.Format(_T("%i"),(INT)dblData);
							}
							//Å£1.0.0.1072c
						}
						break;
					}
				}

				//Å•1.0.0.1032
				//m_szValue += szValue;
				if( szValue.Left(1)==_T("\"") && szValue.Right(1)==_T("\"") )
				{
					szValue = szValue.Mid(1);
					szValue = szValue.Left(szValue.GetLength()-1);
				}
				CString szCheckString = m_szValue;
				if( szCheckString.Left(1)==_T("\"") && szCheckString.Right(1)==_T("\"") )
				{
					szCheckString = szCheckString.Left(szCheckString.GetLength()-1);
					szCheckString += szValue;
					szCheckString += _T("\"");
				}
				else
				{
					szCheckString += szValue;
				}
				m_szValue = szCheckString;
				//Å£1.0.0.1032
				//Å£1.0.0.1031
				bReval = TRUE;
			}
		}
	}

	return bReval;
}

