//
// Created by agustin.alexander on 11/6/21.
//

#include "sniffer.h"
#include <ESP8266WiFi.h>
#include <Arduino.h>

void upload(IPAddress ip, int port,
            int aggregation, int data_index, const unsigned int SEQ_DATA[],
            const int RSSI_DATA[],
            void (*sendFunction)(IPAddress, int, char buff[])) {
    // Max Packet Size 1020/31 = 33

    int ITER = (int) ((data_index + 1) / aggregation);
    int r = (data_index + 1) % aggregation;

    if (r != 0) {
        ITER += 1;
    }
    Serial.println("Iterations");
    Serial.println(ITER);
    for (int i = 0; i < ITER; i += 1) {
        char *buffer = new char[1020];
        int offset = sprintf(buffer, "[");

        for (int j = i * aggregation; j < (i + 1) * aggregation && j <= data_index; j += 1) {
            int rssi = RSSI_DATA[j];
            unsigned int seq = SEQ_DATA[j];
            offset += sprintf(buffer + offset, R"({"s":%u,"r":%d})", seq, rssi);
            boolean isNotLastIter = ( j + 1 < (i + 1) * aggregation) && (j + 1 <= data_index) ;
            if ( isNotLastIter) {
                offset += sprintf(buffer + offset, ",");
            }
        }
        sprintf(buffer + offset, "]");
        Serial.println("Batch");
        Serial.println(i+1);
        sendFunction(ip, port, buffer);
        delay(100);
    }


}
