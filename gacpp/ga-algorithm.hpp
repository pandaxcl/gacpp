namespace algorithm {

    template<typename Resident, typename Real=double>
    struct population {
        std::vector<Resident>      &residents;
        std::vector<Resident>      &residents_crossover;
        struct {
            std::vector<Resident>       residents_front;
            std::vector<Resident>       residents_back;
        }buffer;
        
        std::vector<Real>           fitnesses;
        std::default_random_engine  random;
        
        population():residents(buffer.residents_front), residents_crossover(buffer.residents_back)
        {
        }
        void swap_buffers()
        {
            auto&tmp = residents;
            residents = std::forward<Resident>(residents_crossover) ;
            residents_crossover = std::forward<Resident>(tmp);
        }
        void random_initialize()
        {
            for (auto&&resident:residents)
                resident.random_initialize();
            this->fitnesses.resize(residents.size());
        }
        Real compute_fitnesses()
        {
            Real fTotalFitness = Real(0);
            for (size_t i=0; i<residents.size(); i++)
            {
                this->fitnesses[i] = residents[i].compute_fitness();
                fTotalFitness += this->fitnesses[i];
            }
            return fTotalFitness;
        }
        
        void epoch()
        {
            // 1. compute fitness
            Real fTotalFitness = this->compute_fitnesses();
            
            // 2. select to crossover
            {
                auto select_one = [this]
                {
                    typedef typename decltype(residents)::iterator resident_iterator;
                    Real fSlice = static_cast<Real>(random())/random.max()*fTotalFitness;
                    return selection::roulette_one(std::begin(residents), std::end(residents), fSlice, [this](resident_iterator it){
                        auto i = std::distance(std::begin(residents), it);
                        return this->fitnesses.at(i);
                    });
                };
                assert(0 == this->residents.size()%2);// must be even number
                for (size_t i=0; i<this->residents.size()/2; )
                {
                    auto it_1 = select_one();
                    auto it_2 = select_one();
                    if (it_1 != it_2)
                    {
                        std::pair<Resident&&, Resident&&> result = (*it_1).crossover(*it_2);
                        this->residents_crossover[i*2+0] = result.first;
                        this->residents_crossover[i*2+1] = result.second;
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
                    auto random_0_1 = [this]{
                        return static_cast<Real>(random())/random.max();
                    };
                    resident.mutate(random_0_1);
                }
            }
        }
    };

} // namespace algorithm {
