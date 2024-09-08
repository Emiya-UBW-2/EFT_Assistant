// 頂点シェーダーの入力
struct VS_INPUT
{
	float3 Position        : POSITION;		// 座標( ローカル空間 )
	float3 Normal          : NORMAL0;      // 法線( ローカル空間 )
	float4 Diffuse         : COLOR0;       // ディフューズカラー
	float4 Specular        : COLOR1;       // スペキュラカラー
	float4 TexCoords0      : TEXCOORD0;		// テクスチャ座標
	float4 TexCoords1      : TEXCOORD1;		// サブテクスチャ座標
	float3 Tan             : TANGENT0;			// 接線( ローカル空間 )
	float3 Bin             : BINORMAL0;		// 従法線( ローカル空間 )
};
// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float2 TexCoords0      : TEXCOORD0;    // テクスチャ座標
	float3 VPosition       : TEXCOORD1;    // 座標( ビュー空間 )
	float3 VTan            : TEXCOORD2;    // 接線( ビュー空間 )
	float3 VBin            : TEXCOORD3;    // 従法線( ビュー空間 )
	float3 VNormal         : TEXCOORD4;    // 法線( ビュー空間 )
	float  Fog			   : TEXCOORD5;		// フォグパラメータ
	float4 Position        : SV_POSITION;	// 座標( プロジェクション空間 )
};
// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse;				// ディフューズカラー
	float4		Specular;				// スペキュラカラー
	float4		Ambient_Emissive;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	float		Power;					// スペキュラの強さ
	float		TypeParam0;			// マテリアルタイプパラメータ0
	float		TypeParam1;			// マテリアルタイプパラメータ1
	float		TypeParam2;			// マテリアルタイプパラメータ2
};

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd;				// フォグ用パラメータ end / ( end - start )
	float		LinearDiv;				// フォグ用パラメータ -1  / ( end - start )
	float		Density;				// フォグ用パラメータ density
	float		E;						// フォグ用パラメータ 自然対数の低

	float4		Color;					// カラー
};

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT
{
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
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[6];			// ライトパラメータ
	DX_D3D11_CONST_MATERIAL		Material;				// マテリアルパラメータ
	DX_D3D11_VS_CONST_FOG		Fog;					// フォグパラメータ
};
// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE {
	float4		AntiViewportMatrix[4];				// アンチビューポート行列
	float4		ProjectionMatrix[4];				// ビュー　→　プロジェクション行列
	float4		ViewMatrix[3];						// ワールド　→　ビュー行列
	float4		LocalWorldMatrix[3];				// ローカル　→　ワールド行列

	float4		ToonOutLineSize;					// トゥーンの輪郭線の大きさ
	float		DiffuseSource;						// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor;					// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding;
};
// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[3][4];			// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
	float4		TextureMatrix[3][2];								// テクスチャ座標操作用行列
};
// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0)
{
	DX_D3D11_CONST_BUFFER_COMMON				g_Common;
};
// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register(b1)
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base;
};
// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register(b2)
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix;
};
// 頂点座標に加算する値
cbuffer cbADDPOSITION_CBUFFER						: register(b4)
{
	float4										g_AddPosition;
};
// main関数
VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT VSOutput;
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;
	float3 lWorldNrm;
	float3 lWorldTan;
	float3 lWorldBin;
	float3 lViewNrm;
	float3 lViewTan;
	float3 lViewBin;

	float	g_time = g_AddPosition.x;

	VSInput.Position.y = sin(VSInput.Position.z + g_time * 0.5f)-0.5f;
	VSInput.Position.z += sin((g_time * 8) + VSInput.Position.y) / 16;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標のセット
	lLocalPosition.xyz = VSInput.Position;
	lLocalPosition.w = 1.0f;

	// ローカル座標をワールド座標に変換
	lWorldPosition.x = dot(lLocalPosition, g_Base.LocalWorldMatrix[0]);
	lWorldPosition.y = dot(lLocalPosition, g_Base.LocalWorldMatrix[1]);
	lWorldPosition.z = dot(lLocalPosition, g_Base.LocalWorldMatrix[2]);
	lWorldPosition.w = 1.0f;

	// ワールド座標をビュー座標に変換
	lViewPosition.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
	lViewPosition.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
	lViewPosition.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
	lViewPosition.w = 1.0f;

	// ビュー座標を射影座標に変換
	VSOutput.Position.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
	VSOutput.Position.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
	VSOutput.Position.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
	VSOutput.Position.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 法線をビュー空間の角度に変換 =========================================( 開始 )
	//
	lWorldTan.x = dot(VSInput.Tan, g_Base.LocalWorldMatrix[0].xyz);
	lWorldTan.y = dot(VSInput.Tan, g_Base.LocalWorldMatrix[1].xyz);
	lWorldTan.z = dot(VSInput.Tan, g_Base.LocalWorldMatrix[2].xyz);
	//
	lWorldBin.x = dot(VSInput.Bin, g_Base.LocalWorldMatrix[0].xyz);
	lWorldBin.y = dot(VSInput.Bin, g_Base.LocalWorldMatrix[1].xyz);
	lWorldBin.z = dot(VSInput.Bin, g_Base.LocalWorldMatrix[2].xyz);
	// ローカルベクトルをワールドベクトルに変換
	lWorldNrm.x = dot(VSInput.Normal, g_Base.LocalWorldMatrix[0].xyz);
	lWorldNrm.y = dot(VSInput.Normal, g_Base.LocalWorldMatrix[1].xyz);
	lWorldNrm.z = dot(VSInput.Normal, g_Base.LocalWorldMatrix[2].xyz);
	//
	lViewTan.x = dot(lWorldTan, g_Base.ViewMatrix[0].xyz);
	lViewTan.y = dot(lWorldTan, g_Base.ViewMatrix[1].xyz);
	lViewTan.z = dot(lWorldTan, g_Base.ViewMatrix[2].xyz);
	//
	lViewBin.x = dot(lWorldBin, g_Base.ViewMatrix[0].xyz);
	lViewBin.y = dot(lWorldBin, g_Base.ViewMatrix[1].xyz);
	lViewBin.z = dot(lWorldBin, g_Base.ViewMatrix[2].xyz);
	// ワールドベクトルをビューベクトルに変換
	lViewNrm.x = dot(lWorldNrm, g_Base.ViewMatrix[0].xyz);
	lViewNrm.y = dot(lWorldNrm, g_Base.ViewMatrix[1].xyz);
	lViewNrm.z = dot(lWorldNrm, g_Base.ViewMatrix[2].xyz);

	// 法線をビュー空間の角度に変換 =========================================( 終了 )



	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// 接線を保存
	VSOutput.VTan = lViewTan;

	// 従法線を保存
	VSOutput.VBin = lViewBin;


	// 頂点座標を保存
	VSOutput.VPosition = lViewPosition.xyz;

	// 法線を保存
	VSOutput.VNormal = lViewNrm;

	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
	VSOutput.TexCoords0.x = dot(VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[0][0]);
	VSOutput.TexCoords0.y = dot(VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[0][1]);

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// 線形フォグ計算
	VSOutput.Fog = lViewPosition.z * g_Common.Fog.LinearDiv + g_Common.Fog.LinearAdd;


	// 出力パラメータを返す
	return VSOutput;
}

