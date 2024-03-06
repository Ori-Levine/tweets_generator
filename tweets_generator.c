#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "markov_chain.h"

// messages
#define ARG_ERR_MSG "Usage: The number of arguments is invalid.\n"
#define FILE_ERR_MSG "Error: The given file is invalid.\n"
#define ALLOCATION_ERR_MSG "Allocation failure: there was problem to create markov_chain"
// constants
#define TWEET_MAX_LEN 1001
#define MAX_WORDS_IN_TWEET 20
#define MIN_ARGS_NUM 4
#define MAX_ARGS_NUM 5
#define SEED_IND 1
#define TWEETS_IND 2
#define TEXT_CORPUS_IND 3
#define WORDS_TO_READ_IND 4
#define WHITE_SPACE " "
#define END_LINE "\n"
#define DECIMAL 10

static bool is_last_str (void *data)
/**
 * Check if the given string ends with a dot.
 * @param data pointer to a string
 * @return true if the string ends with a dot, false otherwise
 */
{
  char *string = (char *) data;
  unsigned long int last_ind = strlen (string) - 1;
  if (last_ind == -1)
  {
    return false;
  }
  if (string[last_ind] == '.')
  {
    return true;
  }
  return false;
}

static void *copy_str (void *data)
/**
 * Copy the given string.
 * @param data pointer to a string
 * @return pointer to the new string
 */
{
  char *str = (char *) data;
  char *new_str = malloc (strlen (str) + 1);
  if (new_str == NULL)
  {
    return NULL;
  }
  strcpy (new_str, str);
  return new_str;
}

static void print_str (void *data)
/**
 * Print the given string.
 * @param data pointer to a string
 */
{
  char *str = (char *) data;
  printf (" %s", str);
}

static int comp_str (void *data1, void *data2)
/**
 * Compare two strings.
 * @param data1 pointer to the first string
 * @param data2 pointer to the second string
 * @return 0 if the strings are equal, a positive value if the first string is
 * greater than the second string, and a negative value if the first string is
 * smaller than the second string
 */
{
  char *str1 = (char *) data1;
  char *str2 = (char *) data2;
  return strcmp (str1, str2);
}

static MarkovNode *process_word (char *word, struct MarkovChain *markov_chain,
                                 MarkovNode **last_word)
/**
 * Process the given word and add it to the database.
 * @param word - the word to process and add to the database
 * @param markov_chain - the markov_chain to add the word to
 * @param last_word - the last word that was processed
 * @return the markov_node that was created or found in the database
 */
{
  word[strcspn (word, END_LINE)] = 0;
  unsigned int len_word = strlen (word);
  char *tweet_copy = malloc (len_word + 1);
  strcpy (tweet_copy, word);
  Node *node = get_node_from_database (markov_chain, tweet_copy);

  if (node == NULL) // if markov_node not in markov_chain
  {
    node = add_to_database (markov_chain, tweet_copy);
    if (markov_chain->database->size == 1)
    {
      *last_word = node->data;
      free (tweet_copy); // Freeing memory before returning
      return node->data;
    }
  }
  if (!is_last_str ((*last_word)->data)) // word doesn't end with "."
  {
    add_node_to_counter_list (*last_word, node->data, markov_chain);
  }
  free (tweet_copy);
  return node->data;
}

static void
process_tweet (char *tweet, int *words_to_read,
               struct MarkovChain *markov_chain, MarkovNode **last_word)
/**
 * The function reads 'words_to_read' words from the tweet and adds them to
 * the database and updates the last word that was processed.
 * @param tweet - the tweet to process and add to the database
 * @param words_to_read - the number of words to read from the tweet
 * @param markov_chain - the markov_chain to add the tweet to
 * @param last_word - the last word that was processed
 */
{
  char *word = strtok (tweet, WHITE_SPACE);
  while (word && *words_to_read)
  {
    *last_word = process_word (word, markov_chain, last_word);
    word = strtok (NULL, WHITE_SPACE);
    (*words_to_read)--;
  }
}

static int
fill_database (FILE *fp, int words_to_read, struct MarkovChain *markov_chain)
/**
 * Fill the markov_chain's database with the given words from the given file.
 * @param fp pointer to the file
 * @param words_to_read number of words to read from the file. If
 * words_to_read is -1, the function will read the entire file.
 * @param markov_chain pointer to the markov_chain
 * @return EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
{
  char tweet[TWEET_MAX_LEN];
  MarkovNode **last_word = malloc (sizeof (MarkovNode *));

  while (fgets (tweet, TWEET_MAX_LEN, fp) && words_to_read != 0)
  {
    process_tweet (tweet, &words_to_read, markov_chain, last_word);
  }

  free (last_word);
  fclose (fp);
  return EXIT_SUCCESS;
}

static int check_valid_args (int args)
/**
 * Check if the number of arguments is valid.
 * @param args the number of arguments
 * @return EXIT_SUCCESS if the number of arguments is valid, EXIT_FAILURE
 * otherwise
 */
{
  if (args < MIN_ARGS_NUM || args > MAX_ARGS_NUM)
  {
    printf (ARG_ERR_MSG);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int check_file (char *const *argv)
/**
 * Check if the given file is valid.
 * @param argv the arguments
 * @return EXIT_SUCCESS if the file is valid, EXIT_FAILURE otherwise
 */
{
  FILE *input = NULL;
  input = fopen (argv[TEXT_CORPUS_IND], "r");
  if (input == NULL)
  {
    printf (FILE_ERR_MSG);
    return EXIT_FAILURE;
  }
  fclose (input);
  return EXIT_SUCCESS;
}

static MarkovChain *initiate_markov_chain ()
/**
 * Allocate memory for a new markov_chain.
 * @return a pointer to the new markov_chain
 */
{
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  if (markov_chain == NULL)
  {
    printf (ALLOCATION_ERR_MSG);
    return NULL;
  }
  return markov_chain;
}

static MarkovChain *initiate_linked_list (MarkovChain **markov_chain)
/**
 * Allocate memory for a new linked_list and initiate the markov_chain.
 * @param markov_chain a pointer to the markov_chain
 * @return a pointer to the new markov_chain
 */
{
  LinkedList *list = malloc (sizeof (LinkedList));
  if (list == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  *list = (LinkedList) {NULL, NULL, 0};
  (**markov_chain) = (MarkovChain)
      {list, print_str, comp_str,
       free, copy_str, is_last_str};
  return *markov_chain;
}

int main (int args, char **argv)
{
  if (check_valid_args (args))
  {
    return EXIT_FAILURE;
  }
  if (check_file (argv))
  {
    return EXIT_FAILURE;
  }
  long int seed = strtol (argv[SEED_IND], NULL, DECIMAL);
  long int words_to_read = -1;
  if (args == MAX_ARGS_NUM)
  {
    words_to_read = strtol (argv[WORDS_TO_READ_IND], NULL, DECIMAL);
  }
  srand (seed);
  MarkovChain *markov_chain = initiate_markov_chain ();
  if (!markov_chain)
  { return EXIT_FAILURE; }
  initiate_linked_list (&markov_chain);
  if (!markov_chain->database)
  { return EXIT_FAILURE; }
  FILE *input = NULL;
  input = fopen (argv[TEXT_CORPUS_IND], "r");
  fill_database (input, words_to_read, markov_chain);
  long int max_tweets = strtol
      (argv[TWEETS_IND], NULL, DECIMAL);
  int tweet_counter = 1;
  while (tweet_counter <= max_tweets)
  {
    printf ("Tweet %d:", tweet_counter);
    generate_random_sequence (markov_chain, NULL, MAX_WORDS_IN_TWEET);
    tweet_counter++;
  }
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}