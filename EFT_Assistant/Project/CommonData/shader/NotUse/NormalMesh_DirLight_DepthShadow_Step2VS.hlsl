// 頂点シェーダーの入力
struct VS_INPUT
{
	float3 Position        : POSITION ;			// 座標( ローカル空間 )
	float3 Normal          : NORMAL ;			// 法線( ローカル空間 )
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float4 Specular        : COLOR1 ;			// スペキュラカラー
	float4 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標
	float4 TexCoords1      : TEXCOORD1 ;		// サブテクスチャ座標
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
	float4 Specular        : COLOR1 ;		// スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;	// テクスチャ座標
	float4 LPPosition      : TEXCOORD1 ;    // ライトからみた座標( ライトの射影空間 )
	float4 Position        : SV_POSITION ;	// 座標( プロジェクション空間 )
} ;

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

// 影用の深度記録画像を作成した際のカメラのビュー行列と射影行列
struct LIGHTCAMERA_MATRIX
{
	MATRIX		ViewMatrix ;
	MATRIX		ProjectionMatrix ;
} ;

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

cbuffer cbLIGHTCAMERA_MATRIX						: register( b4 )
{
	LIGHTCAMERA_MATRIX							g_LightMatrix ;
} ;


// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalPosition ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float4 lLViewPosition ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標のセット
	lLocalPosition.xyz = VSInput.Position ;
	lLocalPosition.w = 1.0f ;

	// ローカル座標をワールド座標に変換
	lWorldPosition.x = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ワールド座標をビュー座標に変換
	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// ビュー座標を射影座標に変換
	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
	VSOutput.TexCoords0.x = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	// 深度影用のライトから見た射影座標を算出 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ワールド座標をライトのビュー座標に変換
	lLViewPosition = mul( g_LightMatrix.ViewMatrix, lWorldPosition ) ;

	// ライトのビュー座標をライトの射影座標に変換
	VSOutput.LPPosition = mul( g_LightMatrix.ProjectionMatrix, lLViewPosition ) ;

	// Ｚ値だけはライトのビュー座標にする
	VSOutput.LPPosition.z = lLViewPosition.z ;

	// 出力パラメータを返す
	return VSOutput ;
}

