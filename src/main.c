#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>

static void set_angle(void *obj, int32_t v)
{
    printk("angle: %d\n", v);
    lv_obj_set_style_transform_rotation(obj, v, 0);
}

int main(void)
{
    /* Get display */
    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (!device_is_ready(display)) {
        printk("Display not ready\n");
        return -1;
    }

    printk("Display ready\n");

    /* LVGL must be initialized by Zephyr already, but we ensure clean screen */
    lv_obj_t *scr = lv_scr_act();

    /* Create label */
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "Zephyr LVGL");
    lv_obj_center(label);

    /* IMPORTANT: set pivot to center of SSD1331 (96x64) */
    lv_obj_set_style_transform_pivot_x(label, 48, 0);
    lv_obj_set_style_transform_pivot_y(label, 32, 0);

    /* Optional: make text easier to see */
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);

    /* Animation */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, label);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_values(&a, 0, 3600); /* 0 → 360° (0.1° units) */
    lv_anim_set_time(&a, 3000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);

    /* LVGL task loop (ONLY needed if Zephyr config doesn't already handle it) */
    while (1) {
        lv_timer_handler();
        k_msleep(10);
    }


    return 0;
}

/*
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <lvgl.h>

int main(void)
{
    printk("Starting LVGL SSD1331 demo\n");

    printk("display: %p\n", device_get_binding("SSD1331"));

    const struct device *display = DEVICE_DT_GET_ANY(solomon_ssd1331);

    if (!device_is_ready(display)) {
        printk("Display not ready!\n");
        return -1;
    }


    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello ESP32");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (1) {
        lv_task_handler();
        k_sleep(K_MSEC(5));
    }

    return 0;
}
*/
