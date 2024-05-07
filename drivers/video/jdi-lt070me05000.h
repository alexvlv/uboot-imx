#pragma once

#include <common.h>
#include <asm/gpio.h>

struct lt070me_panel_priv {
	struct gpio_desc **gpios;
	struct udevice   **regulators;
};

int  lt070me_gpio_init(struct udevice *dev);
void lt070me_reset_activate(struct udevice *dev);
void lt070me_reset_release(struct udevice *dev);

int lt070me_power_init(struct udevice *dev);
int lt070me_power_on(struct udevice *dev);
