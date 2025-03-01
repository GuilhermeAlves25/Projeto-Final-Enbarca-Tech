#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Definições dos pinos
#define LED_BLUE 12   // GPIO conectado ao terminal azul do LED RGB
#define BUTTON_A 5 
#define BUTTON_B 6   // GPIO conectado ao Botão A
const uint I2C_SDA = 14; // Pino SDA do I2C
const uint I2C_SCL = 15; // Pino SCL do I2C

// Bitmap da imagem (128x64 pixels)
const uint8_t bitmap_128x64[] = { 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x77, 0xef, 0xfd, 0xff, 0xff, 0xff, 
	0xff, 0xbf, 0x73, 0xe6, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x73, 0xe6, 0xfc, 0xff, 0xff, 0xff, 
	0xff, 0xbf, 0x73, 0xee, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 
	0xff, 0x01, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 
	0xff, 0x01, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 0xff, 
	0x0f, 0xe0, 0x3f, 0xfe, 0x03, 0xf8, 0xff, 0xff, 0xff, 0xe1, 0x1f, 0xfc, 0x83, 0xff, 0xff, 0xff, 
	0xff, 0xe1, 0x3f, 0xfe, 0x83, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0x7f, 0xfe, 0x03, 0xf8, 0xff, 0xff, 
	0x1f, 0xe0, 0x3f, 0xfe, 0x03, 0xf8, 0xff, 0xff, 0xff, 0xe1, 0x07, 0xf0, 0x83, 0xff, 0xff, 0xff, 
	0xff, 0xe1, 0x03, 0xc0, 0x83, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0x71, 0xc6, 0x03, 0xf8, 0xff, 0xff, 
	0xff, 0xe1, 0x79, 0x8e, 0x83, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x79, 0x9e, 0x83, 0xff, 0xff, 0xff, 
	0xff, 0xe1, 0x79, 0x8e, 0x83, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0x71, 0x8e, 0x03, 0xf8, 0xff, 0xff, 
	0xff, 0xe1, 0x03, 0xc6, 0x83, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x07, 0xee, 0x83, 0xff, 0xff, 0xff, 
	0xff, 0xe1, 0xff, 0xff, 0x83, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 0xff, 
	0xff, 0xc1, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 
	0xff, 0x01, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 
	0xff, 0xbf, 0x73, 0xe6, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x73, 0xee, 0xfc, 0xff, 0xff, 0xff, 
	0xff, 0xbf, 0x73, 0xe6, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x73, 0xee, 0xfc, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x3f, 0x00, 0x00, 0xfc, 
	0xff, 0xff, 0x01, 0x00, 0xff, 0xfb, 0xff, 0xfd, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xfb, 0xff, 0xfb, 
	0xff, 0xff, 0xf1, 0xff, 0xff, 0xfb, 0xff, 0xfb, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x01, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xff, 0x3f, 0x00, 0xff, 
	0xff, 0xff, 0x07, 0x00, 0xff, 0xcf, 0xfb, 0xfe, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xf7, 0xfb, 0xfd, 
	0xff, 0xff, 0xf9, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 
	0xff, 0xff, 0xf8, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 
	0xff, 0xff, 0x01, 0x00, 0xff, 0xfb, 0xfb, 0xfb, 0xff, 0xff, 0x03, 0x00, 0xff, 0xf7, 0xfb, 0xfd, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x7b, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0x07, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x03, 0x81, 0xff, 0x1f, 0x00, 0xff, 0xff, 0xff, 0xf1, 0x9f, 0xff, 0xef, 0xff, 0xfe, 
	0xff, 0xff, 0xf9, 0x1f, 0xff, 0xf7, 0xff, 0xfd, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xfb, 0xff, 0xfb, 
	0xff, 0xff, 0xf8, 0x1f, 0xff, 0xfb, 0xff, 0xfb, 0xff, 0xff, 0xe1, 0x0f, 0xff, 0xfb, 0xff, 0xfb, 
	0xff, 0xff, 0x01, 0x80, 0xff, 0xfb, 0xff, 0xfb, 0xff, 0xff, 0x03, 0xc0, 0xff, 0xfb, 0xff, 0xfb, 
	0xff, 0xff, 0x1f, 0xf0, 0xff, 0xf7, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x3f, 0xfe, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xe7, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x61, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x71, 0x3e, 0x0f, 0x00, 0x00, 0xf8, 
	0xff, 0xff, 0x78, 0x3e, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0x78, 0x3e, 0xff, 0xfb, 0xff, 0xff, 
	0xff, 0xff, 0x79, 0x9e, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x61, 0x8e, 0xff, 0xfb, 0xff, 0xff, 
	0xff, 0xff, 0x03, 0x00, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0xff, 0xfb, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xf8, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x01, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xe0, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x03, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf1, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf1, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x87, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xc3, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xf7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xe7, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x61, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x71, 0x3c, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x78, 0x3e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x78, 0x3e, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x79, 0x1e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x71, 0x8e, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x01, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const uint8_t bitmap_128x642[] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x80, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x01, 0x00, 0x00, 
	0x00, 0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 
	0x00, 0x00, 0xe0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x07, 0x00, 0x00, 
	0x00, 0x00, 0xf0, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x0f, 0x00, 0x00, 
	0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 
	0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x7f, 0xfe, 0x3f, 0x00, 0x00, 
	0x00, 0x00, 0xfc, 0x0f, 0xe0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x80, 0x7f, 0x00, 0x00, 
	0x00, 0x00, 0xfe, 0x0e, 0x20, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x2e, 0x00, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0x0e, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x0e, 0x08, 0xf8, 0x01, 0x00, 
	0x00, 0x80, 0x0f, 0x0e, 0x00, 0xf0, 0x01, 0x00, 0x00, 0xc0, 0x0f, 0x0e, 0x00, 0xf0, 0x03, 0x00, 
	0x00, 0xc0, 0x07, 0x0e, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xc0, 0x07, 0x0e, 0x00, 0xe0, 0x03, 0x00, 
	0x00, 0xe0, 0x03, 0x0e, 0x20, 0xc0, 0x07, 0x00, 0x00, 0xe0, 0x03, 0x0e, 0x00, 0xc0, 0x07, 0x00, 
	0x00, 0xf0, 0x01, 0x0e, 0x00, 0x81, 0x0f, 0x00, 0x00, 0xf0, 0x01, 0x0a, 0x01, 0x81, 0x0f, 0x00, 
	0x00, 0xf8, 0x01, 0x0e, 0x00, 0x80, 0x1f, 0x00, 0x00, 0xf8, 0x01, 0x0e, 0x00, 0x80, 0x1f, 0x00, 
	0x00, 0xfc, 0x00, 0x0e, 0x00, 0x00, 0x3f, 0x00, 0x00, 0xfc, 0x00, 0x0e, 0x00, 0x00, 0x3f, 0x00, 
	0x00, 0xfe, 0x00, 0x0c, 0x00, 0x00, 0x7f, 0x00, 0x00, 0xfe, 0x00, 0x0c, 0x00, 0x00, 0x7f, 0x00, 
	0x00, 0xfe, 0x00, 0x0c, 0x00, 0x00, 0x7f, 0x00, 0x00, 0xff, 0x00, 0x1c, 0x84, 0x00, 0xff, 0x00, 
	0x00, 0xff, 0x00, 0x1c, 0x84, 0x00, 0xff, 0x00, 0x00, 0xfe, 0x00, 0x1c, 0x00, 0x00, 0x7f, 0x00, 
	0x00, 0xfe, 0x00, 0x18, 0x00, 0x00, 0x7f, 0x00, 0x00, 0xfe, 0x00, 0x18, 0x00, 0x00, 0x7f, 0x00, 
	0x00, 0xfc, 0x00, 0x38, 0x00, 0x00, 0x3f, 0x00, 0x00, 0xfc, 0x00, 0x38, 0x00, 0x00, 0x3f, 0x00, 
	0x00, 0xf8, 0x01, 0x38, 0x00, 0x84, 0x1f, 0x00, 0x00, 0xf8, 0x01, 0x70, 0x00, 0x80, 0x1f, 0x00, 
	0x00, 0xf0, 0x01, 0x70, 0x00, 0x82, 0x0f, 0x00, 0x00, 0xf0, 0x01, 0x70, 0x00, 0x80, 0x0f, 0x00, 
	0x00, 0xe0, 0x03, 0xe0, 0x00, 0xc0, 0x07, 0x00, 0x00, 0xe0, 0x03, 0xe0, 0x00, 0xc4, 0x07, 0x00, 
	0x00, 0xc0, 0x07, 0xe0, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xc0, 0x07, 0xc0, 0x01, 0xe2, 0x03, 0x00, 
	0x00, 0xc0, 0x0f, 0xc0, 0x01, 0xf0, 0x03, 0x00, 0x00, 0x80, 0x0f, 0x80, 0x23, 0xf1, 0x01, 0x00, 
	0x00, 0x80, 0x1f, 0x80, 0x83, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x07, 0xfc, 0x00, 0x00, 
	0x00, 0x00, 0x7f, 0x00, 0x27, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x0e, 0x7f, 0x00, 0x00, 
	0x00, 0x00, 0xfe, 0x01, 0x8e, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x07, 0xfc, 0x3f, 0x00, 0x00, 
	0x00, 0x00, 0xfc, 0x7f, 0xfe, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 
	0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 
	0x00, 0x00, 0xf0, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x0f, 0x00, 0x00, 
	0x00, 0x00, 0xe0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x07, 0x00, 0x00, 
	0x00, 0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 
	0x00, 0x00, 0x80, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x01, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int main() {
    // Inicializa a stdio
    stdio_init_all();

    // Configuração do GPIO do LED como saída
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, false);  // Inicialmente, o LED está apagado

    // Configuração do GPIO do Botão A como entrada com pull-up interno
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Inicialização do I2C
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização do display OLED
    ssd1306_t ssd_bm;
    ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd_bm);

    

    while (true) {
        // Lê o estado do Botão A
        bool button_a_state = gpio_get(BUTTON_A);  // HIGH = solto, LOW = pressionado
        bool button_b_state = gpio_get(BUTTON_B);
        // Atualiza o estado do LED com base no estado do Botão A
        gpio_put(LED_BLUE, button_a_state);  // Se solto (HIGH), LED acende; se pressionado (LOW), apaga

        // Verifica se o botão foi pressionado
        if (!button_a_state) {  // Botão pressionado (LOW)
            // Desenha o bitmap no display
            ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
        } 
        if(!button_b_state){
            ssd1306_draw_bitmap(&ssd_bm, bitmap_128x642);
        }

        // Pequeno delay para evitar leituras inconsistentes (debounce simples)
        sleep_ms(50);
    }

    return 0;
}