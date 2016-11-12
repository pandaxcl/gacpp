namespace model {

    struct basic_gene
    {
        template<typename Random>
        void random_initialize(Random&&random)
        {
            
        }
//        template<typename ForwardIterator, typename Random>
//        static void random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
//        {
//    
//        }
        template<typename ForwardIterator, typename Random>
        static double compute_fitness(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            return double(0);
        }
        
        template<typename ForwardIterator, typename Random>
        static void crossover(ForwardIterator begin1, ForwardIterator end1,
                              ForwardIterator begin2, ForwardIterator end2, Random&&random)
        {
            
        }
        template<typename Random>
        void mutate(Random&&random)
        {
            
        }
//        template<typename ForwardIterator, typename Random>
//        static void mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
//        {
//            
//        }
    };
    
    struct simple_gene_concept
    {
        template<typename F>
        struct random_initialize
        {
            typedef typename F::random_engine Random;
            template<typename X> static typename std::enable_if<
            /**/!std::is_void<decltype(std::declval<X>().random_initialize(0,std::declval<Random>()))>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<F>(0))>::value };
            
            //template<typename R> static value_type&& random_initialize(int i, R&&random) { }
        };
        
        template<typename F>
        struct crossover_with_single_point
        {
            typedef typename F::random_engine Random;
            template<typename X> static typename std::enable_if<
            /**/std::is_floating_point<decltype(std::declval<X>().rate_for_crossover_with_single_point())>::value
            >::type check(int);
            template<typename X> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<F>(0))>::value };
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
            template<typename X,typename Iterator> static typename std::enable_if<
            /**/!std::is_void<decltype(std::declval<X>().mutate(std::declval<Iterator>(),std::declval<Random>()))>::value
            /**/&&
            /**/std::is_floating_point<decltype(std::declval<X>().rate_for_mutate())>::value
            >::type check(int);
            template<typename X,typename Iterator> static std::false_type check(...);
            enum { enabled = std::is_void<decltype(check<F,ForwardIterator>(0))>::value };
        };
    };
    
    template<typename Sample>
    struct simple_gene
    {
        typedef typename Sample::value_type value_type;
        typedef typename Sample::real_type real_type;
        value_type _value;
        value_type&value() { return _value; }
        operator value_type() { return _value; }
        operator value_type&() { return _value; }
        operator const value_type() { return _value; }
        operator const value_type&() { return _value; }
        
        template<typename Random>
        static real_type rate(Random&&random)
        {
            return static_cast<real_type>(random())/static_cast<real_type>(random.max());
        }
        //template<typename Random> void random_initialize(Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            detail::template random_initialize<Sample>(begin, end, random);
        }
        template<typename ForwardIterator, typename Random>
        static real_type compute_fitness(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            static_assert(std::is_floating_point<decltype(std::declval<Sample>().fitness(begin,end,random))>::value,
                          "Function must have fitness(begin,end,random)->Real function");
            return Sample::fitness(begin, end, random);
        }
        
        template<typename ForwardIterator, typename Random>
        static void crossover(ForwardIterator begin1, ForwardIterator end1,
                              ForwardIterator begin2, ForwardIterator end2, Random&&random)
        {
            detail::template crossover_for_chromosome_with_only_one_gene<Sample>(begin1, end1, begin2, end2, random);
            detail::template crossover_with_single_point                <Sample>(begin1, end1, begin2, end2, random);
        }
        //template<typename Random> void mutate(Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            detail::template mutate<Sample>(begin, end, random);
        }
        
        
        struct detail {
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator, typename Random>
            static typename std::enable_if<simple_gene_concept::template random_initialize<F>::enabled>::type
            random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
            {
                static_assert(!std::is_void<decltype(std::declval<Sample>().random_initialize(0,std::declval<Random>()))>::value,
                              "Function must have random_initialize(int,random)->Unit function");
                for (auto it=begin; it!=end; it++)
                {
                    auto i = std::distance(begin, it);
                    it->_value = Sample::random_initialize(i, random);
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
                static_assert(std::is_floating_point<decltype(std::declval<Sample>().rate_for_crossover_with_single_point())>::value,
                              "Function must have mutate_rate()->Real function");
                auto n = std::distance(begin1, end1);
                if (n > 1)
                {
                    if (rate(random) < Sample::rate_for_crossover_with_single_point())
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
                static_assert(std::is_floating_point<decltype(std::declval<Sample>().rate_for_crossover_chromosome_with_only_one_gene())>::value,
                              "Function must have mutate_rate()->Real function");
                auto n = std::distance(begin1, end1);
                if (1 == n)
                {
                    if (rate(random) < Sample::rate_for_crossover_chromosome_with_only_one_gene())
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
                static_assert(!std::is_void<decltype(std::declval<Sample>().mutate(std::declval<ForwardIterator>(),std::declval<Random>()))>::value,
                              "Function must have mutate(int,random)->Unit function");
                static_assert(std::is_floating_point<decltype(std::declval<Sample>().rate_for_mutate())>::value,
                              "Function must have mutate_rate()->Real function");
                
                for (auto it=begin; it!=end; it++)
                {
                    if (rate(random) < Sample::rate_for_mutate())
                        it->_value = Sample::mutate(it, random);
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
        enum { N_gene_count = _N_gene_count };
        typedef Gene             gene_type;
        typedef chromosome<Gene,N_gene_count> this_type;
        
        std::vector<Gene>        genes;
        typedef decltype(std::begin(genes)) gene_iterator;
        
        chromosome(std::size_t n):genes(n)
        {
        }
        chromosome() = default;
        
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
        
        template<typename G, typename Random>
        typename std::enable_if<
        /**/std::is_void<
        /*    */decltype(G::random_initialize(std::declval<gene_iterator>(),
                                              std::declval<gene_iterator>(),
                                              std::declval<Random>()))
        /**/>::value
        >::type
        random_initialize(Random&&random)
        {
            this->resize(N_gene_count);
            G::random_initialize(std::begin(genes), std::end(genes), random);
        }
        template<typename G, typename Random>
        typename std::enable_if<
        /**/std::is_void<
        /*    */decltype(std::declval<G>().random_initialize(std::declval<Random>()))
        /**/>::value
        >::type
        random_initialize(Random&&random)
        {
            for (auto it=std::begin(genes); it!=std::end(genes); it++)
                it->random_initialize(std::forward<Random>(random));
        }
        
        template<typename G>
        void random_initialize(...);
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename G, typename Real, typename Random>
        typename std::enable_if<
        /**/std::is_floating_point<
        /*    */decltype(G::compute_fitness(std::declval<gene_iterator>(),
                                            std::declval<gene_iterator>(),
                                            std::declval<Random>()))
        /**/>::value,
        Real>::type
        compute_fitness(Random&&random)
        {
            return G::compute_fitness(std::begin(genes), std::end(genes), random);
        }
        
//        template<typename G, typename Random, typename Real=double>
//        typename std::enable_if<
//        /**/std::is_same<Real,
//        /*    */decltype(std::declval<G>().compute_fitness(std::declval<Random>()))
//        /**/>::value,
//        Real>::type
//        compute_fitness(Random&&random)
//        {
//            for (auto it=std::begin(genes); it!=std::end(genes); it++)
//                it->template compute_fitness<Random,Real>(std::forward<Random>(random));
//        }
        
        template<typename G, typename Real=double>
        Real compute_fitness(...);
        
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename G, typename Random, typename Real=double>
        typename std::enable_if<
        /**/std::is_void<
        /*    */decltype(G::crossover(std::declval<gene_iterator>(),
                                      std::declval<gene_iterator>(),
                                      std::declval<gene_iterator>(),
                                      std::declval<gene_iterator>(),
                                      std::declval<Random>()))
        /**/>::value
        >::type
        crossover(this_type&o, Random&&random)
        {
            G::crossover(std::begin(genes), std::end(genes),
                         std::begin(o.genes), std::end(o.genes),
                         random);
        }
        
        template<typename G>
        void crossover(...);
        
        ////////////////////////////////////////////////////////////////////////////////
        
        template<typename G, typename Random>
        typename std::enable_if<
        /**/std::is_void<
        /*    */decltype(std::declval<G>().mutate(std::declval<gene_iterator>(),
                                                  std::declval<gene_iterator>(),
                                                  std::declval<Random>()))
        /**/>::value
        >::type
        mutate(Random&&random)
        {
            G::mutate(std::begin(genes), std::end(genes), random);
        }
        
        template<typename G, typename Random>
        typename std::enable_if<
        /**/std::is_void<
        /*    */decltype(std::declval<G>().mutate(std::declval<Random>()))
        /**/>::value
        >::type
        mutate(Random&&random)
        {
            for (auto it=std::begin(genes); it!=std::end(genes); it++)
                it->mutate(std::forward<Random>(random));
        }
        
        template<typename G>
        void mutate(...);
    };
    
} //namespace model {
