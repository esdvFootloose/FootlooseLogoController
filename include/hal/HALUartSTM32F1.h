/*************************************************
 * @file HALUartSTM32F1.h
 * @brief HAL uart
 *
 *************************************************/
#pragma once

#include "HALUart.h"
#include "HALUartSTM32F1.h"

#include "stm32f1xx.h"

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALUartSTM32F1 : public HALUart
{
public:
    enum UartMode
    {
        UartModeRx   = UART_MODE_RX,
        UartModeTx   = UART_MODE_TX,
        UartModeRxTx = UART_MODE_TX_RX
    };

    enum CallBackType
    {
        HAL_UART_TxCpltCallback,
        HAL_UART_TxHalfCpltCallback,
        HAL_UART_RxCpltCallback,
        HAL_UART_RxHalfCpltCallback,
        HAL_UART_ErrorCallback,
        HAL_UART_AbortCpltCallback,
        HAL_UART_AbortTransmitCpltCallback,
        HAL_UART_AbortReceiveCpltCallback,

    };

    HALUartSTM32F1(USART_TypeDef* uart, uint32_t baudRate, UartMode mode);

    virtual void send(uint8_t* data, uint8_t length, uint32_t timeout = 0);

    virtual void receive(uint8_t* data, uint8_t bufferLength, uint32_t timeout = 0);

    virtual void open();

    virtual void close();

    virtual void sendByte(uint8_t);

    virtual uint32_t readByte();

    virtual void registerCallback(CallbackFunction f, CallBack type);

    static void callBack(UART_HandleTypeDef* uart, CallBackType type);

private:
    UART_HandleTypeDef* m_handle;

    HALUart::CallBack getCallback(CallBackType);
};
