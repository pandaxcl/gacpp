#include <catch_with_main.hpp>
#include "gacpp/gacpp.hpp"
#include <random>
#include <iterator>


SCENARIO("roulette_one", "[GA]")
{
    GIVEN("a random [0, 1] generator")
    {
        const size_t N = 6;
        const float V[N] = { 0.3, 0.9, 0.1, /* _ */ 0.7, 0.5, 0.87, };
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
            WHEN("choose by random 0.1")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.1) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 1st member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 0);
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
            WHEN("choose by random 0.87")
            {
                auto random = random_0_1();
                REQUIRE(std::abs(random - 0.87) < 0.0001);
                auto it = gacpp::selection::roulette_one(std::begin(members), std::end(members), slice(random), fitness);
                THEN("should select the 2nd member")
                {
                    REQUIRE(std::distance(std::begin(members), it) == 4);
                }
            }
        }
    }
}

SCENARIO("crossover", "[GA]")
{
    GIVEN("A: {0, 1, 2, 3, 4, }")
    {
        int A[5] = {0, 1, 2, 3, 4, };
        REQUIRE(std::distance(std::begin(A), std::end(A)) == 5);
        GIVEN("B: {5, 6, 7, 8, 9, }")
        {
            int B[5] = {5, 6, 7, 8, 9, };
            REQUIRE(std::distance(std::begin(B), std::end(B)) == 5);
            
            WHEN("crossover::with_single_point")
            {
                AND_WHEN("at point 1")
                {
                    gacpp::crossover::with_single_point(std::begin(A), std::end(A),
                                                        std::begin(B), std::end(B),
                                                        1);
                    
                    THEN("A should be: {0, 6, 7, 8, 9} and B should be: {5, 1, 2, 3, 4}")
                    {
                        REQUIRE(A[0] == 0);
                        REQUIRE(A[1] == 6);
                        REQUIRE(A[2] == 7);
                        REQUIRE(A[3] == 8);
                        REQUIRE(A[4] == 9);
                        
                        REQUIRE(B[0] == 5);
                        REQUIRE(B[1] == 1);
                        REQUIRE(B[2] == 2);
                        REQUIRE(B[3] == 3);
                        REQUIRE(B[4] == 4);
                    }
                }
                AND_WHEN("at point 2")
                {
                    gacpp::crossover::with_single_point(std::begin(A), std::end(A),
                                                        std::begin(B), std::end(B),
                                                        2);
                    
                    THEN("A should be: {0, 1, 7, 8, 9} and B should be: {5, 6, 2, 3, 4}")
                    {
                        REQUIRE(A[0] == 0);
                        REQUIRE(A[1] == 1);
                        REQUIRE(A[2] == 7);
                        REQUIRE(A[3] == 8);
                        REQUIRE(A[4] == 9);
                        
                        REQUIRE(B[0] == 5);
                        REQUIRE(B[1] == 6);
                        REQUIRE(B[2] == 2);
                        REQUIRE(B[3] == 3);
                        REQUIRE(B[4] == 4);
                    }
                }
                AND_WHEN("at point 3")
                {
                    gacpp::crossover::with_single_point(std::begin(A), std::end(A),
                                                        std::begin(B), std::end(B),
                                                        3);
                    
                    THEN("A should be: {0, 1, 2, 8, 9} and B should be: {5, 6, 7, 3, 4}")
                    {
                        REQUIRE(A[0] == 0);
                        REQUIRE(A[1] == 1);
                        REQUIRE(A[2] == 2);
                        REQUIRE(A[3] == 8);
                        REQUIRE(A[4] == 9);
                        
                        REQUIRE(B[0] == 5);
                        REQUIRE(B[1] == 6);
                        REQUIRE(B[2] == 7);
                        REQUIRE(B[3] == 3);
                        REQUIRE(B[4] == 4);
                    }
                }
                AND_WHEN("at point 4")
                {
                    gacpp::crossover::with_single_point(std::begin(A), std::end(A),
                                                        std::begin(B), std::end(B),
                                                        4);
                    
                    THEN("A should be: {0, 1, 2, 3, 9} and B should be: {5, 6, 7, 8, 4}")
                    {
                        REQUIRE(A[0] == 0);
                        REQUIRE(A[1] == 1);
                        REQUIRE(A[2] == 2);
                        REQUIRE(A[3] == 3);
                        REQUIRE(A[4] == 9);
                        
                        REQUIRE(B[0] == 5);
                        REQUIRE(B[1] == 6);
                        REQUIRE(B[2] == 7);
                        REQUIRE(B[3] == 8);
                        REQUIRE(B[4] == 4);
                    }
                }
            }
        }
    }
}
