/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:35:55 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/08 18:03:35 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H
# include <pthread.h>

typedef struct s_node
{
    void    *data;
    struct s_node *next;
} t_node;

typedef struct s_queue
{
    pthread_mutex_t mtx;
    t_node  *head;
    t_node  *tail;
} t_queue;

t_queue *queue_create();
void    queue_destroy(t_queue **ptr_q, void (*del)(void *ptr));
int queue_append(t_queue *q, void *data);
#endif