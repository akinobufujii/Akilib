//==============================================================================
/// @file	AkiLib.h
/// @brief	ライブラリヘッダー
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_H__
#define __AKILIB_H__

//==============================================================================
// 外部ライブラリ
//==============================================================================
#include <map>
#include <string>
#include <memory>
#include <list>
#include <algorithm>
#include <vector>
#include <fstream>
#include <ctime>

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <tchar.h>

#include <XAudio2.h>
#include <XAudio2fx.h>

#include <XNAMath/xnamath.h>

#include <DirectXTK/CommonStates.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/ScreenGrab.h>

//#include <DirectXTK/VertexTypes.h>
//#include <DirectXTK/Effects.h>
//#include <DirectXTK/GeometricPrimitive.h>
//#include <DirectXTK/Model.h>
//#include <DirectXTK/SimpleMath.h>
//#include <DirectXTK/SpriteBatch.h>
//#include <DirectXTK/SpriteFont.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define FBXSDK_SHARED
#include <fbxsdk.h>

//==============================================================================
// 自作ライブラリ
//==============================================================================
#include "AkiLib/Utility.h"
#include "AkiLib/FPSManager.h"
#include "AkiLib/ApplicationWindow.h"
#include "AkiLib/ConstantBuffer.h"
#include "AkiLib/D3D11Manager.h"
#include "AkiLib/Shader.h"
#include "AkiLib/Matrix.h"
#include "AkiLib/Vector.h"
#include "AkiLib/SoundManager.h"
#include "AkiLib/InputManager.h"
#include "AkiLib/Texture.h"
#include "AkiLib/Board.h"
#include "AkiLib/Model.h"
#include "AkiLib/MQOFile.h"
#include "AkiLib/SamplerState.h"
#include "AkiLib/FBXManager.h"
#include "AkiLib/FontManager.h"

#endif // !__AKILIB_H__