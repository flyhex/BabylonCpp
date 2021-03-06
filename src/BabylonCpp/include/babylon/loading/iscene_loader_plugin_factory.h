#ifndef BABYLON_LOADING_ISCENE_LOADER_PLUGIN_FACTORY_H
#define BABYLON_LOADING_ISCENE_LOADER_PLUGIN_FACTORY_H

#include <string>
#include <variant>

#include <babylon/babylon_api.h>
#include <babylon/babylon_fwd.h>
#include <babylon/loading/iscene_loader_plugin.h>
#include <babylon/loading/iscene_loader_plugin_async.h>

namespace BABYLON {

FWD_STRUCT_SPTR(ISceneLoaderPlugin)
FWD_STRUCT_SPTR(ISceneLoaderPluginAsync)

/**
 * @brief Interface used by SceneLoader plugin factory.
 */
struct BABYLON_SHARED_EXPORT ISceneLoaderPluginFactory {
  virtual ~ISceneLoaderPluginFactory() = default;
  /**
   * Defines the name of the factory
   */
  std::string factoryName;
  /**
   * @brief Function called to create a new plugin.
   * @return the new plugin
   */
  virtual std::variant<ISceneLoaderPluginPtr, ISceneLoaderPluginAsyncPtr> createPlugin() = 0;
  /**
   * Boolean indicating if the plugin can direct load specific data
   */
  std::function<bool(const std::string& data)> factoryCanDirectLoad = nullptr;
}; // end of struct ISceneLoaderPluginFactory

} // end of namespace BABYLON

#endif // end of BABYLON_LOADING_ISCENE_LOADER_PLUGIN_FACTORY_H
