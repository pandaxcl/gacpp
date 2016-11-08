#include <catch.hpp>
#include "gacpp/gacpp.hpp"
#include <random>
#include <iterator>
#include <vector>
#include "gacpp/ga-fsm.hpp"

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
        size_t valid_cross_point(size_t i)
        {
            return i;
        }
    };
    
    typedef gacpp::model::chromosome<Gene, 100, size_t> chromosome_t;
    
    GIVEN("turn on/off one light")
    {
        
    }
    
    GIVEN("living room's lights")
    {
        
    }
    
    GIVEN("clock")
    {
        
    }
}
