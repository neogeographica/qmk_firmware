#include QMK_KEYBOARD_H
#include "version.h"

// XXX TODO:
// Do we really need to overwrite page up/down keys on layer 3?

bool l1_lock;
bool l2_lock;
bool l3_lock;
bool l3_left_shift_down;
bool l3_right_shift_down;
bool sys_chord;
bool sys_chord_flash;

enum custom_keycodes {
    JKC_L1 = SAFE_RANGE,
    JKC_L2_L,
    JKC_L2_R,
    JKC_L3,
    JKC_NOTL3,
    JKC_SYS,
    JKC_SYSFL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Layer 0: default/base
    // has keys for momentarily enabling or locking/unlocking layer 1 or 2
    // has key for locking/unlocking layer 3
    [0] = LAYOUT_ergodox(
        // left hand
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_MINS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     JKC_SYS,
        KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
        JKC_L2_L, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     JKC_SYS,
        KC_GRV,   KC_PGUP,  KC_PGDN,  KC_LALT,  JKC_L1,
                                                          JKC_L3,   KC_MUTE,
                                                                    KC_HOME,
                                                KC_LSFT,  KC_BSPC,  KC_LGUI,
        // right hand
        KC_EQL,   KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_F11,
        KC_LBRC,  KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
                  KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
        KC_RBRC,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  JKC_L2_R,
                            JKC_L1,   KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,
        KC_VOLD,  KC_VOLU,
                  KC_END,
        KC_RGUI,  KC_ENT,   KC_SPC
    ),

    // Layer 1: symbols/special
    // is mutually exclusive with layer 2
    // can be activated from any combo of layers that excludes layer 2
    // does not prevent locking/unlocking layer 3
    [1] = LAYOUT_ergodox(
        // left hand
        KC_PWR,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_SLEP,
        KC_TRNS,  KC_SPC,   KC_UP,    KC_ENT,   KC_UNDS,  KC_TILD,  KC_TRNS,
        KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_MINS,  KC_GRV,
        JKC_SYSFL,KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                                                          KC_TRNS,  KC_TRNS,
                                                                    KC_INS,
                                                KC_CAPS,  KC_DEL,   KC_TRNS,
        // right hand
        KC_PAUS,  KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F12,
        KC_TRNS,  KC_DQUO,  KC_PLUS,  KC_PIPE,  KC_LCBR,  KC_RCBR,  KC_TRNS,
                  KC_QUOT,  KC_EQL,   KC_BSLS,  KC_LBRC,  KC_RBRC,  KC_TRNS,
        KC_TRNS,  KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  JKC_SYSFL,
                            KC_TRNS,  KC_MRWD,  KC_MSTP,  KC_MPLY,  KC_MFFD,
        KC_BRID,  KC_BRIU,
                  KC_MPRV,
        KC_TRNS,  KC_MPLY,  KC_MNXT
    ),

    // Layer 2: mouse/numpad
    // is mutually exclusive with layers 1 and 3
    // can only be activated while layer 0 is the only active layer
    [2] = LAYOUT_ergodox(
        // left hand
        KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
        KC_TRNS,  KC_BTN1,  KC_MS_U,  KC_BTN2,  KC_WH_U,  KC_NO,    KC_TRNS,
        KC_TRNS,  KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_WH_D,  KC_NO,
        KC_TRNS,  KC_BTN3,  KC_BTN4,  KC_BTN5,  KC_NO,    KC_NO,    KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  JKC_SYSFL,
                                                          KC_NO,    KC_NO,
                                                                    KC_TRNS,
                                                KC_TRNS,  KC_TRNS,  KC_TRNS,
        // right hand
        KC_NO,    KC_NO,    KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,  KC_NO,
        KC_NO,    KC_NO,    KC_P7,    KC_P8,    KC_P9,    KC_PPLS,  KC_NO,
                  KC_NO,    KC_P4,    KC_P5,    KC_P6,    KC_PCMM,  KC_NO,
        KC_NO,    KC_NO,    KC_P1,    KC_P2,    KC_P3,    KC_PEQL,  KC_TRNS,
                            JKC_SYSFL,KC_P0,    KC_PDOT,  KC_PENT,  KC_NO,
        KC_NO,    KC_NO,
                  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_P0),

    // Layer 3: more normal shift and lefthand-mod key placement
    // is mutually exclusive with layer 2
    // can be activated from any combo of layers that excludes layer 2
    // does not prevent momentarily enabling or locking/unlocking layer 1
    // has key to momentarily disable its changes (except for L/R shift keys)
    [3] = LAYOUT_ergodox(
        // left hand
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_CAPS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_LCTL,  KC_LCTL,  KC_LALT,  KC_TRNS,  KC_TRNS,
                                                          KC_TRNS,  KC_TRNS,
                                                                    KC_TRNS,
                                                KC_SPC,   KC_TRNS,  JKC_NOTL3,
        // right hand
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,
                  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS
    )
};

// Make double-sure our state and the layer state agree that there are no
// layers active on startup. Also behave as if none of the JKC_SYS keys are
// currently depressed.
void matrix_init_user(void) {
    l1_lock = false;
    l2_lock = false;
    l3_lock = false;
    l3_left_shift_down = false;
    l3_right_shift_down = false;
    sys_chord = false;
    sys_chord_flash = false;
    layer_clear();
}

// Our hook for special actions on key events. Currently this handles the
// enable and lock behaviors for layers 1/2/3, and the JKC_SYS/JKC_SYSFL keys.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {

        // For layer 1 and 2 control keys: It is assumed that there will be two
        // of these (one each for left and right hand boards). Depressing a key
        // will do a layer enable while the key is held. Simultaneously
        // depressing both of the same kind of key will lock the layer on. If
        // the layer is locked on, tap the same kind of key again to unlock.
        case JKC_L1:
            if (record->event.pressed) {
                // Key Down
                if (IS_LAYER_OFF(1)) {
                    // Layer 1 isn't on yet, so turn it on.
                    layer_on(1);
                } else {
                    // Layer 1 is already on. If l1_lock is not true yet, this
                    // is because we are pushing both L1 control keys, so set
                    // l1_lock to true. Otherwise this means we are pushing an
                    // L1 control key AGAIN after locking, so unlock.
                    l1_lock = !l1_lock;
                    if (l1_lock) {
                        ergodox_right_led_1_on();
                    } else {
                        ergodox_right_led_1_off();
                    }
                }
            } else {
                // Key Up
                if (!l1_lock) {
                    // If not locked, clear layer 1.
                    layer_off(1);
                }
            }
            return false; // Skip all further processing of this key

        // Same kind of logic as for L1. An additional condition
        // here though, in that if L3 is true when key is pressed then we
        // we should treat it as KC_LSFT (even if layer 3 is temporarily
        // disabled by JKC_NOTL3).
        case JKC_L2_L:
        case JKC_L2_R:
            if (record->event.pressed) {
                // Key Down
                if (l3_lock) {
                    // Treat as shift key.
                    if (keycode == JKC_L2_L) {
                        register_code(KC_LSFT);
                        l3_left_shift_down = true;
                    } else {
                        register_code(KC_RSFT);
                        l3_right_shift_down = true;
                    }
                }
                else if (IS_LAYER_OFF(2)) {
                    // Layer 2 isn't on yet, so turn it on.
                    layer_on(2);
                } else {
                    // Layer 2 is already on; we are locking or unlocking it.
                    l2_lock = !l2_lock;
                    if (l2_lock) {
                        ergodox_right_led_2_on();
                    } else {
                        ergodox_right_led_2_off();
                    }
                }
            } else {
                // Key Up
                if (l3_left_shift_down) {
                    // Key was pressed as a shift key, so should un-shift now.
                    unregister_code(KC_LSFT);
                    l3_left_shift_down = false;
                } else if (l3_right_shift_down) {
                    // Ditto.
                    unregister_code(KC_RSFT);
                    l3_right_shift_down = false;
                }
                else if (!l2_lock) {
                    // If not locked, clear layer 2.
                    layer_off(2);
                }
            }
            return false; // Skip all further processing of this key

        // For layer 3 we have a "normal" behaving layer lock key, but we'll
        // track it specially so we can make sure it interacts properly with
        // the other layer 3 control below, which is a momentary layer state
        // flip, and the left/right shift key behavior described above. It's
        // also good to keep the layer 3 lock LED active even during a
        // momentary disable.
        case JKC_L3:
            if (record->event.pressed) {
                // Key Down
                // We are locking or unlocking layer 3.
                l3_lock = !l3_lock;
                if (l3_lock) {
                    layer_on(3);
                    ergodox_right_led_3_on();
                } else {
                    layer_off(3);
                    ergodox_right_led_3_off();
                }
            }
            return false; // Skip all further processing of this key

        // For this keycode, flip the state of layer 3 on both key down and key
        // up. However, don't do anything if l3_lock is false. The main thing
        // this protects against is any case where we happened to exit layer 3
        // while this key was being held down... we don't want to turn layer 3
        // back on with the key up event in that case.
        case JKC_NOTL3:
            if (l3_lock) {
                // Layer 3 lock is currently active. Flip layer 3 state on
                // both key down and key up.
                layer_invert(3);
            }
            return false; // Skip all further processing of this key

        // There should be two keys with this keycode assigned. If both are
        // depressed together, normally this will cause a string macro to be
        // emitted that prints out keyboard info. If a JKC_SYSFL
        // key is also being held down, then reset the keyboard for flashing.
        case JKC_SYS:
            if (record->event.pressed) {
                // Key Down
                if (sys_chord) {
                    // The other JKC_SYS key is already pressed.
                    if (sys_chord_flash) {
                        // A FLASH-enable key is pressed too, so let's flash.
                        reset_keyboard();
                    } else {
                        // Just do the normal sys chord behavior: dump info.
                        send_string_with_delay_P(PSTR(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION " locked layers: "), 10);
                        if (l1_lock || l2_lock || l3_lock) {
                            if (l1_lock) {
                                send_string_with_delay_P(PSTR("1 "), 10);
                            }
                            if (l2_lock) {
                                send_string_with_delay_P(PSTR("2 "), 10);
                            }
                            if (l3_lock) {
                                send_string_with_delay_P(PSTR("3 "), 10);
                            }
                        } else {
                            send_string_with_delay_P(PSTR("none"), 10);
                        }
                    }
                } else {
                    sys_chord = true;
                }
            } else {
                // Key Up
                sys_chord = false;
            }
            return false; // Skip all further processing of this key

        // This keycode is entirely used just to require another keypress
        // before enabling the flash behavior of the JKC_SYS keys. Since this
        // key itself is not on a base layer, that means that flash-enable
        // is a four key combo.
        case JKC_SYSFL:
            sys_chord_flash = record->event.pressed;
            return false; // Skip all further processing of this key

        default:
            return true; // Process all other keycodes normally
    }
}
