//==============================================================================
/// @file	InputManager.h
/// @brief	���͊֌W���C�u����
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_INPUTMANAGER_H__
#define __AKILIB_INPUTMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	���̓}�l�[�W���[�N���X
	//==============================================================================
	class CInputManager : public CSingletonTemplate<CInputManager>
	{
	public:
		//==============================================================================
		// �t�����h�N���X�錾
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// �萔�錾
		//==============================================================================
		/// @brief	���͏��
		enum EInputState
		{
			FREE	= 0x00,	// ������Ă�
			PUSH	= 0x01,	// �����ꂽ	
			HOLD	= 0x02,	// ������Ă���
			PULL	= 0x04	// �����ꂽ
		};

		/// @brief	�}�E�X�{�^���萔
		enum EMouseButton
		{
			MBUTTON_L,			///< ���{�^��
			MBUTTON_R,			///< �E�{�^��
			MBUTTON_CENTER,		///< �����{�^��
			MBUTTON_NUM			///< �}�E�X�{�^����
		};


		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CInputManager();

		/// @brief	�f�X�g���N�^
		~CInputManager(){}

		/// @brief	���͏�ԍX�V
		void UpdateState();

		/// @brief	������
		/// @param[in]	hinst	�C���X�^���X�n���h��
		/// @param[in]	hwnd	�E�B���h�E�n���h��
		/// @retval true  ����
		/// @retval false ���s
		bool Init( HINSTANCE hinst, HWND hwnd );

		/// @brief	���͊֌W�f�o�C�X���J������
		void Release();

		//==============================================================================
		// �L�[�{�[�h�֘A
		//==============================================================================

		/// @brief	�L�[�{�[�h�̓��͐����ݒ肷��
		/// @param[in]	flg	true�œ��͐���L���Afalse�Ŗ���
		void SetKeyboardAcquire( bool flg );

		/// @brief	�L�[�{�[�h�̓��͏�Ԃ��擾����
		/// @param[in]	key		DIK_�Ŏn�܂�L�[�ԍ�
		/// @param[in]	state	EInputState�̒萔(�r�b�g�t���O�����\)
		/// @retval true  ���͏�Ԉ�v
		/// @retval false ���͏�ԕs��v
		bool IsKeyboardState( BYTE key, BYTE state );

		//==============================================================================
		// �}�E�X�֘A
		//==============================================================================

		/// @brief	�}�E�X�̓��͐����ݒ肷��
		/// @param[in]	flg	true�œ��͐���L���Afalse�Ŗ���
		void SetMouseAcquire( bool flg );

		/// @brief	���͏�Ԏ擾
		/// @param[in]	button	EMouseButton�񋓒萔�̒l
		/// @param[in]	state	EInputState�̒萔(�r�b�g�t���O�����\)
		/// @retval true  ���͏�Ԉ�v
		/// @retval false ���͏�ԕs��v
		bool IsMouseButtonState( EMouseButton button, BYTE state );

		/// @brief	�E�B���h�E����݂��}�E�X���W�l��
		/// @return	�ʒu
		inline const POINT& GetMousePosByWindow()
		{
			return m_MousePos;
		}

		/// @brief	���΃}�E�X���W�l��
		/// @return	�ʒu
		inline POINT GetMousePosByRef()
		{
			POINT point = { m_MouseState.lX, m_MouseState.lY };
			return point;
		}

	private:
		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		LPDIRECTINPUT8			m_lpInput;						///< ���̓C���^�[�t�@�C�X
		LPDIRECTINPUTDEVICE8	m_lpKeyboard;					///< �L�[�{�[�h�f�o�C�X
		LPDIRECTINPUTDEVICE8	m_lpMouse;						///< �}�E�X�f�o�C�X
		BYTE					m_KeyboardState[256];			///< �L�[�{�[�h��ԕۑ��z��
		BYTE					m_MouseButtonState[MBUTTON_NUM];///< �}�E�X�{�^����ԕۑ��z��
		POINT					m_MousePos;						///< �}�E�X�ʒu(�E�B���h�E����̐�΍��W)
		DIMOUSESTATE2			m_MouseState;					///< �}�E�X���
		HWND					m_hWnd;							///< �E�B���h�E�n���h��(�}�E�X�ʒu�l���p)
		
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�C���v�b�g�C���^�[�t�F�C�X�쐬
		/// @param[in]	hinst	�C���X�^���X�n���h��
		void CreateInterFace( HINSTANCE hinst );

		/// @brief	�L�[�{�[�h�f�o�C�X������
		/// @param[in]	hwnd	�E�B���h�E�n���h��
		/// @retval true  ����
		/// @retval false ���s
		bool InitKeyboard( HWND hwnd );

		/// @brief	�}�E�X�f�o�C�X������
		/// @param[in]	hwnd	�E�B���h�E�n���h��
		/// @retval true  ����
		/// @retval false ���s
		bool InitMouse( HWND hwnd );

		/// @brief	�L�[�{�[�h��ԍX�V
		void UpdateKeyboardState();

		/// @brief	�L�[�{�[�h��ԍX�V
		void UpdateMouseState();
	};
}

#define AKIINPUTMGR		AkiLib::CInputManager::GetInstance()
#define AKIINPUTSTATE	AkiLib::CInputManager

#endif // #define __AKILIB_INPUTMANAGER_H__
