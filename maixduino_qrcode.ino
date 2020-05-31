// qrcode encode and display
#include <Sipeed_ST7789.h>
#include "qrcode.h"

#define RGB2COLOR(r, g, b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GPIO_OUTPUT 4

SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_);
QRCode qrcode;
uint8_t* data;
int depth = 2;
static int sn = 0;
char qrcode_content[64];

// update lcd pixel buffer data
void update_lcd_buffer(uint8_t* data, int width, int height, int depth) {
  int i, j;
  int k = 0;
  int ox, oy;
  int p;

  uint8_t pixel_r = 255;//random(255);
  uint8_t pixel_g = 255;//random(255);
  uint8_t pixel_b = 255;//random(255);
  // set qrcode to center of screen
  Serial.print("QR szie: ");
  Serial.print(qrcode.size);
  Serial.print("\n");
  ox = (width - qrcode.size) / 2;
  oy = (height - qrcode.size) / 2;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width * depth; j += depth) {
      if ( j / 2 >= ox && j / 2 < (ox + qrcode.size) && i >= oy && i < (oy + qrcode.size) ) {
        p = j / 2 - ox + (i - oy) * qrcode.size;

        if (qrcode_getModule(&qrcode, j / 2 - ox, i - oy) == true) {
          pixel_r = 0;
          pixel_g = 0;
          pixel_b = 0;
        } else {
          pixel_r = 255;
          pixel_g = 255;
          pixel_b = 255;
        }
      } else {
        pixel_r = 255;
        pixel_g = 255;
        pixel_b = 255;
      }
      unsigned int pixel = (pixel_r & 0xFF << 16 ) | (pixel_g & 0xFF << 8 ) | pixel_b & 0xFF;
      unsigned short rgb565 = RGB2COLOR(pixel_r, pixel_g, pixel_b);
      *(data + i * width * depth + j) = (rgb565 & 0xFF00) >> 8;
      *(data + i * width * depth + j + 1) = rgb565 & 0xFF ;
    }
  }
}

void show_qrcode_on_lcd() {
  memset(data, 0, sizeof(uint8_t)*SCREEN_WIDTH*SCREEN_HEIGHT);
  update_lcd_buffer(data, SCREEN_WIDTH, SCREEN_HEIGHT, depth);
  lcd.drawImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t*)data);
}

// Create the QR code
void gen_qrcode(char* data, uint8_t qrcode_version, uint8_t ecc) {
  uint8_t qrcodeData[qrcode_getBufferSize(qrcode_version)];
  Serial.print("qrcodeData len: ");
  Serial.print(qrcode_getBufferSize(qrcode_version));
  Serial.print("\n");

  uint32_t dt = millis();
  qrcode_initText(&qrcode, qrcodeData, qrcode_version, ecc, data);

  // Delta time
  dt = millis() - dt;
  Serial.print("QR Code Generation Time: ");
  Serial.print(dt);
  Serial.print("\n");
  /*
    // Top quiet zone
    Serial.print("\n\n\n\n");
    for (uint8_t y = 0; y < qrcode.size; y++) {

      // Left quiet zone
      Serial.print("        ");

      // Each horizontal module
      for (uint8_t x = 0; x < qrcode.size; x++) {

        // Print each module (UTF-8 \u2588 is a solid block)
        Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588" : "  ");
      }

      Serial.print("\n");
    }

    // Bottom quiet zone
    Serial.print("\n\n\n\n");
  */
}


void setup() {
  // LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // LCD
  lcd.begin(15000000, COLOR_RED);
  lcd.setTextSize(2);
  lcd.setTextColor(COLOR_RED);

  // LCD Pixel Buffer
  data = (uint8_t*) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * depth * sizeof(uint8_t));
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);

  snprintf(qrcode_content, sizeof(char)*64, "Hello Maixduino %d", sn);
  sn++;
  gen_qrcode(qrcode_content, 3, 0);
  show_qrcode_on_lcd();

  lcd.setCursor(100, 30);
  lcd.println(qrcode_content);

  
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
