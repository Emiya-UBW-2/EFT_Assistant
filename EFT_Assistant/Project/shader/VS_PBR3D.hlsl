// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position        : POSITION;         // ���W( ���[�J����� )
	float3 Normal          : NORMAL0;          // �@��( ���[�J����� )
	float4 Diffuse         : COLOR0;           // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1;           // �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0;        // �e�N�X�`�����W
	float4 TexCoords1      : TEXCOORD1;		// �T�u�e�N�X�`�����W
	float3 Tan             : TANGENT0;		// �ڐ�( ���[�J����� )
	float3 Bin             : BINORMAL0;		// �]�@��( ���[�J����� )
	int4   BlendIndices0   : BLENDINDICES0;    // �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 BlendWeight0    : BLENDWEIGHT0;     // �X�L�j���O�����p�E�G�C�g�l
} ;

// ���_�V�F�[�_�[�̏o��
struct GS_INPUT
{
	float2 TexCoords0	 : TEXCOORD0;    // �e�N�X�`�����W
	float4 WPosition	 : TEXCOORD1;    // ���W(���[���h)
    float3 WTan			 : TEXCOORD2; // �ڐ�( ���[���h )
    float3 WBin			 : TEXCOORD3; // �]�@��( ���[���h )
    float3 WNormal		 : TEXCOORD4; // �@��( ���[���h )
    float4 Specular		 : COLOR1; // �X�y�L�����J���[
} ;


// �}�e���A���p�����[�^
struct DX_D3D11_CONST_MATERIAL {
	float4		Diffuse;				// �f�B�t���[�Y�J���[
	float4		Specular;				// �X�y�L�����J���[
	float4		Ambient_Emissive;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

	float		Power;					// �X�y�L�����̋���
	float		TypeParam0;			// �}�e���A���^�C�v�p�����[�^0
	float		TypeParam1;			// �}�e���A���^�C�v�p�����[�^1
	float		TypeParam2;			// �}�e���A���^�C�v�p�����[�^2
};

// �t�H�O�p�����[�^
struct DX_D3D11_VS_CONST_FOG {
	float		LinearAdd;				// �t�H�O�p�p�����[�^ end / ( end - start )
	float		LinearDiv;				// �t�H�O�p�p�����[�^ -1  / ( end - start )
	float		Density;				// �t�H�O�p�p�����[�^ density
	float		E;						// �t�H�O�p�p�����[�^ ���R�ΐ��̒�

	float4		Color;					// �J���[
};

// ���C�g�p�����[�^
struct DX_D3D11_CONST_LIGHT {
	int			Type;					// ���C�g�^�C�v( DX_LIGHTTYPE_POINT �Ȃ� )
	int3		Padding1;				// �p�f�B���O�P

	float3		Position;				// ���W( �r���[��� )
	float		RangePow2;				// �L�������̂Q��

	float3		Direction;				// ����( �r���[��� )
	float		FallOff;				// �X�|�b�g���C�g�pFallOff

	float3		Diffuse;				// �f�B�t���[�Y�J���[
	float		SpotParam0;			// �X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )

	float3		Specular;				// �X�y�L�����J���[
	float		SpotParam1;			// �X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient;				// �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

	float		Attenuation0;			// �����ɂ�錸�������p�p�����[�^�O
	float		Attenuation1;			// �����ɂ�錸�������p�p�����[�^�P
	float		Attenuation2;			// �����ɂ�錸�������p�p�����[�^�Q
	float		Padding2;				// �p�f�B���O�Q
};

// �s�N�Z���V�F�[�_�[�E���_�V�F�[�_�[���ʃp�����[�^
struct DX_D3D11_CONST_BUFFER_COMMON {
	DX_D3D11_CONST_LIGHT		Light[6];			// ���C�g�p�����[�^
	DX_D3D11_CONST_MATERIAL		Material;				// �}�e���A���p�����[�^
	DX_D3D11_VS_CONST_FOG		Fog;					// �t�H�O�p�����[�^
};


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
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX {
	float4		LWMatrix[54 * 3];					// ���[�J���@���@���[���h�s��
};

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0) {
	DX_D3D11_CONST_BUFFER_COMMON				g_Common;
};

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
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register(b3) {
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix;
};

float2 SetTexCoords0(float4 TexCoords0)
{
    float2 Ret;
	// �e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
    Ret.x = dot(TexCoords0, g_OtherMatrix.TextureMatrix[0][0]);
    Ret.y = dot(TexCoords0, g_OtherMatrix.TextureMatrix[0][1]);
    return Ret;
}
// main�֐�
GS_INPUT main(VS_INPUT VSInput)
{
    GS_INPUT VSOutput;
	float4 lLocalWorldMatrix[3];

	// �����̃t���[���̃u�����h�s��̍쐬
	lLocalWorldMatrix[0] = g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.x + 0] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[1] = g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.x + 1] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[2] = g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.x + 2] * VSInput.BlendWeight0.x;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.y + 0] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.y + 1] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.y + 2] * VSInput.BlendWeight0.y;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.z + 0] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.z + 1] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.z + 2] * VSInput.BlendWeight0.z;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.w + 0] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.w + 1] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.LWMatrix[VSInput.BlendIndices0.w + 2] * VSInput.BlendWeight0.w;


    VSOutput.WPosition.x = dot(VSInput.Position, lLocalWorldMatrix[0]);
    VSOutput.WPosition.y = dot(VSInput.Position, lLocalWorldMatrix[1]);
    VSOutput.WPosition.z = dot(VSInput.Position, lLocalWorldMatrix[2]);
    VSOutput.WPosition.w = 1.0f;

    VSOutput.WTan.x = dot(VSInput.Tan, lLocalWorldMatrix[0].xyz);
    VSOutput.WTan.y = dot(VSInput.Tan, lLocalWorldMatrix[1].xyz);
    VSOutput.WTan.z = dot(VSInput.Tan, lLocalWorldMatrix[2].xyz);

    VSOutput.WBin.x = dot(VSInput.Bin, lLocalWorldMatrix[0].xyz);
    VSOutput.WBin.y = dot(VSInput.Bin, lLocalWorldMatrix[1].xyz);
    VSOutput.WBin.z = dot(VSInput.Bin, lLocalWorldMatrix[2].xyz);

    VSOutput.WNormal.x = dot(VSInput.Normal, lLocalWorldMatrix[0].xyz);
    VSOutput.WNormal.y = dot(VSInput.Normal, lLocalWorldMatrix[1].xyz);
    VSOutput.WNormal.z = dot(VSInput.Normal, lLocalWorldMatrix[2].xyz);

    VSOutput.TexCoords0 = SetTexCoords0(VSInput.TexCoords0);
    VSOutput.Specular = (g_Base.SpecularSource > 0.5f ? VSInput.Specular : g_Common.Material.Specular) * g_Base.MulSpecularColor;


	// �o�̓p�����[�^��Ԃ�
    return VSOutput;
}

