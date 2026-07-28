#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include <esp_gap_ble_api.h>

namespace esphome {
namespace ble_button_advertiser {

class BLEButtonAdvertiser : public Component {
 public:
  void setup() override;
  void dump_config() override;
  
  void set_button_pin(GPIOPin *pin) { button_pin_ = pin; }
  void advertise_press();

 private:
  GPIOPin *button_pin_{nullptr};
  
  // BLE advertisement parameters
  esp_ble_adv_data_t adv_data_{};
  esp_ble_adv_params_t adv_params_{};
  
  // BLE event callback
  static void gap_event_handler(esp_gap_ble_cb_event_t event, 
                                 esp_ble_gap_cb_param_t *param);
};

}  // namespace ble_button_advertiser
}  // namespace esphome
