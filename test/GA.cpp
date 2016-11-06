#include <catch_with_main.hpp>
#include "gacpp/gacpp.hpp"
#include <random>
#include <iterator>


SCENARIO("roulette_one", "[GA]")
{
    GIVEN("a random [0, 1] generator")
    {
        const size_t N = 6;
        const float V[N] = { 0.3, 0.9, 0.2, /* _ */ 0.7, 0.5, 0.4, };
        auto random_0_1=[V]()->float{
            static size_t i = 0;
            i++;
            return V[i-1];
        };
        
        GIVEN("an array of members")
        {
            struct Member {
                float fitness;
            } members[N] = {
                {1.0},
                {1.5},
                {0.8},
                
                {2.0},
                {0.2},
                {0.7},
            };
            
            auto fTotalFitness = 6.2;
            
            REQUIRE(std::abs(fTotalFitness - (1.0+1.5+0.8+2.0+0.2+0.7)) < 0.001);
            
            auto fitness = [](Member&member)->float
            {
                return member.fitness;
            };
            auto slice = [fTotalFitness](float random)
            {
                return random * fTotalFitness;
            };

            WHEN("choose by random 0.3")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.3) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 2nd member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 1);
                }
            }
            WHEN("choose by random 0.9")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.9) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 6th member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 5);
                }
            }
            WHEN("choose by random 0.2")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.2) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 2nd member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 1);
                }
            }
            WHEN("choose by random 0.7")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.7) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 4th member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 3);
                }
            }
            WHEN("choose by random 0.5")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.5) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 3rd member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 2);
                }
            }
            WHEN("choose by random 0.4")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.4) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 2nd member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 1);
                }
            }
        }
    }
}
