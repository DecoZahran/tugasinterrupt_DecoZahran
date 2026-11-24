#include "stm32f4xx.h"

void init_hardware(void) {
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB1ENR |= (1 << 1);

    GPIOA->MODER &= ~(3 << 20);
    GPIOA->MODER |= (1 << 20);

    GPIOA->MODER &= ~(3 << 0);
    GPIOA->PUPDR &= ~(3 << 0);
    GPIOA->PUPDR |= (1 << 0);

    GPIOA->ODR &= ~(1 << 10);

    TIM3->PSC = 15999;
    TIM3->ARR = 99;
    TIM3->DIER |= (1 << 0);
    NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & (1 << 0)) {
        TIM3->SR &= ~(1 << 0);
        GPIOA->ODR ^= (1 << 10);
    }
}

int main(void) {
    init_hardware();

    while (1) {
        if ((GPIOA->IDR & (1 << 0)) == 0) {
            TIM3->CR1 |= (1 << 0);
        } else {
            TIM3->CR1 &= ~(1 << 0);
            TIM3->CNT = 0;
            GPIOA->ODR &= ~(1 << 10);
        }
    }
}
