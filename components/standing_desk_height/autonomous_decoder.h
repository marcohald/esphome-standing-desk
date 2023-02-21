#pragma once

#include <stdint.h>
#include "decoder.h"

namespace esphome {
namespace standing_desk_height {

class AutonomousDecoder : public Decoder {
  protected:
    enum state_t {
      SYNC1,    // waiting for 0x01
      SYNC2,    // waiting for 0x01 (second)
      PARAM1,   // STOPPED=0x00; SAVE_TRIGGED=0x01; MOVING=0x03;
      PARAM2,   // STOPPED=0x00; SAVE_TRIGGED=0x01; MOVING=0x03;
      HEIGHT1,  // waiting for first height arg, usually 0x01 but 0x00 if at the bottom
      HEIGHT2,  // waiting for second height arg
    } state_ = SYNC1;
    uint8_t buf_[2];

  public:
    AutonomousDecoder() { }
    ~AutonomousDecoder() { }

    bool put(uint8_t b);
    float decode();
};

}
}
