#ifndef BABYLON_SPRITES_SPRITE_H
#define BABYLON_SPRITES_SPRITE_H

#include <babylon/babylon_api.h>
#include <babylon/babylon_fwd.h>

#include <babylon/animations/animation.h>
#include <babylon/animations/ianimatable.h>
#include <babylon/maths/color4.h>
#include <babylon/maths/vector3.h>
#include <babylon/misc/observable.h>
#include <babylon/sprites/thin_sprite.h>

namespace BABYLON {

class ActionManager;
FWD_CLASS_SPTR(ISpriteManager)
FWD_CLASS_SPTR(Sprite)

/**
 * @brief Class used to represent a sprite.
 * @see https://doc.babylonjs.com/babylon101/sprites
 */
class BABYLON_SHARED_EXPORT Sprite : public ThinSprite, public IAnimatable {

public:
  template <typename... Ts>
  static SpritePtr New(Ts&&... args)
  {
    auto sprite = std::shared_ptr<Sprite>(new Sprite(std::forward<Ts>(args)...));
    sprite->addToSpriteManager(sprite);

    return sprite;
  }
  virtual ~Sprite(); // = default

  /**
   * @brief Hidden
   */
  void addToSpriteManager(const SpritePtr& newSprite);

  /**
   * @brief Hidden
   */
  Type type() const override;

  /**
   * @brief Returns the string "Sprite".
   * @returns "Sprite"
   */
  std::string getClassName() const;

  /**
   * @brief Starts an animation.
   * @param from defines the initial key
   * @param to defines the end key
   * @param loop defines if the animation must loop
   * @param delay defines the start delay (in ms)
   * @param onAnimationEnd defines a callback to call when animation ends
   */
  void playAnimation(int from, int to, bool loop, float delay = 1.f,
                     const std::function<void()>& onAnimationEnd = nullptr);

  /**
   * @brief Release associated resources.
   */
  void dispose();

  /**
   * @brief Serializes the sprite to a JSON object.
   * @returns the JSON object
   */
  json serialize() const;

  /**
   * @brief Parses a JSON object to create a new sprite.
   * @param parsedSprite The JSON object to parse
   * @param manager defines the hosting manager
   * @returns the new sprite
   */
  static SpritePtr Parse(const json& parsedSprite, const ISpriteManagerPtr& manager);

protected:
  /**
   * @brief Creates a new Sprite.
   * @param name defines the name
   * @param manager defines the manager
   */
  Sprite(const std::string& name, const ISpriteManagerPtr& manager);

  /**
   * @brief Gets the sprite size.
   */
  float get_size() const;

  /**
   * @brief Sets the sprite size.
   */
  void set_size(float value);

  /**
   * @brief Gets the manager of this sprite.
   */
  ISpriteManagerPtr& get_manager();

  /**
   * @brief Gets the initial key for the animation (setting it will restart the animation).
   */
  int get_fromIndex() const override;

  /**
   * @brief Sets the initial key for the animation (setting it will restart the animation).
   */
  void set_fromIndex(int value) override;

  /**
   * @brief Gets the end key for the animation (setting it will restart the animation).
   */
  int get_toIndex() const override;

  /**
   * @brief Sets the end key for the animation (setting it will restart the animation).
   */
  void set_toIndex(int value) override;

  /**
   * @brief Gets a boolean indicating if the animation is looping (setting it will restart the
   * animation).
   */
  bool get_loopAnimation() const override;

  /**
   * @brief Sets a boolean indicating if the animation is looping (setting it will restart the
   * animation).
   */
  void set_loopAnimation(bool value) override;

  /**
   * @brief Gets the delay between cell changes (setting it will restart the animation).
   */
  float get_delay() const override;

  /**
   * @brief Sets the delay between cell changes (setting it will restart the animation).
   */
  void set_delay(float value) override;

private:
  void _endAnimation();

public:
  /**
   * Defines the name
   */
  std::string name;

  /**
   * Gets or sets the current world position
   */
  Vector3 position;

  /**
   * Gets or sets the main color
   */
  std::optional<Color4> color;

  /**
   * Gets or sets a boolean indicating that this sprite should be disposed after
   * animation ends
   */
  bool disposeWhenFinishedAnimating;

  /**
   * Gets the list of attached animations
   */
  std::vector<Animation*> animations;

  /**
   * Gets or sets a boolean indicating if the sprite can be picked
   */
  bool isPickable;

  /**
   * Gets or sets a boolean indicating that sprite texture alpha will be used
   * for precise picking (false by default).
   */
  bool useAlphaForPicking;

  /**
   * Gets or sets the associated action manager
   */
  ActionManager* actionManager;

  /**
   * An event triggered when the control has been disposed
   */
  Observable<Sprite> onDisposeObservable;

  /**
   * Gets or sets the sprite size
   */
  Property<Sprite, float> size;

  /**
   * Gets or sets the unique id of the sprite
   */
  size_t uniqueId;

  /**
   * Gets the manager of this sprite
   */
  ReadOnlyProperty<Sprite, ISpriteManagerPtr> manager;

private:
  ISpriteManagerPtr _manager;
  std::function<void()> _onAnimationEnd;

}; // end of class Sprite

} // end of namespace BABYLON

#endif // end of BABYLON_SPRITES_SPRITE_H
