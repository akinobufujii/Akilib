//==============================================================================
/// @file	FBXManager.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_FBXMANAGER_H__
#define __AKILIB_FBXMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	FBXモデルマネージャー(シングルトン)
	//==============================================================================
	class CFBXManager
	{
	public:
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	インスタンス獲得
		/// @return	インスタンスを返す
		inline static CFBXManager* GetInstance()
		{
			static CFBXManager instance;
			return &instance;
		}

		/// @brief	初期化(このメソッドを必ず最初に呼び出す)
		/// @retval	true	初期化成功
		/// @retval	false	初期化失敗
		bool Init();

		/// @brief	解放
		void Release();

		/// @brief	ファイルを読み込み
		/// @param[in]	lpdevice	デバイス
		/// @param[in]	lpcontext	デバイスコンテキスト
		/// @param[in]	filename	読み込むファイル名
		/// @return	モデルデータ(NULLなら読み込み失敗)
		CModel* LoadFile(
			ID3D11Device1*			lpdevice,
			ID3D11DeviceContext1*	lpcontext,
			const char*				filename);

	private:

		//==============================================================================
		// フィールド
		//==============================================================================
		FbxManager*		m_lpFBXManager;	/// FBXマネージャー
		FbxIOSettings*	m_lpFBXIO;		/// FBXIOオブジェクト
		FbxScene*		m_lpFBXScene;	/// FBXシーン
		//std::map<std::string, >
		
		//==============================================================================
		// メソッド
		//==============================================================================	
		/// @brief	コンストラクタ
		CFBXManager();

		/// @brief	デストラクタ
		~CFBXManager();

		/// @brief	メッシュ読み込み
		/// @param[in]	lpMesh	メッシュ
		void LoadMesh(FbxMesh* lpMesh, CModel** lppModel);

		/// @brief	マテリアル読み込み
		/// @param[in]	lpNode	ノード
		void LoadMaterial(FbxNode* lpNode, ID3D11Device1*			lpdevice,
			ID3D11DeviceContext1*	lpcontext,CModel** lppModel);

		/// @brief	三角化
		/// @param[in]	lpNode	ノード
		void AllTriangulate(FbxNode* lpNode);
	};
}

#define AKIFBXMGR	AkiLib::CFBXManager::GetInstance()

#endif	// __AKILIB_FBXMANAGER_H__