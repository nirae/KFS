/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 19:25:00 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 12:08:46 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keyboard.h"
#include "interrupts.h"

/*
 *  Keystatus:
 *      Contain the state of the pressed/lock keys
 */
char keystatus = 0;

/*
 *  last_char:
 *      Contain the last typed char
 */
char last_char = 0;

/*
 *  last_char:
 *      Contain the last typed char
 */
char choosed_layout = QWERTY;

/*
 * https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html#ss1.1
 * http://www.osdever.net/bkerndev/Docs/keyboard.htm
 * https://wiki.osdev.org/PS2_Keyboard
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
	CAPSLOCK_KEY,   /* caps lock */
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

static char qwerty_shift_kb_table[128] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A',
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X',
    'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
	'0', '.', '6', 0, 0, 0, 0, 0
};

static char azerty_shift_kb_table[128] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, '_', '\b', 0,
    'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0, '*', '\n', 0, 'Q', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', '>', 0, 0, 'W', 'X', 'C', 'V',
    'B', 'N', '?', '.', '/', '+', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
    '6', 0, 0, 0, 0, 0
};

static char azerty_kb_table[128] = {
	0, 0, '&', 0, '"', '\'', '(', 0, 0, '!', 0, 0, ')', '-', '\b', 0, 'a', 'z',
    'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', 0, 'q', 's', 'd',
    'f', 'g', 'h', 'j', 'k', 'l', 'm', 0, '<', 0, '`', 'w', 'x', 'c', 'v', 'b',
    'n', ',', ';', ':', '=', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '6',
	0, 0, 0, 0, 0
};

char read_last_typed_char(void)
{
    char ret;

    ret = last_char;
    last_char = 0;
    return ret;
}

char get_keystatus(void)
{
    return keystatus;
}

int get_layout(void)
{
    return choosed_layout;
}

void change_layout(int layout)
{
    if (layout == QWERTY || layout == AZERTY) {
        choosed_layout = layout;
    }
}

void switch_layout(void)
{
    if (choosed_layout == QWERTY) {
        choosed_layout = AZERTY;
    } else if (choosed_layout == AZERTY) {
        choosed_layout = QWERTY;
    }
}


/*
 * Return the keycode and update the keystatus
 */
// uint16 keyboard_handler(void)
// {
//     uint16      keycode = 0;

//     while (1) {
//         keycode = inb(KEYBOARD_DATA_PORT);

//         /* Release */
//         if (IS_RELEASED(keycode)) {
//             /* Special keys cases */
//             switch (qwerty_kb_table[GET_KEYCODE_FROM_RELEASED(keycode)]) {
//                 case SHIFT_KEY:
//                     UNSET_KEY_STATUS(keystatus, SHIFT_BIT);
//                 case CTRL_KEY:
//                     UNSET_KEY_STATUS(keystatus, CTRL_BIT);
//             }
//         }
//         /* Pressed */
//         else {
//             /* Reset the Keyboard data area */
//             outb(KEYBOARD_DATA_PORT, 0);
//             /* Between 0 (uint16) and 128 */
//             if (keycode > 128)
//                 continue;
//             /* Special keys cases */
//             switch (qwerty_kb_table[keycode]) {
//                 case SHIFT_KEY:
//                     SET_KEY_STATUS(keystatus, SHIFT_BIT);
//                     break;
//                 case CTRL_KEY:
//                     SET_KEY_STATUS(keystatus, CTRL_BIT);
//                     break;
//                 case CAPSLOCK_KEY:
//                     if (GET_KEY_STATUS(keystatus, CAPSLOCK_BIT))
//                         UNSET_KEY_STATUS(keystatus, CAPSLOCK_BIT);
//                     else
//                         SET_KEY_STATUS(keystatus, CAPSLOCK_BIT);
//                     break;
//             }
//             break;
//         }
//     }
//     return keycode;
// }

uint16 keyboard_handler(void)
{
    uint16      keycode = 0;
    char        *table;

    if (choosed_layout == AZERTY)
        table = azerty_kb_table;
    else
        table = qwerty_kb_table;

    keycode = inb(KEYBOARD_DATA_PORT);
    /* Release */
    if (IS_RELEASED(keycode)) {
        /* Special keys cases */
        switch (table[GET_KEYCODE_FROM_RELEASED(keycode)]) {
            case SHIFT_KEY:
                UNSET_KEY_STATUS(keystatus, SHIFT_BIT);
            case CTRL_KEY:
                UNSET_KEY_STATUS(keystatus, CTRL_BIT);
        }
        return 0;
    }
    /* Pressed */
    else {
        /* Reset the Keyboard data area */
        outb(KEYBOARD_DATA_PORT, 0);
        /* Between 0 (uint16) and 128 */
        if (keycode > 128)
            return 0;
        /* Special keys cases */
        switch (table[keycode]) {
            case SHIFT_KEY:
                SET_KEY_STATUS(keystatus, SHIFT_BIT);
            case CTRL_KEY:
                SET_KEY_STATUS(keystatus, CTRL_BIT);
            case CAPSLOCK_KEY:
                if (GET_KEY_STATUS(keystatus, CAPSLOCK_BIT))
                    UNSET_KEY_STATUS(keystatus, CAPSLOCK_BIT);
                else
                    SET_KEY_STATUS(keystatus, CAPSLOCK_BIT);
        }
    }
    return keycode;
}

char get_pressed_char(void)
{
    uint16      keycode = 0;

    keycode = keyboard_handler();
    if (keycode == 0)
        return 0;
    if (GET_KEY_STATUS(keystatus, SHIFT_BIT) || GET_KEY_STATUS(keystatus, CAPSLOCK_BIT)) {
        if (choosed_layout == AZERTY)
            return azerty_shift_kb_table[keycode];
        else
            return qwerty_shift_kb_table[keycode];
    }
    if (choosed_layout == AZERTY)
        return azerty_kb_table[keycode];
    return qwerty_kb_table[keycode];
}

static void keyboard_handler_regs(t_registers regs)
{
    /* Unused parameter */
    (void)regs;
    char pressed_char;

    pressed_char = get_pressed_char();
    if (pressed_char != 0) {
        last_char = pressed_char;
    }
}

void init_keyboard(void)
{
    register_interrupt_handler(IRQ1, &keyboard_handler_regs);
}

