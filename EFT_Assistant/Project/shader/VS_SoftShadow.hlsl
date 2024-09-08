// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position        : POSITION ;         // ���W( ���[�J����� )
	float3 Normal          : NORMAL0 ;          // �@��( ���[�J����� )
	float4 Diffuse         : COLOR0 ;           // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;           // �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0 ;        // �e�N�X�`�����W
	float4 TexCoords1      : TEXCOORD1 ;		// �T�u�e�N�X�`�����W
	float3 Tan             : TANGENT0;		// �ڐ�( ���[�J����� )
	float3 Bin             : BINORMAL0;		// �]�@��( ���[�J����� )
	int4   BlendIndices0   : BLENDINDICES0 ;    // �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 BlendWeight0    : BLENDWEIGHT0 ;     // �X�L�j���O�����p�E�G�C�g�l
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float4 LPPosition      : TEXCOORD1 ;    // ���C�g����݂����W( ���C�g�̎ˉe��� )
	float4 LPPosition2     : TEXCOORD2 ;    // ���C�g����݂����W( ���C�g�̎ˉe��� )
	float4 Position        : SV_POSITION ;	// ���W( �v���W�F�N�V������� )
} ;


// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[ 4 ] ;				// �A���`�r���[�|�[�g�s��
	float4		ProjectionMatrix[ 4 ] ;					// �r���[�@���@�v���W�F�N�V�����s��
	float4		ViewMatrix[ 3 ] ;						// ���[���h�@���@�r���[�s��
	float4		LocalWorldMatrix[ 3 ] ;					// ���[�J���@���@���[���h�s��

	float4		ToonOutLineSize ;						// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource ;							// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource ;						// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor ;						// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding ;
} ;

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[ 3 ][ 4 ] ;			// �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
	float4		TextureMatrix[ 3 ][ 2 ] ;								// �e�N�X�`�����W����p�s��
} ;

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	float4		Matrix[ 54 * 3 ] ;					// ���[�J���@���@���[���h�s��
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register( b3 )
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix ;
} ;

cbuffer cbLIGHTCAMERA_MATRIX						: register( b4 )
{
	matrix		g_LightViewMatrix ;			// ���C�g�̃��[���h�@���@�r���[�s��
	matrix		g_LightProjectionMatrix ;	// ���C�g�̃r���[�@�@���@�ˉe�s��
} ;

cbuffer cbLIGHTCAMERA_MATRIX2						: register(b5) {
	matrix		g_LightViewMatrix2;			// ���C�g�̃��[���h�@���@�r���[�s��
	matrix		g_LightProjectionMatrix2;	// ���C�g�̃r���[�@�@���@�ˉe�s��
};


// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float4 lLViewPosition ;
	float4 lLocalWorldMatrix[ 3 ] ;


	// �����̃t���[���̃u�����h�s��̍쐬
	lLocalWorldMatrix[ 0 ]  = g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.x + 0 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 1 ]  = g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.x + 1 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 2 ]  = g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.x + 2 ] * VSInput.BlendWeight0.x;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.y + 0 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.y + 1 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.y + 2 ] * VSInput.BlendWeight0.y;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.z + 0 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.z + 1 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.z + 2 ] * VSInput.BlendWeight0.z;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.w + 0 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.w + 1 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.w + 2 ] * VSInput.BlendWeight0.w;


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.x = dot( VSInput.Position, lLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( VSInput.Position, lLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( VSInput.Position, lLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// �r���[���W���ˉe���W�ɕϊ�
	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
	VSOutput.TexCoords0.x = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[���h���W�����C�g�̃r���[���W�ɕϊ�
	lLViewPosition = mul( g_LightViewMatrix, lWorldPosition ) ;

	// ���C�g�̃r���[���W�����C�g�̎ˉe���W�ɕϊ�
	VSOutput.LPPosition = mul( g_LightProjectionMatrix, lLViewPosition ) ;

	// �y�l�����̓��C�g�̃r���[���W�ɂ���
	VSOutput.LPPosition.z = lLViewPosition.z ;

	// ���[���h���W�����C�g�̃r���[���W�ɕϊ�
	lLViewPosition = mul(g_LightViewMatrix2, lWorldPosition);

	// ���C�g�̃r���[���W�����C�g�̎ˉe���W�ɕϊ�
	VSOutput.LPPosition2 = mul(g_LightProjectionMatrix2, lLViewPosition);

	// �y�l�����̓��C�g�̃r���[���W�ɂ���
	VSOutput.LPPosition2.z = lLViewPosition.z;

	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


	// �o�̓p�����[�^��Ԃ�
	return VSOutput ;
}

