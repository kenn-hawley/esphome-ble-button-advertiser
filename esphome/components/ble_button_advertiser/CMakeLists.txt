idf_component_register(
    SRCS "ble_button_advertiser.cpp"
    INCLUDE_DIRS "." "${IDF_PATH}/components/bt/host/bluedroid/api/include/api"
    REQUIRES "bt"
)
