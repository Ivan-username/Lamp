
#pragma once

enum class EventType : uint8_t
{
  NONE = 0,

  POWER_CHANGE,

  // Effects
  EFF_CHANGE,
  EFF_SET_BRIGHTNESS,
  EFF_SET_SPEED,
  EFF_SET_SCALE,

  // WiFi
  WIFI_CONNECTED,
  WIFI_CONNECTING,
  WIFI_DISCONNECTED,
  WIFI_UPDATE,
};

struct Event
{
  EventType type = EventType::NONE;
  int16_t int16Param = 0;

  const char *charsParam = nullptr;
  void *ptr = nullptr;

  // factory methods

  static Event ev(EventType t)
  {
    return Event{
        .type = t,
    };
  }

  static Event evInt16(EventType t, const int16_t &value)
  {
    return Event{
        .type = t,
        .int16Param = value,
    };
  }

  static Event evChars(EventType t, const char *cstr)
  {
    return Event{
        .type = t,
        .charsParam = cstr,
    };
  }

  static Event evPtr(EventType t, void *p)
  {
    return Event{
        .type = t,
        .ptr = p,
    };
  }
};

// ring buffer event queue
class EventQueue
{
public:
  static const uint8_t QUEUE_SIZE = 32;

  bool post(const Event &e)
  {
    uint8_t next = (writeIndex + 1) % QUEUE_SIZE;

    if (next == readIndex)
      return false; // full

    queue[writeIndex] = e;
    writeIndex = next;
    return true;
  }

  bool poll(Event &out)
  {
    if (readIndex == writeIndex)
      return false; // empty

    out = queue[readIndex];
    readIndex = (readIndex + 1) % QUEUE_SIZE;
    return true;
  }

  bool peek(Event &out) const
  {
    if (readIndex == writeIndex)
      return false; // empty

    out = queue[readIndex];
    return true;
  }

private:
  Event queue[QUEUE_SIZE];
  uint8_t writeIndex = 0;
  uint8_t readIndex = 0;
};
