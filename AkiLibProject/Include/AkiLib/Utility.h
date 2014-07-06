//==============================================================================
/// @file Utility.h
/// @brief	�ėp�v���O�����W
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_UTILITY_H__
#define __AKILIB_UTILITY_H__

/// @brief	���������[�N���o�}�N��
#ifdef _DEBUG
#define AKINEW  new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#else
#define AKINEW  new( std::nothrow )
#endif

/// @brief	�A���C�������g�}�N��
#define AKIALIGN16 _declspec(align(16))
namespace AkiLib
{
	//==============================================================================
	// �^�錾
	//==============================================================================
	typedef std::basic_string<TCHAR> tstring;	///< ���Ή�string�^
	
	//==============================================================================
	// �֐�
	//==============================================================================
	/// @brief	�G���[���b�Z�[�W�{�b�N�X�֐�(Unicode�p)
	/// @param[in]	info	�G���[���e
	/// @param[in]	flg		�{�^���̎�ރt���O
	/// @return	MessageBox�̖߂�l
	inline int ErrorMessageBox( LPCWSTR info, UINT flg = MB_OK )
	{
		return MessageBoxW( nullptr, info, L"Error", MB_ICONERROR | flg );
	}

	/// @brief	�G���[���b�Z�[�W�{�b�N�X�֐�(�}���`�o�C�g�p)
	/// @param[in]	info	�G���[���e
	/// @param[in]	flg		�{�^���̎�ރt���O
	/// @return	MessageBox�̖߂�l
	inline int ErrorMessageBox( LPCSTR info, UINT flg = MB_OK )
	{
		return MessageBoxA( nullptr, info, "Error", MB_ICONERROR | flg );
	}

	/// @brief	COM���
	/// @param[inout]	obj	�J������I�u�W�F�N�g
	template <typename TYPE>
	inline void SafeRelease( TYPE& obj )
	{
		if( obj )
		{
			obj->Release();
			obj = nullptr;
		}
	}

	/// @brief	���������
	/// @param[inout]	obj	�J������I�u�W�F�N�g
	template <typename TYPE>
	inline void SafeDelete( TYPE& obj )
	{
		if( obj )
		{
			delete obj;
			obj = nullptr;
		}
	}

	/// @brief	�������z����
	/// @param[inout]	obj	�J������I�u�W�F�N�g
	template <typename TYPE>
	inline void SafeDeleteArray( TYPE& obj )
	{
		if( obj )
		{
			delete[] obj;
			obj = nullptr;
		}
	}

	/// @brief	�|�C���^�֕ϊ�����
	/// @param[in]	value	�|�C���^�֕ϊ����鐔�l
	template <typename TYPE>
	inline TYPE* ConvertValue( const TYPE& value )
	{
		return &static_cast<TYPE>( value );
	}

	/// @brief	�J���[�}�N��(��ʃr�b�g����RGBA�Ő���)
	/// @param[in]	r	�Ԑ���(0�`255)
	/// @param[in]	g	�ΐ���(0�`255)
	/// @param[in]	b	����(0�`255)
	/// @param[in]	a	������(0�`255)
	/// @return	�����l�ɍ�������32�r�b�g���l��Ԃ�
	inline unsigned int ColorRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		return (r << 24) | (g << 16) | (b << 8) | a;
	}

	//==============================================================================
	// �N���X
	//==============================================================================
	/// @brief �R�s�[�֎~�N���X(private�p�����Ďg���܂�)
	class CNoCopyable
	{
	protected:
		CNoCopyable(){}
		virtual ~CNoCopyable(){}

	private:
		void operator =( const CNoCopyable& obj );
		CNoCopyable( const CNoCopyable& obj );
	};

	/// @brief �V���O���g���N���X�e���v���[�g(friend�N���X�w�肵�Ďg�p���Ă�������)
	template<typename TYPE>
	class CSingletonTemplate : private CNoCopyable
	{
	public:
		inline static TYPE* GetInstance()
		{
			static TYPE inst;
			return &inst;
		}

		virtual ~CSingletonTemplate(){}
	};
}

#endif	// #define __AKILIB_UTILITY_H__