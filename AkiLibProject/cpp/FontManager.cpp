//==============================================================================
/// @file	FontManager.cpp
/// @brief	�t�H���g�֘A����
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CFontManager::CFontManager()
{
}

// �f�X�g���N�^
CFontManager::~CFontManager()
{
}

// �w�蕶���e�N�X�`������(Unicode�p)
SPCHARDATA CFontManager::CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::wstring& charactor, UINT fontsize)
{
	// �t�H���g�̐���
	LOGFONTW logFont = 
	{
		fontsize,
		0, 0, 0, 0, 0, 0, 0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		L"�l�r ����"
	};

	HFONT hFont = CreateFontIndirectW( &logFont );
	if( hFont == nullptr )
	{
		return false;
	}

	// �f�o�C�X�R���e�L�X�g�擾
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC( nullptr );
	HFONT oldFont = reinterpret_cast<HFONT>( SelectObject( hdc, hFont ) );

	// �����R�[�h�擾
	UINT code = static_cast<UINT>( charactor[0] );

	// �t�H���g�r�b�g�}�b�v�擾
	// �e�L�X�g���g���b�N�X�擾
	TEXTMETRICW textMetrix;
	GetTextMetricsW( hdc, &textMetrix );

	// �O���t(�����̎��o�I�ȕ\��)���擾
	GLYPHMETRICS glyphMetrix;
	const MAT2 identityMatrix = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };	// �����̕ϊ��s��(�P�ʍs��)

	// �܂��͔z��쐬�̂��߂̑傫�����擾����
	DWORD size = GetGlyphOutlineW( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, 0, nullptr, &identityMatrix );
	std::shared_ptr<BYTE> spbitmapInfo( AKINEW BYTE[size] );

	// �O���t�擾
	GetGlyphOutlineW( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, size, spbitmapInfo.get(), &identityMatrix );

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject( hdc, oldFont );
	DeleteObject( hFont );
	ReleaseDC( nullptr, hdc );

	// �e�N�X�`���쐬
	SPCHARDATA	spTemp( AKINEW CHARACTORDATA() );

	D3D11_TEXTURE2D_DESC texture2Desc;
	ZeroMemory( &texture2Desc, sizeof( texture2Desc ) );
	texture2Desc.Width				= glyphMetrix.gmCellIncX;
	texture2Desc.Height				= textMetrix.tmHeight;
	texture2Desc.MipLevels			= 1;
	texture2Desc.ArraySize			= 1;
	texture2Desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2Desc.SampleDesc.Count	= 1;
	texture2Desc.Usage				= D3D11_USAGE_DYNAMIC;
	texture2Desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texture2Desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	// ��������ۑ�
	spTemp->Size.x = static_cast<float>( texture2Desc.Width );
	spTemp->Size.y = static_cast<float>( texture2Desc.Height );

	// �e�N�X�`���쐬
	if( FAILED( lpdevice->CreateTexture2D( &texture2Desc, 0, &spTemp->lpTexture ) ) )
	{
		ErrorMessageBox( L"�t�H���g�e�N�X�`���쐬���s" );
	}

	// ���\�[�X�r���[�쐬
	if( FAILED( lpdevice->CreateShaderResourceView( spTemp->lpTexture, nullptr, &spTemp->lpResourceView ) ) )
	{
		ErrorMessageBox( L"�t�H���g���\�[�X�r���[�쐬���s" );
	}

	// �t�H���g���̏�������
	// �����o���ʒu(����)
	int offsetX = glyphMetrix.gmptGlyphOrigin.x;
	int offsetY = textMetrix.tmAscent - glyphMetrix.gmptGlyphOrigin.y;
	
	// �t�H���g�r�b�g�}�b�v�̕���
	int bitmapWidth = glyphMetrix.gmBlackBoxX + ( 4 - ( glyphMetrix.gmBlackBoxX % 4 ) ) % 4;	// 4�o�C�g���E�̂���
	int bitmapHeight = glyphMetrix.gmBlackBoxY;

	// �A���t�@�l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
	int Level = 17;
	DWORD alpha, color;

	// �e�N�X�`���ɏ�������
	D3D11_MAPPED_SUBRESOURCE mapResource;
	lpcontext->Map( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ), D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

	// 0�N���A���Ă���
	ZeroMemory( mapResource.pData, mapResource.RowPitch * textMetrix.tmHeight );

	// �e�N�X�`���ɕ������𔒐F�ŏ�������ł���
	for( int y = offsetY; y < offsetY + bitmapHeight; y++ )
	{
		for( int x = offsetX; x < offsetX + bitmapWidth; x++ )
		{
			alpha = ( 0xff * spbitmapInfo.get()[x - offsetX + bitmapWidth * ( y - offsetY )] ) / ( Level - 1 );
			color = 0x00ffffff | ( alpha << 24 );// ARGB�̏���

			memcpy( (BYTE*)mapResource.pData + mapResource.RowPitch * y + 4 * x, &color, sizeof( DWORD ) );
		}
	}

	lpcontext->Unmap( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ) );

	return spTemp;
}

// �w�蕶���e�N�X�`������(�}���`�o�C�g�p)
SPCHARDATA CFontManager::CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::string& charactor, UINT fontsize)
{
	// �t�H���g�̐���
	LOGFONTA logFont = 
	{
		fontsize,
		0, 0, 0, 0, 0, 0, 0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"�l�r ����"
	};

	HFONT hFont = CreateFontIndirectA( &logFont );
	if( hFont == nullptr )
	{
		return false;
	}

	// �f�o�C�X�R���e�L�X�g�擾
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC( nullptr );
	HFONT oldFont = reinterpret_cast<HFONT>( SelectObject( hdc, hFont ) );

	// �����R�[�h�擾
	UINT code = 0;

	// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]
	if( IsDBCSLeadByte( charactor[0] ) )
	{
		code = charactor[0] * 256 | static_cast<BYTE>( charactor[1] );
	}
	else
	{
		code = charactor[0];
	}

	// �t�H���g�r�b�g�}�b�v�擾
	// �e�L�X�g���g���b�N�X�擾
	TEXTMETRICA textMetrix;
	GetTextMetricsA( hdc, &textMetrix );

	// �O���t(�����̎��o�I�ȕ\��)���擾
	GLYPHMETRICS glyphMetrix;
	const MAT2 identityMatrix = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };	// �����̕ϊ��s��(�P�ʍs��)

	// �܂��͔z��쐬�̂��߂̑傫�����擾����
	DWORD size = GetGlyphOutlineA( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, 0, nullptr, &identityMatrix );
	std::shared_ptr<BYTE> spbitmapInfo( AKINEW BYTE[size] );

	// �O���t�擾
	GetGlyphOutlineA( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, size, spbitmapInfo.get(), &identityMatrix );

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject( hdc, oldFont );
	DeleteObject( hFont );
	ReleaseDC( nullptr, hdc );

	// �e�N�X�`���쐬
	SPCHARDATA	spTemp( AKINEW CHARACTORDATA() );

	D3D11_TEXTURE2D_DESC texture2Desc;
	ZeroMemory( &texture2Desc, sizeof( texture2Desc ) );
	texture2Desc.Width				= glyphMetrix.gmCellIncX;
	texture2Desc.Height				= textMetrix.tmHeight;
	texture2Desc.MipLevels			= 1;
	texture2Desc.ArraySize			= 1;
	texture2Desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2Desc.SampleDesc.Count	= 1;
	texture2Desc.Usage				= D3D11_USAGE_DYNAMIC;
	texture2Desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texture2Desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	// ��������ۑ�
	spTemp->Size.x = static_cast<float>( texture2Desc.Width );
	spTemp->Size.y = static_cast<float>( texture2Desc.Height );

	// �e�N�X�`���쐬
	if( FAILED( lpdevice->CreateTexture2D( &texture2Desc, 0, &spTemp->lpTexture ) ) )
	{
		ErrorMessageBox( "�t�H���g�e�N�X�`���쐬���s" );
	}

	// ���\�[�X�r���[�쐬
	if( FAILED( lpdevice->CreateShaderResourceView( spTemp->lpTexture, nullptr, &spTemp->lpResourceView ) ) )
	{
		ErrorMessageBox( "�t�H���g���\�[�X�r���[�쐬���s" );
	}

	// �t�H���g���̏�������
	// �����o���ʒu(����)
	int offsetX = glyphMetrix.gmptGlyphOrigin.x;
	int offsetY = textMetrix.tmAscent - glyphMetrix.gmptGlyphOrigin.y;
	
	// �t�H���g�r�b�g�}�b�v�̕���
	int bitmapWidth = glyphMetrix.gmBlackBoxX + ( 4 - ( glyphMetrix.gmBlackBoxX % 4 ) ) % 4;	// 4�o�C�g���E�̂���
	int bitmapHeight = glyphMetrix.gmBlackBoxY;

	// �A���t�@�l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
	int Level = 17;
	DWORD alpha, color;

	// �e�N�X�`���ɏ�������
	D3D11_MAPPED_SUBRESOURCE mapResource;
	lpcontext->Map( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ), D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

	// 0�N���A���Ă���
	ZeroMemory( mapResource.pData, mapResource.RowPitch * textMetrix.tmHeight );

	// �e�N�X�`���ɕ������𔒐F�ŏ�������ł���
	for( int y = offsetY; y < offsetY + bitmapHeight; y++ )
	{
		for( int x = offsetX; x < offsetX + bitmapWidth; x++ )
		{
			alpha = ( 0xff * spbitmapInfo.get()[x - offsetX + bitmapWidth * ( y - offsetY )] ) / ( Level - 1 );
			color = 0x00ffffff | ( alpha << 24 );// ARGB�̏���

			memcpy( static_cast<BYTE*>(mapResource.pData) + mapResource.RowPitch * y + sizeof( DWORD ) * x, &color, sizeof( DWORD ) );
		}
	}

	lpcontext->Unmap( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ) );

	return spTemp;
}