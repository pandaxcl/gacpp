#include <catch.hpp>
#include "gacpp/gacpp.hpp"
#include <random>
#include <iterator>
#include <vector>
#include "gacpp/ga-fsm.hpp"
#include <iostream>

SCENARIO("", "[FSM]")
{
    const size_t N = 4;
    struct Gene
    {
        enum class T:size_t {INITIAL, STATE, TRANSITION} type; // 0
        struct {
            size_t state = 0; // 1
        }initial;
        struct {
            size_t on_enter[N]; // [2, 2+N)
            size_t on_leave[N]; // [2+N, 2+2N)
        }state;
        struct {
            size_t condition;   // 2+2N+0
            size_t target_state;// 2+2N+1
            size_t actions[N];  // [2+2N+2, 2+2N+2+N)
        }transition;
        size_t crossover(size_t i)
        {
            return i;
        }
        void mutate()
        {
            
        }
    };
    
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
        typedef gacpp::model::chromosome<Gene, 100, size_t> chromosome_t;
        typedef gacpp::algorithm::population<chromosome_t> population_t;
        
        population_t GA;
        GA.random_initialize();
        GA.epoch();
    }
    
    GIVEN("clock")
    {
        
    }
}
