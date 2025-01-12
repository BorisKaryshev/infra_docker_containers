#include <stdint.h>
#include <functional>
#include "stm32f1xx.h"

#include "base_functions.hpp"

namespace {

    auto update_tim2_period() -> void {
        if (stm32f103_user_code::timer_wait_ms == 0) {
            stm32f103_user_code::timer_wait_ms = 1;
        }
        TIM2->ARR = stm32f103_user_code::timer_wait_ms - 1;  // Модуль счёта таймера (1кГц/1000 = 1с)
        TIM2->CNT = 0;
    }

    extern "C" {
    auto TIM2_IRQHandler() -> void {
        TIM2->SR &= ~TIM_SR_UIF;  // Сброс флага переполнения
        stm32f103_user_code::timer_callback();
        update_tim2_period();
    }
    }

}  // namespace

namespace {
    auto init_pc13_gpio() -> void {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
        GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
        GPIOC->CRH |= GPIO_CRH_MODE13_1;
    }

    auto init_clock() -> void {
        // Enable HSI
        RCC->CR |= RCC_CR_HSION;
        while (!(RCC->CR & RCC_CR_HSIRDY)) {
        };

        // Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        // Flash 2 wait state
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        FLASH->ACR |= FLASH_ACR_LATENCY_2;

        // HCLK = SYSCLK
        RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
        // PCLK2 = HCLK
        RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
        // PCLK1 = HCLK
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

        // PLL configuration: PLLCLK = HSI/2 * 16 = 64 MHz
        RCC->CFGR &= ~RCC_CFGR_PLLSRC;
        RCC->CFGR |= RCC_CFGR_PLLMULL16;

        // Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        // Wait till PLL is ready
        while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
        };

        // Select PLL as system clock source
        RCC->CFGR &= ~RCC_CFGR_SW;
        RCC->CFGR |= RCC_CFGR_SW_PLL;

        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
        };
    }

    void initTIM2(void) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Включить тактирование TIM6

        // Частота APB1 для таймеров = APB1Clk * 2 = 32МГц * 2 = 64МГц
        TIM2->PSC = 64000 - 1;                               // Предделитель частоты (64МГц/64000 = 1кГц)
        TIM2->ARR = stm32f103_user_code::timer_wait_ms - 1;  // Модуль счёта таймера (1кГц/1000 = 1с)
        TIM2->DIER |= TIM_DIER_UIE;  // Разрешить прерывание по переполнению таймера
        TIM2->CR1 |= TIM_CR1_CEN;    // Включить таймер

        NVIC_EnableIRQ(TIM2_IRQn);       // Рарзрешить прерывание от TIM2
        NVIC_SetPriority(TIM2_IRQn, 1);  // Выставляем приоритет
    }

}  // namespace

namespace stm32f103_user_code {

    uint32_t timer_wait_ms = 1000;
    std::function<void()> timer_callback = [] {};

    auto delay(uint32_t ticks) -> void {
        for (volatile uint32_t i = 0; i < ticks; ++i);
    }

    auto init() -> void {
        init_clock();
        initTIM2();
        init_pc13_gpio();
    }

}  // namespace stm32f103_user_code
