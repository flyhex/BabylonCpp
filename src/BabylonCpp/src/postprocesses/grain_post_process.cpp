#include <babylon/postprocesses/grain_post_process.h>

#include <babylon/core/random.h>
#include <babylon/materials/effect.h>

namespace BABYLON {

GrainPostProcess::GrainPostProcess(const std::string& iName,
                                   const std::variant<float, PostProcessOptions>& options,
                                   const CameraPtr& camera,
                                   const std::optional<unsigned int>& samplingMode, Engine* engine,
                                   bool reusable, unsigned int textureType, bool blockCompilation)
    : PostProcess{iName,        "grain",         {"intensity", "animatedSeed"},
                  {},           options,         camera,
                  samplingMode, engine,          reusable,
                  "",           textureType,     "",
                  {},           blockCompilation}
    , intensity{30.f}
    , animated{false}
{
  onApplyObservable.add([&](Effect* effect, EventState& /*es*/) {
    effect->setFloat("intensity", intensity);
    effect->setFloat("animatedSeed", animated ? Math::random() + 1.f : 1.f);
  });
}

GrainPostProcess::~GrainPostProcess() = default;

std::string GrainPostProcess::getClassName() const
{
  return "GrainPostProcess";
}

GrainPostProcessPtr GrainPostProcess::_Parse(const json& /*parsedPostProcess*/,
                                             const CameraPtr& /*targetCamera*/, Scene* /*scene*/,
                                             const std::string& /*rootUrl*/)
{
  return nullptr;
}

} // end of namespace BABYLON
