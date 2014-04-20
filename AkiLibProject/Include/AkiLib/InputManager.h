//==============================================================================
/// @file	InputManager.h
/// @brief	入力関係ライブラリ
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_INPUTMANAGER_H__
#define __AKILIB_INPUTMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	入力マネージャークラス
	//==============================================================================
	class CInputManager : public CSingletonTemplate<CInputManager>
	{
	public:
		//==============================================================================
		// フレンドクラス宣言
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// 定数宣言
		//==============================================================================
		/// @brief	入力状態
		enum EInputState
		{
			FREE	= 0x00,	// 離されてる
			PUSH	= 0x01,	// 押された	
			HOLD	= 0x02,	// 押されている
			PULL	= 0x04	// 離された
		};

		/// @brief	マウスボタン定数
		enum EMouseButton
		{
			MBUTTON_L,			///< 左ボタン
			MBUTTON_R,			///< 右ボタン
			MBUTTON_CENTER,		///< 中央ボタン
			MBUTTON_NUM			///< マウスボタン数
		};


		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CInputManager();

		/// @brief	デストラクタ
		~CInputManager(){}

		/// @brief	入力状態更新
		void UpdateState();

		/// @brief	初期化
		/// @param[in]	hinst	インスタンスハンドル
		/// @param[in]	hwnd	ウィンドウハンドル
		/// @retval true  成功
		/// @retval false 失敗
		bool Init( HINSTANCE hinst, HWND hwnd );

		/// @brief	入力関係デバイスを開放する
		void Release();

		//==============================================================================
		// キーボード関連
		//==============================================================================

		/// @brief	キーボードの入力制御を設定する
		/// @param[in]	flg	trueで入力制御有効、falseで無効
		void SetKeyboardAcquire( bool flg );

		/// @brief	キーボードの入力状態を取得する
		/// @param[in]	key		DIK_で始まるキー番号
		/// @param[in]	state	EInputStateの定数(ビットフラグ合成可能)
		/// @retval true  入力状態一致
		/// @retval false 入力状態不一致
		bool IsKeyboardState( BYTE key, BYTE state );

		//==============================================================================
		// マウス関連
		//==============================================================================

		/// @brief	マウスの入力制御を設定する
		/// @param[in]	flg	trueで入力制御有効、falseで無効
		void SetMouseAcquire( bool flg );

		/// @brief	入力状態取得
		/// @param[in]	button	EMouseButton列挙定数の値
		/// @param[in]	state	EInputStateの定数(ビットフラグ合成可能)
		/// @retval true  入力状態一致
		/// @retval false 入力状態不一致
		bool IsMouseButtonState( EMouseButton button, BYTE state );

		/// @brief	ウィンドウからみたマウス座標獲得
		/// @return	位置
		inline const POINT& GetMousePosByWindow()
		{
			return m_MousePos;
		}

		/// @brief	相対マウス座標獲得
		/// @return	位置
		inline POINT GetMousePosByRef()
		{
			POINT point = { m_MouseState.lX, m_MouseState.lY };
			return point;
		}

	private:
		//==============================================================================
		// フィールド
		//==============================================================================
		LPDIRECTINPUT8			m_lpInput;						///< 入力インターファイス
		LPDIRECTINPUTDEVICE8	m_lpKeyboard;					///< キーボードデバイス
		LPDIRECTINPUTDEVICE8	m_lpMouse;						///< マウスデバイス
		BYTE					m_KeyboardState[256];			///< キーボード状態保存配列
		BYTE					m_MouseButtonState[MBUTTON_NUM];///< マウスボタン状態保存配列
		POINT					m_MousePos;						///< マウス位置(ウィンドウからの絶対座標)
		DIMOUSESTATE2			m_MouseState;					///< マウス状態
		HWND					m_hWnd;							///< ウィンドウハンドル(マウス位置獲得用)
		
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	インプットインターフェイス作成
		/// @param[in]	hinst	インスタンスハンドル
		void CreateInterFace( HINSTANCE hinst );

		/// @brief	キーボードデバイス初期化
		/// @param[in]	hwnd	ウィンドウハンドル
		/// @retval true  成功
		/// @retval false 失敗
		bool InitKeyboard( HWND hwnd );

		/// @brief	マウスデバイス初期化
		/// @param[in]	hwnd	ウィンドウハンドル
		/// @retval true  成功
		/// @retval false 失敗
		bool InitMouse( HWND hwnd );

		/// @brief	キーボード状態更新
		void UpdateKeyboardState();

		/// @brief	キーボード状態更新
		void UpdateMouseState();
	};
}

#define AKIINPUTMGR		AkiLib::CInputManager::GetInstance()
#define AKIINPUTSTATE	AkiLib::CInputManager

#endif // #define __AKILIB_INPUTMANAGER_H__
