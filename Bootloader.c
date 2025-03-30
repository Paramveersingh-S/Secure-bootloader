#include "stm32f4xx.h"

/* Linker symbols (defined in .ld file) */
extern uint32_t _sapp, _eapp, _checksum;

/* MPU Configuration */
void configure_mpu(void) {
  /* Disable MPU */
  MPU->CTRL = 0;

  /* Region 0: Bootloader (Flash, 16K, read-only) */
  MPU->RNR = 0;
  MPU->RBAR = 0x08000000;              /* Bootloader start address */
  MPU->RASR = (0b00101 << 1) |         /* 16K region */
              (0x3 << 24) |            /* AP=RO (privileged access only) */
              (1 << 0);                 /* Enable region */

  /* Region 1: CONFIG (Flash, 4K, read-only) */
  MPU->RNR = 1;
  MPU->RBAR = 0x08010000;              /* CONFIG start address */
  MPU->RASR = (0b00100 << 1) |         /* 4K region */
              (0x3 << 24) |            /* AP=RO */
              (1 << 0);

  /* Enable MPU */
  MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
  __DSB();
  __ISB();
}

/* Simple Checksum Calculation */
uint32_t calculate_checksum(uint32_t *start, uint32_t *end) {
  uint32_t sum = 0;
  while (start < end) sum += *start++;
  return sum;
}

/* Jump to Application */
void jump_to_app(void) {
  uint32_t *app_vector_table = (uint32_t*)&_sapp;
  /* Set new vector table */
  SCB->VTOR = (uint32_t)app_vector_table;
  /* Set stack pointer and jump */
  __set_MSP(app_vector_table[0]);
  ((void (*)(void))app_vector_table[1])();
}

int main(void) {
  configure_mpu();

  /* Verify checksum */
  uint32_t stored_checksum = *(uint32_t*)&_checksum;
  uint32_t calculated_checksum = calculate_checksum(&_sapp, &_eapp);

  if (stored_checksum == calculated_checksum) {
    jump_to_app();  // Valid application
  } else {
    // Handle invalid firmware (e.g., blink LED)
    while(1);
  }
}
