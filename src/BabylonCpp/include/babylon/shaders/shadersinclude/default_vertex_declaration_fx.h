﻿#ifndef BABYLON_SHADERS_SHADERS_INCLUDE_DEFAULT_VERTEX_DECLARATION_FX_H
#define BABYLON_SHADERS_SHADERS_INCLUDE_DEFAULT_VERTEX_DECLARATION_FX_H

namespace BABYLON {

extern const char* defaultVertexDeclaration;

const char* defaultVertexDeclaration
  = R"ShaderCode(

// Uniforms
uniform mat4 viewProjection;
uniform mat4 view;

#ifdef DIFFUSE
uniform mat4 diffuseMatrix;
uniform vec2 vDiffuseInfos;
#endif

#ifdef AMBIENT
uniform mat4 ambientMatrix;
uniform vec2 vAmbientInfos;
#endif

#ifdef OPACITY
uniform mat4 opacityMatrix;
uniform vec2 vOpacityInfos;
#endif

#ifdef EMISSIVE
uniform vec2 vEmissiveInfos;
uniform mat4 emissiveMatrix;
#endif

#ifdef LIGHTMAP
uniform vec2 vLightmapInfos;
uniform mat4 lightmapMatrix;
#endif

#if defined(SPECULAR) && defined(SPECULARTERM)
uniform vec2 vSpecularInfos;
uniform mat4 specularMatrix;
#endif

#ifdef BUMP
uniform vec3 vBumpInfos;
uniform mat4 bumpMatrix;
#endif

#ifdef REFLECTION
uniform mat4 reflectionMatrix;
#endif

#ifdef POINTSIZE
    uniform float pointSize;
#endif

)ShaderCode";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_SHADERS_INCLUDE_DEFAULT_VERTEX_DECLARATION_FX_H
