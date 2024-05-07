// SPDX-License-Identifier: GPL-2.0+

#include "jdi-lt070me05000.h"

#include <linux/err.h>

#include <dm.h>
#include <dm/device_compat.h>

//-------------------------------------------------------------------------
enum lt070me_gpio_idx { 	
	GPIO_DCDC_EN =0, 
	GPIO_RESET, 
	GPIO_LED_EN 
};
//-------------------------------------------------------------------------
static const char * const gpio_names[] = {
	"dcdc-en",
	"reset",
	"enable",
};
//-------------------------------------------------------------------------
static int lt070me_gpio_acquire(struct udevice *dev, struct gpio_desc **pgp , const char *name, unsigned long flags)
{
	int ret = 0;
	struct gpio_desc *gp;

	if(*pgp != NULL) {
		dev_notice(dev,"GPIO pin %s is already acquired: %d\n",name, desc_to_gpio(*pgp));
		return ret;
	}
	
	gp = devm_gpiod_get(dev, name, flags);
	if (IS_ERR(gp)) {
		ret = PTR_ERR(gp);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "Failed to acquire %s gpio pin(%d)\n", name, ret);
	} else if(gp) {
		dev_notice(dev,"GPIO pin %s acquired: %d\n",name, desc_to_gpio(gp));
	} else {
		dev_warn(dev,"No %s gpio pin provided\n", name);
	}
	*pgp = gp;
	return ret;
}
//-------------------------------------------------------------------------
static void lt070me_gpio_set_value(struct udevice *dev, enum lt070me_gpio_idx idx, int value)
{
	struct lt070me_panel_priv *priv = dev_get_priv(dev);

	//dev_dbg(dev, "GPIO %d: (%s@%s:%d)\n", idx, __func__, kbasename(__FILE__), __LINE__);
	assert(idx < ARRAY_SIZE(gpio_names));
	if (priv->gpios[idx]) {
		dm_gpio_set_value(priv->gpios[idx], value);
	}
}
//-------------------------------------------------------------------------
void lt070me_reset_activate(struct udevice *dev) {lt070me_gpio_set_value(dev, GPIO_RESET, 0); }
void lt070me_reset_release(struct udevice *dev)  {lt070me_gpio_set_value(dev, GPIO_RESET, 1); }
//-------------------------------------------------------------------------
int lt070me_gpio_init(struct udevice *dev)
{
	struct lt070me_panel_priv *priv = dev_get_priv(dev);
	int ret, i;

	priv->gpios = calloc(ARRAY_SIZE(gpio_names),sizeof(struct gpio_desc *));

	for (i = 0; i < ARRAY_SIZE(gpio_names) && ret==0; i++) {
		ret = lt070me_gpio_acquire(dev,&priv->gpios[i],gpio_names[i],GPIOD_IS_OUT);
	}
	lt070me_reset_activate(dev);
	printf("!!! %s done, ret:%d !!!!\n", __func__, ret);
	return ret;
}
//-------------------------------------------------------------------------
