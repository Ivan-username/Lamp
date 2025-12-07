

#pragma once
#include <vector>
#include <functional>

struct Event
{
  enum Type
  {
    TogglePower,
    SetEffect,
    NextEffect,
    PrevEffect,
    SetBrightness,
    SetSpeed,
    SetScale,
    StateChanged,
    WiFiSTAUpdated,
    WiFiAPUpdated,
    RebootRequested
  } type;

  int intValue = 0;     // универсальное поле
  String strValue = ""; // при необходимости
};

using EventHandler = std::function<void(const Event &)>;

class EventBus
{
public:
  void subscribe(EventHandler handler)
  {
    handlers.push_back(handler);
  }

  void publish(const Event &ev)
  {
    for (auto &h : handlers)
      h(ev);
  }

private:
  std::vector<EventHandler> handlers;
};