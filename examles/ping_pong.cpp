/**
 * @file ping_pong.cpp
 * @brief Ping Pong example for BIP26_CAN library.
 * 
 * This example demonstrates a simple ping-pong communication between two CAN nodes using the BIP26_CAN library.
 * Each node sends a "ping" message and waits for a "pong" response. Upon receiving a "ping", the node responds with a "pong", and vice versa.
 * The example also shows how to read the group ID from specific pins and use it for CAN communication.
 * 
 * @author Luca Krohn
 */
#include <Arduino.h>
#include <BIP26_CAN.h>

#define groupLsb 20
#define groupMsb 23

uint8_t groupID;
BIP26_CAN can;

void buildGroupID();

void setup() {
  Serial.begin(115200);
  delay(10000);

  buildGroupID();

  Serial.print("GruppenID: ");
  Serial.println(groupID);

  while(!can.begin(groupID)){
    delay(100);
  }

  delay(1000);

  // sends ping message with priority 0 and content ID 0xA
  uint8_t data[] = "ping";
  if(can.send(0x0, 0xA, data)){
    Serial.println("Started ping-pong example, send ping");
  }
}

void loop() {
  Serial.println("Listening...");
  twai_message_t msg;
  // example of how to receive messages and print their content
  if(can.receive(msg)){
    Serial.print("Message received with id: ");
    Serial.println(msg.identifier, HEX);
    for(int i = 0; i < msg.data_length_code; i++){
      Serial.print(msg.data[i]);
    }
    Serial.println();

    // example of how to handle received messages based on their content ID (last 4 bits of the identifier)
    if((msg.identifier & 0xF) == 0xA){
        u_int8_t answer[] = "pong";
        if(can.send(0x1, 0xB, answer)){
            Serial.println("Answered ping with pong");
        }
    } else if ((msg.identifier & 0xF) == 0xB){
        u_int8_t answer[] = "ping";
        if(can.send(0x1, 0xB, answer)){
            Serial.println("Answered pong with ping");
        }
    }   
  }
  delay(1000);
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
