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
/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
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

MarkovNode *node_by_frequency (MarkovNode *markov_node, int random_frequency,
                               int total_frequencies)
/**
 * Get the next markov_node by frequency.
 * @param markov_node the current markov_node
 * @param random_frequency the random frequency to get the next markov_node by
 * @param total_frequencies the total frequencies of the current markov_node
 * @return
 */
{
  if (!markov_node || random_frequency < 0
      || random_frequency >= total_frequencies)
  {
    return NULL;
  }
  int node_ind = 0;
  while (random_frequency >= markov_node->counter_list[node_ind]->frequency)
  {
    random_frequency -= markov_node->counter_list[node_ind]->frequency;
    node_ind++;
  }
  return markov_node->counter_list[node_ind]->markov_node;
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
{
  int total_frequencies = 0;
  for (int i = 0; i < state_struct_ptr->len_counter_list; i++)
  {
    total_frequencies += state_struct_ptr->counter_list[i]->frequency;
  }
  int random_frequency = get_random_number (total_frequencies);
  MarkovNode *node =
      node_by_frequency (state_struct_ptr, random_frequency, total_frequencies);
  return node;
}

void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
/**
 * Generate random sequence of words from the given markov_chain.
 * @param markov_chain
 * @param first_node markov_node to start with, if NULL- choose a random
 * markov_node
 * @param  max_length maximum length of chain to generate
 */
{
  MarkovNode *cur_node = (first_node == NULL) ? get_first_random_node
      (markov_chain) : first_node->data;
  for (int i = 0; i < max_length; i++)
  {
    markov_chain->print_func (cur_node->data);
    if (cur_node->len_counter_list == 0)
    {
      break;
    }
    cur_node = get_next_random_node (cur_node);
  }
  printf (NEW_LINE);
}


void free_markov_chain (MarkovChain **markov_chain)
{
  Node *node = (*markov_chain)->database->first;
  for (int i = 0; i < (*markov_chain)->database->size; i++)
  {
    Node *temp = node->next;
    for (int j = 0; j < node->data->len_counter_list; j++)
    {
      free (node->data->counter_list[j]);
    }
    free (node->data->counter_list);
    free (node->data->data);
    (*markov_chain)->free_data (node->data);
    free (node);
    node = temp;
  }
  (*markov_chain)->database->first = NULL;

  free ((*markov_chain)->database);
  (*markov_chain)->database = NULL;
  free (*markov_chain);
  *markov_chain = NULL;
}

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node the markov_node to add to it's counter list
 * @param second_node the markov_node to add to the counter list of the first
 * @param markov_chain the chain to add to
 * @return true if the process was successful, false in case of
 * allocation error.
 */
{
  for (int i = 0; i < first_node->len_counter_list; i++)
  {
    if (markov_chain->comp_func (second_node->data, first_node->
        counter_list[i]->markov_node->data) == 0)
    {
      first_node->counter_list[i]->frequency++;
      return true;
    }
  }
  first_node->counter_list = realloc
      ((first_node->counter_list), (sizeof (NextNodeCounter *) *
                                    first_node->len_counter_list)
                                   + 1 * sizeof (NextNodeCounter *));
  if (first_node->counter_list == NULL)
  {
    return false;
  }
  NextNodeCounter *next_node = malloc (sizeof (NextNodeCounter));
  *next_node = (NextNodeCounter) {second_node, 1};
  first_node->counter_list[first_node->len_counter_list] = next_node;
  first_node->len_counter_list++;
  return true;
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
/**
 * If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database
 */
{
  Node *node = get_node_from_database (markov_chain, data_ptr);
  if (node)
  {
    return node;
  }
  void *data = markov_chain->copy_func (data_ptr);
  MarkovNode *new_node = malloc (sizeof (MarkovNode));
  NextNodeCounter **p_next_node = malloc (sizeof (NextNodeCounter *));
  *new_node = (MarkovNode) {data, p_next_node, EMPTY_LIST};
  add (markov_chain->database, new_node);
  return markov_chain->database->last;
}
