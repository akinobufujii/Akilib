//==============================================================================
/// @file Utility.h
/// @brief	汎用プログラム集
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_UTILITY_H__
#define __AKILIB_UTILITY_H__

/// @brief	メモリリーク検出マクロ
#ifdef _DEBUG
#define AKINEW  new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#else
#define AKINEW  new( std::nothrow )
#endif

/// @brief	アラインメントマクロ
#define AKIALIGN16 _declspec(align(16))
namespace AkiLib
{
	//==============================================================================
	// 型宣言
	//==============================================================================
	typedef std::basic_string<TCHAR> tstring;	///< 環境対応string型
	
	//==============================================================================
	// 関数
	//==============================================================================
	/// @brief	エラーメッセージボックス関数(Unicode用)
	/// @param[in]	info	エラー内容
	/// @param[in]	flg		ボタンの種類フラグ
	/// @return	MessageBoxの戻り値
	inline int ErrorMessageBox( LPCWSTR info, UINT flg = MB_OK )
	{
		return MessageBoxW( nullptr, info, L"Error", MB_ICONERROR | flg );
	}

	/// @brief	エラーメッセージボックス関数(マルチバイト用)
	/// @param[in]	info	エラー内容
	/// @param[in]	flg		ボタンの種類フラグ
	/// @return	MessageBoxの戻り値
	inline int ErrorMessageBox( LPCSTR info, UINT flg = MB_OK )
	{
		return MessageBoxA( nullptr, info, "Error", MB_ICONERROR | flg );
	}

	/// @brief	COM解放
	/// @param[inout]	obj	開放するオブジェクト
	template <typename TYPE>
	inline void SafeRelease( TYPE& obj )
	{
		if( obj )
		{
			obj->Release();
			obj = nullptr;
		}
	}

	/// @brief	メモリ解放
	/// @param[inout]	obj	開放するオブジェクト
	template <typename TYPE>
	inline void SafeDelete( TYPE& obj )
	{
		if( obj )
		{
			delete obj;
			obj = nullptr;
		}
	}

	/// @brief	メモリ配列解放
	/// @param[inout]	obj	開放するオブジェクト
	template <typename TYPE>
	inline void SafeDeleteArray( TYPE& obj )
	{
		if( obj )
		{
			delete[] obj;
			obj = nullptr;
		}
	}

	/// @brief	ポインタへ変換する
	/// @param[in]	value	ポインタへ変換する数値
	template <typename TYPE>
	inline TYPE* ConvertValue( const TYPE& value )
	{
		return &static_cast<TYPE>( value );
	}

	/// @brief	カラーマクロ(上位ビットからRGBAで整列)
	/// @param[in]	r	赤成分(0～255)
	/// @param[in]	g	緑成分(0～255)
	/// @param[in]	b	青成分(0～255)
	/// @param[in]	a	α成分(0～255)
	/// @return	整数値に合成した32ビット数値を返す
	inline unsigned int ColorRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		return (r << 24) | (g << 16) | (b << 8) | a;
	}

	//==============================================================================
	// クラス
	//==============================================================================
	/// @brief コピー禁止クラス(private継承して使います)
	class CNoCopyable
	{
	protected:
		CNoCopyable(){}
		virtual ~CNoCopyable(){}

	private:
		void operator =( const CNoCopyable& obj );
		CNoCopyable( const CNoCopyable& obj );
	};

	/// @brief シングルトンクラステンプレート(friendクラス指定して使用してください)
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