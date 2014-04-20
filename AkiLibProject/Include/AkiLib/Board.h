//==============================================================================
/// @file	Board.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_BOARD_H__
#define __AKILIB_BOARD_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief �|���S���N���X
	//==============================================================================
	class CBoard
	{
	public:

		//==============================================================================
		// �萔
		//==============================================================================
		static const D3D11_INPUT_ELEMENT_DESC	VERTEX_LAYOUT[];	/// @brief	���_���C�A�E�g
		static const BYTE						NUM_VERTEXELEMENT;	/// @brief	���_�v�f��
		static const BYTE						NUM_VERTEX;			/// @brief	���_��

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CBoard();

		/// @brief	�f�X�g���N�^
		~CBoard();

		/// @brief	������
		/// @param[in]	lpdevice			�f�o�C�X
		/// @retval true	����
		/// @retval false	���s
		bool Init( ID3D11Device1* lpdevice );

		/// @brief	���
		void Release();

		/// @brief	�`��
		/// @param[in]	lpdeviceContext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	topology		�v���~�e�B�u�̃g�|���W(�f�t�H���g�̓g���C�A���O���X�g���b�v)
		void Draw( ID3D11DeviceContext1* lpdeviceContext, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	private:
		//==============================================================================
		// �^�錾
		//==============================================================================
		/// @brief	���_�\����
		struct VERTEX
		{
			XMFLOAT3	Pos;	///< �ʒu
			XMFLOAT4	Color;	///< �F
			XMFLOAT2	UV;		///< UV���W
		};

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		VERTEX			m_Vertex[4];		///< ���_���
		ID3D11Buffer*	m_lpVertexBuffer;	///< ���_�o�b�t�@
	};

}

#endif	// #define __AKILIB_BOARD_H__