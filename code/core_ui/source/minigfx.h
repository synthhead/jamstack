#ifndef MINIGFX_H_
#define MINIGFX_H_

#include "board.h"
#include "pin_mux.h"

#include "fsl_device_registers.h"
#include "fsl_lpspi.h"

#define OLED_DC_GPIO  BOARD_INITPINS_OLED_DC_PERIPHERAL
#define OLED_DC_PIN   BOARD_INITPINS_OLED_DC_CHANNEL
#define OLED_EN_GPIO  BOARD_INITPINS_OLED_EN_PERIPHERAL
#define OLED_EN_PIN   BOARD_INITPINS_OLED_EN_CHANNEL
#define OLED_RST_GPIO BOARD_INITPINS_OLED_RST_PERIPHERAL
#define OLED_RST_PIN  BOARD_INITPINS_OLED_RST_CHANNEL

#define OLED_HEIGHT 64
#define OLED_WIDTH  256

#define LPSPI_MASTER_BASEADDR   (LPSPI1)
#define LPSPI_MASTER_IRQN       LPSPI1_IRQn
#define LPSPI_MASTER_IRQHandler LPSPI1_IRQHandler

#define LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs1)
#define LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs1)
/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (0U)
#define LPSPI_MASTER_CLK_FREQ      (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (LPSPI_CLOCK_SOURCE_DIVIDER + 1U))
#define TARGET_TRANSFER_BAUDRATE   10000000UL

void LPSPI_MasterUserCallback(LPSPI_Type* base, lpspi_master_handle_t* handle, status_t status, void* userData);
void LPSPI_Init(void);
void drive_reset_low(void);
void drive_reset_high(void);
void drive_dc_low(void);
void drive_dc_high(void);
void drive_en_low(void);
void drive_en_high(void);
void SPI_send_cmd(uint8_t cmd);
void SPI_send_byte(uint8_t data);

lpspi_master_handle_t g_spi_handle;
volatile bool         g_spi_isTransferCompleted = false;
// not sure if all has to be globals
uint32_t         spiErrorCount;
lpspi_transfer_t spiXfer;
// should be const

void LPSPI_MasterUserCallback(LPSPI_Type* base, lpspi_master_handle_t* handle, status_t status, void* userData)
{
  if(status == kStatus_Success)
    __NOP();

  g_spi_isTransferCompleted = true;
}

void LPSPI_Init(void)
{
  uint32_t              spiSrcClock_Hz;
  lpspi_master_config_t spiMasterConfig;

  /*Set clock source for LPSPI*/
  CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);

  LPSPI_MasterGetDefaultConfig(&spiMasterConfig);
  spiMasterConfig.baudRate     = TARGET_TRANSFER_BAUDRATE;
  spiMasterConfig.whichPcs     = LPSPI_MASTER_PCS_FOR_INIT;
  spiMasterConfig.direction    = kLPSPI_MsbFirst;
  spiMasterConfig.bitsPerFrame = 8U;

  spiSrcClock_Hz = LPSPI_MASTER_CLK_FREQ;
  LPSPI_MasterInit(LPSPI_MASTER_BASEADDR, &spiMasterConfig, spiSrcClock_Hz);

  LPSPI_MasterTransferCreateHandle(LPSPI_MASTER_BASEADDR, &g_spi_handle, LPSPI_MasterUserCallback, NULL);

  /* Delay to wait slave is ready */
  // if(SysTick_Config(SystemCoreClock / 1000U))
  // {
  //   while(1) {}
  // }

  PRINTF("LPSPI Init done.\r\n");
}

void drive_reset_low(void)
{
  OLED_RST_GPIO->DR &= ~(1UL << OLED_RST_PIN);
}

void drive_reset_high(void)
{
  OLED_RST_GPIO->DR |= (1UL << OLED_RST_PIN);
}

void drive_dc_low(void)
{
  OLED_DC_GPIO->DR &= ~(1UL << OLED_DC_PIN);
}

void drive_dc_high(void)
{
  OLED_DC_GPIO->DR |= (1UL << OLED_DC_PIN);
}

void drive_en_low(void)
{
  OLED_EN_GPIO->DR &= ~(1UL << OLED_EN_PIN);
}

void drive_en_high(void)
{
  OLED_EN_GPIO->DR |= (1UL << OLED_EN_PIN);
}

inline void SPI_start_transfer(void)
{
  LPSPI_MasterTransferNonBlocking(LPSPI_MASTER_BASEADDR, &g_spi_handle, &spiXfer);

  /* Wait transfer complete */
  // Todo: Block ?!
  while(!g_spi_isTransferCompleted) {}
}

void SPI_send_cmd(uint8_t cmd)
{
  drive_dc_low();

  g_spi_isTransferCompleted = false;
  spiXfer.txData            = &cmd;
  spiXfer.rxData            = NULL;
  spiXfer.dataSize          = 1U;
  spiXfer.configFlags       = LPSPI_MASTER_PCS_FOR_TRANSFER | //
                        kLPSPI_MasterPcsContinuous |          //
                        kLPSPI_SlaveByteSwap;
  SPI_start_transfer();
}

void SPI_send_byte(uint8_t data)
{
  drive_dc_high();

  g_spi_isTransferCompleted = false;
  spiXfer.txData            = &data;
  spiXfer.rxData            = NULL;
  spiXfer.dataSize          = 1U;
  spiXfer.configFlags       = LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap;

  SPI_start_transfer();
}

void SPI_send_data_array(uint8_t* data, uint32_t size)
{
  drive_dc_high();

  g_spi_isTransferCompleted = false;
  spiXfer.txData            = data;
  spiXfer.rxData            = NULL;
  spiXfer.dataSize          = size;
  spiXfer.configFlags       = LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap;

  SPI_start_transfer();
}

void fillBuffer(uint8_t* fb, uint8_t brightness)
{
  uint8_t  byte_value  = (brightness << 4) | brightness;
  uint32_t buffer_size = OLED_HEIGHT * OLED_WIDTH / 2;
  while(buffer_size--)
  {
    *fb++ = byte_value;
  }
}

void init_ssd1322(void)
{
  drive_reset_low();

  for(uint16_t i = 0; i < 50000; i++)
    __NOP();

  drive_reset_high();

  for(uint32_t i = 0; i < 100000; i++)
    __NOP();

  SPI_send_cmd(0xFD); //set Command unlock
  SPI_send_byte(0x12);
  SPI_send_cmd(0xAE); //set display off
  SPI_send_cmd(0xB3); //set display clock divide ratio
  SPI_send_byte(0x91);
  SPI_send_cmd(0xCA); //set multiplex ratio
  SPI_send_byte(0x3F);
  SPI_send_cmd(0xA2); //set display offset to 0
  SPI_send_byte(0x00);
  SPI_send_cmd(0xA1); //start display start line to 0
  SPI_send_byte(0x00);
  SPI_send_cmd(0xA0); //set remap and dual COM Line Mode
  // SPI_send_byte(0x14);
  SPI_send_byte(0x06);
  SPI_send_byte(0x11);
  SPI_send_cmd(0xAB);  // Set Function Selection
  SPI_send_byte(0x01); // Internal VDD regulator
  SPI_send_cmd(0xB4);  // Set Display Enhancement A
  SPI_send_byte(0xA0); // Enable external VSL
  SPI_send_byte(0xFD); // Enhanced low GS display quality
  // SPI_send_byte(0xB5); // Normal
  SPI_send_cmd(0xC1);  // Set Contrast Current
  SPI_send_byte(0xFF); // FF is the brightest
  // SPI_send_byte(0x9F);
  SPI_send_cmd(0xC7);  // Set Master Contrast Current
  SPI_send_byte(0x0F); // Highest Current (0b0001111)
  SPI_send_cmd(0xB5);  //disable IO input
  SPI_send_byte(0x00);
  SPI_send_cmd(0xB9); //default grayscale
  SPI_send_cmd(0xB1); //set phase length
  SPI_send_byte(0xE2);
  SPI_send_cmd(0xD1);  // Enhance driving scheme capability
  SPI_send_byte(0xA2); // 0x82 should be A2?
  SPI_send_byte(0x20); // end sequence for ^
  SPI_send_cmd(0xBB);  // First pre charge voltage
  SPI_send_byte(0x1F); // Max
  SPI_send_cmd(0xB6);  // Second pre charge voltage
  SPI_send_byte(0x08);
  SPI_send_cmd(0xBE);  // Set VCOMH Voltage
  SPI_send_byte(0x07); // Max
  SPI_send_cmd(0xA6);  //set normal display mode
  SPI_send_cmd(0xA9);  //no partial mode

  for(uint16_t i = 0; i < 50000; i++)
    __NOP();

  SPI_send_cmd(0xAF); //display on

  for(uint16_t i = 0; i < 50000; i++)
    __NOP();

  // PRINTF("SSD1322 init done.\r\n");
}

void set_window(uint8_t start_column, uint8_t end_column, uint8_t start_row, uint8_t end_row)
{
  SPI_send_cmd(0x15); // SET_COLUMN_ADDR
  SPI_send_byte(28 + start_column);
  SPI_send_byte(28 + end_column);
  SPI_send_cmd(0x75); // SET_ROW_ADDR
  SPI_send_byte(start_row);
  SPI_send_byte(end_row);
}

void send_buffer(uint8_t* buffer, uint32_t buffer_size)
{
  SPI_send_cmd(0x5C); // ENABLE_RAM_WRITE
  SPI_send_data_array(buffer, buffer_size);
}

void send_buffer_to_OLED(uint8_t* fb, uint16_t start_x, uint16_t start_y)
{
  set_window(0, 63, 0, 127);
  send_buffer(fb + (start_y * OLED_WIDTH / 2) + start_x, 8192);
}

void drawPixel(uint8_t* fb, uint16_t x, uint16_t y, uint8_t brightness)
{
  if(x > (OLED_WIDTH - 1) || y > (OLED_HEIGHT - 1))
    return;

  if((y * OLED_WIDTH + x) % 2 == 1)
    fb[((y * OLED_WIDTH) + x) / 2] = (fb[((y * OLED_WIDTH) + x) / 2] & 0xF0) | brightness;
  else
    fb[((y * OLED_WIDTH) + x) / 2] = (fb[((y * OLED_WIDTH) + x) / 2] & 0x0F) | (brightness << 4);
}

void drawVLine(uint8_t* fb, uint16_t x, uint16_t y0, uint16_t y1, uint8_t brightness)
{
  if(y0 < y1)
  {
    for(uint8_t i = y0; i <= y1; i++)
      drawPixel(fb, x, i, brightness);
  }
  else
  {
    for(uint8_t i = y1; i <= y0; i++)
      drawPixel(fb, x, i, brightness);
  }
}

void drawHLine(uint8_t* fb, uint16_t y, uint16_t x0, uint16_t x1, uint8_t brightness)
{
  if(x0 < x1)
  {
    for(uint8_t i = x0; i <= x1; i++)
      drawPixel(fb, i, y, brightness);
  }
  else
  {
    for(uint8_t i = x1; i <= x0; i++)
      drawPixel(fb, i, y, brightness);
  }
}

void drawLine(uint8_t* fb, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t brightness)
{
  //handle horizontal and vertical lines with appropriate functions
  if(x0 == x1)
    drawVLine(fb, x0, y0, y1, brightness);

  if(y0 == y1)
    drawHLine(fb, y0, x0, x1, brightness);

  int16_t steep = abs(y1 - y0) > abs(x1 - x0);

  if(steep == 1)
  {
    uint16_t tmp = y0;

    y0  = x0;
    x0  = tmp;
    tmp = y1;
    y1  = x1;
    x1  = tmp;
  }

  if(x0 > x1)
  {
    uint16_t tmp = x0;

    x0  = x1;
    x1  = tmp;
    tmp = y0;
    y0  = y1;
    y1  = tmp;
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if(y0 < y1)
    ystep = 1;
  else
    ystep = -1;

  for(; x0 <= x1; x0++)
  {
    if(steep)
      drawPixel(fb, y0, x0, brightness);
    else
      drawPixel(fb, x0, y0, brightness);

    err -= dy;
    if(err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void drawRect(uint8_t* fb, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t brightness)
{
  drawVLine(fb, x0, y0, y1, brightness);
  drawVLine(fb, x1, y0, y1, brightness);
  drawHLine(fb, y0, x0, x1, brightness);
  drawHLine(fb, y1, x0, x1, brightness);
}

void fillRect(uint8_t* fb, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t shade)
{
  for(uint8_t i = x0; i <= x1; i++)
  {
    for(uint8_t j = y0; j <= y1; j++)
    {
      drawPixel(fb, i, j, shade);
    }
  }
}

void drawCircle(uint8_t* fb, uint16_t x0, uint16_t y0, uint16_t r, uint8_t shade)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  drawPixel(fb, x0, y0 + r, shade);
  drawPixel(fb, x0, y0 - r, shade);
  drawPixel(fb, x0 + r, y0, shade);
  drawPixel(fb, x0 - r, y0, shade);

  while(x < y)
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(fb, x0 + x, y0 + y, shade);
    drawPixel(fb, x0 - x, y0 + y, shade);
    drawPixel(fb, x0 + x, y0 - y, shade);
    drawPixel(fb, x0 - x, y0 - y, shade);
    drawPixel(fb, x0 + y, y0 + x, shade);
    drawPixel(fb, x0 - y, y0 + x, shade);
    drawPixel(fb, x0 + y, y0 - x, shade);
    drawPixel(fb, x0 - y, y0 - x, shade);
  }
}

///
///
///
///
///

/// OLED
#define MIN(a, b)     (((a) < (b)) ? (a) : (b))
#define MAX(a, b)     (((a) > (b)) ? (a) : (b))
#define CLIP(a, b, c) MIN(MAX((a), (b)), (c))
#define MY_PI         3.14159265358979323846264338327950288

void drawGradients(uint8_t* fb)
{
  for(int j = 0; j < 12; j++)
  {
    for(int i = 0; i < 20; i++)
    {
      drawVLine(fb, i + (j * 20), 0, 63, MAX(1, MIN(i, 15)));
    }
  }
}

void drawSinRecScreen(uint8_t* fb)
{
  static float test_sin = 0;

  //   selectFont(&FreeMono12pt7b);
  float mysin = sin(test_sin);
  //   char text[5];
  //   sprintf(text, "sin %.02f", mysin);
  //   drawText(oled_buf, text, 10, 20, 15);

  fillRect(fb, 210, 5, 220, 15, (int)((mysin + 1.0) * 7.5));
  fillRect(fb, 210, 5, 220, 15, (int)((mysin + 1.0) * 7.5));
  drawPixel(fb, 127 + (int16_t)(mysin * 100.0), 40, 8);
  drawCircle(fb, 127 + (int16_t)(mysin * 100.0), 40, 10, 15);
  send_buffer_to_OLED(fb, 0, 0);

  test_sin += 0.08;
  //   delay(16);
}

void drawReel(uint8_t* fb, uint8_t offset_x, uint8_t offset_y, uint8_t radius, float speed)
{
  static float a1 = 0; //MY_PI;
  static float a2 = MY_PI / 1.5;
  static float a3 = MY_PI / 1.5 * 2;

  float r1 = -10 + offset_y;
  float r2 = -15 + offset_y;

  float sin_a = sin(a1);
  float cos_a = cos(a1);

  float sin_b = sin(a2);
  float cos_b = cos(a2);

  float sin_c = sin(a3);
  float cos_c = cos(a3);

  float ax1 = r1 * sin_a + offset_x;
  float ay1 = r1 * cos_a + offset_y;
  float ax2 = r2 * sin_a + offset_x;
  float ay2 = r2 * cos_a + offset_y;

  float bx1 = r1 * sin_b + offset_x;
  float by1 = r1 * cos_b + offset_y;
  float bx2 = r2 * sin_b + offset_x;
  float by2 = r2 * cos_b + offset_y;

  float cx1 = r1 * sin_c + offset_x;
  float cy1 = r1 * cos_c + offset_y;
  float cx2 = r2 * sin_c + offset_x;
  float cy2 = r2 * cos_c + offset_y;

  drawCircle(fb, offset_x, offset_y, radius, 1);
  drawCircle(fb, offset_x, offset_y, radius - 4, 6);
  drawCircle(fb, offset_x, offset_y, 3, 1);
  drawPixel(fb, offset_x, offset_y, 15);
  drawLine(fb, ax2, ay2, ax1, ay1, 4);
  drawLine(fb, bx2, by2, bx1, by1, 4);
  drawLine(fb, cx2, cy2, cx1, cy1, 4);

  a1 += speed;
  a2 += speed;
  a3 += speed;
}

void drawTapeScreen(uint8_t* fb)
{
  static uint8_t counter      = 0;
  static uint8_t wait_counter = 0;

  fillBuffer(fb, 0);

  drawReel(fb, 50, 23, 23, 0.03);
  drawReel(fb, 130, 23, 18, 0.03);

  // left thingy
  drawCircle(fb, 60, 59, 3, 1);
  // right thingy
  drawCircle(fb, 120, 59, 3, 1);
  // heads deck
  fillRect(fb, 81, 44, 99, 54, 1);
  drawRect(fb, 81, 44, 99, 54, 4);
  // top tape
  drawLine(fb, counter < 25 ? 55 : 53, 4, /* counter < 25 ? 130 : 132 */ 130, 9, 4);
  // left tape
  drawLine(fb, 37, 38, 57, 60, 4);
  // bottom 1
  drawLine(fb, 63, 60, 81, 50, 4);
  // bottom 2
  drawLine(fb, 100, 50, 117, 60, 4);
  // right tape
  drawLine(fb, 140, 34, 123, 60, 4);

  if(wait_counter > 2)
  {
    counter++;
    wait_counter = 0;
  }

  wait_counter++;

  if(counter > 50)
  {
    counter = 0;
  }
}

#define MK_B_WID  8
#define MK_B_HEI  8
#define MK_X_SPA  3
#define MK_Y_SPA  3
#define MK_X_OSET 60
#define MK_Y_OSET 42

#define FK_B_WID  5
#define FK_B_HEI  5
#define FK_X_SPA  3
#define FK_Y_SPA  3
#define FK_X_OSET 60
#define FK_Y_OSET 0

#define EK_B_WID  5
#define EK_B_HEI  5
#define EK_X_SPA  3
#define EK_Y_SPA  3
#define EK_X_OSET 182
#define EK_Y_OSET 0

#define EVK_B_WID  1
#define EVK_X_SPA  3
#define EVK_X_OSET 227
#define EVK_Y_OSET 0

#define JS_B_WID  5
#define JS_B_HEI  5
#define JS_X_OSET 16
#define JS_Y_OSET 6

void drawOneFunc(uint8_t* fb, uint16_t x0, uint16_t y0, uint8_t state)
{
  if(state)
  {
    fillRect(fb, x0, y0, x0 + FK_B_WID, y0 + FK_B_HEI, 15);
    drawPixel(fb, x0, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0 + FK_B_HEI, 0);
    drawPixel(fb, x0, y0 + FK_B_HEI, 0);
  }
  else
  {
    drawRect(fb, x0, y0, x0 + FK_B_WID, y0 + FK_B_HEI, 2);
    drawPixel(fb, x0, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0 + FK_B_HEI, 0);
    drawPixel(fb, x0, y0 + FK_B_HEI, 0);
  }
}

void drawFunctionKeys(uint8_t* fb, uint8_t funcKeys[])
{
  for(uint8_t i = 0; i < 8; i++)
  {
    drawOneFunc(fb, FK_X_OSET + ((FK_B_WID + FK_X_SPA) * i), FK_Y_OSET, funcKeys[i]);
  }
  for(uint8_t i = 0; i < 8; i++)
  {
    drawOneFunc(fb, FK_X_OSET + ((FK_B_WID + FK_X_SPA) * i), FK_Y_OSET + FK_B_HEI + FK_Y_SPA, funcKeys[i + 8]);
  }
}

void drawOneEncKey(uint8_t* fb, uint16_t x0, uint16_t y0, uint8_t state)
{
  if(state)
  {
    fillRect(fb, x0, y0, x0 + FK_B_WID, y0 + FK_B_HEI, 15);
    drawPixel(fb, x0, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0 + FK_B_HEI, 0);
    drawPixel(fb, x0, y0 + FK_B_HEI, 0);
  }
  else
  {
    drawRect(fb, x0, y0, x0 + FK_B_WID, y0 + FK_B_HEI, 2);
    drawPixel(fb, x0, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0, 0);
    drawPixel(fb, x0 + FK_B_WID, y0 + FK_B_HEI, 0);
    drawPixel(fb, x0, y0 + FK_B_HEI, 0);
  }
}

void drawEncKeys(uint8_t* fb, uint8_t encButtons[])
{
  for(uint8_t i = 0; i < 4; i++)
  {
    drawOneEncKey(fb, EK_X_OSET + ((EK_B_WID + EK_X_SPA) * i), EK_Y_OSET, encButtons[i]);
  }
  for(uint8_t i = 0; i < 4; i++)
  {
    drawOneEncKey(fb, EK_X_OSET + ((EK_B_WID + EK_X_SPA) * i), EK_Y_OSET + EK_B_HEI + EK_Y_SPA, encButtons[i + 4]);
  }
}

void drawThinSlider(uint8_t* fb, uint16_t x0, uint16_t y0, uint8_t val)
{
  drawVLine(fb, x0, y0, 63, 2);
  if(val)
  {
    uint8_t remap = 63 - val;
    drawVLine(fb, x0, y0 + remap, 63, 15);
  }
}

void drawEncValues(uint8_t* fb, uint8_t encDials[], uint8_t touchSliderValues[], uint8_t joyXY[], uint8_t joySwitch, uint8_t lbKeys[])
{
  for(uint8_t i = 0; i < 8; i++)
    drawThinSlider(fb, EVK_X_OSET + ((EVK_B_WID + EVK_X_SPA) * i), 0, encDials[i]);

  // just do it 4 times
  for(int i = 4 - 1; i >= 0; i--)
    drawThinSlider(fb, i, 0, touchSliderValues[0] / 4U);

  uint8_t jbColor = joySwitch ? 15 : 2;
  drawHLine(fb, 0, JS_X_OSET, JS_X_OSET + 32, jbColor);
  drawHLine(fb, 1, JS_X_OSET, JS_X_OSET + 32, jbColor);
  drawHLine(fb, 2, JS_X_OSET, JS_X_OSET + 32, jbColor);

  uint8_t jbRemapX = CLIP(31 - (joyXY[0] >> 3), 1, 31);
  uint8_t jbRemapY = CLIP(31 - (joyXY[1] >> 3), 1, 31);

  drawRect(fb, JS_X_OSET, JS_Y_OSET, JS_X_OSET + 32, JS_Y_OSET + 32, 2);
  drawPixel(fb, JS_X_OSET + jbRemapX + 1, JS_Y_OSET + jbRemapY + 1, 8);
  drawPixel(fb, JS_X_OSET + jbRemapX - 1, JS_Y_OSET + jbRemapY - 1, 8);
  drawPixel(fb, JS_X_OSET + jbRemapX + 1, JS_Y_OSET + jbRemapY - 1, 8);
  drawPixel(fb, JS_X_OSET + jbRemapX - 1, JS_Y_OSET + jbRemapY + 1, 8);
  drawPixel(fb, JS_X_OSET + jbRemapX, JS_Y_OSET + jbRemapY, 8);

  drawOneFunc(fb, JS_X_OSET, JS_Y_OSET + 32 + 4, lbKeys[0]);
  drawOneFunc(fb, JS_X_OSET + 10, JS_Y_OSET + 32 + 4, lbKeys[1]);
  drawOneFunc(fb, JS_X_OSET + 20, JS_Y_OSET + 32 + 4, lbKeys[2]);

  // PRINTF("Joystick values: %d\t%d\r\n", jbRemapX, jbRemapY);
}

#define MK_POS0_LEN 10
#define MK_POS1_LEN 14
uint8_t mkPos0[MK_POS0_LEN] = {1, 3, 6, 8, 10, 13, 15, 18, 20, 22};
uint8_t mkPos1[MK_POS1_LEN] = {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23};

void drawOneKey(uint8_t* fb, uint16_t x0, uint16_t y0, uint8_t state)
{
  if(state)
  {
    fillRect(fb, x0, y0, x0 + MK_B_WID, y0 + MK_B_HEI, 15);
  }
  else
  {
    drawRect(fb, x0, y0, x0 + MK_B_WID, y0 + MK_B_HEI, 2);
  }
}

void drawMusicKeyboard(uint8_t* fb, uint8_t musicKeys[])
{
  for(uint8_t i = 0, j = 0; i < MK_POS1_LEN - 1; i++)
  {
    if(i == 2 || i == 6 || i == 9)
      continue;

    drawOneKey(fb, MK_X_OSET + ((MK_B_WID + MK_X_SPA) * i) + (MK_B_WID - 2), MK_Y_OSET, musicKeys[mkPos0[j]]);
    j++;
  }

  for(uint8_t i = 0; i < MK_POS1_LEN; i++)
  {
    drawOneKey(fb, MK_X_OSET + ((MK_B_WID + MK_X_SPA) * i), MK_Y_OSET + MK_B_HEI + MK_Y_SPA, musicKeys[mkPos1[i]]);
  }
}

void drawTestUiScreen(uint8_t* fb,
                      uint8_t  funcKeys[],
                      uint8_t  encButtons[],
                      uint8_t  musicKeys[],
                      uint8_t  encDials[],
                      uint8_t  touchSliderValues[],
                      uint8_t  joyXY[],
                      uint8_t  joySwitch,
                      uint8_t  lbKeys[])
{
  drawFunctionKeys(fb, funcKeys);
  drawEncKeys(fb, encButtons);
  drawMusicKeyboard(fb, musicKeys);
  drawEncValues(fb, encDials, touchSliderValues, joyXY, joySwitch, lbKeys);
}

#endif /* MINIGFX_H_ */
