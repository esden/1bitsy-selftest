/*
 * This file is part of the 1bitsy project.
 *
 * Copyright (C) 2017 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "button_boot.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdbool.h>
#include <stdio.h>

struct pin {
	int id;
	uint32_t bank;
	uint16_t pin;
	union {
		struct {
			bool pulldown : 1;
			bool pullup : 1;
			bool tiedown : 1;
			bool tieup : 1;
		} flags;
		uint32_t all;
	} expect;
};

#define PIN_CNT 38

static const int pin_count = PIN_CNT;

static const struct pin pins[PIN_CNT] = {
	{
		.id = 2,
		.bank = GPIOC,
		.pin = GPIO10,
		.expect.all = 0
	},
	{
		.id = 3,
		.bank = GPIOC,
		.pin = GPIO11,
		.expect.all = 0
	},
	{
		.id = 4,
		.bank = GPIOC,
		.pin = GPIO12,
		.expect.all = 0
	},
	{
		.id = 5,
		.bank = GPIOD,
		.pin = GPIO2,
		.expect.all = 0
	},
#if 0
	{ /* TRST */
		.id = 6,
		.bank = GPIOB,
		.pin = GPIO4,
		.expect.all = 0
	},
#endif
	{
		.id = 7,
		.bank = GPIOB,
		.pin = GPIO5,
		.expect.all = 0
	},
	{
		.id = 8,
		.bank = GPIOB,
		.pin = GPIO6,
		.expect.all = 0
	},
	{
		.id = 9,
		.bank = GPIOB,
		.pin = GPIO7,
		.expect.all = 0
	},
	{
		.id = 10,
		.bank = GPIOB,
		.pin = GPIO8,
		.expect.all = 0
	},
	{
		.id = 11,
		.bank = GPIOB,
		.pin = GPIO9,
		.expect.all = 0
	},
	{
		.id = 12,
		.bank = GPIOC,
		.pin = GPIO2,
		.expect.all = 0
	},
	{
		.id = 13,
		.bank = GPIOC,
		.pin = GPIO3,
		.expect.all = 0
	},
	{
		.id = 17,
		.bank = GPIOB,
		.pin = GPIO0,
		.expect.all = 0
	},
	{
		.id = 18,
		.bank = GPIOB,
		.pin = GPIO1,
		.expect.all = 0
	},
	{
		.id = 19,
		.bank = GPIOB,
		.pin = GPIO10,
		.expect.all = 0
	},
	{
		.id = 20,
		.bank = GPIOB,
		.pin = GPIO11,
		.expect.all = 0
	},
	{
		.id = 21,
		.bank = GPIOB,
		.pin = GPIO12,
		.expect.all = 0
	},
	{
		.id = 22,
		.bank = GPIOB,
		.pin = GPIO13,
		.expect.all = 0
	},
	{
		.id = 23,
		.bank = GPIOB,
		.pin = GPIO14,
		.expect.all = 0
	},
	{
		.id = 24,
		.bank = GPIOB,
		.pin = GPIO15,
		.expect.all = 0
	},
	{
		.id = 25,
		.bank = GPIOC,
		.pin = GPIO6,
		.expect.all = 0
	},
	{
		.id = 26,
		.bank = GPIOC,
		.pin = GPIO7,
		.expect.all = 0
	},
	{
		.id = 27,
		.bank = GPIOC,
		.pin = GPIO8,
		.expect.all = 0
	},
	{
		.id = 28,
		.bank = GPIOC,
		.pin = GPIO9,
		.expect.all = 0
	},
	{
		.id = 31,
		.bank = GPIOA,
		.pin = GPIO0,
		.expect.all = 0
	},
	{
		.id = 32,
		.bank = GPIOA,
		.pin = GPIO1,
		.expect.all = 0
	},
	{
		.id = 33,
		.bank = GPIOA,
		.pin = GPIO2,
		.expect.all = 0
	},
	{
		.id = 34,
		.bank = GPIOA,
		.pin = GPIO3,
		.expect.all = 0
	},
	{
		.id = 35,
		.bank = GPIOA,
		.pin = GPIO4,
		.expect.all = 0
	},
	{
		.id = 36,
		.bank = GPIOA,
		.pin = GPIO5,
		.expect.all = 0
	},
	{
		.id = 37,
		.bank = GPIOA,
		.pin = GPIO6,
		.expect.all = 0
	},
	{
		.id = 38,
		.bank = GPIOA,
		.pin = GPIO7,
		.expect.all = 0
	},
	{
		.id = 39,
		.bank = GPIOC,
		.pin = GPIO4,
		.expect.all = 0
	},
	{
		.id = 40,
		.bank = GPIOC,
		.pin = GPIO5,
		.expect.all = 0
	},
	{
		.id = 41,
		.bank = GPIOC,
		.pin = GPIO13,
		.expect.all = 0
	},
	{
		.id = 42,
		.bank = GPIOC,
		.pin = GPIO0,
		.expect.all = 0
	},
	{
		.id = 43,
		.bank = GPIOB,
		.pin = GPIO2,
		.expect.flags.pulldown = true
	},
	{
		.id = 44,
		.bank = GPIOC,
		.pin = GPIO14,
		.expect.all = 0
	},
	{
		.id = 45,
		.bank = GPIOC,
		.pin = GPIO15,
		.expect.all = 0
	},
};

/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_hse_3v3(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOD);
}

static void gpio_set_pull(bool pullup)
{
	uint32_t gpio_pupd = pullup ? GPIO_PUPD_PULLUP : GPIO_PUPD_PULLDOWN;

	for (int i = 0; i < pin_count; i++) {
		gpio_mode_setup(pins[i].bank, GPIO_MODE_INPUT,
			gpio_pupd, pins[i].pin);
	}
}

static void gpio_setup(void)
{
	/* Set the LED pin GPIO8 (in GPIO port A) to 'output push-pull'. */
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO8);

}


/* Returns true as success. */
static bool gpio_test(int index) {
	uint32_t wait = 0;
	//uint32_t timeout = 0xFFFFFFFF;
	uint32_t timeout = 5;
	bool ret = true;

	/* errm, bye bye */
	if ((index < 0) || (index >= pin_count)) {
		return false;
	}

	/* Set the pin to pullup and check if it worked. */
	gpio_mode_setup(pins[index].bank, GPIO_MODE_INPUT,
		GPIO_PUPD_PULLUP, pins[index].pin);

	/* Check if the pin reacted to the state change with timeout. */
	while (wait < timeout) {
		if (gpio_get(pins[index].bank, pins[index].pin) != 0) {
			break;
		}
		wait++;
	}

	/* Reset the pin to the default state. */
	gpio_mode_setup(pins[index].bank, GPIO_MODE_INPUT,
		GPIO_PUPD_PULLDOWN, pins[index].pin);

	/* Analyze the result. */
	if (wait == timeout) {
		if (pins[index].expect.flags.pulldown) {
			/* we expected pulldown, let's retest pulling hard. */
			gpio_clear(pins[index].bank, pins[index].pin);
			gpio_mode_setup(pins[index].bank, GPIO_MODE_OUTPUT,
				GPIO_PUPD_NONE, pins[index].pin);
			gpio_set(pins[index].bank, pins[index].pin);
			wait = 0;
			while (wait < timeout) {
				if (gpio_get(pins[index].bank, pins[index].pin) != 0) {
					break;
				}
				wait++;
			}
			gpio_mode_setup(pins[index].bank, GPIO_MODE_INPUT,
				GPIO_PUPD_PULLDOWN, pins[index].pin);
			if (wait == timeout) {
				printf("ERROR: tiedown ");
				ret = false;
			} else {
				/* All good pulldown confirmed. */
			}
		} else {
			printf("ERROR: pulldown ");
			ret = false;
		}
	} else {
		if (pins[index].expect.flags.pulldown) {
			printf("ERROR: expected pulldown ");
			ret = false;
		} else {
			/* we did not expect a pulldown and the pin changed state in time, all good. */
		}
	}

	if (!ret) {
		printf("pin %d id %d with wait %ld\n", index, pins[index].id, wait);
	}

	return ret;
}

static void gpio_test_all(void)
{

	/* Enable pulldown resistors on all pins. */
	gpio_set_pull(false);

	/* Test if all pins can be pulled high. */
	for (int i = 0; i < pin_count; i++) {
		gpio_test(i);
	}
}

extern void initialise_monitor_handles(void);

int main(void)
{
	int i;

	button_boot();

	initialise_monitor_handles();

	clock_setup();
	gpio_setup();

	/* Set two LEDs for wigwag effect when toggling. */
	gpio_set(GPIOA, GPIO8);

	gpio_test_all();

	/* Blink the LEDs (PA8) on the board. */
	while (1) {
		/* Toggle LEDs. */
		gpio_toggle(GPIOA, GPIO8);
		for (i = 0; i < 6000000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
