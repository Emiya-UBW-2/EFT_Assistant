// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float3 normalW         : TEXCOORD0 ;
	float3 viewVecW        : TEXCOORD1 ;
	float2 TexCoords0      : TEXCOORD2;    // �e�N�X�`�����W
	float3 VPosition       : TEXCOORD3;    // ���W( �r���[��� )
	float3 VNormal         : TEXCOORD4;    // �@��( �r���[��� )
	float3 VTan            : TEXCOORD5;    // �ڐ�( �r���[��� )
	float3 VBin            : TEXCOORD6;    // �]�@��( �r���[��� )
	float  Fog			   : TEXCOORD7;		// �t�H�O�p�����[�^
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// �F
} ;


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

// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor;			// �A���t�@�l��

	float		MulAlphaColor;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1;

	int			AlphaTestCmpMode;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2;

	float4		IgnoreTextureColor;	// �e�N�X�`���J���[���������p�J���[
};

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0)
{
	DX_D3D11_CONST_BUFFER_COMMON		g_Common;
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

SamplerState g_MetallicMapSampler           : register(s5);		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_MetallicMapTexture           : register(t5);		// �f�B�t���[�Y�}�b�v�e�N�X�`��

SamplerState g_NormalMapSampler             : register(s4);		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_NormalMapTexture             : register(t4);		// �f�B�t���[�Y�}�b�v�e�N�X�`��

// C++ ���Őݒ肷��e�N�X�`����萔�̒�`
SamplerState g_ToonDiffuseGradSampler       : register(s3);		// �g�D�[�������_�����O�p�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`��
Texture2D    g_ToonDiffuseGradTexture       : register(t3);		// �g�D�[�������_�����O�p�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`��

SamplerState cubeMapSampler					: register( s2 ) ;
TextureCube  cubeMapTexture					: register( t2 ) ;

SamplerState dynamicCubeMapSampler			: register( s1 ) ;
TextureCube  dynamicCubeMapTexture			: register( t1 ) ;

SamplerState g_DiffuseMapSampler            : register(s0);		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_DiffuseMapTexture            : register(t0);		// �f�B�t���[�Y�}�b�v�e�N�X�`��



// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float3 vReflect ;
	float4 color ;

	float4 lTextureDiffuseColor;

	float4 lNormalDiffuseColor;
	float4 lMetallicDiffuseColor;


	float3 lSpecularColor;
	float3 lDiffuseColor;
	float3 Normal;
	float lDiffuseAngleGen;
	float3 lTotalDiffuse;
	float3 lTotalSpecular;
	float3 lTotalAmbient;
	float4 lToonColor;
	float lTotalLightGen;
	float3 V_to_Eye;
	float3 TempF3;
	float Temp;
	float3 lLightTemp;
	float3 lLightDir;



	// �@���� 0�`1 �̒l�� -1.0�`1.0 �ɕϊ�����
	Normal = (g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0.xy).rgb - float3(0.5f, 0.5f, 0.5f)) * 2.0f;

	// �@���̏���
	//Normal = normalize(PSInput.VNormal);

	// ���_���W���王�_�ւ̃x�N�g���𐳋K��
	V_to_Eye = normalize(-PSInput.VPosition);

	// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�ƃA���r�G���g�J���[�̍��v�l��������
	lTotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	lTotalSpecular = float3(0.0f, 0.0f, 0.0f);
	lTotalAmbient = float3(0.0f, 0.0f, 0.0f);

	// ���C�g�̌��������v�l�̏�����
	lTotalLightGen = 0.0f;



	// �f�B���N�V���i�����C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���C�g�����x�N�g���̃Z�b�g
	lLightDir = g_Common.Light[0].Direction;


	// �f�B�t���[�Y�p�x�������v�Z
	lDiffuseAngleGen = saturate(dot(Normal, -lLightDir));

	// �f�B�t���[�Y�����������v�l�ɉ��Z
	lTotalLightGen += lDiffuseAngleGen;


	// �X�y�L�����J���[�v�Z

	// �n�[�t�x�N�g���̌v�Z
	TempF3 = normalize(V_to_Eye - lLightDir);

	// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
	Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.Material.Power);

	// �X�y�L�������C�g���v�l += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
	lTotalSpecular += Temp * g_Common.Light[0].Specular;


	// ���C�g�̃f�B�t���[�Y�J���[�����v�l�ɉ��Z
	lTotalDiffuse += g_Common.Light[0].Diffuse;

	// ���C�g�̃A���r�G���g�J���[�����v�l�ɉ��Z
	lTotalAmbient += g_Common.Light[0].Ambient.xyz;

	// �f�B���N�V���i�����C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �A���r�G���g�J���[�̒~�ϒl += �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z��������
	lTotalAmbient += g_Common.Material.Ambient_Emissive.xyz;








	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �e�N�X�`���J���[�̎擾
	lTextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
	lNormalDiffuseColor = g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0);
	lMetallicDiffuseColor = g_MetallicMapTexture.Sample(g_MetallicMapSampler, PSInput.TexCoords0);

	// �g�D�[���e�N�X�`���J���[�����C�g�̃f�B�t���[�Y����������擾
	lToonColor = g_ToonDiffuseGradTexture.Sample(g_ToonDiffuseGradSampler, lTotalLightGen);

	// �f�B�t���[�Y�J���[ = ���C�g�̃f�B�t���[�Y�J���[�~�ϒl * �}�e���A���̃f�B�t���[�Y�J���[
	lDiffuseColor = lTotalDiffuse * g_Common.Material.Diffuse.xyz;

	// �X�y�L�����J���[ = ���C�g�̃X�y�L�����J���[�~�ϒl * �}�e���A���̃X�y�L�����J���[
	lSpecularColor = lTotalSpecular * g_Common.Material.Specular.xyz * 10.f;

	// �o�� = saturate( saturate( �f�B�t���[�Y�J���[ * �A���r�G���g�J���[�̒~�ϒl ) * �g�D�[���e�N�X�`���J���[ + �X�y�L�����J���[ ) * �e�N�X�`���J���[
	PSOutput.Color0.rgb = saturate(saturate(lDiffuseColor + lTotalAmbient) * lToonColor.rgb + lSpecularColor) * lTextureDiffuseColor.rgb;

	// �A���t�@�l = �f�B�t���[�Y�A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	PSOutput.Color0.a = lTextureDiffuseColor.a * g_Common.Material.Diffuse.a * g_Base.FactorColor.a;

	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )






	vReflect = reflect( PSInput.viewVecW, PSInput.normalW ) ;
	color = 1.f * cubeMapTexture.Sample( cubeMapSampler, vReflect ) +
	        0.5f * dynamicCubeMapTexture.Sample( dynamicCubeMapSampler, vReflect ) ;

	color.a = 1.0f;
//	color = cubeMapTexture.Sample( cubeMapSampler, PSInput.normalW ) ;

	PSOutput.Color0 = (0.1f * lMetallicDiffuseColor.r * color) + 1.f * PSOutput.Color0;

	// �t�H�O����
	PSOutput.Color0.rgb = lerp(g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate(PSInput.Fog));

	return PSOutput ;
}


