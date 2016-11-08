namespace model {
    
    template <typename Gene, size_t N_gene, typename Block/* =size_t */>
    struct chromosome
    {
        enum {
            N_gene_count = N_gene,
            N_block_count = sizeof(Gene)/sizeof(Block)*N_gene_count,
        };
        union {
            Block blocks[N_block_count];
            Gene  genes[N_gene_count];
        };
    };
    
} //namespace model {
