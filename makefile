tweets: tweets_generator.c linked_list.c markov_chain.c
	gcc -Wall -Wextra -Wvla -std=c99 tweets_generator.c linked_list.c markov_chain.c -o tweets_generator
snakes: snakes_and_ladders.c linked_list.c markov_chain.c
	gcc -Wall -Wextra -Wvla -std=c99 snakes_and_ladders.c linked_list.c markov_chain.c -o snakes_and_ladders