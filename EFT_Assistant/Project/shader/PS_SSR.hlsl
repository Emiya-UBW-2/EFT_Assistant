// ピクセルシェーダーの入力
struct PS_INPUT {
	float4 dif         : COLOR0;		// ディフューズカラー
	float2 texCoords0  : TEXCOORD0;	// テクスチャ座標
	float4 pos         : SV_POSITION;   // 座標( プロジェクション空間 )
};

// ピクセルシェーダーの出力
struct PS_OUTPUT {
	float4 color0           : SV_TARGET0;	// 色
};

//画面サイズ
static int2 dispsize = {0, 0};

// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE {
	float4		FactorColor;			// アルファ値等

	float		MulAlphaColor;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	float		AlphaTestRef;			// アルファテストで使用する比較値
	float2		Padding1;

	int		    AlphaTestCmpMode;		// アルファテスト比較モード( DX_CMP_NEVER など )
	int3		Padding2;

	float4		IgnoreTextureColor;	// テクスチャカラー無視処理用カラー
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1) {
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

// プログラムとのやり取りのために使うレジスタ
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3) {
	float4 caminfo;
}

cbuffer cbLIGHTCAMERA_MATRIX : register(b4) {
	matrix		g_LightViewMatrix;			// ライトのワールド　→　ビュー行列
	matrix		g_LightProjectionMatrix;	// ライトのビュー　　→　射影行列
};

// プログラムとのやり取りのために使うレジスタ2
cbuffer cbMULTIPLYCOLOR_CBUFFER3 : register(b5) {
	float4	g_param;
}


//テクスチャ
SamplerState g_Register0MapSampler : register(s0); // ディフューズマップサンプラ
Texture2D g_Register0MapTexture : register(t0); // ディフューズマップテクスチャ

SamplerState g_Register1MapSampler : register(s1); // 法線マップサンプラ
Texture2D g_Register1MapTexture : register(t1); // 法線マップテクスチャ

SamplerState g_Register2MapSampler : register(s2); // 深度マップサンプラ
Texture2D g_Register2MapTexture : register(t2); // 深度マップテクスチャ

SamplerState dynamicCubeMapSampler     : register(s3);
TextureCube  dynamicCubeMapTexture     : register(t3);

SamplerState FilterSampler		: register(s4);
Texture2D  FilterTexture		: register(t4);

//関数
float4 GetTexColor0(float2 texCoord, int2 offset = int2(0, 0)) {
	return g_Register0MapTexture.Sample(g_Register0MapSampler, texCoord, offset);
}
float4 GetTexColor1(float2 texCoord, int2 offset = int2(0, 0)) {
	return g_Register1MapTexture.Sample(g_Register1MapSampler, texCoord, offset);
}
float4 GetTexColor2(float2 texCoord, int2 offset = int2(0, 0)) {
	return g_Register2MapTexture.Sample(g_Register2MapSampler, texCoord, offset);
}

float3 DisptoProjNorm(float2 screenUV) {
	float2 pos = screenUV;

	pos /= 0.5f;
	pos.x = pos.x - 1.f;
	pos.y = 1.f - pos.y;

	float3 position;
	position.x = pos.x * caminfo.z * dispsize.x / dispsize.y;
	position.y = pos.y * caminfo.z;
	position.z = 1.f;

	return position;
}

float2 ProjtoDisp(float3 position) {
	position = position / position.z;

	float2 screenUV;
	screenUV.x = position.x / caminfo.z * dispsize.y / dispsize.x;
	screenUV.y = position.y / caminfo.z;

	screenUV.x = screenUV.x + 1.f;
	screenUV.y = 1.f - screenUV.y;
	screenUV *= 0.5f;
	return screenUV;
}

bool Hitcheck(float3 position) {
	float2 screenUV = ProjtoDisp(position);
	if (
		(abs(screenUV.x) <= 1.f) &&
		(abs(screenUV.y) <= 1.f)
		) {
		float depth = GetTexColor2(screenUV).r;
		float z = depth / (caminfo.y * 0.005f);
		return (position.z < z && z < position.z + caminfo.w);
	}
	else {
		return false;
	}
}

static float maxLength = 5000.f; // 反射最大距離
static int BinarySearchIterations = 8; //2分探索最大数

float4 applySSR(float3 normal, float2 screenUV,bool Skip) {
	float pixelStride;
	float3 delta;
	float3 position;

	float depth = GetTexColor2(screenUV).r;

	float4 color = float4(0.f, 0.f, 0.f, 0.f);

	bool isend = (depth <= 0.f || Skip);
	if (!isend) {
		float3 NormPos = DisptoProjNorm(screenUV);
		pixelStride = maxLength / caminfo.x;
		delta = reflect(NormPos, normal); // 反射ベクトル*１回で進む距離
		position = NormPos * (depth / (caminfo.y * 0.005f)); //距離
	}
	[fastopt]
	for (int i = 0; i < caminfo.x; i++) {
		if (!isend) {
			position += delta * pixelStride;

				if (Hitcheck(position)) { //交差したので二分探索
					position -= delta * pixelStride; //元に戻し
					delta /= BinarySearchIterations; //進む量を下げる
					[unroll] // attribute
					for (int j = 0; j < BinarySearchIterations; j++) {
						if (!isend) {
							pixelStride *= 0.5f;
							position += delta * pixelStride;
							if (Hitcheck(position)) {
								pixelStride = -pixelStride;
							}
							if (length(pixelStride) < 1.f) {
								isend = true;
							}
						}
					}
					color = GetTexColor0(ProjtoDisp(position));// 交差したので色をブレンドする
					isend = true;
				}
		}
	}
	return color;
}

PS_OUTPUT main(PS_INPUT PSInput) {
	//戻り値
	PS_OUTPUT PSOutput;
	//画面サイズを取得しておく
	g_Register0MapTexture.GetDimensions(dispsize.x, dispsize.y);
	//反射をどれだけ見せるか
	float Per = GetTexColor2(PSInput.texCoords0).g * FilterTexture.Sample(FilterSampler, PSInput.texCoords0).r;
	//処理
	float4 lWorldPosition;

	//ノーマル座標取得
	float3 normal = GetTexColor1(PSInput.texCoords0).xyz * 2.f - 1.f;

	//キューブマップからの反射
	lWorldPosition.xyz = DisptoProjNorm(PSInput.texCoords0);
	lWorldPosition.x *= -1.f;
	lWorldPosition.w = 0.f;
	
	// ワールド座標を射影座標に変換
	float4 LPPosition1 = mul(g_LightViewMatrix, lWorldPosition);
	LPPosition1.z *= -1.f;

	lWorldPosition.xyz = normal;
	lWorldPosition.w = 0.f;

	// ワールド座標を射影座標に変換
	float4 LPPosition2 = mul(g_LightViewMatrix, lWorldPosition);

	float3 LPPosition3 = reflect(LPPosition1.xyz, LPPosition2.xyz);
	LPPosition3.xz *= -1.f;
	PSOutput.color0 = dynamicCubeMapTexture.Sample(dynamicCubeMapSampler, LPPosition3);

	if (g_param.x >= 2) {
		float4 color = applySSR(normal, PSInput.texCoords0, (Per <= 0.f));
		if (color.a > 0.f) {
			PSOutput.color0 = color;
		}
	}
	PSOutput.color0 = lerp(float4(0.f, 0.f, 0.f, 0.f), PSOutput.color0, Per);

	//戻り値
	//return PSOutput;
	//差分だけを出力する場合はこちら
	float3 Color = GetTexColor0(PSInput.texCoords0).xyz;
	if (
		PSOutput.color0.r == Color.r &&
		PSOutput.color0.g == Color.g &&
		PSOutput.color0.b == Color.b
		) {
		PSOutput.color0.a = 0.0;
	}
	return PSOutput;
}
