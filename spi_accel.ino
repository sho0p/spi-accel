#include <bsp_sd.h>
#include <ffconf_default_32020.h>
#include <ffconf_default_68300.h>
#include <ffconf.h>
#include <Sd2Card.h>
//#include <threads.h>
#include <Scheduler.h>
#include <SdFatFs.h>
#include <STM32SD.h>
#include <SPI.h>

#ifndef SD_DETECT_PIN
#define SD_DETECT_PIN SD_DETECT_NONE
#endif

#define SETUP_FLAG      B10000000
#define SCAN_MODE_NONE  B00000110
#define SCAN_MODE_0_N   B00000000
#define SCAN_MODE_4_4   B00000100
#define CS_PIN_1        41
#define CS_PIN_2        3
#define MISO_PIN        PC12
#define MOSI_PIN        PC11
#define SCK_PIN         PC10


SPIClass spi3;

File myFile;
int acc = 0;

uint32_t adc_buf * = ;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  spi3.setMOSI(PA7);
  spi3.setMISO(PA6);
  spi3.setSCLK(PA5);
  spi3 = SPIClass(PA7, PA6, PA5);
  spi3.begin(2);
  SPISettings settings = SPISettings(100000, MSBFIRST, SPI_MODE0);
  spi3.beginTransaction(1, settings);
  delay(1000);
  while (!Serial){;}
  Serial.println("initializing SD card...");
  while (SD.begin(SD_DETECT_PIN) != TRUE){
    delay(10);
  }
 // delay(100);
  Serial.println("Card initialized!");
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  Scheduler.startLoop(loop2);
}

void test_sd_card_func(){
  myFile = SD.open("duhast.txt", FILE_WRITE);
  String sdcard_str = String(acc++);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.seek(myFile.size());
    myFile.println("testing 1, 2, 3. April 19th");
 //   myFile.println(sdcard_str);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void adc_sd_card_func(uint32_t datum){
  myFile = SD.open("test2.txt", FILE_WRITE);
  //uint32_t datum = readADC(0);
  String datumString = String(datum, HEX);
 // Serial.print("Attempting to write ");
  Serial.println(datumString);
 // Serial.print("...");
  if(myFile){
    myFile.seek(myFile.size());
    myFile.println(datumString);
   // datafile.printNumber((long)datum,4);
  //  Serial.print("Wrote: ");
   // Serial.println(datumString);
    myFile.close();
 //   Serial.println (" Done!");
  }else{
    Serial.println("error opening file");
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  // test_sd_card_func();
  //adc_sd_card_func();
}

void loop2(){

}


uint32_t readADC(int cs_pin){
  uint32_t data;
  uint16_t regData = SETUP_FLAG | 1 << 3 | SCAN_MODE_0_N;

  uint16_t msb = spi3.transfer(2, regData);
  uint16_t lsb = spi3.transfer(2, 0x00);

  data = (msb << 6 | lsb >> 2);
  data <<= 16;

  msb = spi3.transfer(2, 0x00);
  lsb = spi3.transfer(2, 0X00);

  data |= (msb << 6 | lsb >> 2);

  return data;


}
