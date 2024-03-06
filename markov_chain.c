#include <stdlib.h>
#include "markov_chain.h"
#include <string.h>

/**
* Get random number between 0 and max_number [0, max_number).
* @param max_number maximal number to return (not including)
* @return Random number
*/
int get_random_number (int max_number)
{
  int x = rand ();
  return x % max_number;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  while (true)
  {
    int k = get_random_number (markov_chain->database->size);
    Node *node = markov_chain->database->first;
    for (int i = 0; i < k; i++)
    {
      Node *next = node->next;
      node = next;
    }
    if (node->data->len_counter_list != 0)
    {
      return node->data;
    }
  }
}

MarkovNode *node_by_frequency (MarkovNode *markov_node, int i, int total)
{
  if (!markov_node || i < 0 || i >= total)
  {
    return NULL;
  }
  int current = 0;
  while (i >= markov_node->counter_list[current]->frequency)
  {
    i -= markov_node->counter_list[current]->frequency;
    current++;
  }
  return markov_node->counter_list[current]->markov_node;
}
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  int frequencies = 0;
  for (int i = 0; i < state_struct_ptr->len_counter_list; i++)
  {
    frequencies += state_struct_ptr->counter_list[i]->frequency;
  }
  int k = get_random_number (frequencies);
  MarkovNode *node =
      node_by_frequency (state_struct_ptr, k, frequencies);
  return node;
}

void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
    if (first_node == NULL)
    {
      MarkovNode *node = get_first_random_node (markov_chain);
      markov_chain->print_func (node->data);
      for (int i = 0; i < max_length - 1; i++)
      {
        node = get_next_random_node (node);
        markov_chain->print_func (node->data);
        if (node->len_counter_list == 0)
        {
            printf("\n");
            return;
        }
      }
        printf("\n");
    }
    else
    {
      Node *node = markov_chain->database->first;
      while (node->data != first_node)
      {
        Node *next = node->next;
        node = next;
      }
      markov_chain->print_func (node->data->data);
      MarkovNode *cur_node = node->data;
      for (int i = 0; i < max_length-1; i++)
      {
        cur_node = get_next_random_node (cur_node);
        markov_chain->print_func (cur_node->data);
        if (cur_node->len_counter_list == 0)
        {
          printf("\n");
          return;
        }
      }
      printf("\n");
    }
}

void free_markov_chain (MarkovChain **markov_chain)
{
  Node *node = (*markov_chain)->database->first;
  for (int i = 0; i < (*markov_chain)->database->size; i++)
  {
    Node *temp = node->next;
    for (int j = 0; j < node->data->len_counter_list; j++)
    {
        free(node->data->counter_list[j]);
    }
    free(node->data->counter_list);
    free(node->data->data);
    (*markov_chain)->free_data (node->data);
    free(node);
    node = temp;
  }
  (*markov_chain)->database->first = NULL;

  free((*markov_chain)->database);
  (*markov_chain)->database = NULL;
  free (*markov_chain);
  *markov_chain = NULL;
}

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{
  for (int i = 0; i < first_node->len_counter_list; i++)
  {
    if (markov_chain->comp_func (second_node->data, first_node->
        counter_list[i]->markov_node->data) == 0)
    {
      first_node->counter_list[i]->frequency++;
      return 1;
    }
  }
  first_node->counter_list = realloc
      ((first_node->counter_list), (sizeof (NextNodeCounter *) *
                                    first_node->len_counter_list)
                                   + 1 * sizeof (NextNodeCounter *));
  if (first_node->counter_list == NULL)
  {
    return 0;
  }
  NextNodeCounter *next_node = malloc (sizeof (NextNodeCounter));
  *next_node = (NextNodeCounter) {second_node, 1};
  first_node->counter_list[first_node->len_counter_list] = next_node;
  first_node->len_counter_list++;
  return 1;
}

Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  Node *temp = markov_chain->database->first;
  for (int i = 0; i < markov_chain->database->size; i++)
  {
    if (markov_chain->comp_func (temp->data->data, data_ptr) == 0)
    {
      return temp;
    }
    Node *next = temp->next;
    temp = next;
  }
  return NULL;
}

Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  Node *node = get_node_from_database (markov_chain, data_ptr);
  if (node)
  {
    return node;
  }
  void *data = markov_chain->copy_func(data_ptr);
  MarkovNode *new_node = malloc (sizeof (MarkovNode));
  NextNodeCounter **p_next_node = malloc (sizeof (NextNodeCounter *));
//  NextNodeCounter *next_node = malloc (sizeof (NextNodeCounter));
//  *next_node = (NextNodeCounter) {NULL, 0};
//  p_next_node[0] = next_node;
  *new_node =
      (MarkovNode) {data, p_next_node, 0};
  add (markov_chain->database, new_node);
  return markov_chain->database->last;
}
