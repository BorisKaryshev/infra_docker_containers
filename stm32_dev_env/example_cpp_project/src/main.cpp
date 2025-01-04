#include "stddef.h"
#include "stm32f1xx.h"

#include "base_functions.hpp"

auto pc13_high() -> void {
    GPIOC->BSRR = GPIO_BSRR_BS13;
}

auto pc13_low() -> void {
    GPIOC->BSRR = GPIO_BSRR_BR13;
}

auto stm32f103_user_code::loop() -> void {
    constexpr uint32_t delay_val = 300000;
    pc13_high();       // включить первый светодиод
    delay(delay_val);  // вызов подпрограммы задержки
    pc13_low();        // выключить первый светодиод
    delay(delay_val);  // вызов подпрограммы задержки
}

auto main() -> int {
    stm32f103_user_code::init();
    while (true) {
        stm32f103_user_code::loop();
    }
    return 0;
}
