/**
 * @file BIP26_CAN.cpp
 * @brief Implementation of the BIP26_CAN class for CAN bus communication using the ESP32 TWAI driver.
 * 
 * This file contains the implementation of the BIP26_CAN class, which provides an interface for initializing the CAN bus, sending and receiving messages, and retrieving CAN bus status information. The class uses the ESP32 TWAI driver to manage CAN communication.
 * 
 * @author Luca Krohn
 */
#include "BIP26_CAN.h"

/**
 * init can bus with given tx and rx pins, group id and filter config.
 * @param txPin GPIO pin number for CAN TX, default is 18
 * @param rxPin GPIO pin number for CAN RX, default is 19
 * @param groupID 4 bit group id
 * @param mode TWAI operating mode, default is normal mode
 * @param f_config filter config for receiving messages, default is to accept all messages
 * @return true if initialization was successful, false otherwise
 */
bool BIP26_CAN::begin(uint8_t groupID, int txPin, int rxPin, twai_mode_t mode, twai_filter_config_t f_config){
    this->groupID = groupID;

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        (gpio_num_t)txPin,
        (gpio_num_t)rxPin,
        mode
    );

    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();

    if(twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK){
        return false;
    }
    return twai_start() == ESP_OK;
}

/**
 * send a message to the can bus with given priority, content id, and data
 * @param prio 3 bit priority of the message, lower value means higher priority
 * @param contentID 4 bit content id of the message, used to identify the type of the message
 * @param data byte array containing the data to be sent, max length is 8 bytes. 
 * If data is longer than 8 bytes, it will be truncated to 8 bytes
 * @param len length of the data byte array
 * @return true if the message was sent successfully, false if unsuccessful
 */
bool BIP26_CAN::send(uint8_t prio, uint8_t contentID, uint8_t* data, uint8_t len){
    twai_message_t msg = {};

    msg.identifier = buildID(prio, groupID, contentID);
    msg.data_length_code = (len > 8) ? 8 : len;

    memcpy(msg.data, data, msg.data_length_code);

    return twai_transmit(&msg, pdMS_TO_TICKS(10)) == ESP_OK;
}

/**
 * receive a message from the can bus
 * @param msg reference to a twai_message_t struct where the received message will be stored
 * @param timeout maximum time to wait for a message in FreeRTOS ticks, defaults to 10 ms, 
 * se portMAX_DELAY to wait indefinitely (should only be used in async tasks)
 * @return true if a message was received successfully, false if no message was received within timeout condition
 */
bool BIP26_CAN::receive(twai_message_t &msg, TickType_t timeout){
    if (twai_receive(&msg, timeout) == ESP_OK) {
        return true;
    }
    return false;
}

/**
 * get information about the can bus status
 * @param info reference to a twai_status_info_t struct where the status information will be stored
 * @return true if the status information was retrieved successfully, false otherwise
 */
bool BIP26_CAN::getCanInfo(twai_status_info_t &info){
    if(twai_get_status_info(&info) == ESP_OK){
        return true;
    }
    return false;
}

/**
 * build a filter configuration for a specific group ID
 * @param groupID 4 bit group id
 * @return filter configuration for the specified group
 */
twai_filter_config_t BIP26_CAN::buildGroupFilter(uint8_t groupID){
    twai_filter_config_t f_config = {
        .acceptance_code = (uint32_t)((groupID & 0xF) << 4) << 21,
        .acceptance_mask = (uint32_t)(0xF << 4) << 21,
        .single_filter = true
    };
    return f_config;
}

/**
 * build a message ID from its components
 * @param prio 3 bit priority of the message
 * @param groupID 4 bit group id
 * @param contentID 4 bit content id
 * @return the built message ID as uint16_t
 */
uint16_t BIP26_CAN::buildID(uint8_t prio, uint8_t groupID, uint8_t contentID) {
    return ((prio & 0x7) << 8) | ((groupID & 0xF) << 4) | (contentID & 0xF);
}