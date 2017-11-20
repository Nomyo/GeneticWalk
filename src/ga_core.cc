#include <ga_core.hh>
#include <ctime>
#include <functional>
#include <limits>

using CState = Character::CharacterState;

std::vector<Character::Action> uniform_crossover(const Character& c1,
						 const Character& c2)
{
  const auto& dna_c1 = c1.get_DNA();
  const auto& dna_c2 = c2.get_DNA();

  auto new_dna = std::vector<Character::Action>{};
  new_dna.reserve(dna_c1.size());

  // We perform crossover in a sequence of gene with a minimum
  // crossover of 1% of total DNA length
  auto min_crossover_length = dna_c1.size() / 100;
  if (!min_crossover_length)
    min_crossover_length++;

  auto of_c1 = false;

  for (auto n = 0u; n < dna_c1.size(); ++n)
  {
    // We compute which are the next parent DNA
    if (n % min_crossover_length == 0)
    {
      if (std::rand() % 2 == 0)
	of_c1 = true;
      else
	of_c1 = false;
    }

    if (of_c1)
      new_dna.push_back(dna_c1[n]);
    else
      new_dna.push_back(dna_c2[n]);
  }

  return new_dna;
}

void mutation(Character& c)
{
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr_dna(0, 7);
  std::uniform_int_distribution<> distr_mutation(0, 1000);

  auto rand_dna = std::bind(distr_dna, eng);
  auto rand_mut = std::bind(distr_mutation, eng);

  // for each gene there is 0.1% chance to mutate
  // FIXME SHOULD CHANGE THROUGH THE TIME
  for (auto n = 0u; n <  c.get_DNA().size(); ++n)
  {
    if (rand_mut() == 0)
      c.switch_DNA(n, static_cast<Character::Action>(rand_dna()));
  }
}

Character gen_offspring(const Character& c1, const Character& c2, const World& w)
{
  auto model = Model("models/characters/Models/Non-rigged/basicCharacter.obj",
		     choose_texture(), "", false);
  Character offspring(Entity(model, w.get_startpoint(),
			     glm::vec3{0.0f, 0.0f, 0.0f}, 0.11f));
  offspring.set_state(Character::CharacterState::ALIVE);
  offspring.replace_DNA(uniform_crossover(c1, c2));
  mutation(offspring);

  return offspring;
}

double fitness(const Character& c, const World& w)
{
  const auto& c_state = c.get_state();
  double fit = 0.0;

  auto nb_step = c.get_position_passed().size();
  auto nb_repeat_step = 0;
  auto total_distance = 0.0;
  for (const auto& p : c.get_position_passed())
  {
    if(p.second > 1)
      nb_repeat_step += p.second;

    total_distance += w.dist_to_endzone(p.first);
  }

  fit = nb_step + nb_repeat_step + total_distance / nb_step;

  if (c_state == CState::FALLING || c_state == CState::DEAD)
    fit += DEATH_PENALTY;

  return -fit;
}

Character tournament_selection(const std::vector<Character>& pop,
			       unsigned int tournament_size,
			       const World& w)
{
  auto best = pop[rand() % pop.size()];

  for (auto n = 0u; n < tournament_size - 1; n++)
  {
    auto individual = pop[rand() % pop.size()];
    if (fitness(individual, w) > fitness(best, w))
      best = individual;
  }

  return best;
}

std::vector<Character> create_next_generation(const std::vector<Character>& pop,
					      const World& w)
{
  auto next_pop = std::vector<Character>{};
  next_pop.reserve(pop.size());
  auto nb_new_pop = 0;
  std::srand(std::time(0));

  // 80% of the next generation is composed of new offspring
  while (nb_new_pop != SIZE_POPULATION - (SIZE_POPULATION / 100 * 20))
  {
    auto parent1 = tournament_selection(pop, 2, w);
    auto parent2 = tournament_selection(pop, 2, w);
    auto offspring = gen_offspring(parent1, parent2, w);
    next_pop.push_back(offspring);
    nb_new_pop++;
  }

  // 20% of the next generation is composed of prev generation individual
  while (nb_new_pop != SIZE_POPULATION)
  {
    auto individual = tournament_selection(pop, 2, w);
    next_pop.push_back(individual);
    nb_new_pop++;
  }

  return next_pop;
}
