namespace model {

    struct sample_gene
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
    
    template<typename Value/*int or float*/, typename Function, typename Real=double>
    struct simple_gene
    {
        Value _value;
        Value&value() { return _value; }
        operator Value() { return _value; }
        operator Value&() { return _value; }
        operator const Value() { return _value; }
        operator const Value&() { return _value; }
        
        template<typename Random>
        static Real rate(Random&&random)
        {
            return static_cast<Real>(random())/static_cast<Real>(random.max());
        }
        //template<typename Random> void random_initialize(Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            detail::template random_initialize<Function>(begin, end, random);
        }
        template<typename ForwardIterator, typename Random>
        static Real compute_fitness(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            static_assert(std::is_floating_point<decltype(std::declval<Function>().fitness(begin,end,random))>::value,
                          "Function must have fitness(begin,end,random)->Real function");
            return Function::fitness(begin, end, random);
        }
        
        template<typename ForwardIterator, typename Random>
        static void crossover(ForwardIterator begin1, ForwardIterator end1,
                              ForwardIterator begin2, ForwardIterator end2, Random&&random)
        {
            detail::template crossover_for_chromosome_with_only_one_gene<Function>(begin1, end1, begin2, end2, random);
            detail::template crossover_with_single_point                <Function>(begin1, end1, begin2, end2, random);
        }
        //template<typename Random> void mutate(Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            detail::template mutate<Function>(begin, end, random);
        }
        
        struct detail {
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator, typename Random>
            static typename std::enable_if<
            /**/!std::is_void<decltype(std::declval<F>().random_initialize(0,std::declval<Random>()))>::value
            >::type
            random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
            {
                static_assert(!std::is_void<decltype(std::declval<Function>().random_initialize(0,std::declval<Random>()))>::value,
                              "Function must have random_initialize(int,random)->Unit function");
                for (auto it=begin; it!=end; it++)
                {
                    auto i = std::distance(begin, it);
                    it->_value = Function::random_initialize(i, random);
                }
            }
            template<typename F>
            static void random_initialize(...){}
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            template<typename F, typename ForwardIterator, typename Random>
            static typename std::enable_if<
            /**/std::is_floating_point<decltype(std::declval<F>().rate_for_crossover_with_single_point())>::value
            >::type
            crossover_with_single_point(ForwardIterator begin1, ForwardIterator end1,
                                        ForwardIterator begin2, ForwardIterator end2, Random&&random)
            {
                static_assert(std::is_floating_point<decltype(std::declval<Function>().rate_for_crossover_with_single_point())>::value,
                              "Function must have mutate_rate()->Real function");
                auto n = std::distance(begin1, end1);
                if (n > 1)
                {
                    if (rate(random) < Function::rate_for_crossover_with_single_point())
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
            static typename std::enable_if<
            /**/std::is_floating_point<decltype(std::declval<F>().rate_for_chromosome_with_only_one_gene())>::value
            >::type
            crossover_for_chromosome_with_only_one_gene(ForwardIterator begin1, ForwardIterator end1,
                                                        ForwardIterator begin2, ForwardIterator end2, Random&&random)
            {
                static_assert(std::is_floating_point<decltype(std::declval<Function>().rate_for_chromosome_with_only_one_gene())>::value,
                              "Function must have mutate_rate()->Real function");
                auto n = std::distance(begin1, end1);
                if (1 == n)
                {
                    if (rate(random) < Function::rate_for_chromosome_with_only_one_gene())
                    {
                        auto&&ratio_0_1 = static_cast<Real>(random())/random.max();
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
            static typename std::enable_if<
            /**/!std::is_void<decltype(std::declval<F>().mutate(std::declval<ForwardIterator>(),std::declval<Random>()))>::value
            /**/&&
            /**/std::is_floating_point<decltype(std::declval<F>().rate_for_mutate())>::value
            >::type
            mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
            {
                static_assert(!std::is_void<decltype(std::declval<Function>().mutate(std::declval<ForwardIterator>(),std::declval<Random>()))>::value,
                              "Function must have mutate(int,random)->Unit function");
                static_assert(std::is_floating_point<decltype(std::declval<Function>().rate_for_mutate())>::value,
                              "Function must have mutate_rate()->Real function");
                
                for (auto it=begin; it!=end; it++)
                {
                    if (rate(random) < Function::rate_for_mutate())
                        it->_value = Function::mutate(it, random);
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
