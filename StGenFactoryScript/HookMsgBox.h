// HookMsgBox.h �w�b�_�t�@�C��
// 1�x�����ǂ܂��Ȃ�����
#pragma once

// �v���g�^�C�v�錾
LRESULT CALLBACK MsgBoxHookProc (int nCode, WPARAM wParam ,LPARAM lParam );
int AfxMessageBoxHooked( LPCTSTR message , UINT nType );
int AfxMessageBoxHookedID( UINT nIDPrompt, UINT nType );

