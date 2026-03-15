#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PPM_PIN 3           // PPM sinyal çıkış pini
#define CHANNEL_NUMBER 8    // PPM kanalı sayısı
#define AGF_SENSITIVITY 5   // AGF hassasiyet değeri
#define PPM_FRAME_LENGTH 22500 // PPM çerçeve süresi (22.5ms)
#define PPM_PULSE_LENGTH 300   // Her bir PPM darbe süresi (300µs)
#define PPM_MIN_PULSE 830     // Minimum kanal sinyali (1000µs)
#define PPM_MAX_PULSE 1900     // Maksimum kanal sinyali (2000µs)

int ppm[CHANNEL_NUMBER];  // PPM sinyali için kanal değerleri (8 kanal örnek)
struct signal {
  byte joy_y1;
  byte joy_x1;
  byte joy_y2;
  byte joy_x2;
  byte switch1;
  byte switch2;
};
signal data;

RF24 radio(9, 8);  // CE, CSN
const byte address[6] = "00001";

// Function prototype for sendPPM
void sendPPM(int *channels, int numberOfChannels);

void setup() {
  pinMode(PPM_PIN, OUTPUT);
  digitalWrite(PPM_PIN, LOW);

  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    // Joystick verilerini uçuş kontrolü kanallarına atıyoruz
    ppm[0] = constrain(map(data.joy_y1, 0, 255, PPM_MIN_PULSE, PPM_MAX_PULSE), PPM_MIN_PULSE, PPM_MAX_PULSE); // YAW
    ppm[1] = constrain(map(data.joy_x1, 255, 0, PPM_MIN_PULSE, PPM_MAX_PULSE), PPM_MIN_PULSE, PPM_MAX_PULSE); // THROTTLE
    ppm[2] = constrain(map(data.joy_y2, 0, 255, PPM_MIN_PULSE, PPM_MAX_PULSE), PPM_MIN_PULSE, PPM_MAX_PULSE); // PITCH
    ppm[3] = constrain(map(data.joy_x2, 255, 0, PPM_MIN_PULSE, PPM_MAX_PULSE), PPM_MIN_PULSE, PPM_MAX_PULSE); // ROLL

    if (data.switch1 == 0 && data.switch2 == 0) {
  ppm[4] = 1100; // Mod 1
  ppm[5] = 1100;
} else if (data.switch2 == 255 && data.switch1 == 0) {
  ppm[4] = 1300; // Mod 2
  ppm[5] = 1100;
} else if (data.switch1 == 127 && data.switch2 == 0) {
  ppm[4] = 1400; // Mod 3
  ppm[5] = 1100;
} else if (data.switch1 == 127 && data.switch2 == 255) {
  ppm[4] = 1500; // Mod 4
  ppm[5] = 1400;
} else if (data.switch1 == 255 && data.switch2 == 0) {
  ppm[4] = 1700; // Mod 5
  ppm[5] = 1100;
} else if (data.switch2 == 255 && data.switch1 == 255) {
  ppm[4] = 1900; // Mod 6
  ppm[5] = 1900;
}

    // Diğer kanalları sabit değerlere ayarlıyoruz
    ppm[6] = PPM_MIN_PULSE; // Boş kanal
    ppm[7] = PPM_MIN_PULSE; // Boş kanal

    Serial.print("4 değer");
    Serial.println(ppm[4]);
    Serial.print("5 değer" );
    Serial.println(ppm[5]);

    // PPM sinyali gönderme
    sendPPM(ppm, CHANNEL_NUMBER);
  } else {
    Serial.println("No data available");
  }
}

// PPM sinyali gönderme fonksiyonu
void sendPPM(int *channels, int numberOfChannels) {
  unsigned long startTime = micros();

  // PPM başlatıcı sinyali
  digitalWrite(PPM_PIN, LOW);
  delayMicroseconds(PPM_PULSE_LENGTH);
  digitalWrite(PPM_PIN, HIGH);

  for (int i = 0; i < numberOfChannels; i++) {
    delayMicroseconds(channels[i] - PPM_PULSE_LENGTH);
    digitalWrite(PPM_PIN, LOW);
    delayMicroseconds(PPM_PULSE_LENGTH);
    digitalWrite(PPM_PIN, HIGH);
  }

  // Sync pulse
  unsigned long frameTime = micros() - startTime;
  delayMicroseconds(PPM_FRAME_LENGTH - frameTime);
}
