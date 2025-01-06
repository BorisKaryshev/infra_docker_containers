#include "stddef.h"
#include "stm32f1xx.h"

#include <cstdint>
#include <ctime>
#include "base_functions.hpp"

auto pc13_high() -> void {
    GPIOC->BSRR = GPIO_BSRR_BS13;
}

auto pc13_low() -> void {
    GPIOC->BSRR = GPIO_BSRR_BR13;
}

auto loop(bool is_pc13_state_high) -> void {
    if (is_pc13_state_high) {
        pc13_high();
    } else {
        pc13_low();
    }
}

auto main() -> int {
    volatile bool is_pc13_state_high = true;
    stm32f103_user_code::timer_wait_ms = 100;
    stm32f103_user_code::init();

    stm32f103_user_code::timer_callback = [&is_pc13_state_high] { is_pc13_state_high = !is_pc13_state_high; };

    while (true) {
        loop(is_pc13_state_high);
    }
    return 0;
}
