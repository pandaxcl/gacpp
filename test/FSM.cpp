#include <catch.hpp>
#include "gacpp/gacpp.hpp"
#include <random>
#include <iterator>
#include <vector>
#include "gacpp/ga-fsm.hpp"
#include <iostream>

template<int N_max_states, int N_conditions, int N_actions>
struct FindFiniteStateMachine
{
    typedef FindFiniteStateMachine                              this_type;
    typedef std::vector<size_t>                                 actions_type;
    
    struct Value;
    typedef Value                                               value_type;
    typedef std::default_random_engine                          random_engine_type;
    typedef double                                              real_type;
    typedef gacpp::model::basic_gene<this_type>                 gene_type;
    typedef gacpp::model::chromosome<gene_type,N_max_states>    member_type;
    typedef typename member_type::iterator                      gene_iterator;
    typedef gacpp::algorithm::simple_team<this_type>            team_t;
   
    struct Value
    {
        static void random_generate_actions(actions_type&actions, random_engine_type&random)
        {
            actions.resize(random()%5);
            for (auto&&action:actions)
                action = random()%N_actions;
        };
        static void mutate_one_action_for_actions(actions_type&actions, random_engine_type&random)
        {
            if (actions.size() == 1)
                actions[0] = random()%N_actions;
            else if (actions.size() > 1)
                actions[random()%actions.size()] = random()%N_actions;
        }
        enum class T:char {INITIAL, STATE, TRANSITION} type;
        struct {
            size_t          state = 0;
            void random_initialize(random_engine_type&random)
            {
                state = random()%N_max_states;
            }
            void mutate(random_engine_type&random)
            {
                state = random()%N_max_states;
            }
        }initial;
        struct {
            actions_type    on_enter;
            actions_type    on_leave;
            void random_initialize(random_engine_type&random)
            {
                Value::random_generate_actions(on_enter, random);
                Value::random_generate_actions(on_leave, random);
            }
            void mutate(random_engine_type&random)
            {
                if(0 == random()%2) Value::mutate_one_action_for_actions(on_enter, random);
                if(0 == random()%2) Value::mutate_one_action_for_actions(on_leave, random);
                if(0 == random()%10) Value::random_generate_actions(on_enter, random);
                if(0 == random()%10) Value::random_generate_actions(on_leave, random);
            }
        }state;
        struct {
            size_t          condition;
            size_t          target_state;
            actions_type    actions;
            void random_initialize(random_engine_type&random)
            {
                condition = random()%N_conditions;
                target_state = random()%N_max_states;
                Value::random_generate_actions(actions, random);
            }
            void mutate(random_engine_type&random)
            {
                if (0 == random()%3) condition = random()%N_conditions;
                if (0 == random()%3) target_state = random()%N_max_states;
                if (0 == random()%3) Value::mutate_one_action_for_actions(actions, random);
                if (0 == random()%10) Value::random_generate_actions(actions, random);
            }
        }transition;
        void random_initialize(random_engine_type&random)
        {
            type = static_cast<T>(random()%3);
            initial.random_initialize(random);
            state.random_initialize(random);
            transition.random_initialize(random);
        }
        void mutate(random_engine_type&random)
        {
            if (0 == random()%10) type = static_cast<T>(random()%3);
            if (0 == random()%5) initial.mutate(random);
            if (0 == random()%2) state.mutate(random);
            if (0 == random()%2) transition.mutate(random);
        }
    };
    
    static void random_initialize(gene_iterator it, random_engine_type&random)
    {
        it->value().random_initialize(random);
    }
    
    static real_type compute_fitness(gene_iterator begin, gene_iterator end, random_engine_type&random)
    {
        return real_type(0);
    }
    
    static void mutate(gene_iterator it, random_engine_type&random)
    {
        it->value().mutate(random);
    }
};

SCENARIO("", "[FSM]")
{
    GIVEN("turn on/off one light to check finite_state_machine code")
    {
        enum {UNKNOWN, NOT_CONNECTED, CONNECTED} powerState = UNKNOWN;
        enum {OFF, ON} lightState = OFF;
        finite_state_machine::machine lightMachine;
        lightMachine.actions = {
            /*0*/[&lightState](){ lightState = OFF; },
            /*1*/[&lightState](){ lightState = ON; },
        };
        lightMachine.conditions = {
            /*0*/[&powerState](){ return NOT_CONNECTED == powerState; },
            /*1*/[&powerState](){ return CONNECTED == powerState; },
        };
        lightMachine.initial_state = 0;
        lightMachine.states = {
            /*0: OFF*/{
                /* on_enter = */{},
                /* on_leave = */{},
                /* transitions = */{
                    {
                        /*    condition */1,
                        /*      actions */{},
                        /* target_state */1
                    },
                }
            },
            /*1: ON */{
                /* on_enter = */{1},
                /* on_leave = */{0},
                /* transitions = */{
                    {
                        /*    condition */0,
                        /*      actions */{},
                        /* target_state */0
                    },
                }
            },
        };
        
        auto switch_turn_on = [&powerState, &lightMachine](){
            powerState = CONNECTED;
            lightMachine.update();
            powerState = UNKNOWN;
        };
        auto switch_turn_off = [&powerState, &lightMachine](){
            powerState = NOT_CONNECTED;
            lightMachine.update();
            powerState = UNKNOWN;
        };
        
        WHEN("start the light machine")
        {
            lightMachine.start();
            THEN("must be started with OFF state")
            {
                REQUIRE(lightState == OFF);
            }
            THEN("check logical normal")
            {
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_off();
                REQUIRE(lightState == OFF);
            }
            THEN("check logical unusual 1")
            {
                switch_turn_off();
                REQUIRE(lightState == OFF);
                switch_turn_off();
                REQUIRE(lightState == OFF);
                switch_turn_off();
                REQUIRE(lightState == OFF);
                switch_turn_off();
                REQUIRE(lightState == OFF);
            }
            THEN("check logical unusual 2")
            {
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_on();
                REQUIRE(lightState == ON);
            }
            THEN("check logical unusual 3")
            {
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_off();
                REQUIRE(lightState == OFF);
                switch_turn_off();
                REQUIRE(lightState == OFF);
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_on();
                REQUIRE(lightState == ON);
                switch_turn_off();
                REQUIRE(lightState == OFF);
                switch_turn_off();
                REQUIRE(lightState == OFF);
            }
        }
    }
    
    GIVEN("living room's lights")
    {
        const int N_max_states = 10;
        const int N_conditions = 2;
        const int N_actions = 2;
        typedef FindFiniteStateMachine<N_max_states, N_conditions, N_actions> Solution;
        static_assert(gacpp::model::basic_gene_concept::random_initialize::single<Solution>::enabled, "");
        static_assert(gacpp::model::basic_gene_concept::compute_fitness::range<Solution>::enabled, "");
        static_assert(!gacpp::model::basic_gene_concept::crossover::two_ranges<Solution>::enabled, "");
        static_assert(gacpp::model::basic_gene_concept::mutate::single<Solution>::enabled, "");
        
        
        Solution::team_t GA(100);
        REQUIRE(GA.size() == 100);
        GA.resize(200);
        REQUIRE(GA.size() == 200);
        GA.random_initialize();
        GA.epoch();
    }
    
    GIVEN("clock")
    {
        
    }
}
