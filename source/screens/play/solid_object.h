#ifndef _solid_object_
#define _solid_object_

#include "framework.h"
#include "play_event.h"
#include "level_render_object.h"

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
    virtual auto LevelIsComplete() const -> bool = 0;
    virtual auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void = 0;
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

    auto LevelIsComplete() const -> bool override
    {
      return object.LevelIsComplete();
    }

    virtual auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
    {
      object.RenderTo(renderTarget, viewRect);
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

  void HitByBullet()
  {
    objectConcept->HitByBullet();
  }

  auto LevelIsComplete() const -> bool
  {
    return objectConcept->LevelIsComplete();
  }

  auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
  {
    objectConcept->RenderTo(renderTarget, viewRect);
  }

private:
  std::unique_ptr<object_concept> objectConcept;
};

#endif
