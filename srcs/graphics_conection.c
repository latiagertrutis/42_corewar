/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_conection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrodrigu <mrodrigu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/03 16:02:34 by mrodrigu          #+#    #+#             */
/*   Updated: 2018/10/03 16:10:05 by mrodrigu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic_corewar.h"
#include "graphics.h"

static void	init_board(void)
{
	unsigned int	i;
	unsigned int	j;
	unsigned char	id;

	i = 0;
	id = 0;
	ft_memset(g_board, 0, sizeof(t_board) * MEM_SIZE);
	j = 0;
	while (j < MEM_SIZE)
	{
		i = 0;
		while (i < g_players[id].prog_size)
		{
			g_board[j].mem = g_mem[j];
			g_board[j].id = id + 1;
			i++;
			j++;
		}
		id++;
		j = id * (MEM_SIZE / g_n_players);
	}
}

static void	copy_pcs(void)
{
	t_pc			*aux_pc;
	unsigned int	i;

	i = 0;
	aux_pc = g_pc;
	while (i < g_nb_pc && aux_pc)
	{
		g_aux->pcs[i] = (((unsigned int)aux_pc->id) << 24) | aux_pc->pc;
		aux_pc = aux_pc->next;
		i++;
	}
}

void		init_graph(void)
{
	if (!(g_aux = (t_graphics *)malloc(sizeof(t_graphics))))
		ft_error("Error: malloc failed in init_graph\n");
	init_board();
	ft_memcpy(g_aux->board, g_board, sizeof(t_board) * MEM_SIZE);
	ft_memcpy(g_aux->players, g_players, sizeof(t_player) * g_n_players);
	g_aux->cycle_to_die = g_cycle_to_die;
	g_aux->cycle_pre_die = 0;
	g_aux->cycle = g_nb_cycles;
	g_aux->nb_pc = g_nb_pc;
	if (!(g_aux->pcs = (unsigned int *)malloc(sizeof(unsigned int) * g_nb_pc)))
		ft_error("Error: malloc failed in unit_graph\n");
	copy_pcs();
	g_aux->prog_end = 0x0;
	g_aux->next = NULL;
	g_graph_tail = g_aux;
	g_frame = g_aux;
	g_aux = NULL;
}

void		add_frame(void)
{
	pthread_mutex_lock(&g_lock);
	if (g_frame)
	{
		g_graph_tail->next = g_aux;
		g_graph_tail = g_graph_tail->next;
	}
	else
	{
		g_frame = g_aux;
		g_graph_tail = g_frame;
	}
	g_aux = NULL;
	pthread_mutex_unlock(&g_lock);
}

void		end_frame(void)
{
	if (!(g_aux = (t_graphics *)malloc(sizeof(t_graphics))))
		ft_error("Error: malloc failed in end_frame\n");
	g_aux->prog_end = 0x1;
	add_frame();
}
