// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2019 NXP
 *
 */

#include <common.h>
#include <dm.h>
#include <dm/device_compat.h>
#include <mipi_dsi.h>
#include <panel.h>
#include <asm/gpio.h>
#include <linux/err.h>
#include <linux/delay.h>


static const struct display_timing default_timing = {
	.pixelclock.typ		= 121000000,
	.hactive.typ		= 1080,
	.hfront_porch.typ	= 20,
	.hback_porch.typ	= 34,
	.hsync_len.typ		= 2,
	.vactive.typ		= 1920,
	.vfront_porch.typ	= 10,
	.vback_porch.typ	= 4,
	.vsync_len.typ		= 2,
	.flags = DISPLAY_FLAGS_HSYNC_LOW |
		 DISPLAY_FLAGS_VSYNC_LOW |
		 DISPLAY_FLAGS_DE_LOW |
		 DISPLAY_FLAGS_PIXDATA_NEGEDGE,
};

static int lt070me05000_panel_enable_backlight(struct udevice *dev)
{
	return 0;
}

static int lt070me05000_panel_get_display_timing(struct udevice *dev,
					    struct display_timing *timings)
{
	struct mipi_dsi_panel_plat *plat = dev_get_plat(dev);
	struct mipi_dsi_device *device = plat->device;

	memcpy(timings, &default_timing, sizeof(*timings));

	/* fill characteristics of DSI data link */
	if (device) {
		device->lanes = 4;
		device->format = MIPI_DSI_FMT_RGB888;
		device->mode_flags = MIPI_DSI_MODE_VIDEO_HSE 
		| MIPI_DSI_MODE_VIDEO_SYNC_PULSE | MIPI_DSI_MODE_VIDEO;
	}

	return 0;
}

static int lt070me05000_panel_probe(struct udevice *dev)
{
	return 0;
}

static int lt070me05000_panel_disable(struct udevice *dev)
{
	return 0;
}

static const struct panel_ops lt070me05000_panel_ops = {
	.enable_backlight =lt070me05000_panel_enable_backlight,
	.get_display_timing = lt070me05000_panel_get_display_timing,
};

static const struct udevice_id lt070me05000_panel_ids[] = {
	{ .compatible = "jdi,lt070me05000", },
	{ }
};

U_BOOT_DRIVER(lt070me05000_panel) = {
	.name			  = "lt070me05000_panel",
	.id			  = UCLASS_PANEL,
	.of_match		  = lt070me05000_panel_ids,
	.ops			  = &lt070me05000_panel_ops,
	.probe			  = lt070me05000_panel_probe,
	.remove			  = lt070me05000_panel_disable,
	.plat_auto = sizeof(struct mipi_dsi_panel_plat),
	//.priv_auto = sizeof(struct rm67191_panel_priv),
};
