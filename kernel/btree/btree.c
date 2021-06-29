/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:13:57 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 10:32:56 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "btree.h"
#include "kput.h"

int btree_nodes_count(t_btree *tree)
{
	if (tree == NULL)
		return (0);
	return (btree_nodes_count(tree->left) + btree_nodes_count(tree->right) + 1);
}

void		btree_apply_infix(t_btree *tree, void (*func)(void *))
{
	if (tree == NULL)
		return ;
	if (tree->left != NULL)
		btree_apply_infix(tree->left, func);
	(*func)((void *)tree);
	if (tree->right != NULL)
		btree_apply_infix(tree->right, func);
}

void		btree_apply_prefix(t_btree *tree, void (*func)(void *))
{
	if (tree == NULL)
		return ;
	(*func)((void *)tree);
	if (tree->left != NULL)
		btree_apply_prefix(tree->left, func);
	if (tree->right != NULL)
		btree_apply_prefix(tree->right, func);
}

// void btree_del_elem(t_btree **tree, void (*del)(void *))
// {
//     if (!tree || !(*tree))
// 		return;
//     (*tree)->left
// 	btree_del(&((*tree)->left), del);
// 	btree_del(&((*tree)->right), del);
// 	del((*tree)->data);
// }

void btree_del(t_btree **tree, void (*del)(void *))
{
	if (!tree || !(*tree))
		return;
	btree_del(&((*tree)->left), del);
	btree_del(&((*tree)->right), del);
	del((*tree)->data);
    // kfree(tree);
}

t_btree *btree_create_node(void *data)
{
	t_btree		*tree;

	if (!(tree = kmalloc(sizeof(t_btree))))
		return (0);
    memset(tree, 0, sizeof(t_btree));
	tree->data = data;
	return tree;
}

void *btree_search(t_btree *tree, void *ref, int (*cmpf)(void *, void *))
{
	void *result = 0;

	if ((*cmpf)(ref, tree) == 0)
		return (tree);
	if (tree->left != NULL)
		result = btree_search(tree->left, ref, cmpf);
	if (tree->right != NULL)
		result = btree_search(tree->right, ref, cmpf);
	return result;
}

void btree_insert(t_btree **tree, t_btree *parent, t_btree *new, int (*cmpf)(void *, void *))
{
	if (*tree == NULL)
	{
        *tree = new;
        (*tree)->parent = parent;
	}
	else
	{
		if ((*cmpf)(new, (*tree)) <= 0)
			btree_insert(&(*tree)->left, *tree, new, cmpf);
		else
			btree_insert(&(*tree)->right, *tree, new, cmpf);
	}
}
