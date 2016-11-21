namespace model {
    
    struct basic_gene_concept
    {
        struct random_initialize
        {
            template<typename Solution>
            struct single
            {
                typedef typename Solution::random_engine_type   Random;
                typedef typename Solution::gene_iterator        Iterator;
                typedef typename Solution::gene_type            Gene;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::random_initialize(std::declval<Iterator>(),
                                                               *static_cast<Random*>(nullptr)))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
            template<typename Solution>
            struct range
            {
                typedef typename Solution::random_engine_type   Random;
                typedef typename Solution::gene_iterator        Iterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::random_initialize(std::declval<Iterator>(),
                                                               std::declval<Iterator>(),
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
                typedef typename Solution::random_engine_type   Random;
                typedef typename Solution::gene_iterator        Iterator;
                typedef typename Solution::real_type            real_type;
                template<typename X> static typename std::enable_if<
                /**/std::is_floating_point<decltype(X::compute_fitness(std::declval<Iterator>(),
                                                                       std::declval<Iterator>(),
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
                typedef typename Solution::random_engine_type   Random;
                typedef typename Solution::gene_iterator        Iterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::crossover(std::declval<Iterator>(),
                                                       std::declval<Iterator>(),
                                                       std::declval<Iterator>(),
                                                       std::declval<Iterator>(),
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
                typedef typename Solution::random_engine_type   Random;
                typedef typename Solution::gene_iterator        Iterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::mutate(std::declval<Iterator>(),
                                                    *static_cast<Random*>(nullptr)))>::value
                >::type check(int);
                template<typename X> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            };
            template<typename Solution>
            struct range
            {
                typedef typename Solution::random_engine_type   Random;
                typedef typename Solution::gene_iterator        Iterator;
                template<typename X> static typename std::enable_if<
                /**/std::is_void<decltype(X::mutate(std::declval<Iterator>(),
                                                    std::declval<Iterator>(),
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
        typedef typename Solution::value_type           value_type;
        typedef typename Solution::real_type            real_type;
        typedef typename Solution::gene_type            gene_type;
        typedef typename Solution::gene_iterator        gene_iterator;
        typedef typename Solution::random_engine_type   random_engine_type;
        
        value_type _value;
        value_type&value() { return _value; }
        const value_type&value() const { return _value; }
        void set_value(value_type&&x) { _value = x; }
        void set_value(const value_type&x) { _value = x; }
        
        operator value_type() { return _value; }
        operator value_type&() { return _value; }
        operator const value_type&() { return _value; }
        
        gene_type&operator = (value_type&&x) { this->_value = x; return *this; }
        gene_type&operator = (value_type&x) { this->_value = x; return *this; }
        gene_type&operator = (const value_type&x) { this->_value = x; return *this; }
#if 0
        void random_initialize(gene_iterator it, random_engine_type&random)
        {
            
        }
        
        //static void random_initialize(gene_iterator begin, gene_iterator end, random_engine_type&random)
        //{
        //
        //}
        
        static real_type compute_fitness(gene_iterator begin, gene_iterator end, random_engine_type&random)
        {
            return real_type(0);
        }
        
        static void crossover(gene_iterator begin1, gene_iterator end1,
                              gene_iterator begin2, gene_iterator end2, random_engine_type&random)
        {
            
        }
        
        void mutate(gene_iterator it, random_engine_type&random)
        {
            
        }
        
        //static void mutate(gene_iterator begin, gene_iterator end, random_engine_type&random)
        //{
        //
        //}
#endif
    };
    
    struct simple_gene_concept
    {
        //template<typename Solution> using random_initialize = basic_gene_concept::random_initialize::single<Solution>;
        using random_initialize = basic_gene_concept::random_initialize;
//        template<typename Solution>
//        struct random_initialize
//        {
//            typedef typename F::random_engine_type Random;
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
            typedef typename Solution::random_engine_type Random;
            template<typename X> static typename std::enable_if<
            /**/std::is_floating_point<decltype(X::rate_for_crossover_with_single_point())>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
            
            //static real_type rate_for_crossover_with_single_point() { return 0.4; }
        };
        
        template<typename Solution>
        struct crossover_with_linear_interpolation
        {
            typedef typename Solution::random_engine_type Random;
            template<typename X> static typename std::enable_if<
            /**/std::is_floating_point<decltype(X::rate_for_crossover_with_linear_interpolation())>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<Solution>(0))>::value };
        };
        
        struct mutate
        {
            template<typename Solution>
            struct single
            {
                typedef typename Solution::random_engine_type Random;
                typedef typename Solution::gene_iterator Iterator;
                template<typename X, typename Iterator> static typename std::enable_if<
                /**/basic_gene_concept::mutate::single<X>::enabled&&
                /**/std::is_floating_point<decltype(X::rate_for_mutate())>::value
                >::type check(int);
                template<typename X,typename Iterator> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution,Iterator>(0))>::value };
            };
            template<typename Solution>
            struct range
            {
                typedef typename Solution::random_engine_type Random;
                typedef typename Solution::gene_iterator Iterator;
                template<typename X, typename Iterator> static typename std::enable_if<
                /**/basic_gene_concept::mutate::range<X>::enabled&&
                /**/std::is_floating_point<decltype(X::rate_for_mutate())>::value
                >::type check(int);
                template<typename X,typename Iterator> static std::false_type check(...);
                enum { enabled = std::is_void<decltype(check<Solution,Iterator>(0))>::value };
            };
        };
        
        template<typename Solution> using compute_fitness = basic_gene_concept::compute_fitness::range<Solution>;
        //template<typename Solution>
        //struct fitness
        //{
        //    typedef typename Solution::random_engine_type Random;
        //    typedef typename Solution::gene_iterator Iterator;
        //    template<typename X, typename Iterator> static typename std::enable_if<
        //    /**/std::is_floating_point<decltype(X::fitness(std::declval<Iterator>(),
        //                                                   std::declval<Iterator>(),
        //                                                   *static_cast<Random*>(nullptr)))>::value
        //    >::type check(int);
        //    template<typename X, typename Iterator> static std::false_type check(...);
        //    enum { enabled = std::is_void<decltype(check<Solution, Iterator>(0))>::value };
        //
        //    //template<typename Iterator, typename R> static real_type fitness(Iterator begin, Iterator end, R&&random){}
        //};
        
    };
    
    template<typename Solution>
    struct simple_gene:public basic_gene<Solution>
    {
        typedef simple_gene                             this_type;
        typedef basic_gene<Solution>                    super_type;
        typedef typename super_type::real_type          real_type;
        typedef typename super_type::random_engine_type random_engine_type;
        
        static real_type rate(random_engine_type&random)
        {
            return static_cast<real_type>(random())/static_cast<real_type>(random.max());
        }
        //template<typename Iterator> static void random_initialize(Iterator it, random_engine_type&random) { }
        template<typename Iterator>
        static void random_initialize(Iterator begin, Iterator end, random_engine_type&random)
        {
            detail::template random_initialize<Solution>(begin, end, random);
        }
        template<typename Iterator>
        static real_type compute_fitness(Iterator begin, Iterator end, random_engine_type&random)
        {
            return Solution::compute_fitness(begin, end, random);
        }
        
        template<typename Iterator>
        static void crossover(Iterator begin1, Iterator end1,
                              Iterator begin2, Iterator end2, random_engine_type&random)
        {
            detail::template crossover_with_linear_interpolation<Solution>(begin1, end1, begin2, end2, random);
            detail::template crossover_with_single_point        <Solution>(begin1, end1, begin2, end2, random);
        }
        //template<typename Iterator> static void mutate(Iterator it, random_engine_type&random) { }
        template<typename Iterator>
        static void mutate(Iterator begin, Iterator end, random_engine_type&random)
        {
            detail::template mutate<Solution>(begin, end, random);
        }
        
        
        struct detail {
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename Iterator>
            static typename std::enable_if<simple_gene_concept::random_initialize::template range<F>::enabled>::type
            random_initialize(Iterator begin, Iterator end, typename F::random_engine_type&random)
            {
                F::random_initialize(begin, end, random);
            }
            template<typename F, typename Iterator>
            static typename std::enable_if<simple_gene_concept::random_initialize::template single<F>::enabled>::type
            random_initialize(Iterator begin, Iterator end, typename F::random_engine_type&random)
            {
                for (auto it=begin; it!=end; it++)
                {
                    F::random_initialize(it, random);
                }
            }
            template<typename F>
            static void random_initialize(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename Iterator>
            static typename std::enable_if<simple_gene_concept::template crossover_with_single_point<F>::enabled>::type
            crossover_with_single_point(Iterator begin1, Iterator end1,
                                        Iterator begin2, Iterator end2, typename F::random_engine_type&random)
            {
                assert(std::distance(begin1, end1) > 1);
                if (rate(random) < F::rate_for_crossover_with_single_point())
                {
                    auto i = 1;
                    auto n = std::distance(begin1, end1);
                    if (n > 2)
                    {
                        i = random()%(n-1)+1;
                    }
                    crossover::with_single_point(begin1, end1, begin2, end2, i);
                }
            }
            template<typename F>
            static void crossover_with_single_point(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename Iterator>
            static typename std::enable_if<simple_gene_concept::template crossover_with_linear_interpolation<F>::enabled>::type
            crossover_with_linear_interpolation(Iterator begin1, Iterator end1,
                                                Iterator begin2, Iterator end2, typename F::random_engine_type&random)
            {
                for (auto it=begin1; it!=end1; it++)
                {
                    if (rate(random) < F::rate_for_crossover_with_linear_interpolation())
                    {
                        auto&&A = begin1->value();
                        auto&&B = begin2->value();
                        crossover::with_linear_interpolation<real_type>(A, B, random);
                    }
                }
            }
            template<typename F>
            static void crossover_with_linear_interpolation(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            
            template<typename F, typename Iterator>
            static typename std::enable_if<simple_gene_concept::mutate::template single<F>::enabled>::type
            mutate(Iterator begin, Iterator end, typename F::random_engine_type&random)
            {
                for (auto it=begin; it!=end; it++)
                {
                    if (rate(random) < F::rate_for_mutate())
                        F::mutate(it, random);
                }
            }
            template<typename F, typename Iterator>
            static typename std::enable_if<simple_gene_concept::mutate::template range<F>::enabled>::type
            mutate(Iterator begin, Iterator end, typename F::random_engine_type&random)
            {
                F::mutate(begin, end, random);
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
        random_initialize(typename Solution::random_engine_type&random)
        {
            gene_of<Solution>::has_random_initialize::type::random_initialize(std::begin(*this), std::end(*this), random);
        }
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::random_initialize::single<typename gene_of<Solution>::has_random_initialize::type>::enabled>::type
        random_initialize(typename Solution::random_engine_type&random)
        {
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
        compute_fitness(typename Solution::random_engine_type&random)
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
        crossover(this_type&o, typename Solution::random_engine_type&random)
        {
            gene_of<Solution>::has_crossover::type::crossover(std::begin(*this), std::end(*this), std::begin(o), std::end(o), random);
        }
        
        template<typename Solution>
        typename std::enable_if<!basic_gene_concept::crossover::two_ranges<typename gene_of<Solution>::has_crossover::type>::enabled>::type
        crossover(this_type&o, ...){}
        
        ////////////////////////////////////////////////////////////////////////////////
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::mutate::range<typename gene_of<Solution>::has_mutate::type>::enabled>::type
        mutate(typename Solution::random_engine_type&random)
        {
            gene_of<Solution>::has_mutate::type::mutate(std::begin(*this), std::end(*this), random);
        }
        
        template<typename Solution>
        typename std::enable_if<basic_gene_concept::mutate::single<typename gene_of<Solution>::has_mutate::type>::enabled>::type
        mutate(typename Solution::random_engine_type&random)
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
