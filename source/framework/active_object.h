#ifndef _active_object_
#define _active_object_

#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"

template <typename collision_data_type, typename collision_effect_type>
class active_object
{
private:
  struct object_concept
  {
    virtual ~object_concept() {}
    [[nodiscard]] virtual auto clone() -> std::unique_ptr<object_concept> = 0;
    // virtual auto Initialize(ID2D1RenderTarget* renderTarget) -> void = 0;
    virtual auto Update(const object_input_data& inputData, int64_t clockCount) -> void = 0;
    virtual auto Render(D2D1_RECT_F viewRect) const -> void = 0;
    [[nodiscard]] virtual auto GetCollisionData() const -> const collision_data_type& = 0;
    [[nodiscard]] virtual auto HasCollidedWith(const collision_data_type& collisionData) const -> bool = 0;
    [[nodiscard]] virtual auto GetCollisionEffect() const -> collision_effect_type = 0;
    virtual auto ApplyCollisionEffect(const collision_effect_type& effect) -> void = 0;
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

    // auto Initialize(ID2D1RenderTarget* renderTarget) -> void override
    // {
    //   object.Initialize(renderTarget);
    // }

    auto Update(const object_input_data& inputData, int64_t clockCount) -> void override
    {
      object.Update(inputData, clockCount);
    }

    auto Render(D2D1_RECT_F viewRect) const -> void override
    {
      object.Render(viewRect);
    }

    [[nodiscard]] auto GetCollisionData() const -> const collision_data_type& override
    {
      return object.GetCollisionData();
    }

    [[nodiscard]] auto HasCollidedWith(const collision_data_type& collisionData) const -> bool override
    {
      return object.HasCollidedWith(collisionData);
    }

    [[nodiscard]] auto GetCollisionEffect() const -> collision_effect_type override
    {
      return object.GetCollisionEffect();
    }

    auto ApplyCollisionEffect(const collision_effect_type& effect) -> void override
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
  active_object(const object_type& object)
  : objectConcept(std::make_unique<object_model<object_type>>(object))
  {
  }

  active_object(active_object&& solidObject)
  : objectConcept(std::move(solidObject.objectConcept))
  {
  }

  void operator=(active_object&& solidObject)
  {
    objectConcept = std::move(solidObject.objectConcept);
  }

  active_object(const active_object& solidObject) = delete;
  void operator=(const active_object& solidObject) = delete;

  // auto Initialize(ID2D1RenderTarget* renderTarget) -> void
  // {
  //   objectConcept->Initialize(renderTarget);
  // }

  auto Update(const object_input_data& inputData, int64_t clockCount) -> void
  {
    objectConcept->Update(inputData, clockCount);
  }

  auto Render(D2D1_RECT_F viewRect) const -> void
  {
    objectConcept->Render(viewRect);
  }

  [[nodiscard]] auto GetCollisionData() const -> const collision_data_type&
  {
    return objectConcept->GetCollisionData();
  }

  [[nodiscard]] auto HasCollidedWith(const collision_data_type& collisionData) const -> bool
  {
    return objectConcept->HasCollidedWith(collisionData);
  }

  [[nodiscard]] virtual auto GetCollisionEffect() const -> collision_effect_type
  {
    return objectConcept->GetCollisionEffect();
  }

  auto ApplyCollisionEffect(const collision_effect_type& effect) -> void
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
