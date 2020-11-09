/*
  Scan Callback

  This example scans for BLE peripherals and prints out their advertising details:
  address, local name, advertised service UUIDs. Unlike the Scan example, it uses
  the callback style APIs and disables filtering so the peripheral discovery is
  reported for every single advertisement it makes.

  The circuit:
  - STEVAL-MKSBOX1V1, B-L475E-IOT01A1, B_L4S5I_IOT01A, or a Nucleo board plus the X-NUCLEO-IDB05A2 or the X-NUCLEO-IDB05A1 or the X-NUCLEO-BNRG2A1

  This example code is in the public domain.
*/

#include <STM32duinoBLE.h>

#if defined(ARDUINO_STEVAL_MKSBOX1V1)
/* STEVAL-MKSBOX1V1 */
SPIClass SpiHCI(PC3, PD3, PD1);
HCISpiTransportClass HCISpiTransport(SpiHCI, SPBTLE_1S, PD0, PD4, PA8, 1000000, SPI_MODE1);
BLELocalDevice BLE(&HCISpiTransport);
#elif defined(ARDUINO_DISCO_L475VG_IOT) || defined(ARDUINO_B_L4S5I_IOT01A)
/* B-L475E-IOT01A1 or B_L4S5I_IOT01A */
SPIClass SpiHCI(PC12, PC11, PC10);
HCISpiTransportClass HCISpiTransport(SpiHCI, SPBTLE_RF, PD13, PE6, PA8, 8000000, SPI_MODE0);
BLELocalDevice BLE(&HCISpiTransport);
#elif defined(ARDUINO_PNUCLEO_WB55RG)
/* PNUCLEO_WB55RG */
HCISharedMemTransportClass HCISharedMemTransport;
BLELocalDevice BLE(&HCISharedMemTransport);
#else
/* Shield IDB05A2 with SPI clock on D3 */
SPIClass SpiHCI(D11, D12, D3);
HCISpiTransportClass HCISpiTransport(SpiHCI, BLUENRG_M0, A1, A0, D7, 8000000, SPI_MODE0);
BLELocalDevice BLE(&HCISpiTransport);
/* Shield IDB05A2 with SPI clock on D13 */
/*#define SpiHCI SPI
HCISpiTransportClass HCISpiTransport(SpiHCI, BLUENRG_M0, A1, A0, D7, 8000000, SPI_MODE0);
BLELocalDevice BLE(&HCISpiTransport); */
/* Shield IDB05A1 with SPI clock on D3 */
/*SPIClass SpiHCI(D11, D12, D3);
HCISpiTransportClass HCISpiTransport(SpiHCI, SPBTLE_RF, A1, A0, D7, 8000000, SPI_MODE0);
BLELocalDevice BLE(&HCISpiTransport); */
/* Shield IDB05A1 with SPI clock on D13 */
/*#define SpiHCI SPI
HCISpiTransportClass HCISpiTransport(SpiHCI, SPBTLE_RF, A1, A0, D7, 8000000, SPI_MODE0);
BLELocalDevice BLE(&HCISpiTransport); */
/* Shield BNRG2A1 with SPI clock on D3 */
/*SPIClass SpiHCI(D11, D12, D3);
HCISpiTransportClass HCISpiTransport(SpiHCI, BLUENRG_M2SP, A1, A0, D7, 1000000, SPI_MODE1);
BLELocalDevice BLE(&HCISpiTransport); */
/* Shield BNRG2A1 with SPI clock on D13 */
/*#define SpiHCI SPI
HCISpiTransportClass HCISpiTransport(SpiHCI, BLUENRG_M2SP, A1, A0, D7, 1000000, SPI_MODE1);
BLELocalDevice BLE(&HCISpiTransport); */
#endif

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("BLE Central scan callback");

  // set the discovered event handle
  BLE.setEventHandler(BLEDiscovered, bleCentralDiscoverHandler);

  // start scanning for peripherals with duplicates
  int ret = 1;
  do
  {
    ret = BLE.scan(true);
    if (ret == 0)
    {
      BLE.end();
      BLE.begin();
    }
  } while(ret == 0);
}

void loop() {
  // poll the central for events
  BLE.poll();
}

void bleCentralDiscoverHandler(BLEDevice peripheral) {
  // discovered a peripheral
  Serial.println("Discovered a peripheral");
  Serial.println("-----------------------");

  // print address
  Serial.print("Address: ");
  Serial.println(peripheral.address());

  // print the local name, if present
  if (peripheral.hasLocalName()) {
    Serial.print("Local Name: ");
    Serial.println(peripheral.localName());
  }

  // print the advertised service UUIDs, if present
  if (peripheral.hasAdvertisedServiceUuid()) {
    Serial.print("Service UUIDs: ");
    for (int i = 0; i < peripheral.advertisedServiceUuidCount(); i++) {
      Serial.print(peripheral.advertisedServiceUuid(i));
      Serial.print(" ");
    }
    Serial.println();
  }

  // print the RSSI
  Serial.print("RSSI: ");
  Serial.println(peripheral.rssi());

  Serial.println();
}
