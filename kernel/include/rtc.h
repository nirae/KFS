/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 12:19:16 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 15:14:00 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTC_H
# define RTC_H

#include "kfs.h"

typedef struct  rtc_time
{
    uint8 second;
    uint8 minute;
    uint8 hour;
    uint8 day;
    uint8 month;
    uint16 year;
}               t_rtc_time;

void init_rtc(void);
t_rtc_time get_rtc_time(void);

#endif
