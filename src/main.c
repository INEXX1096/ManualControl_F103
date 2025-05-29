// #include "main.h"
// #include "stm32f1xx_hal.h"

// #include "lvgl.h"
// #include "tft_driver.h"

// void SystemClock_Config(void);
// void MX_GPIO_Init(void);
// void lvgl_init_display(void);

// void MX_GPIO_Init(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     // 使能GPIOB和GPIOA时钟
//     __HAL_RCC_GPIOB_CLK_ENABLE();
//     __HAL_RCC_GPIOA_CLK_ENABLE();

//     // 初始化PB0为推挽输出
//     GPIO_InitStruct.Pin = GPIO_PIN_0;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     // 初始化PA8为推挽输出
//     GPIO_InitStruct.Pin = GPIO_PIN_8;
//     // Mode 和 Speed 保持不变
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     // 初始化PA5为推挽输出（板载LED通常接在这里）
//     GPIO_InitStruct.Pin = GPIO_PIN_5;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
// }

// void lvgl_init_display(void)
// {
//     static lv_color_t buf1[128 * 64];  // 单缓冲区

//     lv_display_t* disp = lv_display_create(128, 160);  // 创建显示驱动对象，分辨率设置

//     lv_display_set_buffers(disp, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
//     lv_display_set_flush_cb(disp, tft_flush);  // 设置刷屏函数
// }

// void SystemClock_Config(void)
// {
//     RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//     RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//     RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//     RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//     RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
//     RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//     RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//     RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//     RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
//     if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//     {
//         while (1);
//     }

//     RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//                                 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//     RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//     RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//     RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//     RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//     if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
//     {
//         while (1);
//     }
// }

// // 主函数示例：点亮PA5和PA8上的LED
// int main(void)
// {
//     HAL_Init();
//     SystemClock_Config();
//     MX_GPIO_Init();

//     // 点亮PA5（板载LED）
//     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

//     // 点亮PA8连接的LED（一般低电平点亮，具体看你的电路）
//     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

//     while (1)
//     {
//         // 这里可以做更多的操作
//     }
// }


// #include "main.h"
// #include "stm32f1xx_hal.h"

// // 函数声明
// void SystemClock_Config(void);
// void MX_GPIO_Init(void);

// int main(void)
// {
//     // 初始化 GPIO
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     __HAL_RCC_GPIOB_CLK_ENABLE();

//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     GPIO_InitStruct.Pin = GPIO_PIN_5;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     GPIO_InitStruct.Pin = GPIO_PIN_8;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     while (1)
//     {
//         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
//         for (volatile int i = 0; i < 1000000; i++);

//         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//         for (volatile int i = 0; i < 1000000; i++);
//     }
// }

#include "main.h"
#include "stm32f1xx_hal.h"

// 函数声明
void SystemClock_Config(void);
void MX_GPIO_Init(void);

volatile uint32_t tick_cnt = 0;

void SysTick_Handler(void)
{
    tick_cnt++;
    HAL_IncTick();
}


int main(void)
{
    HAL_Init();                // 初始化 HAL 库
    SystemClock_Config();      // 配置系统时钟（使用 HSI）
    MX_GPIO_Init();            // 初始化 GPIO

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
        HAL_Delay(200);  // 在此处设置断点观察
    }
}

//hse +pll
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

//hsi + ppl
// void SystemClock_Config(void)
// {
//     RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//     RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//     // 配置 HSI + PLL（默认 STM32F1 支持）72MHz
//     RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//     RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//     RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//     RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//     RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
//     RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 8MHz / 2 * 9 = 72MHz
//     if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//     {
//         Error_Handler();
//     }

//     RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//     RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//     RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//     RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//     RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//     if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
//     {
//         Error_Handler();
//     }
// }


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


// void MX_GPIO_Init(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     // 启用 GPIOA 时钟
//     __HAL_RCC_GPIOA_CLK_ENABLE();

//     // 配置 PA8 为推挽输出
//     GPIO_InitStruct.Pin = GPIO_PIN_8;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
// }


void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能GPIOB和GPIOA时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 初始化PB5为推挽输出
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 初始化PA8为推挽输出
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 点亮PA8，确认MX_GPIO_Init()被执行
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}

