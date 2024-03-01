/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   getters_setters.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 14:35:01 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/01/26 16:17:57 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	get_bool(t_mtx *mutex, bool *value)
{
	bool	bool_value;

	use_mutex(mutex, LOCK);
	bool_value = *value;
	use_mutex(mutex, UNLOCK);
	return (bool_value);
}

void	set_bool(t_mtx *mutex, bool *value, bool changed)
{
	use_mutex(mutex, LOCK);
	*value = changed;
	use_mutex(mutex, UNLOCK);
}

long	get_long(t_mtx *mutex, long *value)
{
	long	long_value;

	use_mutex(mutex, LOCK);
	long_value = *value;
	use_mutex(mutex, UNLOCK);
	return (long_value);
}

void	set_long(t_mtx *mutex, long *value, long changed)
{
	use_mutex(mutex, LOCK);
	*value = changed;
	use_mutex(mutex, UNLOCK);
}
