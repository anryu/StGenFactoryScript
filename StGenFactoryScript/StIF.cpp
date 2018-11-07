#include "StdAfx.h"
#include "StIF.h"


StIF::StIF(void)
{
}

StIF::StIF(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szJudgement)
	:StCheckBase( szClassName, szVarName, NULL )
	, m_szJudgementSentence(szJudgement)
{
	Initialize();
}

StIF::~StIF(void)
{
}
void StIF::Initialize(void)
{
	m_nStartLine = -1;
	m_nElseLine = -1;
	m_nEndLine = -1;
	//m_Judge = TRUE;
	m_nNextLine = -1;
	m_bJudgeIn = FALSE;
	m_vecElseIfLine.clear();
}

//-------------------------------------------------------------------------------
//function
//-------------------------------------------------------------------------------
BOOL StIF::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction==_T("Start") )
	{
		if( _tcslen(szArgument)>0 )
			m_szJudgementSentence = szArgument;
		BOOL bJudge = Judgement();
		if( bJudge )
		{
			m_bJudgeIn = TRUE;
		}
		else
		{
			//éüÇÃElseIfÇ©ElseÇ©EndÇ…îÚÇ‘
			m_nNextLine = GetNextElseLine();
		}
		bReval = TRUE;
	}
	else
	if( szFunction==_T("ElseIf") )
	{
		if( m_bJudgeIn )
		{
			//m_nNextLine = m_nEndLine + 1;
			m_nNextLine = m_nEndLine;
		}
		else
		{
			m_szJudgementSentence = szArgument;
			BOOL bJudge = Judgement();
			if( bJudge )
			{
				m_bJudgeIn = TRUE;
			}
			else
			{
				//éüÇÃElseIfÇ©ElseÇ©EndÇ…îÚÇ‘
				m_nNextLine = GetNextElseLine();
			}
		}
		bReval = TRUE;
	}
	else
	if( szFunction==_T("Else") )
	{
		if( m_bJudgeIn )
		{
			//m_nNextLine = m_nEndLine + 1;
			m_nNextLine = m_nEndLine;
		}
		else
		{
			m_bJudgeIn = TRUE;
		}
		bReval = TRUE;
	}
	else
	if( szFunction==_T("End") )
	{
		//m_nNextLine = m_nEndLine + 1;
		bReval = TRUE;
	}
	else
	{
		m_nNextLine = -1;
	}
	return bReval;
}

//-------------------------------------------------------------------------------
//Judgement
//BOOL &bVariable  :  ïœêîÇ™ÇÕÇ¢Ç¡ÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
//BOOL &bString    : false:êîíl true:ï∂éöóÒ
//-------------------------------------------------------------------------------
//Å•1.0.0.1072d
#ifdef IF_OLD
BOOL OperationMain( std::vector <StValue *> &vecValue, CString szString, double &dblValue, BOOL &bVariable );
//BOOL OperationMain2( std::vector <StValue *> &vecValue, CString szString, double &dblValue, CString &szValueString, BOOL &bVariable, BOOL &bRetString );
#else
BOOL OperationMain2( std::vector <StValue *> &vecValue, CString szString, double &dblValue, CString &szValueString, BOOL &bVariable, BOOL &bRetString );
#endif
//Å£1.0.0.1072d

BOOL StIF::Judgement(void)
{
	BOOL bReval=FALSE;
	PTCHAR szExpresssion[] = {_T("=="),_T("!="),_T("<="),_T(">="),_T("<"),_T(">")};
	CString szJudgementSentence = m_szJudgementSentence;
	for( int i=0; i<_countof(szExpresssion); i++ )
	{
		INT nPos = szJudgementSentence.Find(szExpresssion[i]);
		if( nPos>0 )
		{
			CString szLeft = szJudgementSentence.Left(nPos);
			CString szRight = szJudgementSentence.Mid(nPos+_tcslen(szExpresssion[i]));
			szLeft.Trim();
			szRight.Trim();
			INT nMode = -1;
			CString szLeftValue;
			double dblLeftValue=-1;
			//Left---------------------
			//Å•1.0.0.1072d
			//double dblValue;
			//Å£1.0.0.1072d
			BOOL bVariable = FALSE;
			nMode = 0;
			//Å•1.0.0.1072d

#ifdef IF_OLD
			OperationMain( m_vecValue, szLeft, dblLeftValue, bVariable );
#else
			BOOL bLeftString = FALSE;
			OperationMain2( m_vecValue, szLeft, dblLeftValue, szLeftValue, bVariable, bLeftString );
#endif
			//Å£1.0.0.1072d

			//Å•1.0.0.1072d
#ifdef IF_OLD
			if( bVariable )
			{
				nMode = 0;
			}

			for( int j=0; j<m_vecValue.size() && nMode<0; j++ )
			{
				if( szLeft==m_vecValue[j]->GetVarName()->GetUnicode() )
				{
					nMode = m_vecValue[j]->GetMode();
					if( nMode==0 )
					{
						dblLeftValue = m_vecValue[j]->GetValue();
					}
					else
					{
						szLeftValue = m_vecValue[j]->GetValueString();
					}
					break;
				}
			}
			if( nMode<0 )
			{
				nMode = 0;
				if( szLeft.Left(1)==_T("\"") )
				{
					nMode = 1;
				}
				if( nMode==0 )
				{
					dblLeftValue = _ttof(szLeft);
				}
				else
				{
					szLeftValue = szLeft.Mid(1);
					INT iPos = szLeftValue.Find(_T("\""));
					if( iPos>=0 )
					{
						szLeftValue = szLeftValue.Left(iPos);
					}
				}
			}
#endif
			//Å£1.0.0.1072d

			//Right---------------------
			CString szRightValue;
			double dblRightValue=-1;
			INT nRightMode=-1;
			//Å•1.0.0.1072d
#ifdef IF_OLD
			OperationMain( m_vecValue, szRight, dblRightValue, bVariable );
#else
			BOOL bRightString = FALSE;
			OperationMain2( m_vecValue, szRight, dblRightValue, szRightValue, bVariable, bRightString );
#endif

#ifdef IF_OLD
			if( bVariable )
			{
				nRightMode = 0;
			}

			for( int j=0; j<m_vecValue.size() && nRightMode<0; j++ )
			{
				if( szRight==m_vecValue[j]->GetVarName()->GetUnicode() )
				{
					if( m_vecValue[j]->GetMode()!=nMode ) continue;
					nRightMode = nMode;
					if( nMode==0 )
					{
						dblRightValue = m_vecValue[j]->GetValue();
					}
					else
					{
						szLeftValue = m_vecValue[j]->GetValueString();
					}
					break;
				}
			}
			if( nRightMode<0 )
			{
				nRightMode = nMode;
				if( nRightMode==0 )
				{
					dblRightValue = _ttof(szRight);
				}
				else
				{
					szRightValue = szRight.Mid(1);
					INT iPos = szRightValue.Find(_T("\""));
					if( iPos>=0 )
					{
						szRightValue = szRightValue.Left(iPos);
					}
				}

			}
#endif
			//Å£1.0.0.1072d




			//Judge--------------------------------------
			//Å•1.0.0.1072d
#ifdef IF_OLD
			if( nMode==0 )
#else
			if( bLeftString==FALSE && bRightString==FALSE )
#endif
			//Å£1.0.0.1072d
			{
				if( i==0 )	//==
				{
					if( dblLeftValue==dblRightValue ) bReval = TRUE;
				}
				else
				if( i==1 )	//!=
				{
					if( dblLeftValue!=dblRightValue ) bReval = TRUE;
				}
				else
				if( i==2 )	//<=
				{
					if( dblLeftValue<=dblRightValue ) bReval = TRUE;
				}
				else
				if( i==3 )	//>=
				{
					if( dblLeftValue>=dblRightValue ) bReval = TRUE;
				}
				else
				if( i==4 )	//<
				{
					if( dblLeftValue<dblRightValue ) bReval = TRUE;
				}
				else
				if( i==5 )	//>
				{
					if( dblLeftValue>dblRightValue ) bReval = TRUE;
				}

			}
			else
			//Å•1.0.0.1072d
#ifdef IF_OLD
#else
			if( bLeftString==TRUE && bRightString==TRUE )
#endif
			//Å£1.0.0.1072d
			{
				if( i==0 )	//==
				{
					if( szLeftValue==szRightValue ) bReval = TRUE;
				}
				else
				if( i==1 )	//!=
				{
					if( szLeftValue!=szRightValue ) bReval = TRUE;
				}
			}
			break;
		}
	}
	return bReval;
}

INT StIF::GetNextLine(void)
{
	INT nReturn = m_nNextLine;
	m_nNextLine = -1;
	return nReturn;
}

INT StIF::GetNextElseLine(void)
{
	INT nOutLine = -1;
	for( int i=0; i<m_vecElseIfLine.size() && nOutLine<0; i++  )
	{
		if( m_nNowLine<m_vecElseIfLine[i] )
		{
			nOutLine = m_vecElseIfLine[i];
		}
	}
	if( nOutLine<0 && m_nElseLine>0 )
	{
		if( m_nNowLine<m_nElseLine )
			nOutLine = m_nElseLine;
	}
	if( nOutLine<0 && m_nEndLine>0 )
	{
		nOutLine = m_nEndLine;
	}
	return nOutLine;
}



