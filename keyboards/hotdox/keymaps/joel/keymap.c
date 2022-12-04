#include QMK_KEYBOARD_H
#include "version.h"

bool l1_lock;
bool l2_lock;
bool sys_chord;
bool sys_chord_flash;

enum custom_keycodes {
    JKC_L1 = SAFE_RANGE,
    JKC_L2,
    JKC_SYS,
    JKC_SYSFL
};

// Notes about LEDs:

// LED 1 is closest to the USB cables, and separated a bit from the other two.
// So that one will be used for the "ready to flash" indicator.

// LED 2 is in the middle and LED 3 is rightmost. I ended up using the
// rightmost LED for layer 1 lock and the middle LED for layer 2 lock.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Layer 0: default/base
    // has keys for momentarily enabling or locking/unlocking layer 1 or 2
    [0] = LAYOUT_ergodox(
        // left hand
        QK_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_MINS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     JKC_SYS,
        KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
        JKC_L2,   KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     JKC_SYS,
        CW_TOGG,  KC_PGUP,  KC_PGDN,  KC_LALT,  JKC_L1,
                                                          LGUI(KC_L), KC_MUTE,
                                                                    KC_HOME,
                                                KC_LSFT,  KC_BSPC,  KC_LGUI,
        // right hand
        KC_EQL,   KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_F11,
        KC_LBRC,  KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
                  KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
        KC_RBRC,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  JKC_L2,
                            JKC_L1,   KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,
        KC_VOLD,  KC_VOLU,
                  KC_END,
        KC_RGUI,  KC_ENT,   KC_SPC
    ),

    // Layer 1: symbols/special
    // is mutually exclusive with layer 2
    [1] = LAYOUT_ergodox(
        // left hand
        KC_PWR,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_SLEP,
        KC_TRNS,  KC_SPC,   KC_UP,    KC_ENT,   KC_UNDS,  KC_TILD,  KC_TRNS,
        KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_MINS,  KC_GRV,
        JKC_SYSFL,KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                                                          LCTL(LGUI(KC_Q)), KC_TRNS,
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
    // is mutually exclusive with layer 1
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
        KC_TRNS,  KC_TRNS,  KC_P0)
};

// Make double-sure our state and the layer state agree that there are no
// layers active on startup. Also behave as if none of the JKC_SYS keys are
// currently depressed.
void matrix_init_user(void) {
    l1_lock = false;
    l2_lock = false;
    sys_chord = false;
    sys_chord_flash = false;
    layer_clear();
}

// Our hook for special actions on key events. Currently this handles the
// enable and lock behaviors for layers 1/2, and the JKC_SYS/JKC_SYSFL keys.
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
                        ergodox_right_led_3_on();
                    } else {
                        ergodox_right_led_3_off();
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

        case JKC_L2:
            if (record->event.pressed) {
                // Key Down
                if (IS_LAYER_OFF(2)) {
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
                if (!l2_lock) {
                    // If not locked, clear layer 2.
                    layer_off(2);
                }
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
                        if (l1_lock || l2_lock) {
                            if (l1_lock) {
                                send_string_with_delay_P(PSTR("1 "), 10);
                            }
                            if (l2_lock) {
                                send_string_with_delay_P(PSTR("2 "), 10);
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
            if (record->event.pressed) {
                // Key Down
                sys_chord_flash = true;
                ergodox_right_led_1_on();
            } else {
                // Key Up
                sys_chord_flash = false;
                ergodox_right_led_1_off();
            }
            return false; // Skip all further processing of this key

        default:
            return true; // Process all other keycodes normally
    }
}
