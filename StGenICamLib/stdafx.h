// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B
#ifndef WINVER				// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define WINVER 0x0501		// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINNT		// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B                   
#define _WIN32_WINNT 0x0501	// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_WINDOWS 0x0410 // ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE			// IE 6.0 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_IE 0x0600	// ����� IE. �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
// Windows �w�b�_�[ �t�@�C��:
//#include <windows.h>
#include <afxwin.h>



// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B
//#undef GCTLIDLL


//��1.0.0.73 GenApi3.0
#define _GENAPI3


//��_GENAPI3���L���̎�
//�ǉ��̃C���N���[�h�f�B���N�g��
//C:\GenICam_Development\GenICam_V3_0_2-Win32_i86_VS120-Release-Development\library\CPP\include
//�ǉ��̃��C�u�����f�B���N�g��
//C:\GenICam_Development\GenICam_V3_0_2-Win32_i86_VS120-Release-Development\library\CPP\lib\Win32_i86
//�ǉ��̈ˑ��t�@�C��
//GCBase_MD_VC120_v3_0.lib
//GenApi_MD_VC120_v3_0.lib
//�v���v���Z�b�T�̒�`(Debug�̂�)
//GENICAM_USER_ALWAYS_LINK_RELEASE

//��_GENAPI3�������̎�
//�ǉ��̃C���N���[�h�f�B���N�g��
//C:\Program Files\GenICam_v2_4\library\CPP\include
//�ǉ��̃��C�u�����f�B���N�g��
//C:\Program Files\GenICam_v2_4\library\CPP\lib\Win32_i86

//�ǉ��̈ˑ��t�@�C��
//GCBase_MD_VC80_v2_4.lib
//GenApi_MD_VC80_v2_4.lib

//��StGenICamLib�����łȂ�StGenFactoryScript�ł��s���B

#ifdef _GENAPI3
#pragma comment(lib,"GCBase_MD_VC120_v3_0.lib")
#pragma comment(lib,"GenApi_MD_VC120_v3_0.lib")
#ifdef _DEBUG
#define GENICAM_USER_ALWAYS_LINK_RELEASE
#endif
#include <TLI/TLI_v3.h>
#else
#pragma comment(lib,"GCBase_MD_VC80_v2_4.lib")
#pragma comment(lib,"GenApi_MD_VC80_v2_4.lib")
#include <TLI/TLI.h>
#endif
//��1.0.0.73 GenApi3.0


//#include <TLI/TLI.h>
#include <GenApi/GenApi.h>
//#include <GenApi/EventAdapterGeneric.h>
using namespace GenICam;
//using namespace GenICam::Client;
using namespace GenApi;

#define ST_GENICAMLIB_EXPORTS


