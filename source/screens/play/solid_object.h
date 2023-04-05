#ifndef _solid_object_
#define _solid_object_

#include "framework.h"
#include "play_event.h"
#include "level_render_object.h"
#include "object_outline.h"

class solid_object
{
private:
  struct object_concept
  {
    virtual ~object_concept() {}
    [[nodiscard]] virtual auto clone() -> std::unique_ptr<object_concept> = 0;
    virtual auto Update(int64_t clockFrequency, int64_t clockCount, play_event_inserter playEventInserter) -> void = 0;
    [[nodiscard]] virtual auto HasCollided(float x, float y) const -> bool = 0;
    virtual auto HitByBullet() -> void = 0;
    [[nodiscard]] virtual auto LevelIsComplete() const -> bool = 0;
    virtual auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void = 0;
    [[nodiscard]] virtual auto GetOutline() -> object_outline = 0;
    [[nodiscard]] virtual auto HasCollidedWith(const object_outline& outline) const -> bool = 0;
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

    [[nodiscard]] auto HasCollided(float x, float y) const -> bool override
    {
      return object.HasCollided(x, y);
    }

    auto HitByBullet() -> void override
    {
      object.HitByBullet();
    }

    [[nodiscard]] auto LevelIsComplete() const -> bool override
    {
      return object.LevelIsComplete();
    }

    auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void override
    {
      object.RenderTo(renderTarget, viewRect);
    }

    [[nodiscard]] auto GetOutline() -> object_outline override
    {
      return object.GetOutline();
    }

    [[nodiscard]] auto HasCollidedWith(const object_outline& outline) const -> bool override
    {
      return false;
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

  [[nodiscard]] auto HasCollided(float x, float y) const -> bool
  {
    return objectConcept->HasCollided(x, y);
  }

  auto HitByBullet() -> void
  {
    objectConcept->HitByBullet();
  }

  [[nodiscard]] auto LevelIsComplete() const -> bool
  {
    return objectConcept->LevelIsComplete();
  }

  auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
  {
    objectConcept->RenderTo(renderTarget, viewRect);
  }

  [[nodiscard]] auto GetOutline() -> object_outline
  {
    return objectConcept->GetOutline();   
  }

  [[nodiscard]] auto HasCollidedWith(const object_outline& outline) const -> bool
  {
    return objectConcept->HasCollidedWith(outline);
  }

private:
  std::unique_ptr<object_concept> objectConcept;
};

#endif
