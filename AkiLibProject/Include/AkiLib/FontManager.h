//==============================================================================
/// @file	FontManager.h
/// @brief	�t�H���g�֘A
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_FONTMANAGER_H__
#define __AKILIB_FONTMANAGER_H__

namespace AkiLib
{
	/// @brief	1�������̃f�[�^
	struct CHARACTORDATA
	{
		ID3D11Texture2D*			lpTexture;		///< �e�N�X�`��
		ID3D11ShaderResourceView*	lpResourceView;	///< ���\�[�X�r���[
		XMFLOAT2					Size;			///< �t�H���g�̑傫��

		CHARACTORDATA():
			lpTexture		( nullptr ),
			lpResourceView	( nullptr ),
			Size			( 0, 0 )
		{
		}
	};
	typedef std::shared_ptr<CHARACTORDATA>		SPCHARDATA;		///< �����f�[�^�\���̃X�}�[�g�|�C���^�^

	//==============================================================================
	/// @brief	�t�H���g�}�l�[�W���[�N���X(�V���O���g��)
	//==============================================================================
	class CFontManager : public CSingletonTemplate<CFontManager>
	{
	public:
		//==============================================================================
		// �t�����h�w��
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// �^�錾
		//==============================================================================
		typedef std::map<std::wstring, SPCHARDATA>	FONTMAPW;		///< �����f�[�^�}�b�v�^(Unicode�p)
		typedef std::pair<std::wstring, SPCHARDATA>	FONTMAPPAIRW;	///< �����f�[�^�}�b�v�v�f�^
		typedef std::map<std::string, SPCHARDATA>	FONTMAPA;		///< �����f�[�^�}�b�v�^(Unicode�p)
		typedef std::pair<std::string, SPCHARDATA>	FONTMAPPAIRA;	///< �����f�[�^�}�b�v�v�f�^

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CFontManager();

		/// @brief	�f�X�g���N�^
		~CFontManager();

		/// @brief	�w�蕶���e�N�X�`������(Unicode�p)
		/// @param[in]	lpdevice	�f�o�C�X
		/// @param[in]	lpcontext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	charactor	�쐬���镶��
		/// @return	1�������̃e�N�X�`���f�[�^���܂񂾃X�}�[�g�|�C���^
		SPCHARDATA CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::wstring& charactor, UINT fontsize );

		/// @brief	�w�蕶���e�N�X�`������(�}���`�o�C�g�p)
		/// @param[in]	lpdevice	�f�o�C�X
		/// @param[in]	lpcontext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	charactor	�쐬���镶��
		/// @return	1�������̃e�N�X�`���f�[�^���܂񂾃X�}�[�g�|�C���^
		SPCHARDATA CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::string& charactor, UINT fontsize );

	private:
		FONTMAPW	m_FontMapW;	///< �t�H���g�}�b�v(Unicode�p)
		FONTMAPA	m_FontMapA;	///< �t�H���g�}�b�v(�}���`�o�C�g�p)
	};
}

#define AKIFONTMGR	AkiLib::CFontManager::GetInstance()

#endif // #ifndef __AKILIB_FONTMANAGER_H__