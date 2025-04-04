#include <Wire.h>
//Adarfuitの画像描写ライブラリー
#include <Adafruit_GFX.h> 
//AdarfuitのSSD1306用ライブラリー
#include <Adafruit_SSD1306.h>
const int SCREEN_WIDTH = 128;     //ディスプレイのサイズ指定
const int SCREEN_HEIGHT = 64;     //ディスプレイのサイズ指定
const int SCREEN_ADDRESS = 0x3C;  //I2Cのアドレス指定
Adafruit_SSD1306 disply( SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);  //ディスプレイ制御用のインスタンスを作成。この時にデイスプレのサイズを渡す。
//Font
#include "/Users/shigu/Library/CloudStorage/Dropbox/Arduino_inDropbox/libraries/Adafruit_GFX_Library/Fonts/FreeSans9pt7b.h"

// SCD30ライブラリ
#include "SparkFun_SCD30_Arduino_Library.h"
#define RHandT_SensorOffset -3.5
SCD30 airSensor;

void setup() {
  Serial.begin(115200);
  delay(1400);

  Serial.println("SCD30 Sensor Test Program");
  Wire.begin();

  if (airSensor.begin() == false) {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1)
      ;
  }
  // FRC　強制リキャリブレーション　
  //　他のCO2計の値をいれて、1度リキャリブレーション
  // airSensor.setForcedRecalibrationFactor(1100);
 
  disply.begin( SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS );      //ディスプレイを初期化。この時にI2Cアドレスを渡す。
  disply.clearDisplay();       //画面描写エリアを削除。
  disply.display();            //画面描写エリアをディスプレイに転送。ここで全画面を削除。
  disply.setFont( &FreeSans9pt7b );
  delay( 1000 );                //1秒待機
}

uint16_t co2;
float temp,hum;
unsigned int uiCun= 0;
void loop() { 
  // SCD30からデータを読み出せる場合には
  if (airSensor.dataAvailable()) {
    co2 = airSensor.getCO2();
    temp = airSensor.getTemperature() + RHandT_SensorOffset;
    hum = airSensor.getHumidity()  + RHandT_SensorOffset;
    //シリアルモニタ出力
    if ( uiCun == 0 ) Serial.printf("co2(ppm):%d,temp(C):%.1f,humidity(%):%.1f\n",co2,temp,hum);
 
    //液晶出力
    disply.clearDisplay();
    disply.setTextColor( SSD1306_WHITE, SSD1306_BLACK );

    disply.setCursor(0,20);             // Start at top-left corner
    disply.print(F(" CO2:") );
    disply.setCursor(50, 20);
    disply.print((co2));

    disply.setCursor(3,48);             // Start at top-left corner
    disply.print((temp), 1);
    disply.print(F(" C") );

    disply.setCursor(64, 48); 
    disply.print((hum), 1);
    disply.print(F("%") );

    disply.display();            //上記で設定したprint()をディスプレイに転送し表示する。
  }
  else
    Serial.print(".");

  delay(30000);
  uiCun ++;
  if ( uiCun > 60 ) uiCun = 0;
}