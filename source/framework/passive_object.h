#ifndef _passive_object_
#define _passive_object_

#include "framework.h"
#include "play_event.h"

class passive_object
{
private:

  struct object_concept
  {
    virtual ~object_concept() {}
    [[nodiscard]] virtual auto clone() -> std::unique_ptr<object_concept> = 0;
    virtual auto Update(int64_t clockFrequency, int64_t clockCount) -> void = 0;
    virtual auto RenderTo(ID2D1RenderTarget* renderTarget) const -> void = 0;
  };

  template <typename object_type>
  struct object_model : public object_concept
  {
    object_model(const object_type& objectIn) : object(std::forward<const object_type>(objectIn)) {}

    [[nodiscard]] auto clone() -> std::unique_ptr<object_concept> override
    {
      return std::make_unique<object_model<object_type>>(*this);
    }

    auto Update(int64_t clockFrequency, int64_t clockCount) -> void
    {
      object.Update(clockFrequency, clockCount);
    }

    auto RenderTo(ID2D1RenderTarget* renderTarget) const -> void override
    {
      object.RenderTo(renderTarget);
    }

    object_type object;
  };

public:

  template <typename object_type>
  passive_object(const object_type& object)
  : objectConcept(std::make_unique<object_model<object_type>>(object))
  {
  }

  passive_object(passive_object&& passiveObject)
  : objectConcept(std::move(passiveObject.objectConcept))
  {
  }

  void operator=(passive_object&& passiveObject)
  {
    objectConcept = std::move(passiveObject.objectConcept);
  }

  passive_object(const passive_object& passiveObject) = delete;
  void operator=(const passive_object& passiveObject) = delete;

  auto Update(int64_t clockFrequency, int64_t clockCount) -> void
  {
    objectConcept->Update(clockFrequency, clockCount);
  }

  auto RenderTo(ID2D1RenderTarget* renderTarget) const -> void
  {
    objectConcept->RenderTo(renderTarget);
  }

private:
  std::unique_ptr<object_concept> objectConcept;
};

#endif
