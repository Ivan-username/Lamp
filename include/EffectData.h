#pragma once

#include <FileData.h>
#include <LittleFS.h>

enum class EffectId : uint8_t
{
  JUST_LAMP = 0,
  RAINBOW_HORIZONTAL = 1,
  RAINBOW_VERTICAL = 2,

  COUNT
};

struct EffSets
{
  uint8_t brightness = 30;
  uint8_t scale = 30;
  uint8_t speed = 30;
} effSets[static_cast<uint8_t>(EffectId::COUNT)];

FileData effSetsFD(
    &LittleFS,
    "/eff_sets.bin",
    'S',
    &effSets,
    sizeof(effSets),
    10000);