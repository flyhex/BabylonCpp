﻿#ifndef BABYLON_SHADERS_SHADERS_INCLUDE_PBR_FUNCTIONS_FX_H
#define BABYLON_SHADERS_SHADERS_INCLUDE_PBR_FUNCTIONS_FX_H

namespace BABYLON {

extern const char* pbrFunctions;

const char* pbrFunctions
  = "// Constants\n"
    "#define RECIPROCAL_PI2 0.15915494\n"
    "#define FRESNEL_MAXIMUM_ON_ROUGH 0.25\n"
    "\n"
    "// PBR CUSTOM CONSTANTS\n"
    "const float kRougnhessToAlphaScale = 0.1;\n"
    "const float kRougnhessToAlphaOffset = 0.29248125;\n"
    "\n"
    "float convertRoughnessToAverageSlope(float roughness)\n"
    "{\n"
    "  // Calculate AlphaG as square of roughness; add epsilon to avoid numerical issues\n"
    "  const float kMinimumVariance = 0.0005;\n"
    "  float alphaG = square(roughness) + kMinimumVariance;\n"
    "  return alphaG;\n"
    "}\n"
    "\n"
    "// From Microfacet Models for Refraction through Rough Surfaces, Walter et al. 2007\n"
    "float smithVisibilityG1_TrowbridgeReitzGGX(float dot, float alphaG)\n"
    "{\n"
    "  float tanSquared = (1.0 - dot * dot) / (dot * dot);\n"
    "  return 2.0 / (1.0 + sqrt(1.0 + alphaG * alphaG * tanSquared));\n"
    "}\n"
    "\n"
    "float smithVisibilityG_TrowbridgeReitzGGX_Walter(float NdotL, float NdotV, float alphaG)\n"
    "{\n"
    "  return smithVisibilityG1_TrowbridgeReitzGGX(NdotL, alphaG) * smithVisibilityG1_TrowbridgeReitzGGX(NdotV, alphaG);\n"
    "}\n"
    "\n"
    "// Trowbridge-Reitz (GGX)\n"
    "// Generalised Trowbridge-Reitz with gamma power=2.0\n"
    "float normalDistributionFunction_TrowbridgeReitzGGX(float NdotH, float alphaG)\n"
    "{\n"
    "  // Note: alphaG is average slope (gradient) of the normals in slope-space.\n"
    "  // It is also the (trigonometric) tangent of the median distribution value, i.e. 50% of normals have\n"
    "  // a tangent (gradient) closer to the macrosurface than this slope.\n"
    "  float a2 = square(alphaG);\n"
    "  float d = NdotH * NdotH * (a2 - 1.0) + 1.0;\n"
    "  return a2 / (PI * d * d);\n"
    "}\n"
    "\n"
    "vec3 fresnelSchlickGGX(float VdotH, vec3 reflectance0, vec3 reflectance90)\n"
    "{\n"
    "  return reflectance0 + (reflectance90 - reflectance0) * pow(clamp(1.0 - VdotH, 0., 1.), 5.0);\n"
    "}\n"
    "\n"
    "vec3 fresnelSchlickEnvironmentGGX(float VdotN, vec3 reflectance0, vec3 reflectance90, float smoothness)\n"
    "{\n"
    "  // Schlick fresnel approximation, extended with basic smoothness term so that rough surfaces do not approach reflectance90 at grazing angle\n"
    "  float weight = mix(FRESNEL_MAXIMUM_ON_ROUGH, 1.0, smoothness);\n"
    "  return reflectance0 + weight * (reflectance90 - reflectance0) * pow(clamp(1.0 - VdotN, 0., 1.), 5.0);\n"
    "}\n"
    "\n"
    "// Cook Torance Specular computation.\n"
    "vec3 computeSpecularTerm(float NdotH, float NdotL, float NdotV, float VdotH, float roughness, vec3 reflectance0, vec3 reflectance90)\n"
    "{\n"
    "  float alphaG = convertRoughnessToAverageSlope(roughness);\n"
    "  float distribution = normalDistributionFunction_TrowbridgeReitzGGX(NdotH, alphaG);\n"
    "  float visibility = smithVisibilityG_TrowbridgeReitzGGX_Walter(NdotL, NdotV, alphaG);\n"
    "  visibility /= (4.0 * NdotL * NdotV); // Cook Torance Denominator  integated in viibility to avoid issues when visibility function changes.\n"
    "  float specTerm = max(0., visibility * distribution) * NdotL;\n"
    "\n"
    "  vec3 fresnel = fresnelSchlickGGX(VdotH, reflectance0, reflectance90);\n"
    "  return fresnel * specTerm;\n"
    "}\n"
    "\n"
    "float computeDiffuseTerm(float NdotL, float NdotV, float VdotH, float roughness)\n"
    "{\n"
    "  // Diffuse fresnel falloff as per Disney principled BRDF, and in the spirit of\n"
    "  // of general coupled diffuse/specular models e.g. Ashikhmin Shirley.\n"
    "  float diffuseFresnelNV = pow(clamp(1.0 - NdotL, 0.000001, 1.), 5.0);\n"
    "  float diffuseFresnelNL = pow(clamp(1.0 - NdotV, 0.000001, 1.), 5.0);\n"
    "  float diffuseFresnel90 = 0.5 + 2.0 * VdotH * VdotH * roughness;\n"
    "  float fresnel =\n"
    "  (1.0 + (diffuseFresnel90 - 1.0) * diffuseFresnelNL) *\n"
    "  (1.0 + (diffuseFresnel90 - 1.0) * diffuseFresnelNV);\n"
    "\n"
    "  return fresnel * NdotL / PI;\n"
    "}\n"
    "\n"
    "float adjustRoughnessFromLightProperties(float roughness, float lightRadius, float lightDistance)\n"
    "{\n"
    "  #ifdef USEPHYSICALLIGHTFALLOFF\n"
    "  // At small angle this approximation works. \n"
    "  float lightRoughness = lightRadius / lightDistance;\n"
    "  // Distribution can sum.\n"
    "  float totalRoughness = clamp(lightRoughness + roughness, 0., 1.);\n"
    "  return totalRoughness;\n"
    "  #else\n"
    "  return roughness;\n"
    "  #endif\n"
    "}\n"
    "\n"
    "float computeDefaultMicroSurface(float microSurface, vec3 reflectivityColor)\n"
    "{\n"
    "  const float kReflectivityNoAlphaWorkflow_SmoothnessMax = 0.95;\n"
    "\n"
    "  float reflectivityLuminance = getLuminance(reflectivityColor);\n"
    "  float reflectivityLuma = sqrt(reflectivityLuminance);\n"
    "  microSurface = reflectivityLuma * kReflectivityNoAlphaWorkflow_SmoothnessMax;\n"
    "\n"
    "  return microSurface;\n"
    "}\n"
    "\n"
    "// For typical incident reflectance range (between 4% to 100%) set the grazing reflectance to 100% for typical fresnel effect.\n"
    "// For very low reflectance range on highly diffuse objects (below 4%), incrementally reduce grazing reflecance to 0%.\n"
    "float fresnelGrazingReflectance(float reflectance0) {\n"
    "  float reflectance90 = clamp(reflectance0 * 25.0, 0.0, 1.0);\n"
    "  return reflectance90;\n"
    "}\n"
    "\n"
    "// To enable 8 bit textures to be used we need to pack and unpack the LOD\n"
    "//inverse alpha is used to work around low-alpha bugs in Edge and Firefox\n"
    "#define UNPACK_LOD(x) (1.0 - x) * 255.0\n"
    "\n"
    "float getLodFromAlphaG(float cubeMapDimensionPixels, float alphaG, float NdotV) {\n"
    "  float microsurfaceAverageSlope = alphaG;\n"
    "\n"
    "  // Compensate for solid angle change between half-vector measure (Blinn-Phong) and reflected-vector measure (Phong):\n"
    "  //  dWr = 4*cos(theta)*dWh,\n"
    "  // where dWr = solid angle (delta omega) in environment incident radiance (reflection-vector) measure;\n"
    "  // where dWh = solid angle (delta omega) in microfacet normal (half-vector) measure;\n"
    "  // so the relationship is proportional to cosine theta = NdotV.\n"
    "  // The constant factor of four is handled elsewhere as part of the scale/offset filter parameters.\n"
    "  microsurfaceAverageSlope *= sqrt(abs(NdotV));\n"
    "\n"
    "  float microsurfaceAverageSlopeTexels = microsurfaceAverageSlope * cubeMapDimensionPixels;\n"
    "  float lod = log2(microsurfaceAverageSlopeTexels);\n"
    "  return lod;\n"
    "}\n"
    "\n"
    "float environmentRadianceOcclusion(float ambientOcclusion, float NdotVUnclamped) {\n"
    "  // Best balanced (implementation time vs result vs perf) analytical environment specular occlusion found.\n"
    "  // http://research.tri-ace.com/Data/cedec2011_RealtimePBR_Implementation_e.pptx\n"
    "  float temp = NdotVUnclamped + ambientOcclusion;\n"
    "  return clamp(square(temp) - 1.0 + ambientOcclusion, 0.0, 1.0);\n"
    "}\n"
    "\n"
    "float environmentHorizonOcclusion(vec3 reflection, vec3 normal) {\n"
    "  // http://marmosetco.tumblr.com/post/81245981087\n"
    "  float temp = clamp( 1.0 + 1.1 * dot(reflection, normal), 0.0, 1.0);\n"
    "  return square(temp);\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_SHADERS_INCLUDE_PBR_FUNCTIONS_FX_H
