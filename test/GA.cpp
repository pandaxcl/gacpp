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
            
            auto fitness = [](Member*member)->float
            {
                return member->fitness;
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

SCENARIO("mutate", "[GA]")
{
    GIVEN("A: {0, 1, 2, 3, 4, }")
    {
        int A[5] = {0, 1, 2, 3, 4, };
        REQUIRE(std::distance(std::begin(A), std::end(A)) == 5);
        GIVEN("mutate function [](int&m){ m=9; }")
        {
            auto mutate_function = [](int&m){ m=9; };
            WHEN("mutate at point 0")
            {
                gacpp::mutate::at_single_point(std::begin(A), std::end(A), 0, mutate_function);
                THEN("A should be {9, 1, 2, 3, 4}")
                {
                    REQUIRE(A[0] == 9);
                    REQUIRE(A[1] == 1);
                    REQUIRE(A[2] == 2);
                    REQUIRE(A[3] == 3);
                    REQUIRE(A[4] == 4);
                }
            }
            WHEN("mutate at point 1")
            {
                gacpp::mutate::at_single_point(std::begin(A), std::end(A), 1, mutate_function);
                THEN("A should be {0, 9, 2, 3, 4}")
                {
                    REQUIRE(A[0] == 0);
                    REQUIRE(A[1] == 9);
                    REQUIRE(A[2] == 2);
                    REQUIRE(A[3] == 3);
                    REQUIRE(A[4] == 4);
                }
            }
            WHEN("mutate at point 2")
            {
                gacpp::mutate::at_single_point(std::begin(A), std::end(A), 2, mutate_function);
                THEN("A should be {0, 1, 9, 3, 4}")
                {
                    REQUIRE(A[0] == 0);
                    REQUIRE(A[1] == 1);
                    REQUIRE(A[2] == 9);
                    REQUIRE(A[3] == 3);
                    REQUIRE(A[4] == 4);
                }
            }
            WHEN("mutate at point 3")
            {
                gacpp::mutate::at_single_point(std::begin(A), std::end(A), 3, mutate_function);
                THEN("A should be {0, 1, 2, 9, 4}")
                {
                    REQUIRE(A[0] == 0);
                    REQUIRE(A[1] == 1);
                    REQUIRE(A[2] == 2);
                    REQUIRE(A[3] == 9);
                    REQUIRE(A[4] == 4);
                }
            }
            WHEN("mutate at point 4")
            {
                gacpp::mutate::at_single_point(std::begin(A), std::end(A), 4, mutate_function);
                THEN("A should be {0, 1, 2, 3, 9}")
                {
                    REQUIRE(A[0] == 0);
                    REQUIRE(A[1] == 1);
                    REQUIRE(A[2] == 2);
                    REQUIRE(A[3] == 3);
                    REQUIRE(A[4] == 9);
                }
            }
        }
        
    }
}

SCENARIO("c++11", "[GA]")
{
    GIVEN("two objects A and B of class Class")
    {
        struct Local
        {
            int copy_constructed = 0;
            int assign_operator_called = 0;
        };
        auto local = std::make_shared<Local>();
        
        class Class
        {
            std::shared_ptr<Local>local;
        public:
            Class() = delete;
            explicit Class(std::shared_ptr<Local>x):local(x){}
            Class(const Class&o):local(o.local) { local->copy_constructed++; }
            Class&operator=(const Class&o){ local->assign_operator_called++; return *this; }
        };
        
        Class A(local);
        REQUIRE(local->copy_constructed == 0);
        REQUIRE(local->assign_operator_called == 0);
        Class B(local);
        REQUIRE(local->copy_constructed == 0);
        REQUIRE(local->assign_operator_called == 0);
        
        GIVEN("two references to A and B")
        {
            Class&ref1 = A;
            Class&ref2 = B;
            THEN("copy_constructed and assign_operator_called should be not changed")
            {
                REQUIRE(local->copy_constructed == 0);
                REQUIRE(local->assign_operator_called == 0);
            }
            WHEN("swap ref1 and ref2")
            {
                std::swap(ref1, ref2);
                REQUIRE(local->copy_constructed == 1);
                REQUIRE(local->assign_operator_called == 2);
            }
            WHEN("swap ref1 and ref2 with rvalue")
            {
                auto&&tmp = ref1;
                ref1 = std::forward<Class>(ref2);
                ref2 = std::forward<Class>(tmp);
                REQUIRE(local->copy_constructed == 0);
                REQUIRE(local->assign_operator_called == 2);
            }
        }
    }
}


SCENARIO("f(x) = x*sin(10*pi*x)+2.0", "[GA][minimum][maximum]")
{
    GIVEN("simple_gene")
    {
        typedef std::default_random_engine Random;
        typedef char   Unit;
        typedef double Real;
        
        struct Function
        {
            static Real rate_for_crossover_with_single_point()
            {
                return 0.4;
            }
            static Real rate_for_mutate()
            {
                return 0.044;
            }
            
            static Real mutate(int i, Random&random)
            {
                return 0;
            }
            typedef gacpp::model::simple_gene<Unit, Function, Real> gene_t;
            typedef gacpp::model::chromosome<gene_t> chromosome_t;
            typedef chromosome_t::gene_iterator ForwardIterator;
            static Real fitness(ForwardIterator begin, ForwardIterator end, Random&random)
            {
                return 0;
            }
        };
        typedef gacpp::model::simple_gene<Unit, Function, Real> gene_t;
        typedef gacpp::model::chromosome<gene_t> chromosome_t;
        gacpp::algorithm::team<chromosome_t> GA(100);
        GA.random_initialize();
        GA.epoch();
    }
}
