#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// Set up the NeoPixel Stick
#define NUM_LEDS 8
#define LED_PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// i2c setup
#define I2C_ADDR 8

// Define the arrays for LED settings.
int LED_mode[NUM_LEDS];
int LED_r[NUM_LEDS];
int LED_g[NUM_LEDS];
int LED_b[NUM_LEDS];
int LED_brightness[NUM_LEDS];
int LED_flashcycles[NUM_LEDS];

void setup() {
  // Init the arrays.
  for ( int i = 0; i < NUM_LEDS; i++ ) {
    LED_mode[i] = 0;
    LED_r[i] = 255;
    LED_g[i] = 0;
    LED_b[i] = 0;
    LED_brightness[i] = 0;
    LED_flashcycles[i] = 0;
  }
  
  strip.begin();
  strip.show();
  Serial.begin(9600);
  Wire.begin(I2C_ADDR);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Show the LEDs.
  for ( int i = 0; i < NUM_LEDS; i++ ) {
    int red = (LED_r[i] / 255) * LED_brightness[i];
    int green = (LED_g[i] / 255) * LED_brightness[i];
    int blue = (LED_b[i] / 255) * LED_brightness[i];

    // Mode 0 = Uninit
    if ( LED_mode[i] == 0 ) {
      if ( LED_brightness[i] > 100 ) {
        LED_brightness[i] = 0;
      }
      LED_brightness[i]++;
      strip.setPixelColor(i, red, green, blue);
    }
    
    // Mode 1 = Solid
    else if ( LED_mode[i] == 1 ) {
      strip.setPixelColor(i, red, green, blue);        
    }

    // Mode 2 = Flash
    else if ( LED_mode[i] == 2 ) {
      if ( LED_flashcycles[i] > 16 ) {
        uint32_t color = strip.getPixelColor(i);
        if ( color == 0 ) {
          strip.setPixelColor(i, red, green, blue);
        } else {
          strip.setPixelColor(i, 0, 0, 0);
        }
        LED_flashcycles[i] = 0;
      } else {
        LED_flashcycles[i]++;
      }
    }
    
  }
  strip.show();
  delay(10);
}

void receiveEvent(int Bytes) {
  Serial.print("Received packet - Length: ");
  Serial.print(Bytes, DEC);
  Serial.println("");

  int Packet[Bytes];
  for (int i = 0; i < Bytes; i++) {
    Packet[i] = Wire.read();
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(Packet[i], HEX);
  }
  
  // Evaluate the packet (in the following order):
  // LED number (0-7)
  // LED Mode (0 = uninit, 1 = solid, 2 = flash)
  // LED Red value
  // LED Green value
  // LED Blue value
  // LED Brightness.

  // Handle i2c packet in SMBus format.
  if ( Bytes == 7 && Packet[2] < NUM_LEDS && Packet[1] == 5) {
    Serial.println("Processing SMBus packet...");
    int LED = Packet[0];
    LED_mode[LED] = Packet[2];
    LED_r[LED] = Packet[3];
    LED_g[LED] = Packet[4];
    LED_b[LED] = Packet[5];
    LED_brightness[LED] = Packet[6];

    strip.setPixelColor(LED,
      (LED_r[LED] / 255) * LED_brightness[LED],
      (LED_g[LED] / 255) * LED_brightness[LED],
      (LED_b[LED] / 255) * LED_brightness[LED]);
    strip.show();
  }
}

