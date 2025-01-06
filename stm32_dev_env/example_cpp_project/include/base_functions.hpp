#ifndef STM32_F103_BASE_FUNCTIONS_HPP
#define STM32_F103_BASE_FUNCTIONS_HPP

#include <functional>
#include "stdint.h"

namespace stm32f103_user_code {

    auto loop() -> void;
    auto init() -> void;

    auto delay(uint32_t ticks) -> void;

    extern uint32_t timer_wait_ms;
    extern std::function<void()> timer_callback;

};  // namespace stm32f103_user_code

#endif
