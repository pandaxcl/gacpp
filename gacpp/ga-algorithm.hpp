
namespace algorithm {
    
    template<typename Real, int N_epoch_width=4, int N_precision=16>
    struct simple_report
    {
        typedef Real real_type;
        std::string message;
        std::size_t n_epoch = 0;
        std::pair<real_type, real_type> minmax;
        
        bool assign(real_type min, real_type max)
        {
            bool needRecord = false;
            if (max > this->minmax.second)
            {
                real_type deltaMin = min  - this->minmax.first;
                real_type deltaMax = max - this->minmax.second;
                this->minmax.first  = min;
                this->minmax.second = max;
                
                {
                    const size_t n = N_precision;
                    std::ostringstream oss;
                    oss << std::setprecision(n);
                    oss << "[" << std::setw(N_epoch_width) << n_epoch << "]: {";
                    oss << std::setw(n) << this->minmax.first  << "(" << (deltaMin>=0?"+":"") << std::setw(n) << deltaMin << "), ";
                    oss << std::setw(n) << this->minmax.second << "(" << (deltaMax>=0?"+":"") << std::setw(n) << deltaMax << ")";
                    oss << "}";
                    message = oss.str();
                }

                needRecord = true;
            }
            n_epoch ++;
            return needRecord;
        }
        template<typename MinMax>
        typename std::enable_if<
        /**/std::is_convertible<decltype(std::declval<MinMax>().first ), double>::value &&
        /**/std::is_convertible<decltype(std::declval<MinMax>().second), double>::value ,
        bool>::type
        operator = (MinMax minmax)
        {
            return this->assign(minmax.first, minmax.second);
        }
        template<typename MinMax>
        typename std::enable_if<
        /**/std::is_convertible<decltype(*std::declval<MinMax>().first ), double>::value &&
        /**/std::is_convertible<decltype(*std::declval<MinMax>().second), double>::value ,
        bool>::type
        operator = (MinMax minmax)
        {
            return this->assign(*minmax.first, *minmax.second);
        }
        
        operator std::string& () { return this->message; }
        operator const std::string& () const { return this->message; }
        
        template<typename Stream>
        friend Stream&operator << (Stream&os, const simple_report&reporter)
        {
            os << static_cast<const std::string&>(reporter);
            return os;
        }
    };

    template<typename Member, typename Solution>
    struct team
    {
        typedef Solution                                solution_type;
        typedef typename solution_type::real_type       real_type;
        typedef typename solution_type::random_engine   random_engine;
		typedef std::vector<Member>                     members_type;
		typedef std::vector<real_type>					fitnesses_type;
        
        members_type      *members_ptr = nullptr;
        members_type      *members_next_ptr = nullptr;
        struct {
            members_type  members_front;
            members_type  members_back;
        }buffer;
        
		fitnesses_type  fitnesses;
        std::random_device      rd;
        random_engine           random;
        
        members_type&members() {
            assert(nullptr != this->members_ptr);
            return *this->members_ptr;
        }
        members_type&members_next() {
            assert(nullptr != this->members_next_ptr);
            return *this->members_next_ptr;
        }
        
        typename members_type::iterator member_for_fitness(typename fitnesses_type::iterator it) {
            auto index = std::distance(std::begin(fitnesses), it);
            auto it_member = std::begin(members()); std::advance(it_member, index);
            return it_member;
        }
        
        team():random(rd())
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
                member.template random_initialize<solution_type>(random);
            
        }
        real_type compute_fitnesses()
        {
            auto&&members = *this->members_ptr;
            real_type fTotalFitness = real_type(0);
            for (size_t i=0; i<members.size(); i++)
            {
                this->fitnesses[i] = members[i].template compute_fitness<solution_type>(random);
                fTotalFitness += this->fitnesses[i];
            }
            return fTotalFitness;
        }
        
        void epoch()
        {
            // 1. compute fitness
            real_type fTotalFitness = this->compute_fitnesses();
            
            // 2. select to crossover
            {
                auto select_one = [this, fTotalFitness]
                {
                    auto&&members = *this->members_ptr;
                    typedef typename members_type::iterator member_iterator;
                    real_type fSlice = static_cast<real_type>(random())/random.max()*fTotalFitness;
                    return selection::roulette_one(std::begin(members), std::end(members), fSlice,
                                                   [this](member_iterator it)
                    {
                        auto&&members = *this->members_ptr;
                        assert(it != std::end(members));
                        auto i = std::distance(std::begin(members), it);
                        return this->fitnesses.at(i);
                    });
                };
                auto&&members = *this->members_ptr;
                assert(0 == members.size()%2);// must be even number
                for (size_t i=0; i<members.size()/2; )
                {
                    auto&&members_next = *this->members_next_ptr;
                    
                    auto it_1 = select_one();
                    auto it_2 = select_one();
                    
                    members_next[i*2+0] = *it_1;
                    members_next[i*2+1] = *it_2;
                    
                    if (it_1 != it_2)
                    {
                        auto&&A = members_next[i*2+0];
                        auto&&B = members_next[i*2+1];
                        
                        A.template crossover<solution_type>(B, random);
                    }
                    i++;
                }
            }
            
            // 3. mutate
            {
                auto&&members_next = *this->members_next_ptr;
                for (auto&&member:members_next)
                {
                    member.template mutate<solution_type>(random);
                }
            }
        }
        
        void keep_best_for_ratio(real_type ratio)
        {
            struct Data
            {
                real_type                       fitness;
                typename members_type::iterator it_member;
                Data(decltype(fitness)f, decltype(it_member)it):fitness(f),it_member(it){}
            };
            std::vector<Data> tmp;
            typename members_type::iterator it_member = std::begin(members());
            for (auto it_fitness=std::begin(fitnesses); it_fitness!=std::end(fitnesses); )
            {
                tmp.push_back(Data(*it_fitness, it_member));
                it_fitness++;
                it_member++;
            }
            std::sort(std::begin(tmp), std::end(tmp), [](typename decltype(tmp)::value_type&a,
                                                         typename decltype(tmp)::value_type&b){
                return a.fitness > b.fitness;
            });
            
            auto n = static_cast<std::size_t>(members().size() * ratio);
            auto&&members = *this->members_ptr;
            auto&&members_next = *this->members_next_ptr;
            for (auto i=0; i<n; i++)
            {
                auto it_member = tmp.at(i).it_member;
                auto index = std::distance(std::begin(members), it_member);
                auto it = std::begin(members_next); std::advance(it, index);
                members_next[index] = *it;
            }
        }
    };

} // namespace algorithm {
