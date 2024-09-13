// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position        : POSITION;			// ���W( ���[�J����� )
	float3 Normal          : NORMAL0;			// �@��( ���[�J����� )
	float4 Diffuse         : COLOR0;			// �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1;			// �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0;			// �e�N�X�`�����W
	float4 TexCoords1      : TEXCOORD1;			// �T�u�e�N�X�`�����W
	int4   BlendIndices0   : BLENDINDICES0;		// �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 BlendWeight0    : BLENDWEIGHT0;		// �X�L�j���O�����p�E�G�C�g�l
	float3 Tan             : TANGENT0;			// �ڐ�( ���[�J����� )
	float3 Bin             : BINORMAL0;			// �]�@��( ���[�J����� )
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float3 normalW         : TEXCOORD0 ;
	float3 viewVecW        : TEXCOORD1 ;
	float2 TexCoords0      : TEXCOORD2;    // �e�N�X�`�����W
	float3 VPosition       : TEXCOORD3;    // ���W( �r���[��� )
	float3 VNormal         : TEXCOORD4;    // �@��( �r���[��� )
	float3 VTan            : TEXCOORD5;    // �ڐ�( �r���[��� )
	float3 VBin            : TEXCOORD6;    // �]�@��( �r���[��� )
	float  Fog			   : TEXCOORD7;		// �t�H�O�p�����[�^
	float4 Position        : SV_POSITION ;	// ���W( �v���W�F�N�V������� )
};


// �}�e���A���p�����[�^
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse;				// �f�B�t���[�Y�J���[
	float4		Specular;				// �X�y�L�����J���[
	float4		Ambient_Emissive;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

	float		Power;					// �X�y�L�����̋���
	float		TypeParam0;			// �}�e���A���^�C�v�p�����[�^0
	float		TypeParam1;			// �}�e���A���^�C�v�p�����[�^1
	float		TypeParam2;			// �}�e���A���^�C�v�p�����[�^2
};

// �t�H�O�p�����[�^
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd;				// �t�H�O�p�p�����[�^ end / ( end - start )
	float		LinearDiv;				// �t�H�O�p�p�����[�^ -1  / ( end - start )
	float		Density;				// �t�H�O�p�p�����[�^ density
	float		E;						// �t�H�O�p�p�����[�^ ���R�ΐ��̒�

	float4		Color;					// �J���[
};

// ���C�g�p�����[�^
struct DX_D3D11_CONST_LIGHT
{
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
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[6];			// ���C�g�p�����[�^
	DX_D3D11_CONST_MATERIAL		Material;				// �}�e���A���p�����[�^
	DX_D3D11_VS_CONST_FOG		Fog;					// �t�H�O�p�����[�^
};

// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[4];				// �A���`�r���[�|�[�g�s��
	float4		ProjectionMatrix[4];					// �r���[�@���@�v���W�F�N�V�����s��
	float4		ViewMatrix[3];						// ���[���h�@���@�r���[�s��
	float4		LocalWorldMatrix[3];					// ���[�J���@���@���[���h�s��

	float4		ToonOutLineSize;						// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource;							// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource;						// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor;						// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding;
};

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[3][4];			// �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
	float4		TextureMatrix[3][2];								// �e�N�X�`�����W����p�s��
};

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	float4		Matrix[54 * 3];					// ���[�J���@���@���[���h�s��
};

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0)
{
	DX_D3D11_CONST_BUFFER_COMMON				g_Common;
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register(b1)
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base;
};

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register(b2)
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix;
};

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register(b3)
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix;
};

//�ǉ��p�����[�^�[
cbuffer cbCAMERAPOSW								: register(b4)
{
	float4										g_CameraPosW;
};


// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalWorldMatrix[3];
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float3 lWorldNrm ;
	float3 lWorldTan;
	float3 lWorldBin;
	float3 lViewNrm ;
	float3 lViewTan;
	float3 lViewBin;


	// �����̃t���[���̃u�����h�s��̍쐬
	lLocalWorldMatrix[0] = g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.x + 0] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[1] = g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.x + 1] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[2] = g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.x + 2] * VSInput.BlendWeight0.x;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.y + 0] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.y + 1] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.y + 2] * VSInput.BlendWeight0.y;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.z + 0] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.z + 1] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.z + 2] * VSInput.BlendWeight0.z;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.w + 0] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.w + 1] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.BlendIndices0.w + 2] * VSInput.BlendWeight0.w;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.x = dot(VSInput.Position, lLocalWorldMatrix[0]);
	lWorldPosition.y = dot(VSInput.Position, lLocalWorldMatrix[1]);
	lWorldPosition.z = dot(VSInput.Position, lLocalWorldMatrix[2]);
	lWorldPosition.w = 1.0f;

	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
	lViewPosition.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
	lViewPosition.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
	lViewPosition.w = 1.0f;

	// �r���[���W���ˉe���W�ɕϊ�
	VSOutput.Position.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
	VSOutput.Position.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
	VSOutput.Position.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
	VSOutput.Position.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �]�@���A�ڐ��A�@�����r���[�x�N�g���ɕϊ�
	lWorldTan.x = dot(VSInput.Tan, lLocalWorldMatrix[0].xyz);
	lWorldTan.y = dot(VSInput.Tan, lLocalWorldMatrix[1].xyz);
	lWorldTan.z = dot(VSInput.Tan, lLocalWorldMatrix[2].xyz);

	lWorldBin.x = dot(VSInput.Bin, lLocalWorldMatrix[0].xyz);
	lWorldBin.y = dot(VSInput.Bin, lLocalWorldMatrix[1].xyz);
	lWorldBin.z = dot(VSInput.Bin, lLocalWorldMatrix[2].xyz);

	// ���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
	lWorldNrm.x = dot(VSInput.Normal, lLocalWorldMatrix[0].xyz);
	lWorldNrm.y = dot(VSInput.Normal, lLocalWorldMatrix[1].xyz);
	lWorldNrm.z = dot(VSInput.Normal, lLocalWorldMatrix[2].xyz);

	lViewTan.x = dot(lWorldTan, g_Base.ViewMatrix[0].xyz);
	lViewTan.y = dot(lWorldTan, g_Base.ViewMatrix[1].xyz);
	lViewTan.z = dot(lWorldTan, g_Base.ViewMatrix[2].xyz);

	lViewBin.x = dot(lWorldBin, g_Base.ViewMatrix[0].xyz);
	lViewBin.y = dot(lWorldBin, g_Base.ViewMatrix[1].xyz);
	lViewBin.z = dot(lWorldBin, g_Base.ViewMatrix[2].xyz);

	// ���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
	lViewNrm.x = dot(lWorldNrm, g_Base.ViewMatrix[0].xyz);
	lViewNrm.y = dot(lWorldNrm, g_Base.ViewMatrix[1].xyz);
	lViewNrm.z = dot(lWorldNrm, g_Base.ViewMatrix[2].xyz);

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �J�n )



	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �I�� )




	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	VSOutput.viewVecW = lWorldPosition.xyz - g_CameraPosW.xyz ;
	VSOutput.normalW = lWorldNrm ;

	// ���_���W��ۑ�
	VSOutput.VPosition = lViewPosition.xyz;

	// �ڐ���ۑ�
	VSOutput.VTan = lViewTan;

	// �]�@����ۑ�
	VSOutput.VBin = lViewBin;

	// �@����ۑ�
	VSOutput.VNormal = lViewNrm;

	// �e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
	VSOutput.TexCoords0.x = dot(VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[0][0]);
	VSOutput.TexCoords0.y = dot(VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[0][1]);

	// ���`�t�H�O�v�Z
	VSOutput.Fog = lViewPosition.z * g_Common.Fog.LinearDiv + g_Common.Fog.LinearAdd;

	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �o�̓p�����[�^��Ԃ�
	return VSOutput ;
}
