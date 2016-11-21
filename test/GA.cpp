#include <catch_with_main.hpp>
#include "gacpp/gacpp.hpp"
#include <random>
#include <iterator>
#include <cstdint>
#include <iostream>


SCENARIO("roulette_one", "[GA]")
{
    GIVEN("a random [0, 1] generator")
    {
        const size_t N = 6;
        const float V[N] = { 0.3, 0.9, 0.1, /* _ */ 0.7, 0.5, 0.87, };
        auto random_0_1=[&V]()->float{
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
        REQUIRE(std::distance(std::begin(A), std::begin(A)) == 0);
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

template<typename Solution, int _N_variables>
struct find_extremum
{
    typedef Solution                                            solution_type;
    enum { N_variables = _N_variables };
    typedef std::default_random_engine                          random_engine;
    typedef double                                              value_type;
    typedef double                                              real_type;
    typedef gacpp::model::simple_gene<solution_type>            gene_type;
    typedef gacpp::model::chromosome<gene_type, N_variables>    member_type;
    typedef typename member_type::iterator                      gene_iterator;
    typedef gacpp::algorithm::simple_team<solution_type>        team_t;
};

SCENARIO("simple_gene", "[GA][minimum][maximum]")
{
    GIVEN("f(x) = x*sin(10*pi*x)+2.0")
    {
        struct FindMaxValue: public find_extremum<FindMaxValue, 1>
        {
            //static real_type rate_for_crossover_with_single_point() { return 0.4; }
            static real_type rate_for_crossover_with_linear_interpolation() {
                return 0.4;
            }
            static real_type rate_for_mutate() {
                return 0.044;
            }
            
            static void random_initialize(gene_iterator it, random_engine&random)
            {
                auto t = gacpp::util::random_0_1<real_type>(random);
                it->_value = gacpp::util::value_in_range_with_ratio(-1.0, 2.0, t);
            }
            
            static void mutate(gene_iterator it, random_engine&random)
            {
                gacpp::mutate::for_real_value_clamped_in_range(it->_value, random, 0.1, -1.0, 2.0);
            }
            
            static real_type compute_fitness(gene_iterator begin, gene_iterator end, random_engine&random)
            {
                assert(std::distance(begin, end) == 1);
                auto&&x = static_cast<real_type&>(*begin);
				const real_type pi = 3.14159265358979323846;
                return x*std::sin(10*pi*x)+2.0;
            }
        };
        
        static_assert(gacpp::model::basic_gene_concept::crossover::two_ranges<FindMaxValue::gene_type>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::random_initialize::single<FindMaxValue>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::mutate::single<FindMaxValue>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::crossover_with_linear_interpolation<FindMaxValue>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::compute_fitness<FindMaxValue>::enabled, "");
        static_assert(!gacpp::model::simple_gene_concept::crossover_with_single_point<FindMaxValue>::enabled, "");
        
        REQUIRE(gacpp::model::basic_gene_concept::crossover::two_ranges<FindMaxValue::gene_type>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::random_initialize::single<FindMaxValue>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::mutate::single<FindMaxValue>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::crossover_with_linear_interpolation<FindMaxValue>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::compute_fitness<FindMaxValue>::enabled);
        REQUIRE_FALSE(gacpp::model::simple_gene_concept::crossover_with_single_point<FindMaxValue>::enabled);
        
        THEN("to find its extremum")
        {
            gacpp::algorithm::simple_report<FindMaxValue::real_type> report;
            FindMaxValue::team_t GA(100);
            GA.random_initialize();
            for (auto i=0; i<10000; i++)
            {
                GA.epoch();
                GA.sort_members_by_fitness_with_descending_order();

                if (report = GA.minmax_fitness_in_sorted_members_with_descending_order())
                {
                    const auto&mwf = GA.members_with_fitnesses().front();
                    auto x = mwf.member.at(0).value();
                    std::cout << std::setprecision(16) << std::fixed << std::showpos;
                    std::cout << report << "\tx = " << x <<std::endl;
                    GA.keep_best_for_ratio(0.05);
                }
                
                GA.swap_buffers();
            }
        }
    }
    GIVEN("f(x,y) = (4-2.1*x^2+x^4/3)*x^2+x*y+(-4+4*y^2)*y^2")
    {
        struct FindMaxValue2: public find_extremum<FindMaxValue2, 2>
        {
            static real_type rate_for_crossover_with_single_point() { return 0.4; }
            static real_type rate_for_crossover_with_linear_interpolation() { return 0.4; }
            static real_type rate_for_mutate() { return 0.044; }
            
            static void random_initialize(gene_iterator begin, gene_iterator end, random_engine&random)
            {
                assert(std::distance(begin, end) == 2);
                auto x_it = begin; std::advance(x_it, 0);
                auto y_it = begin; std::advance(y_it, 1);
                auto&&x = static_cast<real_type&>(*x_it);
                auto&&y = static_cast<real_type&>(*y_it);
                
                x = gacpp::util::value_in_range_with_ratio(-3.0, +3.0, gacpp::util::random_0_1<real_type>(random));
                y = gacpp::util::value_in_range_with_ratio(-2.0, +2.0, gacpp::util::random_0_1<real_type>(random));
            }

            static void mutate(gene_iterator begin, gene_iterator end, random_engine&random)
            {
                auto n = std::distance(begin, end);
                assert(std::distance(begin, end) == 2);
                auto x_it = begin; std::advance(x_it, 0);
                auto y_it = begin; std::advance(y_it, 1);
                auto&&x = static_cast<real_type&>(*x_it);
                auto&&y = static_cast<real_type&>(*y_it);
                
                gacpp::mutate::for_real_value_clamped_in_range(x, random, 6.0, -3.0, +3.0);
                gacpp::mutate::for_real_value_clamped_in_range(y, random, 4.0, -2.0, +2.0);
            }
            
            static real_type compute_fitness(gene_iterator begin, gene_iterator end, random_engine&random)
            {
                auto n = std::distance(begin, end);
                assert(std::distance(begin, end) == 2);
                auto x_it = begin; std::advance(x_it, 0);
                auto y_it = begin; std::advance(y_it, 1);
                auto&&x = static_cast<real_type&>(*x_it);
                auto&&y = static_cast<real_type&>(*y_it);
                auto f = (4-2.1*std::pow(x,2)+std::pow(x,4)/3)*std::pow(x,2)+x*y+(-4+4*std::pow(y,2))*std::pow(y,2);
                return std::exp(-0.005*f);
            }
        };
        
        static_assert(gacpp::model::basic_gene_concept::crossover::two_ranges<FindMaxValue2::gene_type>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::random_initialize::range<FindMaxValue2>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::mutate::range<FindMaxValue2>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::crossover_with_linear_interpolation<FindMaxValue2>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::compute_fitness<FindMaxValue2>::enabled, "");
        static_assert(gacpp::model::simple_gene_concept::crossover_with_single_point<FindMaxValue2>::enabled, "");
        
        REQUIRE(gacpp::model::basic_gene_concept::crossover::two_ranges<FindMaxValue2::gene_type>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::random_initialize::range<FindMaxValue2>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::mutate::range<FindMaxValue2>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::crossover_with_linear_interpolation<FindMaxValue2>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::compute_fitness<FindMaxValue2>::enabled);
        REQUIRE(gacpp::model::simple_gene_concept::crossover_with_single_point<FindMaxValue2>::enabled);
        
        THEN("to find its extremum")
        {
            FindMaxValue2::team_t GA(100);
            GA.random_initialize();
            
            gacpp::algorithm::simple_report<FindMaxValue2::real_type> report;
            for (auto i=0; i<10000; i++)
            {
                GA.epoch();
                GA.sort_members_by_fitness_with_descending_order();
                
                if (report = GA.minmax_fitness_in_sorted_members_with_descending_order())
                {
                    const auto&mwf = GA.members_with_fitnesses().front();
                    auto x = mwf.member.at(0).value();
                    auto y = mwf.member.at(1).value();
                    
                    std::cout << std::setprecision(16) << std::fixed << std::showpos;
                    std::cout << report << "\t(x, y) = (" << x << ", " << y <<")" << std::endl;
                    GA.keep_best_for_ratio(0.05);
                }
                
                GA.swap_buffers();
            }
        }
    }
}
