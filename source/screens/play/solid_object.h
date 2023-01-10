#ifndef _solid_object_
#define _solid_object_

class solid_object
{
private:
  struct object_concept
  {
    virtual ~object_concept() {}
    virtual [[nodiscard]] auto clone() -> std::unique_ptr<object_concept> = 0;
    virtual [[nodiscard]] auto HasCollided(float x, float y) -> bool = 0;
  };

  template <typename object_type>
  struct object_model : public object_concept
  {
    object_model(const object_type& objectIn) : object(std::forward<const object_type>(objectIn)) {}

    [[nodiscard]] auto clone() -> std::unique_ptr<object_concept> override
    {
      return std::make_unique<object_model<object_type>>(*this);
    }

    [[nodiscard]] auto HasCollided(float x, float y) -> bool override
    {
      return ::HasCollided(x, y, object);
    }

    object_type object;
  };

public:
  template <typename object_type>
  explicit solid_object(const object_type& object)
  : objectConcept(std::make_unique<object_model<object_type>>(object))
  {
  }

  solid_object(const solid_object& solidObject)
  : objectConcept(solidObject.objectConcept->clone())
  {
  }

  virtual [[nodiscard]] auto HasCollided(float x, float y) -> bool
  {
    return objectConcept->HasCollided(x, y);
  }

private:
  std::unique_ptr<object_concept> objectConcept;
};

#endif
