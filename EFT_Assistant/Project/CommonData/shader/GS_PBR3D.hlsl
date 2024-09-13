// ���_�V�F�[�_�[�̏o��
struct GS_INPUT
{
	float2 TexCoords0	 : TEXCOORD0;    // �e�N�X�`�����W
	float4 WPosition	 : TEXCOORD1;    // ���W(���[���h)
    float3 WTan			 : TEXCOORD2; // �ڐ�( ���[���h )
    float3 WBin			 : TEXCOORD3; // �]�@��( ���[���h )
    float3 WNormal		 : TEXCOORD4; // �@��( ���[���h )
	float4 Specular        : COLOR1;		// �X�y�L�����J���[
} ;

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float3 VPosition : TEXCOORD1; // ���W( �r���[��� )
    float3 VTan : TEXCOORD2; // �ڐ�( �r���[��� )
    float3 VBin : TEXCOORD3; // �]�@��( �r���[��� )
    float3 VNormal : TEXCOORD4; // �@��( �r���[��� )
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float4 Position : SV_POSITION; // ���W( �v���W�F�N�V������� )
};

// ��{�p�����[�^
struct DX_D3D11_GS_CONST_BUFFER_BASE
{
    float4 ProjectionMatrix[4]; // �r���[�@���@�v���W�F�N�V�����s��
    float4 ViewMatrix[3]; // ���[���h�@���@�r���[�s��
};
// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_GS_BASE : register(b1)
{
    DX_D3D11_GS_CONST_BUFFER_BASE g_Base;
};

float3 ViewTrance(float3 lWorldPosition)
{
    float3 Ret;
	// ���[���h���W���r���[���W�ɕϊ�
    Ret.x = dot(lWorldPosition, g_Base.ViewMatrix[0].xyz);
    Ret.y = dot(lWorldPosition, g_Base.ViewMatrix[1].xyz);
    Ret.z = dot(lWorldPosition, g_Base.ViewMatrix[2].xyz);
    return Ret;
}
float4 ViewTrance(float4 lWorldPosition)
{
    float4 Ret;
	// ���[���h���W���r���[���W�ɕϊ�
    Ret.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
    Ret.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
    Ret.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
    Ret.w = 1.0f;
    return Ret;
}
float4 ProjectionTrance(float4 lViewPosition)
{
    float4 Ret;
	// �r���[���W���ˉe���W�ɕϊ�
    Ret.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
    Ret.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
    Ret.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
    Ret.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);
    return Ret;
}

[maxvertexcount(3)] // �W�I���g���V�F�[�_�[�ŏo�͂���ő咸�_��
// �W�I���g���V�F�[�_�[
void main(triangle GS_INPUT In[3], // �g���C�A���O�� ���X�g���\�����钸�_�z��̓��͏��
				inout TriangleStream<PS_INPUT> TriStream // ���_����ǉ����邽�߂̃X�g���[���I�u�W�F�N�g
			)
{
    PS_INPUT Out;

    float4 lViewPosition;
	// ���Ƃ̒��_���o��
    for (int i = 0; i < 3; i++)
    {
        Out.VTan = ViewTrance(In[i].WTan);
        Out.VBin = ViewTrance(In[i].WBin);
        Out.VNormal = ViewTrance(In[i].WNormal);
		//
        lViewPosition = ViewTrance(In[i].WPosition);
        Out.Position = ProjectionTrance(lViewPosition);
        Out.VPosition = lViewPosition.xyz;
		//
        Out.TexCoords0 = In[i].TexCoords0;
        Out.Specular = In[i].Specular;
		//
        TriStream.Append(Out);
    }
    TriStream.RestartStrip();
}
