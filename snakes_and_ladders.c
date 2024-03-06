#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define ARGS_NUM 3
#define ARG_ERR_MSG "Usage: The number of arguments is invalid.\n"
#define ALLOCATION_ERROR_MASSAGE \
"Allocation failure: Failed to allocate new memory\n"
#define SEED 1
#define TURNS 2
#define DECIMAL 10
#define LAST_CELL 100
/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there
    // is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
  {
    free_markov_chain (database);
  }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    cells[i] = malloc (sizeof (Cell));
    if (cells[i] == NULL)
    {
      for (int j = 0; j < i; j++)
      {
        free (cells[j]);
      }
      handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
      return EXIT_FAILURE;
    }
    *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
  }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
  {
    int from = transitions[i][0];
    int to = transitions[i][1];
    if (from < to)
    {
      cells[from - 1]->ladder_to = to;
    }
    else
    {
      cells[from - 1]->snake_to = to;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    add_to_database (markov_chain, cells[i]);
  }

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    from_node = get_node_from_database (markov_chain, cells[i])->data;

    if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
    {
      index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
      to_node = get_node_from_database (markov_chain, cells[index_to])
          ->data;
      add_node_to_counter_list
      (from_node, to_node, markov_chain);
    }
    else
    {
      for (int j = 1; j <= DICE_MAX; j++)
      {
        index_to = ((Cell *) (from_node->data))->number + j - 1;
        if (index_to >= BOARD_SIZE)
        {
          break;
        }
        to_node = get_node_from_database
            (markov_chain, cells[index_to])->data;
        add_node_to_counter_list
        (from_node, to_node, markov_chain);
      }
    }
  }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    free (cells[i]);
  }
  return EXIT_SUCCESS;
}

static bool is_last_cell (void *data)
{
  Cell *cell = (Cell *) data;
  if (cell->number == LAST_CELL)
  {
    return true;
  }
  return false;
}

static void *copy_cell (void *data)
{
  Cell *cell = (Cell *) data;
  Cell *new_cell = malloc (sizeof (Cell));
  if (new_cell == NULL)
  {
    return NULL;
  }
  *new_cell = (Cell) {cell->number, cell->ladder_to,
                      cell->snake_to};
  return new_cell;
}

static void print_cell (void *data)
{
  Cell *cell = (Cell *) data;
  printf ("[%d]", cell->number);
  if (cell->ladder_to > 0)
  {
    printf ("-ladder to %d", cell->ladder_to);
  }
  else if (cell->snake_to > 0)
  {
    printf ("-snake to %d", cell->snake_to);
  }
  if (cell->number != LAST_CELL)
  {
      printf (" -> ");
  }
}

static int comp_cell (void *data1, void *data2)
{
  Cell *cell1 = (Cell *) data1;
  Cell *cell2 = (Cell *) data2;
  return cell1->number - cell2->number;
}

static int check_valid_args (int argc)
{
  if (argc != ARGS_NUM)
  {
    printf (ARG_ERR_MSG);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
  if (check_valid_args (argc))
  {
    return EXIT_FAILURE;
  }
  long int seed = strtol (argv[SEED], NULL, DECIMAL);
  long int turns = strtol (argv[TURNS], NULL, DECIMAL);
  srand (seed);
  Cell *cells[BOARD_SIZE];
  create_board (cells);
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  if (markov_chain == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  LinkedList *linked_list = malloc (sizeof (LinkedList));
  *linked_list = (LinkedList) {NULL, NULL, 0};
  if (linked_list == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  *markov_chain = (MarkovChain)
      {linked_list, print_cell, comp_cell,
       free, copy_cell, is_last_cell};
  fill_database (markov_chain);
  int steps_counter = 1;
  while (steps_counter <= turns)
  {
    printf ("Random Walk %d: ", steps_counter);
    generate_random_sequence
        (markov_chain, markov_chain->database->first->data,
         MAX_GENERATION_LENGTH);
    steps_counter++;
  }
  for (int i = 0; i < BOARD_SIZE; i++)
  {
      free(cells[i]);
  }
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}
