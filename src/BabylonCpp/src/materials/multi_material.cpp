#include <babylon/materials/multi_material.h>

#include <babylon/babylon_stl_util.h>
#include <babylon/core/json.h>
#include <babylon/engine/scene.h>
#include <babylon/materials/standard_material.h>

namespace BABYLON {

MultiMaterial::MultiMaterial(const string_t iName, Scene* scene)
    : Material{iName, scene, true}
{
  // multimaterial is considered like a push material
  storeEffectOnSubMeshes = true;
}

MultiMaterial::~MultiMaterial()
{
}

const string_t MultiMaterial::getClassName() const
{
  return "MultiMaterial";
}

IReflect::Type MultiMaterial::type() const
{
  return IReflect::Type::MULTIMATERIAL;
}

// Properties
vector_t<Material*>& MultiMaterial::subMaterials()
{
  return _subMaterials;
}

void MultiMaterial::setSubMaterials(const vector_t<Material*>& value)
{
  _subMaterials = value;
  _hookArray(value);
}

void MultiMaterial::_hookArray(const vector_t<Material*>& /*array*/)
{
}

Material* MultiMaterial::getSubMaterial(unsigned int index)
{
  if (index >= _subMaterials.size()) {
    return getScene()->defaultMaterial();
  }

  return _subMaterials[index];
}

vector_t<BaseTexture*> MultiMaterial::getActiveTextures() const
{
  auto activeTextures = Material::getActiveTextures();
  for (auto& subMaterial : _subMaterials) {
    stl_util::concat(activeTextures, subMaterial->getActiveTextures());
  }
  return activeTextures;
}

bool MultiMaterial::isReadyForSubMesh(AbstractMesh* mesh, BaseSubMesh* subMesh,
                                      bool useInstances)
{
  for (auto& subMaterial : _subMaterials) {
    if (subMaterial) {
      if (subMaterial->storeEffectOnSubMeshes) {
        if (!subMaterial->isReadyForSubMesh(mesh, subMesh, useInstances)) {
          return false;
        }
        continue;
      }

      if (!subMaterial->isReady(mesh)) {
        return false;
      }
    }
  }

  return true;
}

Material* MultiMaterial::clone(const string_t& iName, bool cloneChildren) const
{
  auto newMultiMaterial = MultiMaterial::New(iName, getScene());

  for (auto& subMaterial : _subMaterials) {
    Material* newSubMaterial = nullptr;
    if (cloneChildren) {
      newSubMaterial = subMaterial->clone(name + "-" + subMaterial->name);
    }
    else {
      newSubMaterial = subMaterial;
    }
    newMultiMaterial->_subMaterials.emplace_back(newSubMaterial);
  }

  return newMultiMaterial;
}

Json::object MultiMaterial::serialize() const
{
  return Json::object();
}

void MultiMaterial::dispose(bool forceDisposeEffect, bool forceDisposeTextures)
{
  auto scene = getScene();
  if (!scene) {
    return;
  }

  // Remove from scene
  scene->multiMaterials.erase(
    ::std::remove_if(scene->multiMaterials.begin(), scene->multiMaterials.end(),
                     [this](const unique_ptr_t<MultiMaterial>& multiMaterial) {
                       return multiMaterial.get() == this;
                     }),
    scene->multiMaterials.end());

  Material::dispose(forceDisposeEffect, forceDisposeTextures);
}

} // end of namespace BABYLON
