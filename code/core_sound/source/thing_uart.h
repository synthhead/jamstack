#include "fsl_lpuart.h"
#include "central.h"
#include "testseq.h"

#define EXT_UART            LPUART6
#define EXT_LPUART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define UART_BUFFER_LENGTH  4

void LPUART_UserCallback(LPUART_Type* base, lpuart_handle_t* handle, status_t status, void* userData);

lpuart_handle_t g_uart_handle;

static uint8_t       g_txBuffer[UART_BUFFER_LENGTH] = {0};
static uint8_t       g_rxBuffer[UART_BUFFER_LENGTH] = {0};
static volatile bool rxBufferEmpty                  = true;
static volatile bool txBufferFull                   = false;
static volatile bool txOnGoing                      = false;
static volatile bool rxOnGoing                      = false;

lpuart_config_t   config;
lpuart_transfer_t xfer;
lpuart_transfer_t sendXfer;
lpuart_transfer_t receiveXfer;

void LPUART_UserCallback(LPUART_Type* base, lpuart_handle_t* handle, status_t status, void* userData)
{
  userData = userData;

  if(kStatus_LPUART_TxIdle == status)
  {
    // PRINTF("UART Cb TxIdle \r\n");

    txBufferFull = false;
    txOnGoing    = false;
  }

  if(kStatus_LPUART_RxIdle == status)
  {
    // PRINTF("UART Cb RxIdle \r\n");

    rxBufferEmpty = false;
    rxOnGoing     = false;
  }
}

void uart_init()
{
  /*
   LPUART_GetDefaultConfig:
   config.baudRate_Bps = 115200U;
   config.parityMode = kLPUART_ParityDisabled;
   config.stopBitCount = kLPUART_OneStopBit;
   config.txFifoWatermark = 0;
   config.rxFifoWatermark = 0;
   config.enableTx = false;
   config.enableRx = false;
   */
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
  config.enableTx     = true;
  config.enableRx     = true;

  LPUART_Init(EXT_UART, &config, EXT_LPUART_CLK_FREQ);
  LPUART_TransferCreateHandle(EXT_UART, &g_uart_handle, LPUART_UserCallback, NULL);

  sendXfer.data        = g_txBuffer;
  sendXfer.dataSize    = UART_BUFFER_LENGTH;
  receiveXfer.data     = g_rxBuffer;
  receiveXfer.dataSize = UART_BUFFER_LENGTH;
}

void do_uart_thing()
{
  /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
  if((!rxOnGoing) && rxBufferEmpty)
  {
    rxOnGoing = true;
    LPUART_TransferReceiveNonBlocking(EXT_UART, &g_uart_handle, &receiveXfer, NULL);
    // PRINTF("UART Listening... \r\n");
  }

  /* If TX is idle and g_txBuffer is full, start to send data. */
  if((!txOnGoing) && txBufferFull)
  {
    txOnGoing = true;

    // NoteOn (Musical keyboard)
    if(sendXfer.data[0] == 11)
    {
      core_sound::central.InternalNoteOn(0U, sendXfer.data[1], 127U);
    }
    // NoteOff (Musical keyboard)
    else if(sendXfer.data[0] == 12)
    {
      core_sound::central.InternalNoteOff(0U, sendXfer.data[1], 0U);
    }
    // CC (Encoder Dials)
    else if(sendXfer.data[0] == 13)
    {
      core_sound::central.ControlChange(0U, sendXfer.data[1], sendXfer.data[2]);
      PRINTF("Tag 13: %d %d\r\n", sendXfer.data[1], sendXfer.data[2]);
    }
    else
    {
      PRINTF("Tres: %u %u %u\r\n", sendXfer.data[0], sendXfer.data[1], sendXfer.data[2]);
    }

    LPUART_TransferSendNonBlocking(EXT_UART, &g_uart_handle, &sendXfer);
  }

  /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
  if((!rxBufferEmpty) && (!txBufferFull))
  {
    memcpy(g_txBuffer, g_rxBuffer, UART_BUFFER_LENGTH);
    rxBufferEmpty = true;
    txBufferFull  = true;
  }
}
