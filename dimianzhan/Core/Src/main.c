/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_it.h"
#include "shijue.h"
#include "bsp_sys.h"
#include "oled.h"
#include "bsp_delay.h"
#include "stdio.h"
#include "shijue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */



int fputc(int ch,FILE *f){
	HAL_UART_Transmit(&huart2,(uint8_t *)&ch,1,0xffff);
}
void HMI_send_string(char* name, char* showdata)
{
    // printf("t0.txt=\"%d\"\xff\xff\xff", num);
    printf("%s=\"%s\"\xff\xff\xff", name, showdata);
}
void HMI_send_number(char* name, int num)
{
    // printf("t0.txt=\"%d\"\xff\xff\xff", num);
    printf("%s=%d\xff\xff\xff", name, num);
}
void HMI_send_float(char* name, float num)
{
    // printf("t0.txt=\"%d\"\xff\xff\xff", num);
    printf("%s=%d\xff\xff\xff", name, (int)(num * 100));
}
void HMI_send_shuzu(int id,int num)
{
    // printf("t0.txt=\"%d\"\xff\xff\xff", num);
	printf("p[5].b[%d].val=%d\xff\xff\xff",id,num);
}


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#include <stdio.h>


char strArray[24][3] = {
        "A3", "A6", "A5", "A2", "A1", "A4",
        "B6", "B3", "B2", "B5", "B4", "B1",
        "C3", "C6", "C5", "C2", "C1", "C4",
        "D6", "D5", "D4", "D1", "D2", "D3",
    };
 



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
u8 send_time=0;uint8_t buf[5];u8 send_time2=0;
u8 pos_huowu=30;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
u8 str=1;//定义要发送的数据
u8 str_buff1[64];
u8 duiying[24][24]={0};
u8 hangxian_num=0;
u8 a=0,b=0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
u8 i=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim7);  //开启总中断
	HAL_UART_Receive_IT(&huart1,&uart1_rxbuff,1);  //通信
	HAL_UART_Receive_IT(&huart2,&uart2_rxbuff,1);  //通信
	u8 last_huowu_num=0;
	u8 task2_flag=0;
	u8 task2_shibie_flag=0;
	u8 last_huowu=0,task1_over=0;
//	delay_ms(100);    //延时一下，让OLED正常输出
//	oled_init();
	//delay_ms(100);    //延时一下，让OLED正常输出
	
 // oled_clear(); 
	//__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);    //串口1
//char a="X";
//sprintf((char *)str_buff1, "POS_X %d", Pos_x.S16);   

  // 在OLED上显示

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		 //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
   //oled_display_ok_flag(0, 0);

		if(send_time>=15){
		BlueToothTransmitData();
			send_time=0;
		}
		if(send_time2>=5){
		HMI_send_number("page0.n0.val", state);
		if(num!=0){
		HMI_send_number("page1.n1.val", huowu_num[num-1]);
		HMI_send_string("page1.t2.txt", strArray[num-1]);
		HMI_send_shuzu( num,huowu_num[num-1]);
		}
		if(jieshou_flag==1&&huowu_num[24]!=0) {
			for(int i=0;i<=23;i++){
				if(huowu_num[24]==huowu_num[i]){
					hangxian_num=i;
					
				}
			}
			HMI_send_string("page2.b3.txt", strArray[hangxian_num]);
			HMI_send_string("page4.t0.txt", strArray[hangxian_num]);
			HMI_send_number("page4.n0.val", huowu_num[24]);
		}
		send_time2=0;
	}
	
		

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
