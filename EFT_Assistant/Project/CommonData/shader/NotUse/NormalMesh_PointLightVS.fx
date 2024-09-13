// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float3 Position        : POSITION ;		// ���W( ���[�J����� )
	float3 Normal          : NORMAL0 ;      // �@��( ���[�J����� )
	float4 Diffuse         : COLOR0 ;       // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;       // �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0 ;	// �e�N�X�`�����W
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float4 PositionSub	   : TEXCOORD1;	// ���W( �ˉe��� )�s�N�Z���V�F�[�_�[�ŎQ�Ƃ���ׂ̕�
	float4 VPosition	   : TEXCOORD2;	// ���W( �ˉe��� )�s�N�Z���V�F�[�_�[�ŎQ�Ƃ���ׂ̕�
	float4 Position        : SV_POSITION ;	// ���W( �v���W�F�N�V������� )
} ;


// �}�e���A���p�����[�^
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse ;				// �f�B�t���[�Y�J���[
	float4		Specular ;				// �X�y�L�����J���[
	float4		Ambient_Emissive ;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

	float		Power ;					// �X�y�L�����̋���
	float		TypeParam0 ;			// �}�e���A���^�C�v�p�����[�^0
	float		TypeParam1 ;			// �}�e���A���^�C�v�p�����[�^1
	float		TypeParam2 ;			// �}�e���A���^�C�v�p�����[�^2
} ;

// �t�H�O�p�����[�^
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd ;				// �t�H�O�p�p�����[�^ end / ( end - start )
	float		LinearDiv ;				// �t�H�O�p�p�����[�^ -1  / ( end - start )
	float		Density ;				// �t�H�O�p�p�����[�^ density
	float		E ;						// �t�H�O�p�p�����[�^ ���R�ΐ��̒�

	float4		Color ;					// �J���[
} ;

// ���C�g�p�����[�^
struct DX_D3D11_CONST_LIGHT
{
	int			Type ;					// ���C�g�^�C�v( DX_LIGHTTYPE_POINT �Ȃ� )
	int3		Padding1 ;				// �p�f�B���O�P

	float3		Position ;				// ���W( �r���[��� )
	float		RangePow2 ;				// �L�������̂Q��

	float3		Direction ;				// ����( �r���[��� )
	float		FallOff ;				// �X�|�b�g���C�g�pFallOff

	float3		Diffuse ;				// �f�B�t���[�Y�J���[
	float		SpotParam0 ;			// �X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )

	float3		Specular ;				// �X�y�L�����J���[
	float		SpotParam1 ;			// �X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient ;				// �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

	float		Attenuation0 ;			// �����ɂ�錸�������p�p�����[�^�O
	float		Attenuation1 ;			// �����ɂ�錸�������p�p�����[�^�P
	float		Attenuation2 ;			// �����ɂ�錸�������p�p�����[�^�Q
	float		Padding2 ;				// �p�f�B���O�Q
} ;

// �s�N�Z���V�F�[�_�[�E���_�V�F�[�_�[���ʃp�����[�^
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[ 6 ] ;			// ���C�g�p�����[�^
	DX_D3D11_CONST_MATERIAL		Material ;				// �}�e���A���p�����[�^
	DX_D3D11_VS_CONST_FOG		Fog ;					// �t�H�O�p�����[�^
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

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON				g_Common ;
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


// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalPosition ;
	float4 lWorldPosition ;
	float3 lWorldNrm ;
	float3 lViewNrm ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float3 lLightDir ;
	float3 lLightTemp ;
	float lLightDistancePow2 ;
	float lLightGen ;


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�̃Z�b�g
	lLocalPosition.xyz = VSInput.Position ;
	lLocalPosition.w = 1.0f ;

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.x = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ���[���h���W���r���[���W�ɕϊ�
	VSOutput.VPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	VSOutput.VPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	VSOutput.VPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	VSOutput.VPosition.w = 1.0f ;

	// �r���[���W���ˉe���W�ɕϊ�
	VSOutput.Position.x = dot(VSOutput.VPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot(VSOutput.VPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot(VSOutput.VPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot(VSOutput.VPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =====================================================( �J�n )

	// ���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
	lWorldNrm.x = dot( VSInput.Normal, g_Base.LocalWorldMatrix[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, g_Base.LocalWorldMatrix[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, g_Base.LocalWorldMatrix[ 2 ].xyz ) ;

	// ���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
	lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =====================================================( �I�� )


	// ���C�g�����x�N�g���̌v�Z
	lLightDir = normalize(VSOutput.VPosition.xyz - g_Common.Light[ 0 ].Position.xyz ) ;


	// ���������l�v�Z ===================================================================( �J�n )

	// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
	lLightTemp = VSOutput.VPosition.xyz - g_Common.Light[ 0 ].Position.xyz ;
	lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

	// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
	lLightGen = 1.0f / ( g_Common.Light[ 0 ].Attenuation0 + g_Common.Light[ 0 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 0 ].Attenuation2 * lLightDistancePow2 ) ;

	// �L�������O�������猸�������ő�ɂ��鏈��
	lLightGen *= step( lLightDistancePow2, g_Common.Light[ 0 ].RangePow2 ) ;

	// ���������l�v�Z ===================================================================( �I�� )


	// ���C�g�f�B�t���[�Y�J���[�ƃ��C�g�X�y�L�����J���[�̊p�x�����v�Z ===================( �J�n )

	// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓��ς� lLightLitParam.x �ɃZ�b�g
	lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

	// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
	lLightHalfVec = normalize( normalize( -VSOutput.VPosition.xyz ) - lLightDir ) ;

	// �@���ƃn�[�t�x�N�g���̓��ς� lLightLitParam.y �ɃZ�b�g
	lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

	// �X�y�L�������˗��� lLightLitParam.w �ɃZ�b�g
	lLightLitParam.w = g_Common.Material.Power ;

	// ���C�g�p�����[�^�v�Z
	lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

	// ���C�g�f�B�t���[�Y�J���[�ƃ��C�g�X�y�L�����J���[�̊p�x�����v�Z ===================( �I�� )

	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �f�B�t���[�Y�J���[ =
	//            ���������l *
	//            ( �f�B�t���[�Y�p�x�����v�Z���� *
	//              ���C�g�̃f�B�t���[�Y�J���[ *
	//              �}�e���A���̃f�B�t���[�Y�J���[ +
	//              ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� ) +
	//            �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z��������
	VSOutput.Diffuse = g_Common.Material.Diffuse;

	// �X�y�L�����A���t�@�̓}�e���A���̃X�y�L�����J���[�̃A���t�@�����̂܂܎g��
	VSOutput.Specular = g_Common.Material.Specular;


	// �e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
	VSOutput.TexCoords0.x = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �s�N�Z���V�F�[�_�[�Ŏg�p���邽�߂̎ˉe���W���Z�b�g
	VSOutput.PositionSub = VSOutput.Position;


	// �o�̓p�����[�^��Ԃ�
	return VSOutput ;
}

