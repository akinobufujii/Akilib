//==============================================================================
/// @file	MQOFile.h
/// @brief	Metasequoia�t�@�C���ǂݍ���
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_MQOFILE_H__
#define __AKILIB_MQOFILE_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	Metasequoia�t�@�C���ǂݍ��݃N���X
	//==============================================================================
	class CMQOFile : public CModel
	{
	public:
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CMQOFile();

		/// @brief	�f�X�g���N�^
		~CMQOFile();

		/// @brief	�ǂݍ���
		/// @param[in]	lpdevice	�f�o�C�X
		/// @param[in]	file	�t�@�C����
		/// @retval true  ����
		/// @retval false ���s
		bool LoadFile( ID3D11Device1* lpdevice, LPCSTR file );
	};	
}

#endif	// #define __AKILIB_MQOFILE_H__