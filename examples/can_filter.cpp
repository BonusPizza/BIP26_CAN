/**
 * @file can_filter.cpp
 * @brief Example of how to use the buildGroupFilter function in the BIP26_CAN
 * 
 * This example demonstrates how to use the buildGroupFilter function to create a filter configuration for a specific group ID.
 * In this example this group ID is 0x5.
 * 
 * @author Luca Krohn
 */
#include <Arduino.h>
#include <BIP26_CAN.h>

#define groupLsb 20
#define groupMsb 23


BIP26_CAN can;
uint8_t groupID;
void buildGroupID();

void setup(){
    Serial.begin(115200);
    delay(1000);

    buildGroupID();

    uint8_t groupfilter = 0x5; // example group filter, only accept messages from group 0x5

    while(!can.begin(groupID, 18, 19, TWAI_MODE_NORMAL, can.buildGroupFilter(groupfilter))){
        delay(100);
    }
}

/**
 * Just listens for messages and prints their content.
 * only messages from group 0x5 will be received due to the filter config used in the begin function.
 */
void loop(){
    Serial.print("Group ");
    Serial.print(groupID);
    Serial.println(" is listening...");
    twai_message_t msg;
    while(can.receive(msg)){
        Serial.print("Message from Group ");
        Serial.print(msg.identifier >> 4 & 0xF, HEX);
        Serial.print(" received with id: ");
        Serial.println(msg.identifier, HEX);
        for(int i = 0; i < msg.data_length_code; i++){
            Serial.print(msg.data[i], HEX);
        }
        Serial.println();
    }
}

/**
 * Reads the state of the pins defined by groupLsb and groupMsb and builds the groupID for the CAN messages.
 */
void buildGroupID(){
  groupID = 0;
  for (int i = groupLsb; i <= groupMsb; i++){
    pinMode(i, INPUT);
    groupID |= ((digitalRead(i) & 1) << (i-groupLsb));
  }
}

