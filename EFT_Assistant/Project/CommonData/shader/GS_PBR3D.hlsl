// 頂点シェーダーの出力
struct GS_INPUT
{
	float2 TexCoords0	 : TEXCOORD0;    // テクスチャ座標
	float4 WPosition	 : TEXCOORD1;    // 座標(ワールド)
    float3 WTan			 : TEXCOORD2; // 接線( ワールド )
    float3 WBin			 : TEXCOORD3; // 従法線( ワールド )
    float3 WNormal		 : TEXCOORD4; // 法線( ワールド )
	float4 Specular        : COLOR1;		// スペキュラカラー
} ;

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float3 VPosition : TEXCOORD1; // 座標( ビュー空間 )
    float3 VTan : TEXCOORD2; // 接線( ビュー空間 )
    float3 VBin : TEXCOORD3; // 従法線( ビュー空間 )
    float3 VNormal : TEXCOORD4; // 法線( ビュー空間 )
    float4 Specular : COLOR1; // スペキュラカラー
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
};

// 基本パラメータ
struct DX_D3D11_GS_CONST_BUFFER_BASE
{
    float4 ProjectionMatrix[4]; // ビュー　→　プロジェクション行列
    float4 ViewMatrix[3]; // ワールド　→　ビュー行列
};
// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_GS_BASE : register(b1)
{
    DX_D3D11_GS_CONST_BUFFER_BASE g_Base;
};

float3 ViewTrance(float3 lWorldPosition)
{
    float3 Ret;
	// ワールド座標をビュー座標に変換
    Ret.x = dot(lWorldPosition, g_Base.ViewMatrix[0].xyz);
    Ret.y = dot(lWorldPosition, g_Base.ViewMatrix[1].xyz);
    Ret.z = dot(lWorldPosition, g_Base.ViewMatrix[2].xyz);
    return Ret;
}
float4 ViewTrance(float4 lWorldPosition)
{
    float4 Ret;
	// ワールド座標をビュー座標に変換
    Ret.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
    Ret.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
    Ret.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
    Ret.w = 1.0f;
    return Ret;
}
float4 ProjectionTrance(float4 lViewPosition)
{
    float4 Ret;
	// ビュー座標を射影座標に変換
    Ret.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
    Ret.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
    Ret.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
    Ret.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);
    return Ret;
}

[maxvertexcount(3)] // ジオメトリシェーダーで出力する最大頂点数
// ジオメトリシェーダー
void main(triangle GS_INPUT In[3], // トライアングル リストを構成する頂点配列の入力情報
				inout TriangleStream<PS_INPUT> TriStream // 頂点情報を追加するためのストリームオブジェクト
			)
{
    PS_INPUT Out;

    float4 lViewPosition;
	// もとの頂点を出力
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
