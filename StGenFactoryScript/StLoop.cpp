#include "StdAfx.h"
#include "StLoop.h"


StLoop::StLoop(void)
{
	Initialize();
}

StLoop::StLoop(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szJudgement, void *aDialog)
	:StCheckBase( szClassName, szVarName, NULL, aDialog )
	, m_szJudgementSentence(szJudgement)
{
	Initialize();
}

StLoop::~StLoop(void)
{
}
void StLoop::Initialize(void)
{
	m_nStartLine = -1;
	m_nEndLine = -1;
	m_Judge = TRUE;
}

//-------------------------------------------------------------------------------
//function
//-------------------------------------------------------------------------------
BOOL StLoop::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction==_T("Start") )
	{
		if( _tcslen(szArgument)>0 )
			m_szJudgementSentence = szArgument;
		BOOL bJudge = Judgement();
		INT nLine = -1;
		if( !bJudge )
		{
			nLine = m_nEndLine;
			if( nLine>=0 ) nLine++;
		}
		m_nNextLine = nLine;
		bReval = TRUE;
	}
	else
	if( szFunction==_T("End") )
	{
		m_nNextLine = m_nStartLine;
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
//-------------------------------------------------------------------------------
BOOL OperationMain( std::vector <StValue *> &vecValue, CString szString, double &dblValue, BOOL &bVariable );

BOOL StLoop::Judgement(void)
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
			double dblValue;
			BOOL bVariable = FALSE;
			nMode = 0;
			OperationMain( m_vecValue, szLeft, dblLeftValue, bVariable );
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
			//Right---------------------
			CString szRightValue;
			double dblRightValue=-1;
			INT nRightMode=-1;

			OperationMain( m_vecValue, szRight, dblRightValue, bVariable );
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
			//Judge--------------------------------------
			if( nMode==0 )
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

INT StLoop::GetNextLine(void)
{
	return m_nNextLine;
}
