#include <cstdint>
#include "stm32f1xx.h"

#include "base_functions.hpp"
namespace stm32f103_user_code {

    auto delay(uint32_t ticks) -> void {
        for (volatile uint32_t i = 0; i < ticks; ++i);
    }

    void init() {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;  // включить тактирование GPIOC
        GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
        GPIOC->CRH |= GPIO_CRH_MODE13_1;  // PC13 (LD3), выход 2МГц
    }

}  // namespace stm32f103_user_code
