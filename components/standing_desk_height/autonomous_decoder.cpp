#include "autonomous_decoder.h"
#include "esphome.h"

namespace esphome {
namespace standing_desk_height {

// Implementation based off of: https://github.com/rmcgibbo/Jarvis
// Which, despite the name, works for Uplift desks too
bool AutonomousDecoder::put(uint8_t b) {
  ESP_LOGD("AutonomousDecoder::put", "put called"); 
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
      state_ = PARAM1;
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
    ESP_LOGD("AutonomousDecoder::put", "HEIGHT1"); 
    //if (b == 0x00 || b == 0x01) {
    buf_[0] = b;

    // ESP_LOGD("AutonomousDecoder::put-buf_[0]", "put-buf_[0] %02x",  buf_[0]); 
    ESP_LOGD("AutonomousDecoder::put-buf_[0]", "put-buf_[0] %010ld",  buf_[0]); 
    
    state_ = HEIGHT2;
    return false;
    //} else {
    //  state_ = SYNC1;
    //  return false;
    //}
  case HEIGHT2:
    ESP_LOGD("AutonomousDecoder::put", "HEIGHT2"); 
    buf_[1] = b;
    state_ = SYNC1;
    ESP_LOGD("AutonomousDecoder::put-buf_[1]", "put-buf_[1] %010ld",  (buf_[1] << 8) ); 
    return true;
  default:
    return false;
  }
  return false;
}

float AutonomousDecoder::decode() {
  ESP_LOGD("AutonomousDecoder::decode", "decode called"); 
  return (buf_[1] & 0xFF);
}

}
}