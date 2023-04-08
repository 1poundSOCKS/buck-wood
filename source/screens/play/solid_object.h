#ifndef _solid_object_
#define _solid_object_

#include "framework.h"
#include "play_event.h"
#include "level_render_object.h"
#include "collision_data.h"
#include "collision_effect.h"

class solid_object
{
private:
  struct object_concept
  {
    virtual ~object_concept() {}
    [[nodiscard]] virtual auto clone() -> std::unique_ptr<object_concept> = 0;
    virtual auto Update(int64_t clockFrequency, int64_t clockCount, play_event_inserter playEventInserter) -> void = 0;
    [[nodiscard]] virtual auto LevelIsComplete() const -> bool = 0;
    virtual auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void = 0;
    [[nodiscard]] virtual auto GetCollisionData() -> collision_data = 0;
    [[nodiscard]] virtual auto HasCollidedWith(const collision_data& collisionData) const -> bool = 0;
    [[nodiscard]] virtual auto GetCollisionEffect() const -> collision_effect = 0;
    virtual auto ApplyCollisionEffect(const collision_effect& effect) -> void = 0;
    [[nodiscard]] virtual auto Destroyed() const -> bool = 0;
  };

  template <typename object_type>
  struct object_model : public object_concept
  {
    object_model(const object_type& objectIn) : object(std::forward<const object_type>(objectIn)) {}

    [[nodiscard]] auto clone() -> std::unique_ptr<object_concept> override
    {
      return std::make_unique<object_model<object_type>>(*this);
    }

    auto Update(int64_t clockFrequency, int64_t clockCount, play_event_inserter playEventInserter) -> void
    {
      object.Update(clockFrequency, clockCount, playEventInserter);
    }

    [[nodiscard]] auto LevelIsComplete() const -> bool override
    {
      return object.LevelIsComplete();
    }

    auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void override
    {
      object.RenderTo(renderTarget, viewRect);
    }

    [[nodiscard]] auto GetCollisionData() -> collision_data override
    {
      return object.GetCollisionData();
    }

    [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool override
    {
      return object.HasCollidedWith(collisionData);
    }

    [[nodiscard]] auto GetCollisionEffect() const -> collision_effect override
    {
      return object.GetCollisionEffect();
    }

    auto ApplyCollisionEffect(const collision_effect& effect) -> void override
    {
      object.ApplyCollisionEffect(effect);
    }

    [[nodiscard]] auto Destroyed() const -> bool override
    {
      return object.Destroyed();
    }

    object_type object;
  };

public:
  template <typename object_type>
  solid_object(const object_type& object)
  : objectConcept(std::make_unique<object_model<object_type>>(object))
  {
  }

  solid_object(solid_object&& solidObject)
  : objectConcept(std::move(solidObject.objectConcept))
  {
  }

  void operator=(solid_object&& solidObject)
  {
    objectConcept = std::move(solidObject.objectConcept);
  }

  solid_object(const solid_object& solidObject) = delete;
  void operator=(const solid_object& solidObject) = delete;

  auto Update(int64_t clockFrequency, int64_t clockCount, play_event_inserter playEventInserter) -> void
  {
    objectConcept->Update(clockFrequency, clockCount, playEventInserter);
  }

  [[nodiscard]] auto LevelIsComplete() const -> bool
  {
    return objectConcept->LevelIsComplete();
  }

  auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
  {
    objectConcept->RenderTo(renderTarget, viewRect);
  }

  [[nodiscard]] auto GetCollisionData() -> collision_data
  {
    return objectConcept->GetCollisionData();
  }

  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool
  {
    return objectConcept->HasCollidedWith(collisionData);
  }

  [[nodiscard]] virtual auto GetCollisionEffect() const -> collision_effect
  {
    return objectConcept->GetCollisionEffect();
  }

  auto ApplyCollisionEffect(const collision_effect& effect) -> void
  {
    objectConcept->ApplyCollisionEffect(effect);
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    return objectConcept->Destroyed();
  }

private:
  std::unique_ptr<object_concept> objectConcept;
};

#endif
