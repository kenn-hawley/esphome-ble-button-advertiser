#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/automation.h"
#include <esp_gap_ble_api.h>

namespace esphome {
namespace ble_button_advertiser {

class BleButtonAdvertiser : public Component {
 public:
  void setup() override;
  void dump_config() override;
  
  void set_pin(GPIOPin *pin) { pin_ = pin; }
  void advertise_press();

 private:
  GPIOPin *pin_{nullptr};
  
  // BLE advertisement parameters
  esp_ble_adv_data_t adv_data_{};
  esp_ble_adv_params_t adv_params_{};
  
  // BLE event callback
  static void gap_event_handler(esp_gap_ble_cb_event_t event, 
                                 esp_ble_gap_cb_param_t *param);
};

template<typename... Ts>
class AdvertisePressAction : public Action<Ts...> {
 public:
  explicit AdvertisePressAction(BleButtonAdvertiser *parent) : parent_(parent) {}
  void play(Ts... x) override { parent_->advertise_press(); }

 protected:
  BleButtonAdvertiser *parent_;
};

}  // namespace ble_button_advertiser
}  // namespace esphome
