/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_core_st.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrodrigu <mrodrigu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 20:06:38 by mrodrigu          #+#    #+#             */
/*   Updated: 2018/10/03 18:25:52 by mrodrigu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic_corewar.h"
#include "graphics.h"

static void	store_indirect_with_mod(const int inc,
									const unsigned char reg_pos1, t_pc *pc)
{
	unsigned char i;

	i = 0;
	while (i < REG_SIZE)
	{
		g_mem[ft_mod(inc + i, MEM_SIZE)] = pc->reg[reg_pos1][i];
		g_board[ft_mod(inc + i, MEM_SIZE)] =
		(t_board){pc->reg[reg_pos1][i], NEW_DUR, pc->id + 1};
		i++;
	}
}

static void	store_in_ram(const unsigned char reg_pos1, t_pc *pc)
{
	unsigned char	i;
	unsigned int	aux_pc;
	int8_t			board_pos[IND_SIZE];
	int				inc;

	aux_pc = pc->pc + 2;
	if ((aux_pc + 1 + IND_SIZE) < MEM_SIZE)
		*((IND_CAST *)board_pos) = *((IND_CAST *)(g_mem + aux_pc + 1));
	else
	{
		i = 0;
		while (i < IND_SIZE)
		{
			board_pos[i] = g_mem[(aux_pc + 1 + i) % MEM_SIZE];
			i++;
		}
	}
	invert_bytes(board_pos, IND_SIZE);
	inc = pc->pc + (*((IND_CAST *)board_pos) % IDX_MOD);
	store_indirect_with_mod(inc, reg_pos1, pc);
	pc->pc = (aux_pc + 1 + IND_SIZE) % MEM_SIZE;
}

static void	store_in_reg(const unsigned char reg_pos1,
						const unsigned char reg_pos2, t_pc *pc)
{
	*((REG_CAST *)pc->reg[reg_pos2]) = *((REG_CAST *)pc->reg[reg_pos1]);
	pc->pc = (pc->pc + 1 + 1 + 1 + 1) % MEM_SIZE;
}

void		graphics_core_st(t_pc *pc)
{
	unsigned int	pos;
	unsigned char	ocp;
	unsigned char	reg_pos1;
	unsigned char	reg_pos2;

	pos = pc->pc;
	ocp = g_mem[(pos + 1) % MEM_SIZE];
	if ((0xF0 & ocp) == 0x70 &&
		(reg_pos1 = g_mem[(pos + 2) % MEM_SIZE] - 1) < REG_NUMBER)
		store_in_ram(reg_pos1, pc);
	else if ((0xF0 & ocp) == 0x50 &&
			(reg_pos1 = g_mem[(pos + 2) % MEM_SIZE] - 1) < REG_NUMBER &&
			(reg_pos2 = g_mem[(pos + 3) % MEM_SIZE] - 1) < REG_NUMBER)
		store_in_reg(reg_pos1, reg_pos2, pc);
	else
		pc->pc = (pc->pc + 1 + 1 + get_size_arg(ocp, 0, 1) +
					get_size_arg(ocp, 1, 1)) % MEM_SIZE;
}

/*
** st + ocp + rg1 + rg2
*/
