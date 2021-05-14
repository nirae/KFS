/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:07:37 by ndubouil          #+#    #+#             */
/*   Updated: 2021/03/18 10:52:44 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBK_H
# define LIBK_H

int	            intlen(int n);
void		    itoa(int n, char *str);
int		        strcmp(const char *s1, const char *s2);
void	        strncpy(char *dst, const char *src, unsigned int len);
unsigned int	strlen(const char *s);
char	        *strrev(char *str);
void	        *memset(void *b, int c, unsigned int len);
void            hex_to_str(unsigned int addr, char *result, int size);
void            sleep(int time);

#endif
