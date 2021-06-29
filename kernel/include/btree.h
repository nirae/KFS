/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:11:19 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 10:33:04 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BTREE_H
# define BTREE_H

#include "kfs.h"
#include "libk.h"
#include "kmem.h"

typedef struct		btree
{
	struct btree    *parent;
	struct btree	*left;
	struct btree	*right;
	void			*data;
}					t_btree;


void btree_insert(t_btree **tree, t_btree *parent, t_btree *new, int (*cmpf)(void *, void *));
t_btree *btree_create_node(void *data);
t_btree *btree_create_raw_node(void *addr, void *data);
void *btree_search(t_btree *tree, void *ref, int (*cmpf)(void *, void *));
void		btree_apply_infix(t_btree *tree, void (*func)(void *));
void		btree_apply_prefix(t_btree *tree, void (*func)(void *));
int btree_nodes_count(t_btree *tree);

#endif
