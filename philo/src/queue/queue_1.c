/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:40:34 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/08 18:26:29 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include <pthread.h>
#include <stdlib.h>

t_queue *queue_create()
{
    int rc;

    t_queue *q = malloc(sizeof(t_queue));
    if (q == NULL)
        return (NULL);
    rc = pthread_mutex_init(&q->mtx, NULL);
    if (rc != 0)
        return (free(q), NULL);
    q->head = NULL;
    q->tail = NULL;
    return (q);
}

void    queue_destroy(t_queue **ptr_q, void (*del)(void *ptr))
{
    t_queue *q;
    t_node *t;
    t_node *next;

    if (ptr_q == NULL || *ptr_q == NULL)
        return ;
    q = *ptr_q;
    t = q->head;
    while (t != NULL)
    {
        if (del != NULL)
            del(t->data);
        next = t->next;
        free(t);
        t = next;
    }
    pthread_mutex_destroy(&q->mtx);
    free(q);
    ptr_q = NULL;
}

int queue_enqueue(t_queue *q, void *data)
{
    t_node  *node;

    if (q == NULL || data == NULL)
        return (1);
    node = malloc(sizeof(t_node));
    if (node == NULL)
        return (2);
    node->data = data;
    node->next = NULL;
    pthread_mutex_lock(&q->mtx);
    if (q->tail == NULL)
        q->head = node;
    else
        q->tail->next = node;
    q->tail = node;
    pthread_mutex_unlock(&q->mtx);
    return (0);
}

void    *queue_dequeue(t_queue *q)
{
    void    *data;
    t_node  *next;

    if (q == NULL)
        return (NULL);
    data = NULL;
    pthread_mutex_lock(&q->mtx);
    if (q->head != NULL)
    {
        data = q->head->data;
        next = q->head->next;
        if (q->tail == q->head)
            q->tail = NULL;
        free(q->head);
        q->head = next;        
    }
    pthread_mutex_unlock(&q->mtx);
    return (data);
}
