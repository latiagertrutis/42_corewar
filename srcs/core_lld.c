/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_lld.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrodrigu <mrodrigu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/22 17:11:06 by mrodrigu          #+#    #+#             */
/*   Updated: 2018/10/03 15:09:31 by mrodrigu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic_corewar.h"

static void	load_direct(const unsigned char reg_pos, t_pc *pc)
{
	unsigned char	i;
	unsigned int	aux_pc;

	aux_pc = pc->pc + 2;
	if ((aux_pc + DIR_SIZE) < MEM_SIZE)
		*((REG_CAST *)(pc->reg[reg_pos])) = *((DIR_CAST *)(g_mem + aux_pc));
	else
	{
		i = 0;
		while (i < REG_SIZE && i < DIR_SIZE)
		{
			pc->reg[reg_pos][i] = g_mem[(aux_pc + i) % MEM_SIZE];
			i++;
		}
	}
	pc->pc = (aux_pc + 1 + DIR_SIZE) % MEM_SIZE;
	pc->carry = (*((REG_CAST *)pc->reg[reg_pos])) ? 0x0 : 0x1;
}

static void	charge_ind_with_mod(const int inc, const unsigned char reg_pos,
								t_pc *pc)
{
	unsigned char i;

	i = 0;
	while (i < REG_SIZE && i < DIR_SIZE)
	{
		pc->reg[reg_pos][i] = g_mem[ft_mod(inc + i, MEM_SIZE)];
		i++;
	}
}

static void	load_indirect(const unsigned char reg_pos, t_pc *pc)
{
	int8_t			board_pos[IND_SIZE];
	unsigned char	i;
	unsigned int	aux_pc;
	int				inc;

	aux_pc = pc->pc + 2;
	if ((aux_pc + IND_SIZE) < MEM_SIZE)
		*((IND_CAST *)board_pos) = *((IND_CAST *)(g_mem + aux_pc));
	else
	{
		i = 0;
		while (i < IND_SIZE)
		{
			board_pos[i] = g_mem[(aux_pc + i) % MEM_SIZE];
			i++;
		}
	}
	invert_bytes(board_pos, IND_SIZE);
	inc = pc->pc + (*((IND_CAST *)board_pos));
	if ((inc + DIR_SIZE) < MEM_SIZE && inc >= 0)
		*((REG_CAST *)pc->reg[reg_pos]) = *((DIR_CAST *)(g_mem + inc));
	else
		charge_ind_with_mod(inc, reg_pos, pc);
	pc->pc = (aux_pc + 1 + IND_SIZE) % MEM_SIZE;
	pc->carry = (*((REG_CAST *)pc->reg[reg_pos])) ? 0x0 : 0x1;
}

void		core_lld(t_pc *pc)
{
	unsigned int	pos;
	unsigned char	ocp;
	unsigned char	reg_pos;

	pos = pc->pc;
	ocp = g_mem[(pos + 1) % MEM_SIZE];
	if ((0xF0 & ocp) == 0x90 &&
		(reg_pos = g_mem[(pos + 2 + DIR_SIZE) % MEM_SIZE] - 1) < REG_NUMBER)
		load_direct(reg_pos, pc);
	else if ((0xF0 & ocp) == 0xD0 &&
			(reg_pos = g_mem[(pos + 2 + IND_SIZE) % MEM_SIZE] - 1) < REG_NUMBER)
		load_indirect(reg_pos, pc);
	else
		pc->pc = (pc->pc + 1 + 1 + get_size_arg(ocp, 0, 1) +
					get_size_arg(ocp, 1, 1)) % MEM_SIZE;
}

/*
** line 21: jump ld + ocp
** line 32: ld + ocp + reg + dir
** line 74: ld + ocp + ind + reg
** line 93: ld + ocp + a1 + a2
*/
