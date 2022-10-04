/**
 * @file LoRaUWB_demo_single_master_slave.ino
 * @author nguyenmanhthao996tn (mtnguyen@unice.fr)
 * @brief This sketch controls the LoRa-UWB Board in Master & Slave modes for measurement senarios. The board starts in Slave Mode by default.
 *        To change to Master mode, press UserButton 3 times (triple click). In Master Mode, press UserButton 1 time (single click) to trigger the measurement.
 *        Measurement result will be send to LoRaWAN network.
 * 
 *        For further information about requirements, setup/getting started, and LEDs/Button behaviours, check out the README.md file.
 * @version 1.0
 * @date 2022-10-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/****** LIBRARIES ******/
#include <Arduino.h>
#include "LED_Effects.h"
#include "LoRaUWB_Button2.h"
#include "LoRaUWB_LoRaWAN.h"
#include "LoRaUWB_UWB.h"
#include "LoRaUWB_UWB_Parser.h"
#include "config.h"


/****** PROGRAM VARIABLES ******/
uint8_t run_in_master_mode = 0; // 0 = SLAVE | 1 = MASTER (Start in SLAVE Mode)
uint32_t readingID = 0;

uint32_t default_operation_timestamp = 0;

static byte payload[13];
static uint8_t payload_length = 0;


/****** MAIN PROGRAM ******/
void setup() {
  Serial.begin(9600);
  while (!Serial && (millis() < 5000)) {}

  Serial.println("===== LoRaUWB Demo =====");

  // Peripherals init
  LoRaWAN_Init();

  // Init variable
  run_in_master_mode = 0;
  clearLoRaWANPacket();

  // Flash LEDs to indicate program ready
  LedEffect_ProgramStart();
  delay(500);

  // Start with SLAVE Mode
  BoardUWB.begin(SLAVE_MODE);
  Serial.println("LoRaUWB_UWB: Slave Mode");
}

void loop() {
  // Check button
  switch (BoardButton.read())
  {
    case BUTTON_SINGLE_CLICK:
      if (run_in_master_mode) {
        do_measurement();
      }
      break;
    case BUTTON_DOUBLE_CLICK:
      // Do nothing
      break;
    case BUTTON_TRIPLE_CLICK:
      change_mode();
      break;
    default:
      // Do nothing
      break;
  }

  // Default operations
  if (run_in_master_mode) {
    // MASTER
  }
  else
  {
    // SLAVE - Blink red LED every 5s to indicate that the board is running in SLAVE Mode
    uint32_t current_timestamp = millis();
    if (current_timestamp - default_operation_timestamp > 5000)
    {
      default_operation_timestamp = current_timestamp;

      BoardLed.ledRedBlink(3);
    }
  }
}


/****** OTHER FUNCTIONS ******/
void LoRaWAN_Init(void)
{
  BoardLoRaWAN.setNetworkKey(networkKey);
  BoardLoRaWAN.setAppKey(appKey);
  BoardLoRaWAN.setDeviceAddress(deviceAddress);

  BoardLoRaWAN.begin();
  Serial.print("LoRa Module status: ");
  Serial.println(BoardLoRaWAN.getStatusString());
}

void clearLoRaWANPacket(void)
{
  memset((void*)payload, 0, 22);
  payload_length = 0;
}

void change_mode(void)
{
  BoardUWB.end();

  // Toggle mode
  run_in_master_mode &= 1;
  run_in_master_mode ^= 1;

  if (run_in_master_mode)
  {
    // MASTER Mode
    LedEffect_MasterModeStart();
    BoardUWB.begin(MASTER_MODE);
    Serial.println("LoRaUWB_UWB: Master Mode");
  }
  else
  {
    // SLAVE Mode
    LedEffect_SlaveModeStart();
    BoardUWB.begin(SLAVE_MODE);
    Serial.println("LoRaUWB_UWB: Slave Mode");
  }

  delay(500);
}

void do_measurement(void)
{
  // Delay 5 seconds for getting ready
  Serial.println("Master measuring: Be ready!");
  BoardLed.ledBlueOn();
  delay(5000);
  BoardLed.ledBlueBlink(3);
  BoardLed.ledBlueOn();

  // Measure with RFSW Mode 1
  UWB_1D_Data_t data_mode1;
  BoardUWB.RFSW_Mode1();
  delay(1000);
  if (!BoardUWB.readLastestData(&data_mode1))
  {
    Serial.println("Master measuring: Failed to get data1 from UWB Module");
    LedEffect_MasterMeasureFail();
    return; // Fail to get data
  }

  // Measure with RFSW Mode 2
  UWB_1D_Data_t data_mode2;
  BoardUWB.RFSW_Mode2();
  delay(1000);
  if (!BoardUWB.readLastestData(&data_mode2))
  {
    Serial.println("Master measuring: Failed to get data2 from UWB Module");
    LedEffect_MasterMeasureFail();
    return; // Fail to get data
  }

  // Print information
  Serial.println("=== Mode 1 ===");
  Serial.print("ID: ");
  Serial.println(data_mode1.id);

  Serial.print("TAGID: ");
  Serial.println(data_mode1.tag_id);

  Serial.print("DT: ");
  Serial.println(data_mode1.dt);

  Serial.print("LQI: ");
  Serial.println(data_mode1.lqi);

  Serial.print("DIST (cm): ");
  Serial.println((int)(data_mode1.dist * 100));

  Serial.println("=== Mode 2 ===");
  Serial.print("ID: ");
  Serial.println(data_mode2.id);

  Serial.print("TAGID: ");
  Serial.println(data_mode2.tag_id);

  Serial.print("DT: ");
  Serial.println(data_mode2.dt);

  Serial.print("LQI: ");
  Serial.println(data_mode2.lqi);

  Serial.print("DIST (cm): ");
  Serial.println((int)(data_mode2.dist * 100));

  // Check ID
  if ((data_mode1.id != data_mode2.id) || (data_mode1.dt != data_mode2.dt) || (data_mode1.tag_id != data_mode2.tag_id))
  {
    Serial.println("Master measuring: Failed to get data from UWB Module (Multiple device received)");
    LedEffect_MasterMeasureFail();
    return; // Fail to get data
  }

  // Calculate & Build LoRaWAN Packet
  payload[0] = (uint8_t)(data_mode1.dt >> 24) & 0xff;
  payload[1] = (uint8_t)(data_mode1.dt >> 16) & 0xff;
  payload[2] = (uint8_t)(data_mode1.dt >> 8) & 0xff;
  payload[3] = (uint8_t)(data_mode1.dt) & 0xff;

  payload[4] = (uint8_t)(data_mode1.id >> 24) & 0xff;
  payload[5] = (uint8_t)(data_mode1.id >> 16) & 0xff;
  payload[6] = (uint8_t)(data_mode1.id >> 8) & 0xff;
  payload[7] = (uint8_t)(data_mode1.id) & 0xff;

  payload[8] = (uint8_t)(data_mode1.tag_id >> 24) & 0xff;
  payload[9] = (uint8_t)(data_mode1.tag_id >> 16) & 0xff;
  payload[10] = (uint8_t)(data_mode1.tag_id >> 8) & 0xff;
  payload[11] = (uint8_t)(data_mode1.tag_id) & 0xff;

  payload[12] = data_mode1.lqi;
  payload[13] = data_mode2.lqi;

  uint32_t dist1 = data_mode1.dist * 100;
  payload[14] = (uint8_t)(dist1 >> 24) & 0xff;
  payload[15] = (uint8_t)(dist1 >> 16) & 0xff;
  payload[16] = (uint8_t)(dist1 >> 8) & 0xff;
  payload[17] = (uint8_t)(dist1) & 0xff;

  uint32_t dist2 = data_mode2.dist * 100;
  payload[18] = (uint8_t)(dist2 >> 24) & 0xff;
  payload[19] = (uint8_t)(dist2 >> 16) & 0xff;
  payload[20] = (uint8_t)(dist2 >> 8) & 0xff;
  payload[21] = (uint8_t)(dist2) & 0xff;

  // Reading ID
  if (readingID == 0)
  {
    // Generate 2 random octets for the first time only
    readingID = millis();
    readingID = (readingID & 0x00FF) << 16;
  }
  else
  {
    readingID++;
  }
  payload[22] = (uint8_t)(readingID >> 24) & 0xff;
  payload[23] = (uint8_t)(readingID >> 16) & 0xff;
  payload[24] = (uint8_t)(readingID >> 8) & 0xff;
  payload[25] = (uint8_t)(readingID) & 0xff;

  payload_length = 26; // Bytes

  BoardLoRaWAN.sendPacket(payload, payload_length);

  Serial.print("Packet sending status: ");
  Serial.println(BoardLoRaWAN.getStatusString());
  clearLoRaWANPacket();

  // Done
  LedEffect_MasterMeasureSuccess();
  Serial.print("Master measuring: DONE, Reading ID = ");
  Serial.println(readingID);
}
