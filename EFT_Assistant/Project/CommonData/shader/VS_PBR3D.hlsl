// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position        : POSITION;         // 座標( ローカル空間 )
	float3 Normal          : NORMAL0;          // 法線( ローカル空間 )
	float4 Diffuse         : COLOR0;           // ディフューズカラー
	float4 Specular        : COLOR1;           // スペキュラカラー
	float4 TexCoords0      : TEXCOORD0;        // テクスチャ座標
	float4 TexCoords1      : TEXCOORD1;		// サブテクスチャ座標
	float3 Tan             : TANGENT0;		// 接線( ローカル空間 )
	float3 Bin             : BINORMAL0;		// 従法線( ローカル空間 )
	int4   BlendIndices0   : BLENDINDICES0;    // スキニング処理用 Float型定数配列インデックス
	float4 BlendWeight0    : BLENDWEIGHT0;     // スキニング処理用ウエイト値
} ;

// 頂点シェーダーの出力
struct GS_INPUT
{
	float2 TexCoords0	 : TEXCOORD0;    // テクスチャ座標
	float4 WPosition	 : TEXCOORD1;    // 座標(ワールド)
    float3 WTan			 : TEXCOORD2; // 接線( ワールド )
    float3 WBin			 : TEXCOORD3; // 従法線( ワールド )
    float3 WNormal		 : TEXCOORD4; // 法線( ワールド )
    float4 Specular		 : COLOR1; // スペキュラカラー
} ;


// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL {
	float4		Diffuse;				// ディフューズカラー
	float4		Specular;				// スペキュラカラー
	float4		Ambient_Emissive;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	float		Power;					// スペキュラの強さ
	float		TypeParam0;			// マテリアルタイプパラメータ0
	float		TypeParam1;			// マテリアルタイプパラメータ1
	float		TypeParam2;			// マテリアルタイプパラメータ2
};

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG {
	float		LinearAdd;				// フォグ用パラメータ end / ( end - start )
	float		LinearDiv;				// フォグ用パラメータ -1  / ( end - start )
	float		Density;				// フォグ用パラメータ density
	float		E;						// フォグ用パラメータ 自然対数の低

	float4		Color;					// カラー
};

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT {
	int			Type;					// ライトタイプ( DX_LIGHTTYPE_POINT など )
	int3		Padding1;				// パディング１

	float3		Position;				// 座標( ビュー空間 )
	float		RangePow2;				// 有効距離の２乗

	float3		Direction;				// 方向( ビュー空間 )
	float		FallOff;				// スポットライト用FallOff

	float3		Diffuse;				// ディフューズカラー
	float		SpotParam0;			// スポットライト用パラメータ０( cos( Phi / 2.0f ) )

	float3		Specular;				// スペキュラカラー
	float		SpotParam1;			// スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient;				// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	float		Attenuation0;			// 距離による減衰処理用パラメータ０
	float		Attenuation1;			// 距離による減衰処理用パラメータ１
	float		Attenuation2;			// 距離による減衰処理用パラメータ２
	float		Padding2;				// パディング２
};

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct DX_D3D11_CONST_BUFFER_COMMON {
	DX_D3D11_CONST_LIGHT		Light[6];			// ライトパラメータ
	DX_D3D11_CONST_MATERIAL		Material;				// マテリアルパラメータ
	DX_D3D11_VS_CONST_FOG		Fog;					// フォグパラメータ
};


// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[ 4 ] ;				// アンチビューポート行列
	float4		ProjectionMatrix[ 4 ] ;					// ビュー　→　プロジェクション行列
	float4		ViewMatrix[ 3 ] ;						// ワールド　→　ビュー行列
	float4		LocalWorldMatrix[ 3 ] ;					// ローカル　→　ワールド行列

	float4		ToonOutLineSize ;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource ;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource ;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor ;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding ;
} ;

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[ 3 ][ 4 ] ;			// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
	float4		TextureMatrix[ 3 ][ 2 ] ;								// テクスチャ座標操作用行列
} ;

// スキニングメッシュ用の　ローカル　→　ワールド行列
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX {
	float4		LWMatrix[54 * 3];					// ローカル　→　ワールド行列
};

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0) {
	DX_D3D11_CONST_BUFFER_COMMON				g_Common;
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;

// スキニングメッシュ用の　ローカル　→　ワールド行列
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register(b3) {
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix;
};

float2 SetTexCoords0(float4 TexCoords0)
{
    float2 Ret;
	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
    Ret.x = dot(TexCoords0, g_OtherMatrix.TextureMatrix[0][0]);
    Ret.y = dot(TexCoords0, g_OtherMatrix.TextureMatrix[0][1]);
    return Ret;
}
// main関数
GS_INPUT main(VS_INPUT VSInput)
{
    GS_INPUT VSOutput;
	float4 lLocalWorldMatrix[3];

	// 複数のフレームのブレンド行列の作成
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


	// 出力パラメータを返す
    return VSOutput;
}

