cmake_minimum_required(VERSION 3.16)
project(ex3b_ori_levine C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ex3b_ori_levine
        linked_list.h
        linked_list.c
        markov_chain.h
        #snakes_and_ladders.c
        tweets_generator.c
        markov_chain.c)
