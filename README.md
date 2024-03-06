# Markov Chain - Generic Programming Project

**Introduction**
This C-based generic Markov chain project utilizes generic programming principles. The program employs a Markov chain to generate random sequences of words based on an input text corpus. The generated sequences can represent tweets or other data structures, showcasing the versatility of the implementation.

**Tweets Generator**
The primary focus is on the tweets_generator aspect of the project. The program takes an input text corpus and utilizes a Markov chain to generate random sequences of words. Each sequence serves as a representation of a tweet. This implementation demonstrates the power of generic programming by allowing the generation of various types of sequences beyond tweets.

## Files

1. **tweets_generator.c**
   - Implements the Markov chain to generate tweets.

2. **linked_list.h / linked_list.c**
   - Header and source files for a basic linked list implementation.
   - Used for managing the Markov chain database.

3. **markov_chain.h / markov_chain.c**
   - Header and source files defining the Markov chain data structure and related functions.
   - Handles the generation of random sequences based on the Markov chain.

4. **CMakeLists.txt**
   - The CMake configuration file to build the executable.

5. **makefile**
   - Makefile for convenient project compilation and execution.

6. **snakes_and_ladders.c**
   - Test file showcasing the generic implementation of the markov chain with a different data type.

## Compilation and Execution

### Tweets Generator

To compile and execute the tweets generator, use the following commands in the terminal (make sure you are in the directory containing the source code files):

```bash
make tweets
./tweets_generator 123 2 "justdoit_tweets.txt"
```

The `make tweets` command will generate a compiled file that can be run with the provided example command-line arguments (e.g., `123 2 "justdoit_tweets.txt"`). Feel free to modify the arguments based on your requirements.

### Snakes and Ladders Test

To compile and execute the snakes and ladders test, use the following commands:

```bash
make snake
./ladders_and_snakes arg1 arg2 arg3
```


- `<seed>`: Seed for the random number generator.
- `<num_tweets>`: Number of tweets to generate.
- `<text_corpus_file>`: Path to the text corpus file.
- `[words_to_read]` (optional): Number of words to read from the text corpus.

## Error Messages

- `ARG_ERR_MSG`: Indicates an invalid number of command-line arguments.
- `FILE_ERR_MSG`: Indicates an issue with the specified text corpus file.
- `ALLOCATION_ERR_MSG`: Indicates a memory allocation failure.

## Functionality

- Reads the text corpus file and builds a Markov chain database.
- Generates random sequences of words based on the Markov chain.
- Outputs the generated tweets to the console.

## Notes

- The program may display error messages if there are issues with command-line arguments or file access.
- Memory allocation failures are handled with appropriate error messages.

Feel free to explore and modify the source code to enhance the functionality or adapt it to your specific use case.
