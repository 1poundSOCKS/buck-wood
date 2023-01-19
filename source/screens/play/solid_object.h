#ifndef _solid_object_
#define _solid_object_

#include "framework.h"

class solid_object
{
private:
  struct object_concept
  {
    virtual ~object_concept() {}
    [[nodiscard]] virtual auto clone() -> std::unique_ptr<object_concept> = 0;
    [[nodiscard]] virtual auto HasCollided(float x, float y) const -> bool = 0;
    virtual auto HitByBullet() -> void = 0;
    virtual auto GetRenderLines(render_line_inserter_type inserter) const -> void = 0;
    virtual auto LevelIsComplete() const -> bool = 0;
  };

  template <typename object_type>
  struct object_model : public object_concept
  {
    object_model(const object_type& objectIn) : object(std::forward<const object_type>(objectIn)) {}

    [[nodiscard]] auto clone() -> std::unique_ptr<object_concept> override
    {
      return std::make_unique<object_model<object_type>>(*this);
    }

    [[nodiscard]] auto HasCollided(float x, float y) const -> bool override
    {
      return ::HasCollided(object, x, y);
    }

    auto HitByBullet() -> void override
    {
      ::HitByBullet(object);
    }

    auto GetRenderLines(render_line_inserter_type inserter) const -> void override
    {
      ::GetRenderLines(object, inserter);
    }

    auto LevelIsComplete() const -> bool override
    {
      return ::LevelIsComplete(object);
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

  [[nodiscard]] auto HasCollided(float x, float y) const -> bool
  {
    return objectConcept->HasCollided(x, y);
  }

  void HitByBullet()
  {
    objectConcept->HitByBullet();
  }

  auto GetRenderLines(render_line_inserter_type inserter) const -> void
  {
    objectConcept->GetRenderLines(inserter); 
  }

  auto LevelIsComplete() const -> bool
  {
    return objectConcept->LevelIsComplete();
  }

private:
  std::unique_ptr<object_concept> objectConcept;
};

#endif
