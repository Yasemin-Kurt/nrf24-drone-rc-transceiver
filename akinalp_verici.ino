#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Joystick verileri için yapı (struct)
struct signal {
  byte joy_y1;
  byte joy_x1;
  byte joy_y2;
  byte joy_x2;
  byte switch1;
  byte switch2;
  byte switch3;
};
signal data;

// RF24 nesnesi
RF24 radio(9, 8);  // CE, CSN

// İki modül arasındaki adres
const byte address[6] = "00001";

byte previousSwitch1 = 127; // Varsayılan olarak orta konum
byte previousSwitch2 = 127; // Varsayılan olarak orta konum

// Her joystick ekseni için alçak geçiren filtre başlangıç değerleri
float joy_x1_lowpass = 0.0;
float joy_y1_lowpass = 0.0;
float joy_x2_lowpass = 0.0;
float joy_y2_lowpass = 0.0;
float switch1_lowpass = 0.0;
float switch2_lowpass = 0.0;

// Filtreleme katsayısı
float lowpassAlpha = 0.2; // Daha düşük değerler daha fazla filtreleme yapar

// Alçak geçiren filtre fonksiyonu
int lowPassFilter(int newValue, float &filteredValue, float alpha) {
  filteredValue += alpha * (newValue - filteredValue);
  return filteredValue;
}

void setup() {
  // Seri iletişimi başlat
  Serial.begin(115200);

  // Joystick giriş pinlerini tanımla
  pinMode(A0, INPUT); // Yaw
  pinMode(A1, INPUT); // Throttle
  pinMode(A2, INPUT); // Roll
  pinMode(A3, INPUT); // Pitch
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(3,INPUT);
  pinMode(10,INPUT);

  // RF24 başlatma
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening(); // Verici moduna geç
}

void loop() {
  // Joystick verilerini oku
  int rawJoyX1 = analogRead(A2); // Pitch
  int rawJoyY1 = analogRead(A3); // Roll
  int rawJoyX2 = analogRead(A1); // Yaw
  int rawJoyY2 = analogRead(A0); // Throttle


  // Her eksene alçak geçiren filtre uygula ve değerleri 0-255 aralığına haritala
  data.joy_x1 = map(lowPassFilter(rawJoyX1, joy_x1_lowpass, lowpassAlpha), 0, 1023, 255, 0);
  data.joy_y1 = map(lowPassFilter(rawJoyY1, joy_y1_lowpass, lowpassAlpha), 0, 1023, 0, 255);
  data.joy_x2 = map(lowPassFilter(rawJoyX2, joy_x2_lowpass, lowpassAlpha), 0, 1023, 255, 0);
  data.joy_y2 = map(lowPassFilter(rawJoyY2, joy_y2_lowpass, lowpassAlpha), 0, 1023, 255, 0);
 
  bool d5State = digitalRead(5);
  bool d6State = digitalRead(6);
  bool d3State = digitalRead(3);
  bool d10State = digitalRead(10);

  if (d10State ==1) {
    data.switch3 = 255;
  }
  else{
  
  if ((( d5State==0 && d6State==0)&&(d3State==0))) {
    data.switch1 = 0; // Kademe 1
    data.switch2=0;
    data.switch3 =0;
  } else if (((d5State==0&&d6State==0)&&(d3State==1))) {
    data.switch2 = 255; // Kademe 2
    data.switch1=0;
    data.switch3 =0;
  } else if (((d5State==0 && d6State==1)&&(d3State==0))) {//1
    data.switch1 = 127; // Kademe 3
    data.switch2 =0;
    data.switch3 =0;
  } else if (((d5State==0&&d6State==1)&&(d3State==1))){//1
    data.switch1 = 127; // Kademe 3
    data.switch2=255;
    data.switch3 =0;
  } else if (((d5State==1&&d6State==0)&&(d3State==0))) {//1
    data.switch1 = 255;
    data.switch2=0;
    data.switch3 =0;
  } else if(((d6State==0 &&d5State==1)&&(d3State==1))){//1
    data.switch2 = 255; // Varsayılan durum (her ikisi de pasif)
    data.switch1=255;
    data.switch3 =0;
  } }
  

   //Seri monitöre verileri yazdır
  //Serial.print("Switch3: ");
  //Serial.println(data.switch3);
  /*Serial.print("Switch2: ");
  Serial.println(data.switch2);
  Serial.print("Switch1: ");
  Serial.println(data.switch1);*/
/*  Serial.print("Joy_X1 (Pitch): ");
  Serial.println(data.joy_x1);
  Serial.print("Joy_Y1 (Roll): ");
  Serial.println(data.joy_y1);
  Serial.print("Joy_X2 (Yaw): ");
  Serial.println(data.joy_x2);
  Serial.print("Joy_Y2 (Throttle): ");
  Serial.println(data.joy_y2);*/


  // RF24 ile verileri gönder
  bool success = radio.write(&data, sizeof(data));
  if (success) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Failed to send data");
  }

  delay(20); // 20ms (50 Hz)
}