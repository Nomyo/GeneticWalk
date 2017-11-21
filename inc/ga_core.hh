#pragma once
#include <opengl-utils.hh>
#include <character.hh>
#include <world.hh>

#define DEATH_PENALTY 1000
#define DONE_PENALTY 300
#define SUCCESS_BONUS 500
#define SIZE_POPULATION 50

/**
 * Compute the fitness of the corresponding character
 */
double fitness(const Character& c);

/**
 * Perform a uniform crossover between two characters
 */
std::vector<Character::Action> uniform_crossover(const Character& c1,
						 const Character& c2);

/**
 * Perform a mutation on the character with a certain probability
 */
void mutation(Character& c);

/**
 * Creation of a new offspring character with two parents character
 */
Character gen_offspring(const Character& c1, const Character& c2,
			const World& w);

/*
 * Basic tournament selection using fitness
 */
Character tournament_selection(const std::vector<Character>& pop,
			       unsigned int tournament_size,
			       const World& w);

/*
 * Create the next generation using tournament selection
 */
std::vector<Character> create_next_generation(const std::vector<Character>& pop,
					      const World& w);
