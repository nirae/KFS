/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 10:08:48 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/28 12:12:05 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KPANIC_H
# define KPANIC_H

#include "kfs.h"
#include "kput.h"

#define KPANIC(x) printk("[ KFS - KERNEL PANIC at (%s:%d) ] %s\n", __FILE__, __LINE__, x); while (666) {};

#define KERR "[ KFS - KERNEL ERROR ] "

#endif
