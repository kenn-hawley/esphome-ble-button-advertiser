# ESPHome BLE Button Advertiser

A custom ESPHome component that broadcasts BLE advertisement packets when a button is pressed on an ESP32.

## Installation

Add to your ESPHome YAML:

```yaml
external\_components:
  - source: github://kenn-hawley/esphome-ble-button-advertiser

ble\_button\_advertiser:
  button\_pin: GPIO2
