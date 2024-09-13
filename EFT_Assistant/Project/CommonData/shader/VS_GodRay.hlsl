// 頂点シェーダーの入力
struct VS_INPUT
{
	float3 pos             : POSITION0 ;		// 座標( ローカル空間 )
	float4 spos            : POSITION1 ;		// 予備座標
	float3 norm            : NORMAL0 ;			// 法線( ローカル空間 )
	float3 tan             : TANGENT0 ;			// 接線( ローカル空間 )
	float3 binorm          : BINORMAL0 ;		// 従法線( ローカル空間 )
	float4 dif             : COLOR0 ;			// ディフューズカラー
	float4 spc             : COLOR1 ;			// スペキュラカラー
	float2 texCoords0      : TEXCOORD0 ;		// テクスチャ座標
	float2 texCoords1      : TEXCOORD1 ;		// サブテクスチャ座標
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Diffuse : COLOR0; // ディフューズカラー
	float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
	float4 pos         : SV_POSITION;	// 座標( プロジェクション空間 )
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

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE			: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX		: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;

// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalPosition ;
	float4 lWorldPosition ;
	float4 lViewPosition ;


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標のセット
	lLocalPosition.xyz = VSInput.pos ;
	lLocalPosition.w = 1.0f ;

	// 座標計算( ローカル→ビュー→プロジェクション )
	lWorldPosition.x = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( lLocalPosition, g_Base.LocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	VSOutput.pos.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.pos.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.pos.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.pos.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	VSOutput.TexCoords0 = VSInput.texCoords0 ;
	VSOutput.Diffuse = VSInput.dif ;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
	return VSOutput ;
}



