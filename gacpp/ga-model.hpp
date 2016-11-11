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
    
    template<typename Unit/*int or float*/, typename Function, typename Real=double>
    struct simple_gene
    {
        Unit _unit;
        Unit&&unit() { return _unit; }
        template<typename Random>
        static Real rate(Random&&random)
        {
            return static_cast<Real>(random())/static_cast<Real>(random.max());
        }
        //template<typename Random> void random_initialize(Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void random_initialize(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            static_assert(!std::is_void<decltype(std::declval<Function>().mutate(0,std::declval<Random>()))>::value,
                          "Function must have mutate(int,random)->Unit function");
            for (auto it=begin; it!=end; it++)
            {
                auto i = std::distance(begin, it);
                it->_unit = Function::mutate(i, random);
            }
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
            static_assert(std::is_floating_point<decltype(std::declval<Function>().rate_for_crossover_with_single_point())>::value,
                          "Function must have mutate_rate()->Real function");
            assert(std::distance(begin1, end1) > 2);
            
            if (rate(random) < Function::rate_for_crossover_with_single_point())
            {
                auto i = random()%(std::distance(begin1, end1)-1)+1;
                crossover::with_single_point(begin1, end1, begin2, end2, i);
            }
        }
        //template<typename Random> void mutate(Random&&random) { }
        template<typename ForwardIterator, typename Random>
        static void mutate(ForwardIterator begin, ForwardIterator end, Random&&random)
        {
            static_assert(!std::is_void<decltype(std::declval<Function>().mutate(0,std::declval<Random>()))>::value,
                          "Function must have mutate(int,random)->Unit function");
            static_assert(std::is_floating_point<decltype(std::declval<Function>().rate_for_mutate())>::value,
                          "Function must have mutate_rate()->Real function");
            
            for (auto it=begin; it!=end; it++)
            {
                auto i = std::distance(begin, it);
                if (rate(random) < Function::rate_for_mutate())
                    it->_unit = Function::mutate(i, random);
            }
        }
    };
    
    template <typename Gene>
    struct chromosome
    {
        typedef Gene             gene_type;
        typedef chromosome<Gene> self_type;
        
        std::vector<Gene>        genes;
        typedef decltype(std::begin(genes)) gene_iterator;
        
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
        crossover(self_type&o, Random&&random)
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
