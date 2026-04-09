/**
 * @file observer_example.cpp
 * @brief Observer example for BIP26_CAN library.
 * 
 * This example initializes the CAN bus in listen-only mode and continuously listens for incoming messages.
 * When a message is received, it prints the message ID and data to the serial monitor.
 * It also shows how to use the getCanInfo function to retrieve and print the current status of the CAN bus.
 * 
 * @author Luca Krohn
 */

#include <Arduino.h>
#include <BIP26_CAN.h>

BIP26_CAN can;

void printStatus();

void setup(){
    Serial.begin(115200);

    while(!can.begin(1, TWAI_MODE_LISTEN_ONLY)){
        delay(100);
    }

}

void loop(){

  // Example of how to receive messages and print their content
  twai_message_t msg;
  if(can.receive(msg)){
    Serial.print("Message received with id: ");
    Serial.println(msg.identifier, HEX);
    for(int i = 0; i < msg.data_length_code; i++){
      Serial.print(msg.data[i]);
    }
    Serial.println();
    
    /*
    Examples for how to decode the message ID into its components (prio, group, content)
    uint8_t prio = (msg.identifier >> 8) & 0x7;
    uint8_t group = (msg.identifier >> 4) & 0xF;
    uint8_t content = msg.identifier & 0xF;
    */
    
    }
    printStatus();
    delay(10);
}

/**
 * This is how you could use the getCanInfo function
 */
void printStatus(){
  twai_status_info_t status;
  if(can.getCanInfo(status)){
    if(status.state == TWAI_STATE_RUNNING){
      Serial.print("Number of Bus Errors");
      Serial.println(status.bus_error_count);

      Serial.print("Messages in rx: ");
      Serial.println(status.msgs_to_rx);
      
      Serial.print("Messages in rx missed: ");
      Serial.println(status.rx_missed_count);

      Serial.print("rx overrun count: ");
      Serial.println(status.rx_overrun_count);
            
      Serial.print("rx error counter: ");
      Serial.println(status.rx_error_counter);

      Serial.print("Messages to tx: ");
      Serial.println(status.msgs_to_tx);
      
      Serial.print("tx error counter: ");
      Serial.println(status.tx_error_counter);
      
      Serial.print("tx failed count: ");
      Serial.println(status.tx_failed_count);
    } else {
      Serial.print("Can Bus Inaktiv!\nStatus:");
      Serial.println(status.state);
    }
  }
}