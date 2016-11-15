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
                                                               *static_cast<Random*>(nullptr)))>::value
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
                                                               *static_cast<Random*>(nullptr)))>::value
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
                typedef typename Solution::real_type     real_type;
                template<typename X> static typename std::enable_if<
                /**/std::is_floating_point<decltype(X::compute_fitness(std::declval<ForwardIterator>(),
                                                                       std::declval<ForwardIterator>(),
                                                                       *static_cast<Random*>(nullptr)))>::value
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
                                                       *static_cast<Random*>(nullptr)))>::value
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
                /**/std::is_void<decltype(X::mutate(std::declval<ForwardIterator>(),
                                                    *static_cast<Random*>(nullptr)))>::value
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
                                                    *static_cast<Random*>(nullptr)))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
        };
    };
    template<typename Solution>
    struct basic_gene
    {
        typedef typename Solution::value_type    value_type;
        typedef typename Solution::real_type     real_type;
        typedef typename Solution::gene_type     gene_type;
        typedef typename Solution::gene_iterator gene_iterator;
        typedef typename Solution::random_engine random_engine;
        
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
        void random_initialize(gene_iterator it, random_engine&random)
        {
            
        }
        
        //static void random_initialize(gene_iterator begin, gene_iterator end, random_engine&random)
        //{
        //
        //}

        static real_type compute_fitness(gene_iterator begin, gene_iterator end, random_engine&random)
        {
            return real_type(0);
        }
        
        static void crossover(gene_iterator begin1, gene_iterator end1,
                              gene_iterator begin2, gene_iterator end2, random_engine&random)
        {
            
        }

        void mutate(gene_iterator it, random_engine&random)
        {
            
        }

        //static void mutate(gene_iterator begin, gene_iterator end, random_engine&random)
        //{
        //
        //}
#endif
    };
    
    struct simple_gene_concept
    {
        template<typename Solution> using random_initialize = basic_gene_concept::random_initialize::single<Solution>;
//        template<typename Solution>
//        struct random_initialize
//        {
//            typedef typename F::random_engine Random;
//            template<typename X> static typename std::enable_if<
//            /**/!std::is_void<decltype(std::declval<X>().random_initialize(0,
//                                                                           *static_cast<Random*>(nullptr)))>::value
//            >::type check(int);
//            template<typename X> static std::false_type check(...);
//            enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
//
//            //template<typename R> static value_type&& random_initialize(int i, R&&random) { }
//        };
        
        template<typename Solution>
        struct crossover_with_single_point
        {
            typedef typename Solution::random_engine Random;
            template<typename X> static typename std::enable_if<
            /**/std::is_floating_point<decltype(std::declval<X>().rate_for_crossover_with_single_point())>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            
            //static real_type rate_for_crossover_with_single_point() { return 0.4; }
        };
        
        template<typename Solution>
        struct crossover_for_chromosome_with_only_one_gene
        {
            typedef typename Solution::random_engine Random;
            template<typename X> static typename std::enable_if<
            /**/std::is_floating_point<decltype(std::declval<X>().rate_for_crossover_chromosome_with_only_one_gene())>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
        };
        
        template<typename Solution>
        struct mutate
        {
            typedef typename Solution::random_engine Random;
            typedef typename Solution::gene_iterator ForwardIterator;
            template<typename X, typename Iterator> static typename std::enable_if<
            /**/basic_gene_concept::mutate::single<X>::enabled &&
            /**/std::is_floating_point<decltype(std::declval<X>().rate_for_mutate())>::value
            >::type check(int);
            template<typename X,typename Iterator> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<Solution,ForwardIterator>(0))>::value };
        };
        
        template<typename Solution> using compute_fitness = basic_gene_concept::compute_fitness::range<Solution>;
        //template<typename Solution>
        //struct fitness
        //{
        //    typedef typename Solution::random_engine Random;
        //    typedef typename Solution::gene_iterator ForwardIterator;
        //    template<typename X, typename Iterator> static typename std::enable_if<
        //    /**/std::is_floating_point<decltype(X::fitness(std::declval<Iterator>(),
        //                                                   std::declval<Iterator>(),
        //                                                   *static_cast<Random*>(nullptr)))>::value
        //    >::type check(int);
        //    template<typename X, typename Iterator> static std::false_type check(...);
        //    enum { enabled = std::is_void<decltype(check<Solution, ForwardIterator>(0))>::value };
        //    
        //    //template<typename ForwardIterator, typename R> static real_type fitness(ForwardIterator begin, ForwardIterator end, R&&random){}
        //};
        
    };
    
    template<typename Solution>
    struct simple_gene:public basic_gene<Solution>
    {
        typedef simple_gene                         this_type;
        typedef basic_gene<Solution>                super_type;
        typedef typename super_type::real_type      real_type;
        typedef typename super_type::random_engine  random_engine;
        
        static real_type rate(random_engine&random)
        {
            return static_cast<real_type>(random())/static_cast<real_type>(random.max());
        }
        //template<typename ForwardIterator> static void random_initialize(ForwardIterator it, random_engine&random) { }
        template<typename ForwardIterator>
        static void random_initialize(ForwardIterator begin, ForwardIterator end, random_engine&random)
        {
            detail::template random_initialize<Solution>(begin, end, random);
        }
        template<typename ForwardIterator>
        static real_type compute_fitness(ForwardIterator begin, ForwardIterator end, random_engine&random)
        {
            return Solution::compute_fitness(begin, end, random);
        }
        
        template<typename ForwardIterator>
        static void crossover(ForwardIterator begin1, ForwardIterator end1,
                              ForwardIterator begin2, ForwardIterator end2, random_engine&random)
        {
            detail::template crossover_for_chromosome_with_only_one_gene<Solution>(begin1, end1, begin2, end2, random);
            detail::template crossover_with_single_point                <Solution>(begin1, end1, begin2, end2, random);
        }
        //template<typename ForwardIterator> static void mutate(ForwardIterator it, random_engine&random) { }
        template<typename ForwardIterator>
        static void mutate(ForwardIterator begin, ForwardIterator end, random_engine&random)
        {
            detail::template mutate<Solution>(begin, end, random);
        }
        
        
        struct detail {
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator>
            static typename std::enable_if<simple_gene_concept::template random_initialize<F>::enabled>::type
            random_initialize(ForwardIterator begin, ForwardIterator end, typename F::random_engine&random)
            {
                for (auto it=begin; it!=end; it++)
                {
                    F::random_initialize(it, random);
                }
            }
            template<typename F>
            static void random_initialize(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator>
            static typename std::enable_if<simple_gene_concept::template crossover_with_single_point<F>::enabled>::type
            crossover_with_single_point(ForwardIterator begin1, ForwardIterator end1,
                                        ForwardIterator begin2, ForwardIterator end2, typename F::random_engine&random)
            {
                auto n = std::distance(begin1, end1);
                if (n > 1)
                {
                    if (rate(random) < F::rate_for_crossover_with_single_point())
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
            template<typename F, typename ForwardIterator>
            static typename std::enable_if<simple_gene_concept::template crossover_for_chromosome_with_only_one_gene<F>::enabled>::type
            crossover_for_chromosome_with_only_one_gene(ForwardIterator begin1, ForwardIterator end1,
                                                        ForwardIterator begin2, ForwardIterator end2, typename F::random_engine&random)
            {
                auto n = std::distance(begin1, end1);
                
                if (1 == n)
                {
                    if (rate(random) < F::rate_for_crossover_chromosome_with_only_one_gene())
                    {
                        auto&&A = begin1->value();
                        auto&&B = begin2->value();
                        crossover::for_chromosome_with_only_one_gene<real_type>(A, B, random);
                    }
                }
            }
            template<typename F>
            static void crossover_for_chromosome_with_only_one_gene(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            
            template<typename F, typename ForwardIterator>
            static typename std::enable_if<simple_gene_concept::template mutate<F>::enabled>::type
            mutate(ForwardIterator begin, ForwardIterator end, typename F::random_engine&random)
            {
                for (auto it=begin; it!=end; it++)
                {
                    if (rate(random) < F::rate_for_mutate())
                        F::mutate(it, random);
                }
            }
            template<typename F>
            static void mutate(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
        };
    };
    
    template <typename Gene, int _N_gene_count>
    struct chromosome:public std::vector<Gene>
    {
        typedef chromosome              this_type;
        typedef Gene                    gene_type;
        typedef std::vector<gene_type>  super_type;
        
        enum { N_gene_count = _N_gene_count };
        
        chromosome(std::size_t n):super_type(n)
        {
        }
        chromosome():chromosome(N_gene_count)
        {
        }
        
        template<typename Solution>
        struct gene_of
        {
            struct has_mutate
            {
                enum { value = basic_gene_concept::mutate::range<typename Solution::gene_type>::enabled
                    || basic_gene_concept::mutate::single<typename Solution::gene_type>::enabled };
                typedef typename std::conditional<value, typename Solution::gene_type, Solution>::type type;
            };
            
            struct has_random_initialize
            {
                enum { value = basic_gene_concept::random_initialize::range<typename Solution::gene_type>::enabled
                    || basic_gene_concept::random_initialize::single<typename Solution::gene_type>::enabled };
                typedef typename std::conditional<value, typename Solution::gene_type, Solution>::type type;
            };
            
            struct has_compute_fitness
            {
                enum { value = basic_gene_concept::compute_fitness::range<typename Solution::gene_type>::enabled };
                typedef typename std::conditional<value, typename Solution::gene_type, Solution>::type type;
            };
            
            struct has_crossover
            {
                enum { value = basic_gene_concept::crossover::two_ranges<typename Solution::gene_type>::enabled };
                typedef typename std::conditional<value, typename Solution::gene_type, Solution>::type type;
            };
        };
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::random_initialize::range<typename gene_of<Solution>::has_random_initialize::type>::enabled>::type
        random_initialize(typename Solution::random_engine&random)
        {
            this->resize(N_gene_count);
            gene_of<Solution>::has_random_initialize::type::random_initialize(std::begin(*this), std::end(*this), random);
        }
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::random_initialize::single<typename gene_of<Solution>::has_random_initialize::type>::enabled>::type
        random_initialize(typename Solution::random_engine&random)
        {
            this->resize(N_gene_count);
            for (auto it=std::begin(*this); it!=std::end(*this); it++)
            {
                gene_of<Solution>::has_random_initialize::type::random_initialize(it, random);
            }
        }
        
        template<typename Solution>
        typename std::enable_if<
        /**/!basic_gene_concept::random_initialize::range<typename gene_of<Solution>::has_random_initialize::type>::enabled &&
        /**/!basic_gene_concept::random_initialize::single<typename gene_of<Solution>::has_random_initialize::type>::enabled
        >::type
        random_initialize(...){}
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename Solution>
        typename std::enable_if<
        /**/basic_gene_concept::compute_fitness::range<typename gene_of<Solution>::has_compute_fitness::type>::enabled,
        typename Solution::real_type>::type
        compute_fitness(typename Solution::random_engine&random)
        {
            return gene_of<Solution>::has_compute_fitness::type::compute_fitness(std::begin(*this), std::end(*this), random);
        }
        
        template<typename Solution>
        typename std::enable_if<
        /**/!basic_gene_concept::compute_fitness::range<typename gene_of<Solution>::has_compute_fitness::type>::enabled,
        typename Solution::real_type>::type
        compute_fitness(...);
        
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::crossover::two_ranges<typename gene_of<Solution>::has_crossover::type>::enabled>::type
        crossover(this_type&o, typename Solution::random_engine&random)
        {
            gene_of<Solution>::has_crossover::type::crossover(std::begin(*this), std::end(*this), std::begin(o), std::end(o), random);
        }
        
        template<typename Solution>
        typename std::enable_if<!basic_gene_concept::crossover::two_ranges<typename gene_of<Solution>::has_crossover::type>::enabled>::type
        crossover(this_type&o, ...){}
        
        ////////////////////////////////////////////////////////////////////////////////
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::mutate::range<typename gene_of<Solution>::has_mutate::type>::enabled>::type
        mutate(typename Solution::random_engine&random)
        {
            gene_of<Solution>::has_mutate::type::mutate(std::begin(*this), std::end(*this), random);
        }
        
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::mutate::single<typename gene_of<Solution>::has_mutate::type>::enabled>::type
        mutate(typename Solution::random_engine&random)
        {
            for (auto it=std::begin(*this); it!=std::end(*this); it++)
            {
                gene_of<Solution>::has_mutate::type::mutate(it, random);
            }
        }
        
        template<typename Solution>
        typename std::enable_if<
        /**/!basic_gene_concept::mutate::range<typename gene_of<Solution>::has_mutate::type>::enabled &&
        /**/!basic_gene_concept::mutate::single<typename gene_of<Solution>::has_mutate::type>::enabled
        >::type
        mutate(...){}
    };
    
} //namespace model {
