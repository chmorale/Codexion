/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:52:33 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/18 14:53:31 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static void	heapify_up(t_heap *heap, int index)
{
	int		parent;
	t_coder	*tmp;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->array[index]->priority
			< heap->array[parent]->priority)
		{
			tmp = heap->array[index];
			heap->array[index] = heap->array[parent];
			heap->array[parent] = tmp;
			index = parent;
		}
		else
			break ;
	}
}

static void	heapify_down(t_heap *heap, int index)
{
	int		left;
	int		right;
	int		smallest;
	t_coder	*tmp;

	left = index * 2 + 1;
	right = index * 2 + 2;
	smallest = index;
	if (left < heap->size
		&& heap->array[left]->priority
		< heap->array[smallest]->priority)
		smallest = left;
	if (right < heap->size
		&& heap->array[right]->priority
		< heap->array[smallest]->priority)
		smallest = right;
	if (smallest != index)
	{
		tmp = heap->array[index];
		heap->array[index] = heap->array[smallest];
		heap->array[smallest] = tmp;
		heapify_down(heap, smallest);
	}
}

void	heap_push(t_heap *heap, t_coder *coder)
{
	heap->array[heap->size] = coder;
	heap->size += 1;
	heapify_up(heap, heap->size - 1);
}

t_coder	*heap_pop(t_heap *heap)
{
	t_coder	*top;

	if (heap->size == 0)
		return (NULL);
	top = heap->array[0];
	heap->size -= 1;
	heap->array[0] = heap->array[heap->size];
	if (heap->size > 0)
		heapify_down(heap, 0);
	return (top);
}
