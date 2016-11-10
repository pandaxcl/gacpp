
namespace algorithm {

    template<typename Member, typename Operation=typename Member::gene_type, typename Real=double, typename Random = std::default_random_engine>
    struct team {
        typedef std::vector<Member>   members_type;
        typedef Random                random_engine;
        typedef Operation             operation_type;
        
        members_type      *members_ptr = nullptr;
        members_type      *members_next_ptr = nullptr;
        struct {
            members_type  members_front;
            members_type  members_back;
        }buffer;
        
        std::vector<Real>   fitnesses;
        random_engine       random;
        
        team()
        {
            members_ptr = &buffer.members_front;
            members_next_ptr = &buffer.members_back;
        }
        team(size_t n):team()
        {
            resize(n);
        }
        size_t size()
        {
            return this->members_ptr->size();
        }
        void resize(size_t n)
        {
            this->buffer.members_front.resize(n);
            this->buffer.members_back.resize(n);
            this->fitnesses.resize(n);
        }
        void swap_buffers()
        {
            std::swap(members_ptr, members_next_ptr);
        }
        void random_initialize()
        {
            auto&&members = *this->members_ptr;
            for (auto&&member:members)
                member.template random_initialize<operation_type>(random);
            
        }
        Real compute_fitnesses()
        {
            auto&&members = *this->members_ptr;
            Real fTotalFitness = Real(0);
            for (size_t i=0; i<members.size(); i++)
            {
                this->fitnesses[i] = members[i].template compute_fitness<operation_type,Real>(random);
                fTotalFitness += this->fitnesses[i];
            }
            return fTotalFitness;
        }
        
        void epoch()
        {
            auto&&members = *this->members_ptr;
            auto&&members_next = *this->members_next_ptr;
            
            // 1. compute fitness
            Real fTotalFitness = this->compute_fitnesses();
            
            // 2. select to crossover
            {
                auto select_one = [this, fTotalFitness, &members]
                {
                    typedef typename members_type::iterator member_iterator;
                    Real fSlice = static_cast<Real>(random())/random.max()*fTotalFitness;
                    return selection::roulette_one(std::begin(members), std::end(members), fSlice,
                                                   [this,&members](member_iterator it)
                    {
                        auto i = std::distance(std::begin(members), it);
                        return this->fitnesses.at(i);
                    });
                };
                assert(0 == members.size()%2);// must be even number
                for (size_t i=0; i<members.size()/2; )
                {
                    auto it_1 = select_one();
                    auto it_2 = select_one();
                    
                    members_next[i*2+0] = *it_1;
                    members_next[i*2+1] = *it_2;
                    
                    if (it_1 != it_2)
                    {
                        auto&&A = members_next[i*2+0];
                        auto&&B = members_next[i*2+1];
                        
                        A.template crossover<operation_type>(B, random);
                    }
                    i++;
                }
            }
            // 3. swap buffers
            this->swap_buffers();
            
            // 4. mutate
            {
                for (auto&&member:members)
                {
                    member.template mutate<operation_type>(random);
                }
            }
        }
    };

} // namespace algorithm {
