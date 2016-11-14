namespace model {
    
    struct basic_gene_concept
    {
        struct random_initialize
        {
            template<typename Solution>
            struct single
            {
                typedef typename Solution::random_engine Random;
                typedef typename Solution::gene_iterator ForwardIterator;
                typedef typename Solution::gene_type     Gene;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::random_initialize(std::declval<ForwardIterator>(),
                                                               std::declval<Random>()))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
            template<typename Solution>
            struct range
            {
                typedef typename Solution::random_engine Random;
                typedef typename Solution::gene_iterator ForwardIterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::random_initialize(std::declval<ForwardIterator>(),
                                                               std::declval<ForwardIterator>(),
                                                               std::declval<Random>()))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
        };
        
        struct compute_fitness
        {
            template<typename Solution>
            struct range
            {
                typedef typename Solution::random_engine Random;
                typedef typename Solution::gene_iterator ForwardIterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_floating_point<decltype(X::compute_fitness(std::declval<ForwardIterator>(),
                                                                       std::declval<ForwardIterator>(),
                                                                       std::declval<Random>()))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
        };
        
        struct crossover
        {
            template<typename Solution>
            struct two_ranges
            {
                typedef typename Solution::random_engine Random;
                typedef typename Solution::gene_iterator ForwardIterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::crossover(std::declval<ForwardIterator>(),
                                                       std::declval<ForwardIterator>(),
                                                       std::declval<ForwardIterator>(),
                                                       std::declval<ForwardIterator>(),
                                                       std::declval<Random>()))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
        };
        
        struct mutate
        {
            template<typename Solution>
            struct single
            {
                typedef typename Solution::random_engine Random;
                typedef typename Solution::gene_iterator ForwardIterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::mutate(std::declval<ForwardIterator>(), std::declval<Random>()))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
            template<typename Solution>
            struct range
            {
                typedef typename Solution::random_engine Random;
                typedef typename Solution::gene_iterator ForwardIterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::mutate(std::declval<ForwardIterator>(),
                                                    std::declval<ForwardIterator>(),
                                                    std::declval<Random>()))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
        };
    };
    template<typename Solution>
    struct basic_gene
    {
        typedef typename Solution::value_type value_type;
        typedef typename Solution::real_type real_type;
        typedef typename Solution::gene_type gene_type;
        
        value_type _value;
        value_type&value() { return _value; }
        void set_value(value_type&&x) { _value = x; }
        void set_value(const value_type&x) { _value = x; }
        
        operator value_type() { return _value; }
        operator value_type&() { return _value; }
        operator const value_type() { return _value; }
        operator const value_type&() { return _value; }
        
        gene_type&operator = (value_type&&x) { this->_value = x; return *this; }
        gene_type&operator = (value_type&x) { this->_value = x; return *this; }
        gene_type&operator = (const value_type&x) { this->_value = x; return *this; }
#if 0
        template<typename ForwardIterator, typename Random>
        void random_initialize(ForwardIterator it, Random&&random)
        {
            
        }
        //template<typename ForwardIterator, typename Random>
        //static void random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
        //{
        //
        //}
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
        void mutate(ForwardIterator it, Random&&random)
        {
            
        }
        //template<typename ForwardIterator, typename Random>
        //static void mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
        //{
        //
        //}
#endif
    };
    
    struct simple_gene_concept
    {
        template<typename F> using random_initialize = basic_gene_concept::random_initialize::single<F>;
        //        template<typename F>
        //        struct random_initialize
        //        {
        //            typedef typename F::random_engine Random;
        //            template<typename X> static typename std::enable_if<
        //            /**/!std::is_void<decltype(std::declval<X>().random_initialize(0,
        //                                                                           std::declval<Random>()))>::value
        //            >::type check(int);
        //            template<typename X> static std::false_type check(...);
        //            enum { enabled = std::is_void<decltype(check<F>(0))>::value };
        //
        //            //template<typename R> static value_type&& random_initialize(int i, R&&random) { }
        //        };
        
        template<typename F>
        struct crossover_with_single_point
        {
            typedef typename F::random_engine Random;
            template<typename X> static typename std::enable_if<
            /**/std::is_floating_point<decltype(std::declval<X>().rate_for_crossover_with_single_point())>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<F>(0))>::value };
            
            //static real_type rate_for_crossover_with_single_point() { return 0.4; }
        };
        
        template<typename F>
        struct crossover_for_chromosome_with_only_one_gene
        {
            typedef typename F::random_engine Random;
            template<typename X> static typename std::enable_if<
            /**/std::is_floating_point<decltype(std::declval<F>().rate_for_crossover_chromosome_with_only_one_gene())>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<F>(0))>::value };
        };
        
        template<typename F>
        struct mutate
        {
            typedef typename F::random_engine Random;
            typedef typename F::gene_iterator ForwardIterator;
            template<typename X, typename Iterator> static typename std::enable_if<
            /**/std::is_void<decltype(X::mutate(std::declval<Iterator>(),
                                                std::declval<Random>()))>::value
            /**/&&
            /**/std::is_floating_point<decltype(std::declval<X>().rate_for_mutate())>::value
            >::type check(int);
            template<typename X,typename Iterator> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<F,ForwardIterator>(0))>::value };
        };
        
        template<typename F> using compute_fitness = basic_gene_concept::compute_fitness::range<F>;
//        template<typename F>
//        struct fitness
//        {
//            typedef typename F::random_engine Random;
//            typedef typename F::gene_iterator ForwardIterator;
//            template<typename X, typename Iterator> static typename std::enable_if<
//            /**/std::is_floating_point<decltype(X::fitness(std::declval<Iterator>(),
//                                                           std::declval<Iterator>(),
//                                                           std::declval<Random>()))>::value
//            >::type check(int);
//            template<typename X, typename Iterator> static std::false_type check(...);
//            enum { enabled = std::is_void<decltype(check<F, ForwardIterator>(0))>::value };
//            
//            //template<typename ForwardIterator, typename R> static real_type fitness(ForwardIterator begin, ForwardIterator end, R&&random){}
//        };
        
    };
    
    template<typename Solution>
    struct simple_gene:public basic_gene<Solution>
    {
        typedef simple_gene this_type;
        typedef typename basic_gene<Solution>::real_type real_type;
        
        template<typename Random>
        static real_type rate(Random&&random)
        {
            return static_cast<real_type>(random())/static_cast<real_type>(random.max());
        }
        //template<typename ForwardIterator, typename Random> void random_initialize(ForwardIterator it, Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            detail::template random_initialize<Solution>(begin, end, random);
        }
        template<typename ForwardIterator, typename Random>
        static real_type compute_fitness(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            static_assert(std::is_floating_point<decltype(std::declval<Solution>().fitness(begin,end,random))>::value,
                          "Function must have fitness(begin,end,random)->Real function");
            return Solution::fitness(begin, end, random);
        }
        
        template<typename ForwardIterator, typename Random>
        static void crossover(ForwardIterator begin1, ForwardIterator end1,
                              ForwardIterator begin2, ForwardIterator end2, Random&&random)
        {
            detail::template crossover_for_chromosome_with_only_one_gene<Solution>(begin1, end1, begin2, end2, random);
            detail::template crossover_with_single_point                <Solution>(begin1, end1, begin2, end2, random);
        }
        //template<typename ForwardIterator, typename Random> void mutate(ForwardIterator it, Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            detail::template mutate<Solution>(begin, end, random);
        }
        
        
        struct detail {
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator, typename Random>
            static typename std::enable_if<simple_gene_concept::template random_initialize<F>::enabled>::type
            random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
            {
                static_assert(!std::is_void<decltype(std::declval<Solution>().random_initialize(0,std::declval<Random>()))>::value,
                              "Function must have random_initialize(int,random)->Unit function");
                for (auto it=begin; it!=end; it++)
                {
                    auto i = std::distance(begin, it);
                    it->_value = Solution::random_initialize(i, random);
                }
            }
            template<typename F>
            static void random_initialize(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator, typename Random>
            static typename std::enable_if<simple_gene_concept::template crossover_with_single_point<F>::enabled>::type
            crossover_with_single_point(ForwardIterator begin1, ForwardIterator end1,
                                        ForwardIterator begin2, ForwardIterator end2, Random&&random)
            {
                static_assert(std::is_floating_point<decltype(std::declval<Solution>().rate_for_crossover_with_single_point())>::value,
                              "Function must have mutate_rate()->Real function");
                auto n = std::distance(begin1, end1);
                if (n > 1)
                {
                    if (rate(random) < Solution::rate_for_crossover_with_single_point())
                    {
                        auto i = 1;
                        if (n > 2)
                        {
                            i = random()%(std::distance(begin1, end1)-1)+1;
                        }
                        crossover::with_single_point(begin1, end1, begin2, end2, i);
                    }
                }
            }
            template<typename F>
            static void crossover_with_single_point(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator, typename Random>
            static typename std::enable_if<simple_gene_concept::template crossover_for_chromosome_with_only_one_gene<F>::enabled>::type
            crossover_for_chromosome_with_only_one_gene(ForwardIterator begin1, ForwardIterator end1,
                                                        ForwardIterator begin2, ForwardIterator end2, Random&&random)
            {
                static_assert(std::is_floating_point<decltype(std::declval<Solution>().rate_for_crossover_chromosome_with_only_one_gene())>::value,
                              "Function must have rate_for_crossover_chromosome_with_only_one_gene()->Real function");
                auto n = std::distance(begin1, end1);
                if (1 == n)
                {
                    if (rate(random) < Solution::rate_for_crossover_chromosome_with_only_one_gene())
                    {
                        auto&&ratio_0_1 = static_cast<real_type>(random())/random.max();
                        auto&&A = begin1->value();
                        auto&&B = begin2->value();
                        crossover::for_chromosome_with_only_one_gene(A, B, ratio_0_1);
                    }
                }
            }
            template<typename F>
            static void crossover_for_chromosome_with_only_one_gene(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            
            template<typename F, typename ForwardIterator, typename Random>
            static typename std::enable_if<simple_gene_concept::template mutate<F>::enabled>::type
            mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
            {
                static_assert(!std::is_void<decltype(std::declval<Solution>().mutate(std::declval<ForwardIterator>(),std::declval<Random>()))>::value,
                              "Function must have mutate(int,random)->Unit function");
                static_assert(std::is_floating_point<decltype(std::declval<Solution>().rate_for_mutate())>::value,
                              "Function must have mutate_rate()->Real function");
                
                for (auto it=begin; it!=end; it++)
                {
                    if (rate(random) < Solution::rate_for_mutate())
                        it->_value = Solution::mutate(it, random);
                }
            }
            template<typename F>
            static void mutate(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
        };
    };
    
    template <typename Gene, int _N_gene_count>
    struct chromosome
    {
        typedef chromosome this_type;
        typedef Gene       gene_type;
        enum { N_gene_count = _N_gene_count };
        
        std::vector<Gene> genes;
        typedef decltype(std::begin(genes)) gene_iterator;
        
        chromosome(std::size_t n):genes(n)
        {
        }
        chromosome():chromosome(N_gene_count)
        {
        }
        
        std::size_t size()
        {
            return this->genes.size();
        }
        std::size_t size() const
        {
            return this->genes.size();
        }
        void resize(std::size_t n)
        {
            this->genes.resize(n);
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename Solution, typename Random>
        typename std::enable_if<basic_gene_concept::random_initialize::range<Solution>::enabled>::type
        random_initialize(Random&&random)
        {
            this->resize(N_gene_count);
            Solution::random_initialize(std::begin(genes), std::end(genes), random);
        }
        template<typename Solution, typename Random>
        typename std::enable_if<basic_gene_concept::random_initialize::single<Solution>::enabled>::type
        random_initialize(Random&&random)
        {
            for (auto it=std::begin(genes); it!=std::end(genes); it++)
                Solution::random_initialize(it, std::forward<Random>(random));
        }
        
        template<typename G>
        void random_initialize(...){}
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename Solution, typename Real>
        typename std::enable_if<basic_gene_concept::compute_fitness::range<Solution>::enabled, Real>::type
        compute_fitness(typename Solution::random_engine&random)
        {
            return Solution::compute_fitness(std::begin(genes), std::end(genes), random);
        }
        
        template<typename Solution, typename Real>
        typename std::enable_if<!basic_gene_concept::compute_fitness::range<Solution>::enabled, Real>::type
        compute_fitness(...);
        
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::crossover::two_ranges<Solution>::enabled>::type
        crossover(this_type&o, typename Solution::random_engine&random)
        {
            Solution::crossover(std::begin(genes), std::end(genes),
                                std::begin(o.genes), std::end(o.genes),
                                random);
        }
        
        template<typename Solution>
        typename std::enable_if<!basic_gene_concept::crossover::two_ranges<Solution>::enabled>::type
        crossover(this_type&o, ...){}
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename Solution, typename Random>
        typename std::enable_if<basic_gene_concept::mutate::range<Solution>::enabled>::type
        mutate(Random&&random)
        {
            Solution::mutate(std::begin(genes), std::end(genes), random);
        }
        
        template<typename Solution, typename Random>
        typename std::enable_if<basic_gene_concept::mutate::single<Solution>::enabled>::type
        mutate(Random&&random)
        {
            for (auto it=std::begin(genes); it!=std::end(genes); it++)
                Solution::mutate(it, std::forward<Random>(random));
        }
        
        template<typename Solution>
        void mutate(...){}
    };
    
} //namespace model {
