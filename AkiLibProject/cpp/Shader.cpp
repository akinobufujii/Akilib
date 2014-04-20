//==============================================================================
/// @file	Shader.cpp
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CShader::CShader()
{
}

// �f�X�g���N�^
CShader::~CShader()
{
	Release();
}

// ���
void CShader::Release()
{
	// ���_�V�F�[�_���
	std::for_each( m_VertexShaderMap.begin(), m_VertexShaderMap.end(), MapRelease<VERTEXSHADERMAPPAIR> );
	m_VertexShaderMap.clear();

	// �s�N�Z���V�F�[�_���
	std::for_each( m_PixelShaerMap.begin(), m_PixelShaerMap.end(), MapRelease<PIXELSHADERMAPPAIR> );
	m_PixelShaerMap.clear();

	// �R���s���[�g�V�F�[�_���
	std::for_each( m_ComputeShaderMap.begin(), m_ComputeShaderMap.end(), MapRelease<COMPUTESHADERMAPPAIR> );
	m_ComputeShaderMap.clear();

	// �n���V�F�[�_���
	std::for_each( m_HallShaderMap.begin(), m_HallShaderMap.end(), MapRelease<HALLSHADERMAPPAIR> );
	m_HallShaderMap.clear();

	// �h���C���V�F�[�_���
	std::for_each( m_DomainShaderMap.begin(), m_DomainShaderMap.end(), MapRelease<DOMAINSHADERMAPPAIR> );
	m_DomainShaderMap.clear();
}

// �t�@�C������ǂݍ���ŃR���p�C��
ID3DBlob* CShader::CompileShaderFromFile(LPCSTR funcName, LPCSTR version)
{
	LPD3D10BLOB lperror		= nullptr;	// �G���[�p�u���u
	LPD3D10BLOB lptempBlob	= nullptr;	// �߂�l�p�u���u
	HANDLE		hfile		= nullptr;	// �t�@�C���n���h��

	// �s����D��w��
	UINT flg = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;

#ifdef _DEBUG
	flg |= D3D10_SHADER_OPTIMIZATION_LEVEL0;	// �œK�����Ȃ�
#else
	flg |= D3D10_SHADER_OPTIMIZATION_LEVEL3;	// �ő�œK��	
#endif

	try
	{
		// hlsl�t�@�C���̃n���h�����擾
		hfile = CreateFileA( 
			m_FileName.c_str(),		// �t�@�C����
			GENERIC_READ,			// �A�N�Z�X���[�h
			FILE_SHARE_READ,		// ���L���[�h
			nullptr,				// �Z�L�����e�B�L�q�q
			OPEN_EXISTING,			// �쐬���@
			FILE_ATTRIBUTE_NORMAL,	// �t�@�C������
			nullptr );				// �e���v���[�g�t�@�C���n���h��

		if( hfile == INVALID_HANDLE_VALUE )
		{
			throw "HLSL�t�@�C���I�[�v�����s";
		}

		// �t�@�C���̃T�C�Y���擾
		DWORD fileSize = GetFileSize( hfile, nullptr );

		if( fileSize == -1 || fileSize == 0 )
		{
			throw "HlSL�t�@�C���T�C�Y�擾���s";
		}

		// �̈�m��
		std::shared_ptr<BYTE> byteCode( AKINEW BYTE[fileSize] );

		// �t�@�C���ǂݍ���
		DWORD numBytesRead;
		if( ReadFile( hfile, byteCode.get(), fileSize, &numBytesRead, nullptr ) == FALSE )
		{
			throw "HLSL�t�@�C���ǂݍ��ݎ��s";
		}

		// HLSL�R���p�C��
		HRESULT hr = D3DCompile(
			reinterpret_cast<void*>( byteCode.get() ),	// HLSL�o�C�g�R�[�h
			fileSize,									// �t�@�C���̑傫��
			nullptr,									// �}�N��
			nullptr,									// �C���N���[�h
			nullptr,									// �G���g���[�|�C���g��
			funcName,									// �R���p�C���֐���
			version,									// �R���p�C���o�[�W����
			flg,										// �R���p�C���I�v�V����
			0,											// �G�t�F�N�g�I�v�V����
			&lptempBlob,								// �R���p�C���R�[�h���󂯎��u���u
			&lperror );									// �R���p�C���G���[���󂯎��u���u

		if( FAILED( hr ) )
		{
#ifdef _UNICODE
			int strLen = MultiByteToWideChar( CP_ACP, 0, reinterpret_cast<CHAR*>( lperror->GetBufferPointer() ), lperror->GetBufferSize(), nullptr, 0 );

			std::shared_ptr<wchar_t> errorStr( AKINEW wchar_t[strLen] );

			MultiByteToWideChar( CP_ACP, 0, reinterpret_cast<char*>( lperror->GetBufferPointer() ), lperror->GetBufferSize(), errorStr.get(), strLen );

			ErrorMessageBox( errorStr.get() );
#else
			throw reinterpret_cast<LPSTR>( lperror->GetBufferPointer() );
#endif
		}
	}
	catch( LPCSTR& error )
	{
		if( hfile )
		{
			CloseHandle( hfile );
		}
		SafeRelease( lperror );

		ErrorMessageBox( error );
	}

	return lptempBlob;
}

// ���_�V�F�[�_�[�쐬
bool CShader::CreateVertexShader( 
	LPCSTR							funcName,
	LPCSTR							version,
	const D3D11_INPUT_ELEMENT_DESC* layout,
	size_t							layoutNum,
	ID3D11InputLayout**				lppoutLayout )
{
	// �u���u�쐬
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );

	if( lpblob )
	{
		// ���_�V�F�[�_�쐬
		ID3D11VertexShader* lptempVertexShader = nullptr;
		m_lpDevice->CreateVertexShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempVertexShader );

		if( lptempVertexShader )
		{
			// �}�b�v�ɓo�^
			m_VertexShaderMap.insert( VERTEXSHADERMAPPAIR( funcName, lptempVertexShader ) );
		}

		// ���C�A�E�g�쐬
		if( layout && layoutNum > 0 && lppoutLayout )
		{
			HRESULT hr = m_lpDevice->CreateInputLayout( layout, layoutNum, lpblob->GetBufferPointer(), lpblob->GetBufferSize(), lppoutLayout );

			if( FAILED( hr ) )
			{
				ErrorMessageBox( "���̓��C�A�E�g�쐬���s" );
			}
		}

		SafeRelease( lpblob );

		return true;
	}

	return false;
}

// �s�N�Z���V�F�[�_�쐬
bool CShader::CreatePixelShader( LPCSTR funcName, LPCSTR version )
{
	// �u���u�쐬
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// �s�N�Z���V�F�[�_�쐬
		ID3D11PixelShader* lptempPixelShader = nullptr;
		m_lpDevice->CreatePixelShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempPixelShader );

		SafeRelease( lpblob );

		if( lptempPixelShader )
		{
			// �}�b�v�ɓo�^
			m_PixelShaerMap.insert( PIXELSHADERMAPPAIR( funcName, lptempPixelShader ) );
			return true;
		}
	}

	return false;
}

// �R���s���[�g�V�F�[�_�쐬
bool CShader::CreateComputeShader( LPCSTR funcName, LPCSTR version )
{
	// �u���u�쐬
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// �R���s���[�g�V�F�[�_�쐬
		ID3D11ComputeShader* lptempComputeShader = nullptr;
		m_lpDevice->CreateComputeShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempComputeShader );

		SafeRelease( lpblob );

		if( lptempComputeShader )
		{
			// �}�b�v�ɓo�^
			m_ComputeShaderMap.insert( COMPUTESHADERMAPPAIR( funcName, lptempComputeShader ) );
			return true;
		}
	}

	return false;
}

// �n���V�F�[�_�쐬
bool CShader::CreateHallShader( LPCSTR funcName, LPCSTR version )
{
	// �u���u�쐬
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// �n���V�F�[�_�쐬
		ID3D11HullShader* lptempHallShader = nullptr;
		m_lpDevice->CreateHullShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempHallShader );

		SafeRelease( lpblob );

		if( lptempHallShader )
		{
			// �}�b�v�ɓo�^
			m_HallShaderMap.insert( HALLSHADERMAPPAIR( funcName, lptempHallShader ) );
			return true;
		}
	}

	return false;
}

// �h���C���V�F�[�_�쐬
bool CShader::CreateDomainShader( LPCSTR funcName, LPCSTR version )
{
	// �u���u�쐬
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// �h���C���V�F�[�_�쐬
		ID3D11DomainShader* lptempDomainShader = nullptr;
		m_lpDevice->CreateDomainShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempDomainShader );

		SafeRelease( lpblob );

		if( lptempDomainShader )
		{
			// �}�b�v�ɓo�^
			m_DomainShaderMap.insert( DOMAINSHADERMAPPAIR( funcName, lptempDomainShader ) );
			return true;
		}
	}

	return false;
}