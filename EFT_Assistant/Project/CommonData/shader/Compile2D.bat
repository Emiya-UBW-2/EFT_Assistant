set NowDir=%CD%
cd %~dp0

ShaderCompiler.exe /Tvs_4_0 VS_lens.hlsl
ShaderCompiler.exe /Tps_4_0 PS_lens.hlsl

ShaderCompiler.exe /Tvs_4_0 VS_SSR.hlsl
ShaderCompiler.exe /Tps_4_0 PS_SSR.hlsl

ShaderCompiler.exe /Tvs_4_0 VS_DoF.hlsl
ShaderCompiler.exe /Tps_4_0 PS_DoF.hlsl

ShaderCompiler.exe /Tvs_4_0 VS_BlackOut.hlsl
ShaderCompiler.exe /Tps_4_0 PS_BlackOut.hlsl

ShaderCompiler.exe /Tvs_4_0 VS_FXAA.hlsl
ShaderCompiler.exe /Tps_4_0 PS_FXAA.hlsl

ShaderCompiler.exe /Tvs_4_0 VS_GodRay.hlsl
ShaderCompiler.exe /Tps_4_0 PS_GodRay.hlsl

pause

cd %NowDir%