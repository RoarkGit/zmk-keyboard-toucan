#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <stdio.h>
#include <string.h>

#include "layer.h"
#include "../assets/custom_fonts.h"
#include <zmk/physical_layouts.h>
#include <zmk/keymap.h>
#include <zmk/matrix.h>



// The WIN layer swaps GUI for CTRL without touching the letter layout, so it is excluded from the
// layer name and reported by draw_os_mode_status instead. Matched by name, not index, because ZMK
// Studio can reorder layers at runtime.
#define OS_MODE_LAYER_NAME "WIN"

static bool is_os_mode_layer(zmk_keymap_layer_id_t id) {
    const char *name = zmk_keymap_layer_name(id);
    return name != NULL && strcmp(name, OS_MODE_LAYER_NAME) == 0;
}

uint8_t zmk_display_visible_layer_index(void) {
    for (int i = ZMK_KEYMAP_LAYERS_LEN - 1; i >= 0; i--) {
        zmk_keymap_layer_id_t id = zmk_keymap_layer_index_to_id((zmk_keymap_layer_index_t)i);

        if (id == ZMK_KEYMAP_LAYER_ID_INVAL || !zmk_keymap_layer_active(id)) {
            continue;
        }
        if (is_os_mode_layer(id)) {
            continue;
        }

        return (uint8_t)i;
    }

    return 0;
}

bool zmk_display_win_mode_active(void) {
    for (int i = ZMK_KEYMAP_LAYERS_LEN - 1; i >= 0; i--) {
        zmk_keymap_layer_id_t id = zmk_keymap_layer_index_to_id((zmk_keymap_layer_index_t)i);

        if (id != ZMK_KEYMAP_LAYER_ID_INVAL && is_os_mode_layer(id)) {
            return zmk_keymap_layer_active(id);
        }
    }

    return false;
}

void draw_os_mode_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);

    lv_canvas_draw_text(canvas, 0, 100, SCREEN_WIDTH, &label_dsc,
                        state->win_mode ? "WIN" : "MAC");
}

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_24, LV_TEXT_ALIGN_CENTER);

    char fallback_layer_name[16]; 

    const char *layer_name = zmk_keymap_layer_name(zmk_keymap_layer_index_to_id(state->layer_index));

    if (layer_name == NULL || layer_name[0] == '\0') {
        sprintf(fallback_layer_name, "L#%" PRIu8, state->layer_index);
        
        layer_name = fallback_layer_name;
    }

    lv_canvas_draw_text(canvas, 0, 70, SCREEN_WIDTH, &label_dsc, layer_name);
}