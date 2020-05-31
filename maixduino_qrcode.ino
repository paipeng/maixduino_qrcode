// qrcode encode and display
#include <Sipeed_ST7789.h>
#include "qrcode.h"

#define RGB2COLOR(r, g, b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define QRCODE_VERSION 3

SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_);
QRCode qrcode;
uint8_t* data;
int depth = 2;
static int sn = 0;
char qrcode_content[64];
uint8_t* qrcodeData;


// update lcd pixel buffer data
void update_lcd_buffer_Nx(uint8_t* data, int width, int height, int depth, int factor) {
  int x, y;
  int k = 0;
  int ox, oy;
  int m, n;

  // set qrcode to center of screen
  Serial.print("QR szie: ");
  Serial.print(qrcode.size);
  Serial.print("\n");
  ox = (width - qrcode.size * factor) / 2;
  oy = (height - qrcode.size * factor) / 2;
  for (y = 0; y < height; y += factor) {
    for (x = 0; x < width * depth; x += depth * factor) {
      uint8_t pixel_r = 255;//random(255);
      uint8_t pixel_g = 255;//random(255);
      uint8_t pixel_b = 255;//random(255);
      if ( x / 2 >= ox && x / 2 < (ox + qrcode.size * factor) && y >= oy && y < (oy + qrcode.size * factor) ) {
        if (qrcode_getModule(&qrcode, (x / 2 - ox) / factor, (y - oy) / factor) == true) {
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
      *(data + y * width * depth + x) = (rgb565 & 0xFF00) >> 8;
      *(data + y * width * depth + x + 1) = rgb565 & 0xFF ;

      for (n = 0; n < factor; n++) {
        for (m = 0; m < factor; m++) {
          *(data + (y + n) * width * depth + x + 2*m) = (rgb565 & 0xFF00) >> 8;
          *(data + (y + n) * width * depth + x + 2*m + 1) = rgb565 & 0xFF ;
        }
      }
    }
  }
}

// update lcd pixel buffer data
void update_lcd_buffer_4x(uint8_t* data, int width, int height, int depth) {
  int x, y;
  int k = 0;
  int ox, oy;
  int m, n;

  // set qrcode to center of screen
  Serial.print("QR szie: ");
  Serial.print(qrcode.size);
  Serial.print("\n");
  ox = (width - qrcode.size * 4) / 2;
  oy = (height - qrcode.size * 4) / 2;
  for (y = 0; y < height; y += 4) {
    for (x = 0; x < width * depth; x += depth * 4) {
      uint8_t pixel_r = 255;//random(255);
      uint8_t pixel_g = 255;//random(255);
      uint8_t pixel_b = 255;//random(255);
      if ( x / 2 >= ox && x / 2 < (ox + qrcode.size * 4) && y >= oy && y < (oy + qrcode.size * 4) ) {
        if (qrcode_getModule(&qrcode, (x / 2 - ox) / 4, (y - oy) / 4) == true) {
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
      *(data + y * width * depth + x) = (rgb565 & 0xFF00) >> 8;
      *(data + y * width * depth + x + 1) = rgb565 & 0xFF ;

      for (n = 0; n < 4; n++) {
        for (m = 0; m < 4; m++) {
          *(data + (y + n) * width * depth + x + 2*m) = (rgb565 & 0xFF00) >> 8;
          *(data + (y + n) * width * depth + x + 2*m + 1) = rgb565 & 0xFF ;
        }
      }
    }
  }
}

// update lcd pixel buffer data
void update_lcd_buffer_2x(uint8_t* data, int width, int height, int depth) {
  int x, y;
  int k = 0;
  int ox, oy;

  // set qrcode to center of screen
  Serial.print("QR szie: ");
  Serial.print(qrcode.size);
  Serial.print("\n");
  ox = (width - qrcode.size * 2) / 2;
  oy = (height - qrcode.size * 2) / 2;
  for (y = 0; y < height; y += 2) {
    for (x = 0; x < width * depth; x += depth * 2) {
      uint8_t pixel_r = 255;//random(255);
      uint8_t pixel_g = 255;//random(255);
      uint8_t pixel_b = 255;//random(255);
      if ( x / 2 >= ox && x / 2 < (ox + qrcode.size * 2) && y >= oy && y < (oy + qrcode.size * 2) ) {
        if (qrcode_getModule(&qrcode, (x / 2 - ox) / 2, (y - oy) / 2) == true) {
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
      *(data + y * width * depth + x) = (rgb565 & 0xFF00) >> 8;
      *(data + y * width * depth + x + 1) = rgb565 & 0xFF ;


      // x+1
      *(data + y * width * depth + x + 2) = (rgb565 & 0xFF00) >> 8;
      *(data + y * width * depth + x + 2 + 1) = rgb565 & 0xFF ;
      // y+1
      *(data + (y + 1) * width * depth + x) = (rgb565 & 0xFF00) >> 8;
      *(data + (y + 1) * width * depth + x + 1) = rgb565 & 0xFF ;
      // x+1; y+1
      *(data + (y + 1)  * width * depth + x + 2) = (rgb565 & 0xFF00) >> 8;
      *(data + (y + 1)  * width * depth + x + 2 + 1) = rgb565 & 0xFF ;

    }
  }
}

// update lcd pixel buffer data
void update_lcd_buffer(uint8_t* data, int width, int height, int depth) {
  int x, y;
  int k = 0;
  int ox, oy;
  int p;

  // set qrcode to center of screen
  Serial.print("QR szie: ");
  Serial.print(qrcode.size);
  Serial.print("\n");
  ox = (width - qrcode.size) / 2;
  oy = (height - qrcode.size) / 2;
  for (y = 0; y < height; y++) {
    for (x = 0; x < width * depth; x += depth) {
      uint8_t pixel_r = 255;//random(255);
      uint8_t pixel_g = 255;//random(255);
      uint8_t pixel_b = 255;//random(255);
      if ( x / 2 >= ox && x / 2 < (ox + qrcode.size) && y >= oy && y < (oy + qrcode.size) ) {
        p = x / 2 - ox + (y - oy) * qrcode.size;
        if (qrcode_getModule(&qrcode, x / 2 - ox, y - oy) == true) {
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
      *(data + y * width * depth + x) = (rgb565 & 0xFF00) >> 8;
      *(data + y * width * depth + x + 1) = rgb565 & 0xFF ;
    }
  }
}

void show_qrcode_on_lcd() {
  memset(data, 255, sizeof(uint8_t)*SCREEN_WIDTH * SCREEN_HEIGHT);
  update_lcd_buffer_Nx(data, SCREEN_WIDTH, SCREEN_HEIGHT, depth, 8);
  lcd.drawImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t*)data);
}

// Create the QR code
void gen_qrcode(char* data, uint8_t qrcode_version, uint8_t ecc) {
  memset(qrcodeData, 0, sizeof(uint8_t)*qrcode_getBufferSize(qrcode_version));
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
  qrcodeData = (uint8_t*) malloc(sizeof(uint8_t) * qrcode_getBufferSize(QRCODE_VERSION));
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);

  snprintf(qrcode_content, sizeof(char) * 64, "charlotte schoolytftfyryw %d", sn);
  sn++;
  gen_qrcode(qrcode_content, QRCODE_VERSION, 0);
  show_qrcode_on_lcd();

  lcd.setCursor(20, 30);
  lcd.println(qrcode_content);


  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
