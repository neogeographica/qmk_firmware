#include QMK_KEYBOARD_H
#include "version.h"

#define ALT_TAB_TIMEOUT_MS 1500
#define POWER_TIMEOUT_MS 1500

bool mousenum_lock = false;
bool shooter_mode = false;
bool shooter_left_shift_down = false;
bool shooter_right_shift_down = false;
bool shooter_grave_down = false;
bool sys_chord = false;
bool sys_chord_flash = false;
bool alt_tab_active = false;
bool waiting_for_power_code= false;
bool recording = false;

uint16_t lthumb_keycode = KC_LSFT;
uint16_t alt_tab_timer = 0;
uint16_t power_timer = 0;
uint8_t led_cycle = 0;

enum custom_keycodes {
    JKC_ALT_TAB = SAFE_RANGE,
    JKC_ALT_TICK,
    JKC_MN_L,
    JKC_MN_R,
    JKC_CW,
    JKC_LTHM,
    JKC_SYS,
    JKC_SYSFL,
    JKC_PWR
};

enum layers{
  _MAIN = 0,
  _SYMBOLS,
  _MOUSENUM
};

// Notes about LEDs:

// LED 1 is closest to the USB cables, and separated a bit from the other two.
// That one will be used for the "shooter mode" indicator.

// LED 2 is in the middle and LED 3 is rightmost. I ended up using the
// rightmost LED for macro recording and the middle LED for _MOUSENUM lock.


// OK, let's define the keymaps for layers:

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Notes about "shooter mode":
 *
 * This is a mode to make the lefthand keyboard a bit nicer for FPS games.
 *
 * Shooter mode can be entered as long as the mouse/numpad layer is not
 * currently active, by pressing both GUI keys simultaneously. It can be
 * exited in the same way.
 *
 * While shooter mode is enabled:
 * - The two keys in the NUMPAD positions on the keymap will instead act as
 *   left and right shift keys.
 * - The key in the LShift position will instead act as a spacebar.
 * - The key in the CPWD position will instead act as the backtick/tilde key.
 *
 * The layer keymap descriptions below will not mention this; they describe
 * the keymap as it is when shooter mode is disabled.
 *
 * An implication of this arrangement is that the symbols/special layer can
 * still be activated, locked, and deactivated while in shooter mode, but the
 * mouse/numpad layer cannot.
 */

/* Keymap _MAIN: default/base layer
 *
 * - alphanum keys in "normal" QWERTY positions, plus some other symbol keys
 * - ergodox-ish thumb key placement for shift, enter, space, backspace
 * - common thumb/pinky modifier keys where I like them
 * - clusters of cursor-movement keys
 * - keys to activate or lock other layers
 * - volume control
 * - screenlock combo for Windows and (some) Linux
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Esc    |   1  |   2  |   3  |   4  |   5  |   -  |           |   =  |   6  |   7  |   8  |   9  |   0  |   F11  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |  SYS |           |   [  |   Y  |   U  |   I  |   O  |   P  |    \   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtrl  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |    '   |
 * |--------+------+------+------+------+------|  SYS |           |   ]  |------+------+------+------+------+--------|
 * | NUMPAD |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | NUMPAD |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |CPWD/`| PgUp | PgDn | LAlt |  SYM |                                       |  SYM | Left | Down |  Up  | Right|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | WLOCK| Mute |       | Vol- | Vol+ |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |  M1  |       |  M2  |      |      |
 *                                 |LShift| Bksp |------|       |------| Enter| Space|
 *                                 |      |      | LGui |       | RGui |      |      |
 *                                 `--------------------'       `--------------------'
 * 
 *
 * SYM key activates the symbols/special layer. Normally is only active while
 * held; press both SYM keys together to lock the layer. Press one SYM key
 * again to unlock the layer. When the symbols/special layer is locked, LED 3
 * will be lit.
 *
 * NUMPAD key activates the mouse/numpad layer. Normally is only active while
 * held; press both NUMPAD keys together to lock the layer. Press one NUMPAD
 * key again to unlock the layer. When the mouse/numpad layer is locked, LED
 * 2 will be lit.
 *
 * CPWD activates "caps word" mode; letters will be capitalized and hyphens
 * changed to underlines until the space key is pressed. However if any
 * modifier key is held (Ctrl, Alt, Shift, GUI) then this key will behave like
 * the backtick/tilde key instead.
 *
 * WLOCK sends a macro (LGui-L) to lock the screen. This screen lock macro
 * works for Windows and various Linux setups; for macOS see the corresponding
 * key on the symbols/special layer.
 *
 * The M1 and M2 keys play back dynamic macros 1 and 2 respectively.
 *
 * Pressing the two SYS keys together will normally type out information about
 * the build and the currently active layers. If SYSFL is already held however
 * (by chording the SYM and NUMPAD keys), then pressing the two SYS keys will
 * reset the keyboard for flashing.
 */
    [_MAIN] = LAYOUT_ergodox(
        // left hand
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_MINS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     JKC_SYS,
        KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
        JKC_MN_L, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     JKC_SYS,
        JKC_CW,   KC_PGUP,  KC_PGDN,  KC_LALT,  MO(_SYMBOLS),
                                                          LGUI(KC_L), KC_MUTE,
                                                                    DM_PLY1,
                                                JKC_LTHM, KC_BSPC,  KC_LGUI,
        // right hand
        KC_EQL,   KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_F11,
        KC_LBRC,  KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
                  KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
        KC_RBRC,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  JKC_MN_R,
                            MO(_SYMBOLS), KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT,
        KC_VOLD,  KC_VOLU,
        DM_PLY2,
        KC_RGUI,  KC_ENT,  KC_SPC
    ),

/* Keymap _SYMBOLS: symbols/special layer (mutually exclusive with _MOUSENUM)
 *
 * - function keys
 * - power/sleep/pause
 * - access to symbols and arrow keys on the main three rows
 * - media player
 * - screen brightness
 * - less-used "entry state" keys (caps/del/ins)
 * - screenlock combo for macOS
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Power  |  F1  |  F2  |  F3  |  F4  |  F5  | Sleep|           | Pause|  F6  |  F7  |  F8  |  F9  |  F10 |   F12  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | AltTab | Space|  Up  | Enter|   _  |   ~  |      |           | PrSrc|   "  |   +  |   |  |   {  |   }  |  Null  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | Left | Down | Right|   -  |   `  |------|           |------|   '  |   =  |   \  |   [  |   ]  |  Null  |
 * |--------+------+------+------+------+------|      |           |  Ins |------+------+------+------+------+--------|
 * |  SYSFL |   !  |   @  |   #  |   $  |   %  |      |           |      |   ^  |   &  |   *  |   (  |   )  |  SYSFL |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |AltGrv| Home |  End |      |      |                                       |      |  <<  | Stop | Play |  >>  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | MLOCK|      |       | Brt- | Brt+ |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | RecM1|       | RecM2|      |      |
 *                                 | Caps |  Del |------|       |------| Prev | Next |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 * 
 * The POWER key needs to be held for 1.5 seconds to activate. LEDs will cycle
 * during this timeout period.
 *
 * MLOCK sends a macro (LCtl-LGui-Q) to lock the screen. This screen lock macro
 * works for macOS; for Windows and various Linux setups see the corresponding
 * key on the base layer.
 *
 * AltTab sends Alt-Tab; repeated presses continue to send Tab while holding
 * Alt. AltGrv is similar for Alt-Backtick.
 *
 * The RecM1 and RecM2 keys start/stop recording of dynamic macros 1 and 2
 * respectively. (Nested macros are not allowed.)
 *
 * SYSFL unlocks the keyboard-flashing combo; see base layer comments for more.
 */
    [_SYMBOLS] = LAYOUT_ergodox(
        // left hand
        JKC_PWR,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_SLEP,
        JKC_ALT_TAB, KC_SPC,   KC_UP,    KC_ENT,   KC_UNDS,  KC_TILD,  KC_TRNS,
        KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_MINS,  KC_GRV,
        JKC_SYSFL,KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_TRNS,
        JKC_ALT_TICK, KC_HOME,  KC_END,   KC_TRNS,  KC_TRNS,
                                                          LCTL(LGUI(KC_Q)), KC_TRNS,
                                                                    DM_REC1,
                                                KC_CAPS,  KC_DEL,   KC_TRNS,
        // right hand
        KC_PAUS,  KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F12,
        KC_PSCR,  KC_DQUO,  KC_PLUS,  KC_PIPE,  KC_LCBR,  KC_RCBR,  KC_NO,
                  KC_QUOT,  KC_EQL,   KC_BSLS,  KC_LBRC,  KC_RBRC,  KC_NO,
        KC_INS,   KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  JKC_SYSFL,
                            KC_TRNS,  KC_MRWD,  KC_MSTP,  KC_MPLY,  KC_MFFD,
        KC_BRID,  KC_BRIU,
        DM_REC2,
        KC_TRNS,  KC_MPRV,  KC_MNXT
    ),

/* Keymap _MOUSENUM: mouse/numpad layer (mutually exclusive with _SYMBOLS)
 *
 * - left hand keys for mouse movement, wheel, and buttons
 * - right hand keys for numpad
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        | Null | Null | Null | Null | Null | Null |           | Null | Null |NumLck|  KP/ |  KP* |  KP- |  Null  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |LClick| MsUp |RClick| WhUp | Null |      |           | Null | Null |  KP7 |  KP8 |  KP9 |  KP+ |  Null  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |MsLeft|MsDown|MsRght|WhDown| Null |------|           |------| Null |  KP4 |  KP5 |  KP6 |  KP, |  Null  |
 * |--------+------+------+------+------+------|      |           | Null |------+------+------+------+------+--------|
 * |        | Btn3 | Btn4 | Btn5 | Null | Null |      |           |      | Null |  KP1 |  KP2 |  KP3 |  KP= |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | Null |      |      |      | SYSFL|                                       | SYSFL|  KP0 |  KP. | KPEnt| Null |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |  KP0 |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 *
 * SYSFL unlocks the keyboard-flashing combo; see base layer comments for more.
 */
    [_MOUSENUM] = LAYOUT_ergodox(
        // left hand
        KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
        KC_TRNS,  KC_BTN1,  KC_MS_U,  KC_BTN2,  KC_WH_U,  KC_NO,    KC_TRNS,
        KC_TRNS,  KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_WH_D,  KC_NO,
        KC_TRNS,  KC_BTN3,  KC_BTN4,  KC_BTN5,  KC_NO,    KC_NO,    KC_TRNS,
        KC_NO,    KC_TRNS,  KC_TRNS,  KC_TRNS,  JKC_SYSFL,
                                                          KC_TRNS,  KC_TRNS,
                                                                    KC_TRNS,
                                                KC_TRNS,  KC_TRNS,  KC_TRNS,
        // right hand
        KC_NO,    KC_NO,    KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,  KC_NO,
        KC_NO,    KC_NO,    KC_P7,    KC_P8,    KC_P9,    KC_PPLS,  KC_NO,
                  KC_NO,    KC_P4,    KC_P5,    KC_P6,    KC_PCMM,  KC_NO,
        KC_NO,    KC_NO,    KC_P1,    KC_P2,    KC_P3,    KC_PEQL,  KC_TRNS,
                            JKC_SYSFL,KC_P0,    KC_PDOT,  KC_PENT,  KC_NO,
        KC_TRNS,  KC_TRNS,
        KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_P0)
};

// Our hook for special actions on key events. Currently this handles the
// enable and lock behaviors for _SYMBOLS and _MOUSENUM, and the
// JKC_SYS/JKC_SYSFL keys.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {

        // This key normally acts as left shift, but in "shooter mode" it is
        // a spacebar.
        case JKC_LTHM:
            if (record->event.pressed) {
                // Key Down
                if (!shooter_mode) {
                    lthumb_keycode = KC_LSFT;
                } else {
                    lthumb_keycode = KC_SPC;
                }
                register_code(lthumb_keycode);
            } else {
                unregister_code(lthumb_keycode);
            }
            return false; // Skip all further processing of this key

        // This key normally activates Caps Word, but in "shooter mode", or if
        // any modifier key is pressed, it is the backtick/tilde key.
        case JKC_CW:
            if (record->event.pressed) {
                // Key Down
                // Checking for modifier-key-pressed seems like a perfect
                // usecase for key overrides at first glance. However, to
                // preserve the modifier(s) in the resulting emitted key event
                // would require defining at least 8 overrides that need to
                // be cycled through and checked on each press. This is faster
                // and simpler.
                if ((get_mods() == 0) && !shooter_mode) {
                    caps_word_on();
                } else {
                    register_code(KC_GRV);
                    shooter_grave_down = true;
                }
            } else {
                // Key Up
                if (shooter_grave_down) {
                    // Key was pressed as a backtick/tilde key, so should
                    // release that now.
                    unregister_code(KC_GRV);
                    shooter_grave_down = false;
                }
            }
            return false; // Skip all further processing of this key

        // These keys send tab or backtick, but also on first press will
        // register the alt keycode. matrix_scan_user will take care of
        // releasing alt if these keys' layer is deactivated or if
        // ALT_TAB_TIMEOUT_MS elapses w/o more presses of these keys.
        case JKC_ALT_TAB:
        case JKC_ALT_TICK:
            if (record->event.pressed) {
                if (!alt_tab_active) {
                    alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                if (keycode == JKC_ALT_TAB) {
                    register_code(KC_TAB);
                } else {
                    register_code(KC_GRV);
                }
            } else {
                if (keycode == JKC_ALT_TAB) {
                    unregister_code(KC_TAB);
                } else {
                    unregister_code(KC_GRV);
                }
            }
            return false; // Skip all further processing of this key

        // It is assumed that there will be two _MOUSENUM control keys (one
        // each for left and right hand boards). Depressing a key will do a
        // layer enable while the key is held. Simultaneously depressing both
        // of them lock the layer on. If the layer is locked on, tap the same
        // kind of key again to unlock. Finally, an additional consideration:
        // in "shooter mode" these control keys simply act as shift keys.
        case JKC_MN_L:
        case JKC_MN_R:
            if (record->event.pressed) {
                // Key Down
                if (shooter_mode) {
                    // Treat as shift key.
                    if (keycode == JKC_MN_L) {
                        register_code(KC_LSFT);
                        shooter_left_shift_down = true;
                    } else {
                        register_code(KC_RSFT);
                        shooter_right_shift_down = true;
                    }
                } else if (IS_LAYER_OFF(_MOUSENUM)) {
                    // _MOUSENUM isn't on yet, so turn it on.
                    layer_on(_MOUSENUM);
                } else {
                    // _MOUSENUM is already on; we are locking or unlocking it.
                    mousenum_lock = !mousenum_lock;
                    if (mousenum_lock) {
                        ergodox_right_led_2_on();
                    } else {
                        ergodox_right_led_2_off();
                    }
                }
            } else {
                // Key Up
                if (keycode == JKC_MN_L) {
                    if (shooter_left_shift_down) {
                        // Key was pressed as a shift key, so should un-shift now.
                        unregister_code(KC_LSFT);
                        shooter_left_shift_down = false;
                    } else if (!mousenum_lock) {
                        // If not locked, clear _MOUSENUM.
                        layer_off(_MOUSENUM);
                    }
                }
                else {
                    // Same as above for other key.
                    if (shooter_right_shift_down) {
                        unregister_code(KC_RSFT);
                        shooter_right_shift_down = false;
                    } else if (!mousenum_lock) {
                        layer_off(_MOUSENUM);
                    }
                }
            }
            return false; // Skip all further processing of this key

        // The GUI keys typically just emit left/right GUI key events.
        // However if both keys are pressed, and we're not on the mouse/numpad
        // layer, then additionally toggle "shooter mode".
        case KC_LGUI:
        case KC_RGUI:
            if (record->event.pressed) {
                // Key Down
                if ((get_mods() & MOD_MASK_GUI) != 0) {
                    // The other GUI key is already pressed. If we're not on
                    // the mouse/numpad layer, toggle shooter mode.
                    if (IS_LAYER_OFF(_MOUSENUM)) {
                        shooter_mode = !shooter_mode;
                        if (shooter_mode) {
                            ergodox_right_led_1_on();
                        } else {
                            ergodox_right_led_1_off();
                        }
                    }
                }
            }
            return true; // Go ahead and process this key

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
                        ergodox_right_led_1_on();
                        ergodox_right_led_2_on();
                        ergodox_right_led_3_on();
                        _delay_ms(200);
                        reset_keyboard();
                    } else {
                        // Just do the normal sys chord behavior: dump info.
                        send_string_with_delay_P(PSTR(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION " locked layers/modes: "), 10);
                        if (recording || mousenum_lock || shooter_mode) {
                            if (recording) {
                                send_string_with_delay_P(PSTR("RECORDING "), 10);
                            }
                            if (mousenum_lock) {
                                send_string_with_delay_P(PSTR("MOUSE/NUMPAD "), 10);
                            }
                            if (shooter_mode) {
                                send_string_with_delay_P(PSTR("SHOOTER"), 10);
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

        // For the power key, just record whether it is pressed and mark the
        // start time. matrix_scan_user will send the actual power keycode if
        // POWER_TIMEOUT_MS elapses while holding the key.
        case JKC_PWR:
            if (record->event.pressed) {
                // Key Down
                waiting_for_power_code = true;
                power_timer = timer_read();
                // Blank LEDs to prep for cycling.
                ergodox_led_all_off();
            } else {
                // Key Up
                if (!waiting_for_power_code) {
                    // Only do key-up if matrix_scan_user did key-down.
                    unregister_code(KC_PWR);
                }
                waiting_for_power_code = false;
                // Reset the LEDs to non-cycling state.
                ergodox_led_all_off();
                if (shooter_mode) {
                    ergodox_right_led_1_on();
                }
                if (mousenum_lock) {
                    ergodox_right_led_2_on();
                }
                if (recording) {
                    ergodox_right_led_3_on();
                }
            }
            return false; // Skip all further processing of this key

        default:
            return true; // Process all other keycodes normally
    }
}

// Deactivate the alt-hold for the special Alt-Tab and Alt-Backtick keys if
// necessary. Also send the power keycode if the power key has been held long
// enough.
void matrix_scan_user(void) {
    if (alt_tab_active) {
        if (IS_LAYER_OFF(_SYMBOLS) || (timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT_MS)) {
            unregister_code(KC_LALT);
            alt_tab_active = false;
        }
    }
    if (waiting_for_power_code) {
        if (timer_elapsed(power_timer) > POWER_TIMEOUT_MS) {
            register_code(KC_PWR);
            waiting_for_power_code = false;
        } else {
            switch (led_cycle) {
                case 0:
                    ergodox_right_led_3_off();
                    ergodox_right_led_1_on();
                    led_cycle++;
                    break;
                case 1:
                    ergodox_right_led_1_off();
                    ergodox_right_led_2_on();
                    led_cycle++;
                    break;
                case 2:
                    ergodox_right_led_2_off();
                    ergodox_right_led_3_on();
                    led_cycle = 0;
                    break;
            }
            _delay_ms(50);
        }
    }
}

// When either macro starts recording, turn on the recording LED.
void dynamic_macro_record_start_user(void) {
    recording = true;
    ergodox_right_led_3_on();
}

// When current macro recording stops, turn off the recording LED.
void dynamic_macro_record_end_user(int8_t direction) {
    recording = false;
    ergodox_right_led_3_off();
}

