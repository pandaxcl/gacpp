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
    typedef FindFiniteStateMachine this_type;
    
    typedef std::vector<size_t> actions_type;
   
    struct Value
    {
        enum class T:size_t {INITIAL, STATE, TRANSITION} type;
        struct {
            size_t          state = 0;
        }initial;
        struct {
            actions_type    on_enter;
            actions_type    on_leave;
        }state;
        struct {
            size_t          condition;
            size_t          target_state;
            actions_type    actions;
        }transition;
    };
    typedef Value value_type;
    
    typedef double real_type;
    typedef gacpp::model::basic_gene<this_type> gene_type;
    typedef std::default_random_engine random_engine;
    typedef gacpp::model::chromosome<gene_type,N_max_states> chromosome_type;
    typedef typename chromosome_type::iterator gene_iterator;
    typedef gacpp::algorithm::team<chromosome_type, this_type> team_t;
    
//    template<typename ForwardIterator, typename Random>
//    void random_initialize(ForwardIterator it, Random&&random)
//    {
//    }
    template<typename ForwardIterator, typename Random>
    static void random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
    {
        auto random_generate_actions = [&random](actions_type&actions){
            actions.resize(random()%5);
            for (auto&&action:actions)
                action = random()%N_actions;
        };
        auto random_initialize_for_initial = [&random](ForwardIterator it){
            auto&&self = it->_value;
            self.type = value_type::T::INITIAL;
            self.initial.state = random()%N_max_states;
        };
        
        auto random_initialize_for_state = [&random,&random_generate_actions](ForwardIterator it){
            auto&&self = it->_value;
            self.type = value_type::T::STATE;
            random_generate_actions(self.state.on_enter);
            random_generate_actions(self.state.on_leave);
        };
        auto random_initialize_for_transition = [&random,&random_generate_actions](ForwardIterator it){
            auto&&self = it->_value;
            self.type = value_type::T::TRANSITION;
            self.transition.condition = random()%N_conditions;
            self.transition.target_state = random()%N_max_states;
            random_generate_actions(self.transition.actions);
        };
        for (auto it=begin; it!=end; it++)
        {
            size_t index = std::distance(begin, it);
            if (0 == index)
                random_initialize_for_initial(it);
            else if (1 <= index && index < 1+N_max_states)
                random_initialize_for_state(it);
            else
                random_initialize_for_transition(it);
        }
        
    }
    template<typename ForwardIterator, typename Random>
    static real_type compute_fitness(ForwardIterator begin, ForwardIterator end, Random&&random)
    {
        return real_type(0);
    }
    
    template<typename ForwardIterator, typename Random>
    static void crossover(ForwardIterator begin1, ForwardIterator end1,
                          ForwardIterator begin2, ForwardIterator end2, Random&&random)
    {
        
    }
    template<typename ForwardIterator, typename Random>
    static void mutate(ForwardIterator it, Random&&random)
    {
        
    }
//    template<typename ForwardIterator, typename Random>
//    static void mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
//    {
//        
//    }
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
        static_assert(gacpp::model::basic_gene_concept::random_initialize::range<Solution>::enabled, "");
        static_assert(gacpp::model::basic_gene_concept::compute_fitness::range<Solution>::enabled, "");
        static_assert(gacpp::model::basic_gene_concept::crossover::two_ranges<Solution>::enabled, "");
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
