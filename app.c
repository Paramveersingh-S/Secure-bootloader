#include "stm32f4xx.h"

/* Application Vector Table */
__attribute__((section(".app_vector_table")))     // Directs linker to place this in ".app_vector_table"
const uint32_t app_vector_table[] = {             // Array holding the vector table entries  
  0x20010000,   // Initial stack pointer (top of RAM) // Initial Stack Pointer (SP) value: Top of RAM (adjust for your MCU's RAM size)
  (uint32_t)main // Reset handler                     //// Reset Handler: Address of the main() function (entry point)
};

void main(void) {
  // Initialize hardware peripherals
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // Enable GPIOD clock, RCC->AHB1ENR: Register to enable clocks for AHB1 peripherals (GPIO banks)
    // RCC_AHB1ENR_GPIODEN: Bitmask to enable GPIOD (bit 3)
    GPIOD->MODER |= GPIO_MODER_MODER12_0; // Set PD12 (LED) as output

    // Main application loop
    while(1) { // Infinite loop (critical for embedded systems!)
        // Toggle LED on PD12
        GPIOD->ODR ^= GPIO_ODR_ODR_12;     /* Toggle LED on PD12 */
        // GPIOD->ODR: GPIO output data register
        // GPIO_ODR_ODR_12: Bitmask for pin 12 (LED)
        // XOR (^=) flips the bit (toggles LED state)
        
        // Simple delay (replace with timer-based delay in real projects)
        for(volatile uint32_t i = 0; i < 1000000; i++);
  }
}
