#include "ble_button_advertiser.h"
#include "esphome/core/log.h"
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>

namespace esphome {
namespace ble_button_advertiser {

static const char *TAG = "ble_button_advertiser";

void BleButtonAdvertiser::setup() {
  ESP_LOGI(TAG, "Initializing BLE Button Advertiser...");
  
  // Initialize BT controller with default config
  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  bt_cfg.controller_task_stack_size = 2048;
  
  if (esp_bt_controller_init(&bt_cfg) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize BT controller");
    return;
  }
  
  // Enable BT controller
  if (esp_bt_controller_enable(ESP_BT_MODE_BLE) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to enable BLE mode");
    return;
  }
  
  // Initialize Bluetooth stack
  if (esp_bluedroid_init() != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize bluedroid");
    return;
  }
  
  if (esp_bluedroid_enable() != ESP_OK) {
    ESP_LOGE(TAG, "Failed to enable bluedroid");
    return;
  }
  
  // Register GAP callback
  if (esp_ble_gap_register_callback(gap_event_handler) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to register GAP callback");
    return;
  }
  
  // Set device name
  esp_ble_gap_set_device_name("BLE-Button");
  
  // Configure advertisement parameters
  adv_params_.adv_int_min = 0x20;  // 32 * 0.625 = 20ms
  adv_params_.adv_int_max = 0x40;  // 64 * 0.625 = 40ms
  adv_params_.adv_type = ADV_TYPE_IND;  // Connectable undirected
  adv_params_.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
  adv_params_.channel_map = ADV_CHNL_ALL;
  adv_params_.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
  
  ESP_LOGI(TAG, "BLE Button Advertiser setup complete");
}

void BleButtonAdvertiser::advertise_press() {
  ESP_LOGI(TAG, "Button pressed - starting BLE advertisement");
  
  // Build advertisement data with manufacturer-specific data
  // Format: Manufacturer ID (little-endian) + button state byte
  uint8_t adv_data_buffer[31] = {0};
  int adv_data_len = 0;
  
  // Manufacturer Specific Data (type 0xFF)
  adv_data_buffer[adv_data_len++] = 0x05;  // Length of manufacturer data
  adv_data_buffer[adv_data_len++] = 0xFF;  // Type: Manufacturer Specific Data
  adv_data_buffer[adv_data_len++] = 0xFF;  // Manufacturer ID low byte (0xFFFF = test)
  adv_data_buffer[adv_data_len++] = 0xFF;  // Manufacturer ID high byte
  adv_data_buffer[adv_data_len++] = 0x01;  // Button state: pressed
  
  // Set advertisement data
  adv_data_.set_scan_rsp = false;
  adv_data_.include_name = true;
  adv_data_.include_txpower = true;
  adv_data_.min_interval = 0x06;
  adv_data_.max_interval = 0x0C;
  adv_data_.appearance = 0x00;
  adv_data_.manufacturer_len = 5;
  adv_data_.p_manufacturer_data = &adv_data_buffer[2];
  adv_data_.service_data_len = 0;
  adv_data_.p_service_data = nullptr;
  adv_data_.service_uuid_len = 0;
  adv_data_.p_service_uuid = nullptr;
  adv_data_.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);
  
  // Configure BLE advertisement parameters
  if (esp_ble_gap_config_adv_data(&adv_data_) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to config advertisement data");
    return;
  }
  
  // Start advertising
  if (esp_ble_gap_start_advertising(&adv_params_) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to start advertising");
    return;
  }
  
  ESP_LOGI(TAG, "BLE advertisement started");
}

void BleButtonAdvertiser::gap_event_handler(esp_gap_ble_cb_event_t event,
                                             esp_ble_gap_cb_param_t *param) {
  switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
      ESP_LOGI(TAG, "Advertisement data configured");
      break;
      
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
      if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS) {
        ESP_LOGI(TAG, "Advertisement started successfully");
      } else {
        ESP_LOGE(TAG, "Advertisement failed to start, status: %d",
                 param->adv_start_cmpl.status);
      }
      break;
      
    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
      if (param->adv_stop_cmpl.status == ESP_BT_STATUS_SUCCESS) {
        ESP_LOGI(TAG, "Advertisement stopped");
      }
      break;
      
    default:
      break;
  }
}

void BleButtonAdvertiser::dump_config() {
  ESP_LOGCONFIG(TAG, "BLE Button Advertiser:");
  if (pin_ != nullptr) {
    LOG_PIN("  Button Pin: ", pin_);
  }
}

}  // namespace ble_button_advertiser
}  // namespace esphome
