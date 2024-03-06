# tweets_generator

This is a simple text-based tweets generator implemented in C. The program uses a Markov chain to generate random sequences of words based on an input text corpus. Each generated sequence represents a tweet.

## Files

1. **tweets_generator.c**
   - The main program file that orchestrates the tweets generation.
   - Implements the core logic, including processing input arguments, reading a text corpus, and utilizing a Markov chain to generate tweets.

2. **linked_list.h / linked_list.c**
   - Header and source files for a basic linked list implementation.
   - Used for managing the Markov chain database.

3. **markov_chain.h / markov_chain.c**
   - Header and source files defining the Markov chain data structure and related functions.
   - Handles the generation of random sequences based on the Markov chain.

4. **CMakeLists.txt**
   - The CMake configuration file to build the executable.

## Building and Running

Ensure you have CMake installed and execute the following commands to build the project:

```bash
mkdir build
cd build
cmake ..
make
```

Run the executable with the required command-line arguments:

```bash
./ex3b_ori_levine <seed> <num_tweets> <text_corpus_file> [words_to_read]
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
