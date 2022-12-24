#include <Arduino.h>
#include <unity.h>
#include "../lib/sniffer.cpp"

// Sniffing
#define TARGET "2e:f4:32:20:71:d8"
#define TARGETMODE true
#define TARGETINTERVAL 100 // ms

// Upload Node
#define UPLOADWIFISSID "Wi-Fi - 224"
#define UPLOADWIFIPASS "21049510"
#define UPLOAD_IP IPAddress(192,168,0,198)
#define UPLOAD_PORT 5001
#define AGGREGATION 30
// UDP


const int MAXTIME = 9999; // in seconds

boolean uploadMode = false;
// uint8_t from 0 to 255
// In ESP8266 Global variables use 31,663 bytes
const int SIZE = 500; // RSSI(2byte) + SEQ(2bytes) = 4 bytes
int RSSI[SIZE]; // 2 bytes from 0 to something
// max seq number on sender is around 65520
unsigned int SEQ[SIZE]; // 2 bytes from 0 to 4294967295  {"s":,"r":} 11 + 10 + 10 = 31 bytes
int INDEX = 0;
int TEST_AMOUNTOFTIMESCALLED = 1;

void setUp(void) {

    for (int i = 0; i < SIZE; i += 1) {
        SEQ[i] = i;
        RSSI[i] = i;
        INDEX = i;
    }
}

char *generate(int from, int to) {
    char *buff_gen = new char[1020];
    int offset = sprintf(buff_gen, "[");
    for (int i = from; i < to; i += 1) {
        int rssi = RSSI[i];
        unsigned int seq = SEQ[i];
        offset += sprintf(buff_gen + offset, R"({"s":%u,"r":%d})", seq, rssi);
        if (i < to - 1) {
            offset += sprintf(buff_gen + offset, ",");
        }
    }
    sprintf(buff_gen + offset, "]");
    return buff_gen;
}

void UDPSendMock(IPAddress ip, int port, char buff[1020]) {
    //char *expected = generate(TEST_AMOUNTOFTIMESCALLED  * AGGREGATION, TEST_AMOUNTOFTIMESCALLED+1 * AGGREGATION);
    int from = (TEST_AMOUNTOFTIMESCALLED - 1) * AGGREGATION;
    int to = (TEST_AMOUNTOFTIMESCALLED) * AGGREGATION;
    TEST_ASSERT_EQUAL_STRING_MESSAGE(generate(from, to), buff, "Test that buff is Equal");
    TEST_AMOUNTOFTIMESCALLED += 1;
}
// void tearDown(void) {
// // clean stuff up here
// }

void test_upload(void) {
    upload(UPLOAD_IP, UPLOAD_PORT, AGGREGATION, INDEX, SEQ, RSSI, UDPSendMock);
}

void test_times_called(void) {
    int ITER = (int) (INDEX + 1) / AGGREGATION;
    int r = ( INDEX + 1) % AGGREGATION;
    if (r != 0) {
        ITER += 1;
    }
    TEST_ASSERT_EQUAL(SIZE, INDEX + 1);
    TEST_ASSERT_EQUAL(ITER, TEST_AMOUNTOFTIMESCALLED);
}


void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!
    //Serial.begin(115200);
    RUN_TEST(test_upload);
    RUN_TEST(test_times_called);
    pinMode(LED_BUILTIN, OUTPUT);
}

uint8_t i = 0;
uint8_t max_blinks = 5;

void loop() {

}
