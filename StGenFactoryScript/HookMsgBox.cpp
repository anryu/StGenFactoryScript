// HookMsgBox.cpp �\�[�X�t�@�C��
// StfAfx.h�C���N���[�h ��غ��ٍ߲ς݁`�̃G���[���o���Ȃ����߂ɕK�v
#include "StdAfx.h"
// �w�b�_�C���N���[�h
#include "HookMsgBox.h"
#include "Common.h"

//////////////////////////////////////////////////////////////////
// �t�b�N�n���h���p�̕ϐ����O���[�o���Ő錾����B
HHOOK MyHookHandle;	// �t�b�N�n���h���ϐ�
// OK�̕�����ƁA�L�����Z���̕�����
// �{�^���̕�����ύX�������ꍇ�͂�����ύX���Ă�������
LPCTSTR strOK = _T("OK");			// OK�{�^���̕���
LPCTSTR strCancel = _T("Cancel");	// �L�����Z���{�^���̕���
LPCTSTR strYes = _T("Yes");	// OK�{�^���̕���
LPCTSTR strNo = _T("No");	// No�{�^���̕���
LPCTSTR strRetry= _T("Retry");	// No�{�^���̕���

////////////////////////////////////////////////////////////////////
//
//		���b�Z�[�W�{�b�N�X���������̂��߂�
//		�t�b�N�֐��i���[�J���t�b�N�p�j
//		�� �O���[�o���֐��ł���K�v������܂��B
//		���̊֐��̒���ς���Ƃ��͐T�d�ɍs���ĉ������B
//
LRESULT CALLBACK MsgBoxHookProc (int nCode, WPARAM wParam ,LPARAM lParam )
{
	// �R�[�h���f
	if (nCode >= 0)
	{
		if ( nCode == HCBT_ACTIVATE)
		{
			if( GetLocale()!=GetSupportedLCID(0) )
			{

				// OK�{�^��(IDOK)�̓��e������������
				SetDlgItemText( ( HWND )wParam, IDOK, strOK );
				// �L�����Z���{�^��(IDCANCEL)�̓��e������������
				SetDlgItemText( ( HWND )wParam, IDCANCEL, strCancel );

				// �͂��{�^��(IDYES)�̓��e������������
				SetDlgItemText( ( HWND )wParam, IDYES, strYes );

				// �������{�^��(IDNO)�̓��e������������
				SetDlgItemText( ( HWND )wParam, IDNO, strNo );

				// �Ď��s�{�^��(IDRETRY)�̓��e������������
				SetDlgItemText( ( HWND )wParam, IDRETRY, strRetry );

				HRESULT ret;
				// �t�b�N�֐����A���C���X�g�[��(�t�b�N�����I�j
				ret = UnhookWindowsHookEx ( MyHookHandle );
			}
		}
	}
	// ���̃t�b�N�ɓn��
	return CallNextHookEx ( MyHookHandle, nCode, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////
//
//		AfxMessageBoxHooked(���b�Z�[�W, 
//
int AfxMessageBoxHooked( LPCTSTR message , UINT nType )
{
	// �t�b�N�֐�(MsgBoxHookProc)���C���X�g�[������ SetWindowHookEx
	MyHookHandle = SetWindowsHookEx ( WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId( ) ); 
	return ( AfxMessageBox(message, nType) ); 
	//return  MessageBox("�t�b�N���b�Z�[�W�{�b�N�X", "HookedMsgBox",MB_OKCANCEL);

}

int AfxMessageBoxHookedID( UINT nIDPrompt, UINT nType )
{
	// �t�b�N�֐�(MsgBoxHookProc)���C���X�g�[������ SetWindowHookEx
	MyHookHandle = SetWindowsHookEx ( WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId( ) ); 
	return ( AfxMessageBox(nIDPrompt, nType) ); 
	//return  MessageBox("�t�b�N���b�Z�[�W�{�b�N�X", "HookedMsgBox",MB_OKCANCEL);

}
