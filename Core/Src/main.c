/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "stdlib.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define max_tol_mar 111
#define x_avali 0
#define y_avali 8
#define tedad_avali 1
#define n 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int tedad=n;
uint8_t eep_cntr=10;
char score=0;
char buf ;
struct randomm{
	uint8_t x;
	uint8_t y;
}random;

struct flag {
	int state;
	int step;
	uint8_t rec_snake_app;
	int dly;
	uint8_t jahat;
	uint8_t gameover;
};
struct flag flg ={0,0,0,1000,0,0};

struct makanm {
	int x;
	int y;
}makan[max_tol_mar];

struct jayeize{
	int x;
	int y;
};
struct jayeize makan_qarch_jadid = {0,8};

struct jayeize_fli{
	int x;
	int y;
};
struct jayeize makan_qarch_feli = {0,0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void i2c_send(void);
void header(void);
void gameover(void);
void khoshamad(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//////////////////////////////////////////////////////////////////uart
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		if(flg.rec_snake_app==0x31 && flg.jahat != 1)//left
		{	
			flg.state = 4;
		}
			if(flg.rec_snake_app==0x32 && flg.jahat != 4)//right
		{
			flg.state = 1;
		}
			if(flg.rec_snake_app==0x33 && flg.jahat != 3)//up
		{
			flg.state = 2;
		}
			if(flg.rec_snake_app==0x34 && flg.jahat != 2)//down
		{
			flg.state = 3;
		}
	}
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&flg.rec_snake_app,1);
}
/////////////////////////////////////////////////////////////////////timer
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM1){
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
		__HAL_TIM_SET_AUTORELOAD(&htim14,eep_cntr*2);
		__HAL_TIM_SET_COMPARE(&htim14 ,TIM_CHANNEL_1,eep_cntr );
		eep_cntr++;
		if(eep_cntr>=31){
			eep_cntr=10;
		}
	}
	if(htim->Instance == TIM3){
		makan_qarch_jadid.x+=6;
		if(makan_qarch_jadid.x>=121){
			makan_qarch_jadid.x = 0;
		}
	}
	if(htim->Instance == TIM16){
		makan_qarch_jadid.y+=8;
		if(makan_qarch_jadid.y>=53){
			makan_qarch_jadid.y = 0;
		}
	}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_6 && flg.jahat != 1){ // & flg.state == 
		flg.state = 4;
	}
	if(GPIO_Pin == GPIO_PIN_7 && flg.jahat != 3){
		flg.state = 2;
	}
	if(GPIO_Pin == GPIO_PIN_1 && flg.jahat != 2){
		flg.state = 3;
	}
	if(GPIO_Pin == GPIO_PIN_0 && flg.jahat != 4){
		flg.state = 1;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	makan_qarch_feli.x = (rand()%20)*6;
	makan_qarch_feli.y = (rand()%6)*8;
  /* USER CODE BEGIN 1 */
	makan[0].x=x_avali;
	makan[0].y=y_avali;
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM14_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim16);
	ssd1306_Init();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&flg.rec_snake_app,1);
	HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		i2c_send();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void header(void){
	ssd1306_Fill(Black);
	if(flg.state==1 || flg.state==2 || flg.state ==3 || flg.state==4){
		for(int i=tedad;i>0;--i){
				makan[i].x=makan[i-1].x;
				makan[i].y=makan[i-1].y;
			}
		if (flg.state==1){
			makan[0].x=makan[0].x+6;
			flg.jahat=1;
		}
		if (flg.state==2){
			makan[0].y=makan[0].y-8;
			flg.jahat=2;
		}
		if (flg.state==3){
			makan[0].y=makan[0].y+8;
			flg.jahat=3;
		}
		if (flg.state==4){
			makan[0].x=makan[0].x-6;
			flg.jahat=4;
		}
		
		for(int i=1;i<=tedad;i++){
			if(makan[i].x == makan[0].x && makan[i].y == makan[0].y){
				gameover();
			}
		}
	}
	
		if(makan[0].x == makan_qarch_feli.x && makan[0].y == makan_qarch_feli.y){
			score++;
			random.x = makan_qarch_jadid.x;
			random.y = makan_qarch_jadid.y;
			for(int i=1;i<=tedad;i++){
				if(makan[i].x == random.x && makan[i].y == random.y){
					i=1;
					random.x = (rand()%20)*6;
					random.y = (rand()%6)*8;
				}
			}
			makan_qarch_feli.x = random.x;
			makan_qarch_feli.y = random.y;
			tedad ++;
		}
		
		for(int i=0;i<tedad;i++){
			ssd1306_SetCursor(makan[i].x,makan[i].y);
			ssd1306_WriteString(".",Font_6x8 ,White);
		}
		
		ssd1306_SetCursor(makan_qarch_feli.x,makan_qarch_feli.y);
		ssd1306_WriteString(".",Font_6x8,White);
		
		
		ssd1306_UpdateScreen();
		
		HAL_Delay(flg.dly);
		
		if(makan[0].x >119 || makan[0].x <0 || makan[0].y>53 || makan[0].y<0){
			gameover();
		}
}
void i2c_send(void){
	if(flg.step == 0){
		khoshamad();
	}
	if(flg.step == 1){
		header();
	}
//		ssd1306_UpdateScreen();
//		HAL_Delay(1000);
}
void khoshamad(void){
		ssd1306_Fill(Black);
		flg.state=0;
		ssd1306_SetCursor(0,0);
		HAL_Delay(1);
		ssd1306_WriteString("Easy -> Down",Font_6x8 ,White);
		ssd1306_SetCursor(0,40);
		ssd1306_WriteString("Hard -> Up",Font_6x8 ,White);
		ssd1306_UpdateScreen();
		if(flg.state==2 || flg.state==3){
			if (flg.state==2){
				flg.dly = 100;
			}
			if (flg.state==3){
				flg.dly = 500;
			}
			flg.step=1;
			flg.state=0;
		}
		
}
void gameover(void){
	for(int i=0;i<=4;i++){
		ssd1306_Fill(Black);
		HAL_Delay(350);
		ssd1306_SetCursor(0,0);
		HAL_Delay(1);
		ssd1306_WriteString("Game Over",Font_6x8  ,White);
//		sprintf(buf,"score = %d",score);
		
		buf = score/100 + '0';
		ssd1306_SetCursor(0,40);
		ssd1306_WriteString("Your Score",Font_6x8  ,White);
		ssd1306_SetCursor(73,40);
		ssd1306_WriteChar(buf, Font_6x8 , White);
		
		buf = (score%100)/10 + '0';
		ssd1306_SetCursor(80,40);
		ssd1306_WriteChar(buf, Font_6x8 , White);
		
		buf = (score%10) + '0';
		ssd1306_SetCursor(87,40);
		ssd1306_WriteChar(buf, Font_6x8 , White);
		
		ssd1306_UpdateScreen();
		
	}
	tedad=1;
	makan[0].x=0;
	makan[0].y=8;
	flg.state =0;
	flg.step =0;
	flg.jahat=0;
	score=0;
	i2c_send();
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
