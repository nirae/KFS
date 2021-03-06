/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 19:24:43 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 12:08:53 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBOARD_H
# define KEYBOARD_H

#include "kfs.h"
#include "io.h"

extern char keystatus;

#define KEYBOARD_DATA_PORT      0x60
#define KEYBOARD_CTRL_PORT      0x64

/*
 *  Internal keycodes
 */
# define SHIFT_KEY              -1
# define CTRL_KEY               -2
# define CAPSLOCK_KEY           -3
# define LEFT_ARROW_KEY         -10
# define RIGHT_ARROW_KEY        -11

/*
 *  Keys bits for the keystatus
 */
# define SHIFT_BIT              1
# define CTRL_BIT               2
# define CAPSLOCK_BIT           3

/*
 *  Layout options
 */
# define QWERTY                 1
# define AZERTY                 2

/*
 *  Macros to update/get the status of a key
 */
# define SET_KEY_STATUS(status, key) status ^= (-1 ^ status) & (1 << key)
# define UNSET_KEY_STATUS(status, key) status ^= (0 ^ status) & (1 << key)
# define GET_KEY_STATUS(status, key) (status >> key) & 1

/*
 *  Macros for informations about the keycodes from the keyboard
 */
# define IS_RELEASED(keycode) keycode & 0x80
# define GET_KEYCODE_FROM_RELEASED(keycode) keycode ^ 0x80

void init_keyboard(void);
char read_last_typed_char(void);
void change_layout(int layout);
void switch_layout(void);
int get_layout(void);

#endif
