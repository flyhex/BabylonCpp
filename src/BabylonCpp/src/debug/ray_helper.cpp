#include <babylon/debug/ray_helper.h>

#include <babylon/engines/scene.h>
#include <babylon/meshes/abstract_mesh.h>
#include <babylon/meshes/lines_mesh.h>
#include <babylon/meshes/mesh.h>

namespace BABYLON {

std::unique_ptr<RayHelper> RayHelper::CreateAndShow(const Ray& ray, Scene* scene,
                                                    const Color3& color)
{
  auto helper = std::make_unique<RayHelper>(ray);

  helper->show(scene, color);

  return helper;
}

RayHelper::RayHelper(const Ray& iRay)
    : ray{std::make_unique<Ray>(iRay)}
    , _renderLine{nullptr}
    , _renderFunction{nullptr}
    , _scene{nullptr}
    , _onAfterRenderObserver{nullptr}
    , _onAfterStepObserver{nullptr}
    , _attachedToMesh{nullptr}
    , _meshSpaceDirection{Vector3(0.f, 0.f, -1.f)}
{
}

RayHelper::~RayHelper() = default;

void RayHelper::show(Scene* scene)
{
  if (!_renderFunction && ray) {
    _renderFunction         = [this](Scene*, EventState&) { _render(); };
    _scene                  = scene;
    _renderPoints           = {ray->origin, ray->origin.add(ray->direction.scale(ray->length))};
    _renderLine             = Mesh::CreateLines("ray", _renderPoints, scene, true);
    _renderLine->isPickable = false;

    if (_renderFunction) {
      _scene->registerBeforeRender(_renderFunction);
    }
  }
}

void RayHelper::show(Scene* scene, const Color3& color)
{
  show(scene);

  if (_renderLine) {
    _renderLine->color.copyFrom(color);
  }
}

void RayHelper::hide()
{
  if (_renderFunction && _scene) {
    _scene->unregisterBeforeRender(_renderFunction);
    _scene          = nullptr;
    _renderFunction = nullptr;
    if (_renderLine) {
      _renderLine->dispose();
      _renderLine = nullptr;
    }
    _renderPoints.clear();
  }
}

void RayHelper::_render()
{
  if (!ray) {
    return;
  }

  auto& point    = _renderPoints[1];
  const auto len = std::min(ray->length, 1000000.f);

  point.copyFrom(ray->direction);
  point.scaleInPlace(len);
  point.addInPlace(ray->origin);

  _renderPoints[0].copyFrom(ray->origin);

  Mesh::CreateLines("ray", _renderPoints, _scene, true, _renderLine);
}

void RayHelper::attachToMesh(const AbstractMeshPtr& mesh, const Vector3& meshSpaceDirection,
                             const Vector3& meshSpaceOrigin, float length)
{
  _attachedToMesh = mesh;

  if (!ray) {
    return;
  }

  if (length > 0.f) {
    ray->length = length;
  }

  _meshSpaceDirection = meshSpaceDirection;
  _meshSpaceOrigin    = meshSpaceOrigin;

  if (!_scene) {
    _scene = mesh->getScene();
  }

  if (!_onAfterRenderObserver) {
    _onAfterRenderObserver = _scene->onBeforeRenderObservable.add(
      [this](Scene* /*scene*/, EventState& /*es*/) -> void { _updateToMesh(); });
    _onAfterStepObserver = _scene->onAfterStepObservable.add(
      [this](Scene* /*scene*/, EventState& /*es*/) -> void { _updateToMesh(); });
  }

  // force world matrix computation before the first ray helper computation
  _attachedToMesh->computeWorldMatrix(true);

  _updateToMesh();
}

void RayHelper::detachFromMesh()
{
  if (_attachedToMesh && _scene) {
    if (_onAfterRenderObserver) {
      _scene->onBeforeRenderObservable.remove(_onAfterRenderObserver);
      _scene->onAfterStepObservable.remove(_onAfterStepObserver);
    }
    _attachedToMesh        = nullptr;
    _onAfterRenderObserver = nullptr;
    _onAfterStepObserver   = nullptr;
    _scene                 = nullptr;
  }
}

void RayHelper::_updateToMesh()
{
  if (!_attachedToMesh || !ray) {
    return;
  }

  if (_attachedToMesh->_isDisposed) {
    detachFromMesh();
    return;
  }

  _attachedToMesh->getDirectionToRef(_meshSpaceDirection, ray->direction);
  Vector3::TransformCoordinatesToRef(_meshSpaceOrigin, _attachedToMesh->getWorldMatrix(),
                                     ray->origin);
}

void RayHelper::dispose()
{
  hide();
  detachFromMesh();
  ray = nullptr;
}

} // end of namespace BABYLON
