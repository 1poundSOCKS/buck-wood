#ifndef _level_render_object_
#define _level_render_object_

class level_render_object
{
private:
  struct object_concept
  {
    virtual ~object_concept() {}
    virtual auto Render() -> void = 0;
  };

  template <typename object_type>
  struct object_model : public object_concept
  {
    object_model(const object_type& objectIn) : object(std::forward<const object_type>(objectIn))
    {
    }

    auto Render() -> void override
    {
      object.Render();
    }

    object_type object;
  };

public:
  template <typename object_type>
  level_render_object(const object_type& object) : objectConcept(std::make_unique<object_model<object_type>>(object))
  {
  }

  auto Render() -> void
  {
    objectConcept->Render();
  }

  std::unique_ptr<object_concept> objectConcept;
};

#endif
