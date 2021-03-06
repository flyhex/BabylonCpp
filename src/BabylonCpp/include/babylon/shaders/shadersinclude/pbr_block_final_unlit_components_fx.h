﻿#ifndef BABYLON_SHADERS_SHADERS_INCLUDE_PBR_BLOCK_FINAL_UNLIT_COMPONENTS_FX_H
#define BABYLON_SHADERS_SHADERS_INCLUDE_PBR_BLOCK_FINAL_UNLIT_COMPONENTS_FX_H

namespace BABYLON {

extern const char* pbrBlockFinalUnlitComponents;

const char* pbrBlockFinalUnlitComponents
  = R"ShaderCode(

// _____________________________ Diffuse ________________________________________
vec3 finalDiffuse = diffuseBase;
finalDiffuse *= surfaceAlbedo.rgb;
finalDiffuse = max(finalDiffuse, 0.0);
finalDiffuse *= vLightingIntensity.x;

// _____________________________ Ambient ________________________________________
vec3 finalAmbient = vAmbientColor;
finalAmbient *= surfaceAlbedo.rgb;

// _____________________________ Emissive ________________________________________
vec3 finalEmissive = vEmissiveColor;
#ifdef EMISSIVE
vec3 emissiveColorTex = texture2D(emissiveSampler, vEmissiveUV + uvOffset).rgb;
finalEmissive *= toLinearSpace(emissiveColorTex.rgb);
finalEmissive *=  vEmissiveInfos.y;
#endif
finalEmissive *= vLightingIntensity.y;

// ______________________________ Ambient ________________________________________
#ifdef AMBIENT
vec3 ambientOcclusionForDirectDiffuse = mix(vec3(1.), aoOut.ambientOcclusionColor, vAmbientInfos.w);
#else
vec3 ambientOcclusionForDirectDiffuse = aoOut.ambientOcclusionColor;
#endif

finalAmbient *= aoOut.ambientOcclusionColor;
finalDiffuse *= ambientOcclusionForDirectDiffuse;

)ShaderCode";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_SHADERS_INCLUDE_PBR_BLOCK_FINAL_UNLIT_COMPONENTS_FX_H
