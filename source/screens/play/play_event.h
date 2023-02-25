#ifndef _play_event_
#define _play_event_

#include "framework.h"

class play_event
{
private:
  struct object_concept
  {
    virtual ~object_concept() {}
    [[nodiscard]] virtual auto clone() -> std::unique_ptr<object_concept> = 0;
  };

  template <typename object_type>
  struct object_model : public object_concept
  {
    object_model(const object_type& objectIn) : object(std::forward<const object_type>(objectIn)) {}

    [[nodiscard]] auto clone() -> std::unique_ptr<object_concept> override
    {
      return std::make_unique<object_model<object_type>>(*this);
    }

    object_type object;
  };

public:
  template <typename object_type>
  play_event(const object_type& object)
  : objectConcept(std::make_unique<object_model<object_type>>(object))
  {
  }

  play_event(play_event&& solidObject)
  : objectConcept(std::move(solidObject.objectConcept))
  {
  }

  void operator=(play_event&& solidObject)
  {
    objectConcept = std::move(solidObject.objectConcept);
  }

  play_event(const play_event& solidObject) = delete;
  void operator=(const play_event& solidObject) = delete;

private:
  std::unique_ptr<object_concept> objectConcept;
};

using play_event_inserter = std::back_insert_iterator<std::list<play_event>>;

#endif
