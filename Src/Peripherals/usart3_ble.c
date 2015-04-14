/****************************************************************
*																*
*    Created by Xuejilong										*
*		Driver for BLE Chip RF-BM-S02						*
****************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usart3_ble.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GPIO_ResetBits(x, y) HAL_GPIO_WritePin(x, y, GPIO_PIN_RESET)
#define GPIO_SetBits(x, y) HAL_GPIO_WritePin(x, y, GPIO_PIN_SET)
/* Private variables ---------------------------------------------------------*/
Command_Packet gCommand_Packet;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Initialize BLE.
* @param  None
* @retval None
*/

void BLE_Init( void )
{
}

/**
* @brief  Set BLE Mode, must be set before Power On(not EN pin).
* @param  0 test;other normal
* @retval None
*/

void BLE_Mode( uint8_t mode )
{
  if(mode){
	GPIO_ResetBits(BLE_PWM1_OR_MODE_GPIO_PORT, BLE_PWM1_OR_MODE_PIN);
  }
  else{
	GPIO_SetBits(BLE_PWM1_OR_MODE_GPIO_PORT, BLE_PWM1_OR_MODE_PIN);
  }
}

/**
* @brief  Reset Power.
* @param  0 disable;other enable
* @retval None
*/

void BLE_Power_Enable( uint8_t enable )
{
  if(enable){
	GPIO_SetBits(BLE_RES_GPIO_PORT, BLE_RES_PIN);
  }
  else{
	GPIO_ResetBits(BLE_RES_GPIO_PORT, BLE_RES_PIN);
  }
}

/**
* @brief  Enable BLE BroadCast.
* @param  0 disable;other enable
* @retval None
*/

void BLE_BroadCast_Enable( uint8_t enable )
{
  if(enable){
	GPIO_ResetBits(BLE_EN_GPIO_PORT, BLE_EN_PIN);
  }
  else{
	GPIO_SetBits(BLE_EN_GPIO_PORT, BLE_EN_PIN);
  }
}

/**
* @brief  Enable BLE Input.
* @param  0 disable;other enable
* @retval None
*/

void BLE_Input_Enable( uint8_t enable )
{
  if(enable){
	GPIO_ResetBits(BLE_BRTS_GPIO_PORT, BLE_BRTS_PIN);
  }
  else{
	GPIO_SetBits(BLE_BRTS_GPIO_PORT, BLE_BRTS_PIN);
  }
}

/**
* @brief  Get BLE Packet.
* @param  Command_Packet
* @retval None
*/

void BLE_GetPacket( Command_Packet *packet )
{
  while(USART3Buf.RecBufChaOldest != USART3Buf.RecBufCurrent)
  {
	if(USART3Buf.RecBufValLen[USART3Buf.RecBufChaOldest] == 0x0f){
	  memcpy(packet, USART3Buf.RecBuf[USART3Buf.RecBufChaOldest] + 1, 0x0e);
	}

	MOVE_OLDEST_BUFFER_TO_NEXT_CHANNEL(USART3Buf);
  }
}

/**
* @brief  Start Read Packet.
* @param  None
* @retval None
*/

void BLE_StartRead(void)
{
  __HAL_UART_ENABLE(&huart3);
  while(HAL_UART_Receive_DMA(&huart3,
							 USART3Buf.RecBuf[USART3Buf.RecBufCurrent],
							 USART_BufferLength) != HAL_OK);
}