#include "autonomous_decoder.h"

namespace esphome {
namespace standing_desk_height {

// Implementation based off of: https://github.com/rmcgibbo/Jarvis
// Which, despite the name, works for Uplift desks too
bool AutonomousDecoder::put(uint8_t b) {
  switch (state_) {
  case SYNC1:
    if (b == 0x98) {
      state_ = SYNC2;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case SYNC2:
    if (b == 0x98) {
      state_ = SYNC3;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case PARAM1:
    if (b == 0x00 || b == 0x01 || b == 0x03) {
      state_ = PARAM2;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case PARAM2:
    if (b == 0x00 || b == 0x01 || b == 0x03) {
      state_ = HEIGHT1;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case HEIGHT1:
    if (b == 0x00 || b == 0x01) {
      buf_[0] = b;
      state_ = HEIGHT2;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case HEIGHT2:
    buf_[1] = b;
    state_ = SYNC1;
    return true;
  default:
    return false;
  }
  return false;
}

float AutonomousDecoder::decode() {
  return (buf_[1] & 0xFF);
}

}
}