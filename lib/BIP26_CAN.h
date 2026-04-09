/**
 * @file BIP26_CAN.h
 * @brief Header file for the BIP26_CAN class, which provides an interface for CAN bus communication using the ESP32 TWAI driver.
 * 
 * This file defines the BIP26_CAN class, which includes methods for initializing the CAN bus, 
 * sending and receiving messages, and retrieving CAN bus status information. The class is designed 
 * to be used in ESP32 projects and abstracts away the details of the TWAI driver, providing a simple interface for CAN communication.
 * @author Luca Krohn
 */
#ifndef BIP26_CAN_H
#define BIP26_CAN_H

#include "driver/twai.h"
#include "string.h"
/**
 * BIP26_CAN class provides an interface for CAN bus communication using the ESP32 TWAI driver.
 */
class BIP26_CAN{
    public:
        bool begin(uint8_t groupID, int txPin = 18, int rxPin = 19, twai_mode_t mode = TWAI_MODE_NORMAL, twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL());
        bool send(uint8_t prio, uint8_t contentID, uint8_t* data);
        bool receive(twai_message_t &msg, TickType_t timeout = pdMS_TO_TICKS(10));
        bool getCanInfo(twai_status_info_t &info);
        twai_filter_config_t buildGroupFilter(uint8_t groupID);


    private:
        uint8_t groupID;
        uint16_t buildID(uint8_t prio, uint8_t groupID, uint8_t contentID);
};

#endif