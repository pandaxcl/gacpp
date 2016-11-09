namespace model {
    
    template <typename Gene, size_t N_gene, typename Block/* =size_t */>
    struct chromosome
    {
        typedef chromosome<Gene, N_gene, Block> self_type;
        enum {
            N_gene_count = N_gene,
            N_block_count = sizeof(Gene)/sizeof(Block)*N_gene_count,
        };
        union {
            Block blocks[N_block_count];
            Gene  genes[N_gene_count];
        };
        
        template<typename Random>
        void random_initialize(Random&&random)
        {
            //Gene::random_initialize(std::begin(genes), std::end(genes), random);
        }
        
        template<typename Random, typename Real=double>
        Real compute_fitness(Random&&random)
        {
            //return Gene::compute_fitness(std::begin(genes), std::end(genes), random);
            return Real(0);
        }
        
        template<typename Random>
        std::pair<self_type,self_type>&&crossover(const self_type&o, Random&&random)
        {
            //return Gene::crossover(std::begin(genes), std::end(genes), std::begin(o.genes), random);
            return std::move(std::make_pair(*this, o));
        }
        
        template<typename Random>
        void mutate(Random&&random)
        {
            //Gene::mutate(std::begin(genes), std::end(genes), random);
            //for (auto&&gene:genes) gene.mutate(random);
        }
    };
    
} //namespace model {
