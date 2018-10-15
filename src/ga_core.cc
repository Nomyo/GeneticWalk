#include <ga_core.hh>
#include <ctime>
#include <functional>
#include <limits>
#include <chrono>
#include <thread>

using CState = Character::CharacterState;

std::vector<Character::Action> uniform_crossover(const Character& c1,
    const Character& c2)
{
    const auto& dna_c1 = c1.get_DNA();
    const auto& dna_c2 = c2.get_DNA();

    auto new_dna = std::vector<Character::Action>{};
    new_dna.reserve(dna_c1.size());

    // We perform crossover in a sequence of gene with a minimum
    // crossover of 20% and a max of 25 % of total DNA length
    auto compute_crossover_length = [](const size_t DNA_length)
    {
        auto min_crossover_length = DNA_length / 100;
        if (!min_crossover_length)
            min_crossover_length++;

        std::random_device rd;
        std::mt19937 engine(rd());
        std::uniform_int_distribution<int> distribution(0, min_crossover_length * 9);
        auto rand_cross_len = std::bind(distribution, engine);
        min_crossover_length += rand_cross_len();
        return min_crossover_length;
    };

    auto of_c1 = false;
    auto crossover_length = compute_crossover_length(dna_c1.size());
    for (auto n = 0u; n < dna_c1.size(); ++n)
    {
        // We compute which are the next parent DNA
        if (n % crossover_length == 0)
        {
            crossover_length = compute_crossover_length(dna_c1.size());
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
    std::uniform_int_distribution<> distr_mutation(0, 5000);

    auto rand_dna = std::bind(distr_dna, eng);
    auto rand_mut = std::bind(distr_mutation, eng);

    // for each gene there is 1% chance to mutate
    // FIXME SHOULD CHANGE THROUGH THE TIME
    for (auto n = 0u; n < c.get_DNA().size(); ++n)
    {
        if (rand_mut() == 7)
        {
            // we mutate everytime a package of 5 genes
            for (auto i = n; i < (2 + n) && i < c.get_DNA().size(); ++i) {
                c.switch_DNA(i, static_cast<Character::Action>(rand_dna()));
                std::cout << i << std::endl;
            }
        }
    }
}

Character gen_offspring(const Character& c1, const Character& c2, const World& w)
{
    Character offspring(Entity(choose_model(), w.get_startpoint(),
        glm::vec3{ 0.0f, 0.0f, 0.0f }, 0.11f));
    offspring.set_state(Character::CharacterState::ALIVE);
    offspring.replace_DNA(uniform_crossover(c1, c2));
    mutation(offspring);

    return offspring;
}

static double fit_max = -20000.0; // 4536
static int best_nb_steps = 100000;
static int best_repeat_steps = 10000;
static int gen = 0;
double fitness(const Character& c, const World& w)
{
    const auto& c_state = c.get_state();
    double non_fit = 0.0;

    auto nb_step = c.get_position_passed().size();
    auto nb_repeat_step = 0;
    auto total_distance = 0.0;
    for (const auto& p : c.get_position_passed())
    {
        if (p.second > 1)
            nb_repeat_step += p.second - 1;

        // We want to compute the average distance to the destination.
        total_distance += w.dist_to_endzone(p.first);
    }

    non_fit = nb_step * SPEED_PENALTY + nb_repeat_step * REPEAT_PENALTY + (total_distance / nb_step) * DISTANCE_PENALTY;

    if (c_state == CState::FALLING || c_state == CState::DEAD)
        non_fit += DEATH_PENALTY;
    else
    {
        if (c_state == CState::DONE)
            non_fit += DONE_PENALTY;

        if (c_state == CState::SUCCESS)
            non_fit -= SUCCESS_BONUS;
    }

    // Final distance to destination.
    non_fit += w.dist_to_endzone(c.get_position()) * 50.0; // COEFF 100
    double fit = -non_fit;

    if (fit > fit_max)
    {
        fit_max = fit;
        std::cout << "FIT_MAX : " << fit_max << std::endl;
        std::cout << "nb_steps = " << nb_step << std::endl;
        std::cout << "repeat_steps = " << nb_repeat_step << std::endl;
    }

    return fit;
}

Character tournament_selection(const std::vector<Character>& people,
    unsigned int tournament_size,
    const World& w)
{
    auto best = people[std::rand() % people.size()];

    for (auto n = 0u; n < tournament_size - 1; n++)
    {
        auto individual = people[std::rand() % people.size()];
        if (fitness(individual, w) > fitness(best, w))
            best = individual;
    }

    return best;
}

std::vector<Character> create_next_generation(const std::vector<Character>& people,
    const World& w)
{
    gen++;
    auto next_people = std::vector<Character>{};
    next_people.reserve(people.size());
    auto nb_new_people = 0;

    std::srand(std::time(nullptr));

    // 95% of the next generation is composed of new offspring or 1 survive if
    // the population if under 50 people.
    while (nb_new_people != SIZE_POPULATION - (SIZE_POPULATION / 100) - 5)
    {
        auto parent1 = tournament_selection(people, 3, w);
        auto parent2 = tournament_selection(people, 3, w);

        auto offspring = gen_offspring(parent1, parent2, w);
        next_people.push_back(offspring);
        nb_new_people++;
    }

    // 20% of the next generation is composed of prev generation individual
    while (nb_new_people != SIZE_POPULATION)
    {
        auto individual = tournament_selection(people, 3, w);
        individual.reset(w);
        next_people.push_back(individual);
        nb_new_people++;
    }

    return next_people;
}
