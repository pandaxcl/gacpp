
namespace algorithm {

    template<typename Resident, typename Real=double>
    struct population {
        typedef std::vector<Resident> residents_type;
        residents_type      *residents_ptr = nullptr;
        residents_type      *residents_next_ptr = nullptr;
        struct {
            residents_type  residents_front;
            residents_type  residents_back;
        }buffer;
        
        std::vector<Real>           fitnesses;
        std::default_random_engine  random;
        
        population()
        {
            residents_ptr = &buffer.residents_front;
            residents_next_ptr = &buffer.residents_back;
        }
        void swap_buffers()
        {
            std::swap(residents_ptr, residents_next_ptr);
        }
        void random_initialize()
        {
            auto&&residents = *this->residents_ptr;
            for (auto&&resident:residents)
                resident.random_initialize(random);
            this->fitnesses.resize(residents.size());
        }
        Real compute_fitnesses()
        {
            auto&&residents = *this->residents_ptr;
            Real fTotalFitness = Real(0);
            for (size_t i=0; i<residents.size(); i++)
            {
                this->fitnesses[i] = residents[i].compute_fitness(random);
                fTotalFitness += this->fitnesses[i];
            }
            return fTotalFitness;
        }
        
        void epoch()
        {
            auto&&residents = *this->residents_ptr;
            auto&&residents_next = *this->residents_next_ptr;
            
            // 1. compute fitness
            Real fTotalFitness = this->compute_fitnesses();
            
            // 2. select to crossover
            {
                auto select_one = [this, fTotalFitness, &residents]
                {
                    typedef typename residents_type::iterator resident_iterator;
                    Real fSlice = static_cast<Real>(random())/random.max()*fTotalFitness;
                    return selection::roulette_one(std::begin(residents), std::end(residents), fSlice,
                                                   [this,&residents](resident_iterator it)
                    {
                        auto i = std::distance(std::begin(residents), it);
                        return this->fitnesses.at(i);
                    });
                };
                assert(0 == residents.size()%2);// must be even number
                for (size_t i=0; i<residents.size()/2; )
                {
                    auto it_1 = select_one();
                    auto it_2 = select_one();
                    if (it_1 != it_2)
                    {
                        auto && result = it_1->crossover(*it_2, random);
                        residents_next[i*2+0] = result.first;
                        residents_next[i*2+1] = result.second;
                        i++;
                    }
                }
            }
            // 3. swap buffers
            this->swap_buffers();
            
            // 4. mutate
            {
                for (auto&&resident:residents)
                {
                    resident.mutate(random);
                }
            }
        }
    };

} // namespace algorithm {
