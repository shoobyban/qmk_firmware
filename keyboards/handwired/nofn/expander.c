/* Copyright 2018 Mike Roberts
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdbool.h>
#include "action.h"
#include "i2c_master.h"
#include "expander.h"
#include "debug.h"
#include "wait.h"

uint8_t bit_for_pin(uint8_t pin);

uint8_t expander_write(uint8_t reg, uint8_t data);

uint8_t expander_read(uint8_t reg);

void expander_config(void);

void expander_init(void) {
    i2c_init();
    expander_config();
}

// set IN and HI
void expander_unselect_all() {
    expander_write(EXPANDER_REG_GPIOA,0xFF);
    wait_us(EXPANDER_PAUSE);
}

// set OUT and LOW
void expander_select(uint8_t pin) {
    const uint8_t mask = 0xff & (1 << bit_for_pin(pin));
    expander_write(EXPANDER_REG_GPIOA, ~mask);
    wait_us(EXPANDER_PAUSE);
}

void expander_config() {
    // set A to output, B to input
    expander_write(EXPANDER_REG_IODIRA, 0x00);
    expander_write(EXPANDER_REG_IODIRB, 0xff);

    // turn on pull-ups
    expander_write(EXPANDER_REG_GPPUA, 0xff);
    expander_write(EXPANDER_REG_GPPUB, 0xff);

    // disable interrupts
    expander_write(EXPANDER_REG_GPINTENA, 0x0);
    expander_write(EXPANDER_REG_GPINTENB, 0x0);

    // reverse polarity for B so we read 0->1
    expander_write(EXPANDER_REG_IPOLB, 0x0);
}

uint8_t bit_for_pin(uint8_t pin) {
    return pin % 8;
}

uint8_t expander_write(uint8_t reg, unsigned char val) {
    i2c_status_t status;
    status = i2c_writeReg((EXPANDER_ADDR << 1), reg, &val, 1, I2C_TIMEOUT);
    if (status != I2C_STATUS_SUCCESS) {
        xprintf("exp write fail: %d, %d => %d\n", reg, &val,status);
    }
    return 0;
}

uint8_t expander_read_row() {
    return ~expander_read(EXPANDER_REG_GPIOB);
}

uint8_t expander_read(uint8_t reg) {
    uint8_t r[1] = { 0 };
    i2c_status_t status = i2c_readReg((EXPANDER_ADDR << 1), reg, r,1,I2C_TIMEOUT);
    if (status == I2C_STATUS_SUCCESS) {
        return r[0];
    }
    xprintf("read %d failed\n", reg);
    return 0;
}

bool is_pin_on(uint8_t val, uint8_t pin) {
    return 0 == (val & (1 << (pin+1)));
}
