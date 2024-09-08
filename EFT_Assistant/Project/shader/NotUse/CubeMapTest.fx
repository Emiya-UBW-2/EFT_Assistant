// 環境マップ用頂点シェーダ
float4x4 view;
float4x4 proj;
float4x4 world;
float3 cameraPosW; // カメラのワールド位置

struct VS_IN {
    float3 pos : POSITION;
   float3 normal: NORMAL;
};

struct VS_OUT {
   float4 pos : POSITION;
   float3 normalW: TEXCOORD0; // ワールド空間の法線
  float3 viewVecW: TEXCOORD1; // ワールド空間での視線ベクトル
};

VS_OUT vs_main( VS_IN In ) {
  VS_OUT Out;
  Out.pos = mul( float4(In.pos, 1.0f), world );
  Out.viewVecW = Out.pos.xyz - cameraPosW;
  Out.pos = mul( Out.pos, view );
  Out.pos = mul( Out.pos, proj );

  Out.normalW = mul( float4(In.normal, 0.0f), world ).xyz;

  return Out;
}


// 環境マップ用ピクセルシェーダ
textureCUBE cubeTex;
samplerCUBE cubeTexSampler = sampler_state {
  Texture = <cubeTex>;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
};
textureCUBE staticCubeTex;
samplerCUBE staticCubeTexSampler = sampler_state {
  Texture = <staticCubeTex>;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
};

float4 ps_main( VS_OUT In ) : COLOR {
  float3 vReflect = reflect( In.viewVecW, In.normalW );
  float4 color = 0.25f * texCUBE(staticCubeTexSampler, vReflect) + 0.75f * texCUBE(cubeTexSampler, vReflect);
  return color;
}

technique tech {
	pass p0 {
		VertexShader = compile vs_2_0 vs_main();
		PixelShader  = compile ps_2_0 ps_main();
	}
}
