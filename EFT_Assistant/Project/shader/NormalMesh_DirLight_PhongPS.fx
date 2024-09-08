// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0;	// �e�N�X�`�����W
	float3 VPosition       : TEXCOORD1;    // ���W( �r���[��� )
	float3 VTan            : TEXCOORD2;    // �ڐ�( �r���[��� )
	float3 VBin            : TEXCOORD3;    // �]�@��( �r���[��� )
	float3 VNormal         : TEXCOORD4;    // �@��( �r���[��� )
	float  Fog			   : TEXCOORD5;		// �t�H�O�p�����[�^
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0;	// �F
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
// �`��J���[�ɏ�Z����l
cbuffer cbMULTIPLYCOLOR_CBUFFER : register(b3)
{
	float4	g_MultiplyColor;
}

// C++ ���Őݒ肷��e�N�X�`����萔�̒�`
SamplerState g_DiffuseMapSampler            : register(s0);		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_DiffuseMapTexture            : register(t0);		// �f�B�t���[�Y�}�b�v�e�N�X�`��
SamplerState g_NormalMapSampler             : register(s1);		// �@���}�b�v�e�N�X�`��
Texture2D    g_NormalMapTexture             : register(t1);		// �@���}�b�v�e�N�X�`��

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float4 TextureDiffuseColor;
	float3 SpecularColor;
	float3 Normal;
	float DiffuseAngleGen;
	float3 TotalDiffuse;
	float3 TotalSpecular;
	float3 TempF3;
	float Temp;
	float3 V_to_Eye;
	float3 lLightDir;
	float3 VNrm;
	float3 VTan;
	float3 VBin;

	float	g_time = g_MultiplyColor.x;
	// �ڐ��E�]�@���E�@���𐳋K��
	VNrm = normalize(PSInput.VNormal);
	VTan = normalize(PSInput.VTan);
	VBin = normalize(PSInput.VBin);

	// ���_���W���王�_�ւ̃x�N�g����ڒ��Ԃɓ��e�����㐳�K�����ĕۑ�
	TempF3.x = dot(VTan, -PSInput.VPosition.xyz);
	TempF3.y = dot(VBin, -PSInput.VPosition.xyz);
	TempF3.z = dot(VNrm, -PSInput.VPosition.xyz);
	V_to_Eye = normalize(TempF3);

	// �@���� 0�`1 �̒l�� -1.0�`1.0 �ɕϊ�����
	// �@���̏���
	//*
	{
		// �e�N�X�`�����W
		float2 TexCoords0 = PSInput.TexCoords0;
		//�����
		float WaveM;
		float2 WaveUV, WaveMove, DirU, DirV, WaveDir = { 1.0, 0.0 };

		WaveUV.x = dot(WaveDir, TexCoords0);
		WaveUV.y = dot(-WaveDir.yx, TexCoords0);
		WaveM = fmod((g_time + WaveUV.x * 5) * 6.28, 6.28);
		WaveMove.x = sin(WaveM) * 0.01 / 2;
		WaveM = fmod((g_time + WaveUV.y * 5) * 6.28, 6.28);
		WaveMove.y = sin(WaveM) * 0.01 / 2;

		TexCoords0 += WaveMove;
		//�m�[�}���}�b�v��d���v��
		TexCoords0.x += 0.5;
		TexCoords0.y += (sin(g_time * 3 + 8) / 512) + (g_time / 32);
		float h = g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).a;
		TexCoords0 = TexCoords0 + 0.016 * h * V_to_Eye.xy;
		Normal = 2.0f * g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).xyz - 1.0f;

		TexCoords0.x -= 0.5;
		TexCoords0.y -= ((sin(g_time * 3 + 8) / 512) + (g_time / 32))*2.0f;
		h = g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).a;
		TexCoords0 = TexCoords0 + 0.016 * h * V_to_Eye.xy;
		float3 Normal2 = 2.0f * g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).xyz - 1.0f;

		Normal = (Normal + Normal2) / 2;
	}
	//*/
	//Normal = (g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0.xy).rgb - float3(0.5f, 0.5f, 0.5f)) * 2.0f;

	// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl��������
	TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	TotalSpecular = float3(0.0f, 0.0f, 0.0f);
	// �f�B���N�V���i�����C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���C�g�����x�N�g���̃Z�b�g
	// ���C�g�����x�N�g���̌v�Z
	TempF3 = g_Common.Light[0].Direction;
	// ���C�g�̃x�N�g����ڒn��Ԃɕϊ�
	lLightDir.x = dot(VTan, TempF3);
	lLightDir.y = dot(VBin, TempF3);
	lLightDir.z = dot(VNrm, TempF3);

	// �f�B�t���[�Y�F�v�Z
	// DiffuseAngleGen = �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
	// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �}�e���A���̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
	TotalDiffuse += g_Common.Light[0].Diffuse * g_Common.Material.Diffuse.xyz * DiffuseAngleGen + g_Common.Light[0].Ambient.xyz;

	// �X�y�L�����J���[�v�Z
	// �n�[�t�x�N�g���̌v�Z
	TempF3 = normalize(V_to_Eye - lLightDir);
	// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
	Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.Material.Power);
	// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
	TotalSpecular += Temp * g_Common.Light[0].Specular;

	// �f�B���N�V���i�����C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )
	

	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// TotalDiffuse = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
	TotalDiffuse += g_Common.Material.Ambient_Emissive.xyz;

	// SpecularColor = ���C�g�̃X�y�L�����J���[�~�ϒl * �}�e���A���̃X�y�L�����J���[
	SpecularColor = TotalSpecular * g_Common.Material.Specular.xyz;

	// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[ + SpecularColor
	TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
	PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse + SpecularColor;

	PSOutput.Color0.rgb = lerp(g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate(PSInput.Fog));//fog

	// �A���t�@�l = �e�N�X�`���A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	PSOutput.Color0.a = TextureDiffuseColor.a * g_Common.Material.Diffuse.a;// *g_Base.FactorColor.a;

	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}