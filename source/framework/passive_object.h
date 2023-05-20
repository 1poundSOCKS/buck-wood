#ifndef _passive_object_
#define _passive_object_

#include "framework.h"
#include "object_input_data.h"
#include "play_event.h"

class passive_object
{
private:

  struct object_concept
  {
    virtual ~object_concept() {}
    [[nodiscard]] virtual auto clone() -> std::unique_ptr<object_concept> = 0;
    // virtual auto Initialize(ID2D1RenderTarget* renderTarget) -> void = 0;
    virtual auto Update(const object_input_data& inputData, int64_t clockCount) -> void = 0;
    virtual auto Render(D2D1_RECT_F viewRect) const -> void = 0;
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

private:
  std::unique_ptr<object_concept> objectConcept;
};

#endif
