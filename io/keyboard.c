/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 19:25:00 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/12 19:28:24 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keyboard.h"

char keystatus = 0;

/*
 * https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html#ss1.1
 */
static char qwerty_kb_table[128] = {
	0,
    0,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
	'=',
	'\b',           /* backspace */
	'\t',           /* tabulation */
    'q',
	'w',
	'e',
	'r',
	't',
	'y',
	'u',
	'i',
	'o',
	'p',
	'[',
	']',
	'\n',           /* enter */
	CTRL_KEY,       /* control */
	'a',
    's',
	'd',
	'f',
	'g',
	'h',
	'j',
	'k',
	'l',
	';',
	'\'',
	'`',
	SHIFT_KEY,       /* left shift */
	'\\',
	'z',
	'x',
    'c',
	'v',
	'b',
	'n',
	'm',
	',',
	'.',
	'/',
	SHIFT_KEY,      /* right shift */
	'*',
	0,              /* alt */
	' ',            /* space */
	0,              /* caps lock */
	0,              /* F1 */
	0,              /* F2 */
	0,              /* F3 */
	0,              /* F4 */
	0,              /* F5 */
    0,              /* F6 */
	0,              /* F7 */
	0,              /* F8 */
	0,              /* F9 */
	0,              /* F10 */
	0,              /* num lock */
	0,              /* scroll lock */
	0,              /* HOME */
	0,              /* up arrow */
	0,              /* PAGEUP */
	'-',
	LEFT_ARROW_KEY, /* left arrow */
	0,
	RIGHT_ARROW_KEY,/* right arrow */
	'+',
	0,              /* END */
	0,              /* down arrow */
	0,              /* PAGEDOWN */
	0,              /* INSERT */
	0,              /* DEL */
	0,
	0,
	0,
	0,              /* F11 */
	0,              /* F12 */
	0               /* undefined keys */
};

static char qwerty_shift_kb_table[] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A',
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '\|', 'Z', 'X',
    'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
	'0', '.', '6', 0, 0, 0, 0, 0
};

/*
 * Return the keycode
 */
uint16 keyboard_handler(void)
{
    uint16      keycode = 0;

    while (1) {
        keycode = inb(KEYBOARD_DATA_PORT);

        /* Release */
        if (IS_RELEASED(keycode)) {
            /* Shift case */
            if (qwerty_kb_table[GET_KEYCODE_FROM_RELEASED(keycode)] == SHIFT_KEY)
                UNSET_KEY_STATUS(keystatus, SHIFT_BIT);
            else if (qwerty_kb_table[GET_KEYCODE_FROM_RELEASED(keycode)] == CTRL_KEY)
                UNSET_KEY_STATUS(keystatus, CTRL_BIT);
        }
        /* Pressed */
        else {
            /* Reset the Keyboard data area */
            outb(KEYBOARD_DATA_PORT, 0);
            if (keycode < 0 || keycode > 128)
                continue;
            /* Shift case */
            if (qwerty_kb_table[keycode] == SHIFT_KEY)
                SET_KEY_STATUS(keystatus, SHIFT_BIT);
            else if (qwerty_kb_table[keycode] == CTRL_KEY)
                SET_KEY_STATUS(keystatus, CTRL_BIT);
            break;
        }
    }
    return keycode;
}

char get_pressed_char(void)
{
    uint16      keycode = 0;

    keycode = keyboard_handler();
    if (GET_KEY_STATUS(keystatus, SHIFT_BIT)) {
        return qwerty_shift_kb_table[keycode];
    }
    return qwerty_kb_table[keycode];
}
