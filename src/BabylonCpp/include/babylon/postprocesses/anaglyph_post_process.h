#ifndef BABYLON_POSTPROCESSES_ANAGLYPH_POST_PROCESS_H
#define BABYLON_POSTPROCESSES_ANAGLYPH_POST_PROCESS_H

#include <babylon/babylon_api.h>
#include <babylon/babylon_fwd.h>
#include <babylon/postprocesses/post_process.h>

namespace BABYLON {

FWD_CLASS_SPTR(AnaglyphPostProcess)

/**
 * @brief Postprocess used to generate anaglyphic rendering.
 */
class BABYLON_SHARED_EXPORT AnaglyphPostProcess : public PostProcess {

public:
  template <typename... Ts>
  static AnaglyphPostProcessPtr New(Ts&&... args)
  {
    auto postProcess
      = std::shared_ptr<AnaglyphPostProcess>(new AnaglyphPostProcess(std::forward<Ts>(args)...));
    postProcess->add(postProcess);

    return postProcess;
  }
  ~AnaglyphPostProcess() override; // = default

protected:
  /**
   * @brief Creates a new AnaglyphPostProcess.
   * @param name defines postprocess name
   * @param options defines creation options or target ratio scale
   * @param rigCameras defines cameras using this postprocess
   * @param samplingMode defines required sampling mode (BABYLON.Texture.NEAREST_SAMPLINGMODE by
   * default)
   * @param engine defines hosting engine
   * @param reusable defines if the postprocess will be reused multiple times per frame
   */
  AnaglyphPostProcess(const std::string& name,
                      const std::variant<float, PostProcessOptions>& options,
                      const std::vector<CameraPtr>& rigCameras,
                      const std::optional<unsigned int>& samplingMode = std::nullopt,
                      Engine* engine = nullptr, bool reusable = false);

  /**
   * @brief Gets a string identifying the name of the class.
   * @returns "AnaglyphPostProcess" string
   */
  std::string getClassName() const override;

private:
  PostProcessPtr _passedProcess;

}; // end of class AnaglyphPostProcess

} // end of namespace BABYLON

#endif // end of BABYLON_POSTPROCESSES_ANAGLYPH_POST_PROCESS_H
