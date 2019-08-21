/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_three_trees.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allefebv <allefebv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 11:43:53 by allefebv          #+#    #+#             */
/*   Updated: 2019/08/21 15:10:10 by allefebv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	ft_error_tree(t_ls *ls, t_entry *entry, t_trees_management *trees)
{
	entry->error = strerror(errno);
	ft_treeadd(trees->errors, ft_treenew_ptr(entry), ft_name_sort);
	ls->print_dir_name_flag = 2;
	ls->is_arg_error_tree = 1;
}

static void	ft_file_tree(t_ls *ls, t_entry *entry, t_trees_management *trees,
				t_lengths *lengths)
{
	ft_long_format(ls, entry, lengths);
	ft_treeadd(trees->files, ft_treenew_ptr(entry), ls->fptr_sort);
	ls->print_dir_name_flag = 2;
	ls->is_arg_files_tree = 1;
}

static void	ft_dir_tree(t_ls *ls, t_entry *entry, t_trees_management *trees)
{
	ft_treeadd(trees->dirs, ft_treenew_ptr(entry), ls->fptr_sort);
	(ls->print_dir_name_flag)++;
}

static int	ft_opr(t_ls *ls, t_list *opr, t_trees_management *trees,
				t_lengths *lengths)
{
	t_entry	*entry;

	entry = NULL;
	while (opr)
	{
		if (!(entry = (t_entry*)ft_memalloc(sizeof(t_entry))))
			return (ft_error(e_malloc_error));
		entry->path = (char*)opr->content;
		entry->name = ft_strdup(entry->path);
		if (lstat(entry->path, &entry->info))
			ft_error_tree(ls, entry, trees);
		else
		{
			if (S_ISDIR(entry->info.st_mode))
				ft_dir_tree(ls, entry, trees);
			else
				ft_file_tree(ls, entry, trees, lengths);
		}
		opr = opr->next;
	}
	return (1);
}

int			ft_three_trees(t_ls *ls, t_list *opr, t_trees_management *trees)
{
	t_entry		*entry;
	t_lengths	lengths;

	ft_bzero(&lengths, sizeof(t_lengths));
	entry = NULL;
	if (opr)
		ft_opr(ls, opr, trees, &lengths);
	else
	{
		if (!(entry = (t_entry*)ft_memalloc(sizeof(t_entry))))
			return (ft_error(e_malloc_error));
		entry->path = ft_strdup(".");
		lstat(entry->path, &entry->info);
		ft_dir_tree(ls, entry, trees);
	}
	if (ls->is_arg_error_tree)
		ft_tree_inorder_print(*trees->errors, NULL, ft_print_errors);
	if (ls->is_arg_files_tree)
		ft_tree_inorder_print(*trees->files, &lengths, ls->fptr_print);
	return (1);
}
