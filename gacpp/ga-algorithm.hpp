
namespace algorithm {
    
    template<typename Real, int N_epoch_width=6, int N_precision=16>
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
                    oss << std::setprecision(n) << std::fixed;
                    oss << "[" << std::setw(N_epoch_width) << n_epoch << "]: {";
                    oss << std::setw(n) << std::noshowpos << this->minmax.first  << "(" << std::showpos << std::setw(n) << deltaMin << "), ";
                    oss << std::setw(n) << std::noshowpos << this->minmax.second << "(" << std::showpos << std::setw(n) << deltaMax << ")";
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

    template<typename Solution>
    struct simple_team
    {
        typedef Solution                                    solution_type;
        typedef typename solution_type::real_type           real_type;
        typedef typename solution_type::random_engine_type  random_engine_type;
        typedef typename solution_type::member_type         member_type;
        
        struct MemberWithFitness
        {
            real_type   fitness;
            member_type member;
        };
        typedef std::vector<MemberWithFitness> members_with_fitnesses_type;
        
        members_with_fitnesses_type      *members_ptr = nullptr;
        members_with_fitnesses_type      *members_next_ptr = nullptr;
        struct {
            members_with_fitnesses_type  members_front;
            members_with_fitnesses_type  members_back;
        }buffer;

        std::random_device  rd;
        random_engine_type  random;
        
        members_with_fitnesses_type&members_with_fitnesses() {
            assert(nullptr != this->members_ptr);
            return *this->members_ptr;
        }
        members_with_fitnesses_type&members_with_fitnesses_next() {
            assert(nullptr != this->members_next_ptr);
            return *this->members_next_ptr;
        }
        
        simple_team():random(rd())
        {
            members_ptr = &buffer.members_front;
            members_next_ptr = &buffer.members_back;
        }
        simple_team(size_t n):simple_team()
        {
            resize(n);
        }
        size_t size()
        {
            return this->members_with_fitnesses().size();
        }
        void resize(size_t n)
        {
            this->buffer.members_front.resize(n);
            this->buffer.members_back.resize(n);
        }
        void swap_buffers()
        {
            std::swap(members_ptr, members_next_ptr);
        }
        void random_initialize()
        {
            for (auto&&mwf:this->members_with_fitnesses())
                mwf.member.template random_initialize<solution_type>(random);
            
        }
        real_type compute_fitnesses()
        {
            real_type fTotalFitness = real_type(0);
            for (auto it=std::begin(this->members_with_fitnesses()); it!=std::end(this->members_with_fitnesses()); it++)
            {
                it->fitness = it->member.template compute_fitness<solution_type>(random);
                fTotalFitness += it->fitness;
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
                    real_type fSlice = static_cast<real_type>(random())/random.max()*fTotalFitness;
                    return selection::roulette_one(std::begin(this->members_with_fitnesses()),
                                                   std::end(this->members_with_fitnesses()), fSlice,
                                                   [this](typename members_with_fitnesses_type::iterator it)
                    {
                        assert(it != std::end(this->members_with_fitnesses()));
                        return it->fitness;
                    });
                };
                assert(0 == this->members_with_fitnesses().size()%2);// must be even number
                for (size_t i=0; i<this->members_with_fitnesses().size()/2; )
                {
                    auto it_1 = select_one();
                    auto it_2 = select_one();
                    
                    this->members_with_fitnesses_next()[i*2+0] = *it_1;
                    this->members_with_fitnesses_next()[i*2+1] = *it_2;
                    
                    if (it_1 != it_2)
                    {
                        auto&&A = this->members_with_fitnesses_next()[i*2+0].member;
                        auto&&B = this->members_with_fitnesses_next()[i*2+1].member;
                        
                        A.template crossover<solution_type>(B, random);
                    }
                    i++;
                }
            }
            
            // 3. mutate
            {
                for (auto&&m:this->members_with_fitnesses_next())
                {
                    m.member.template mutate<solution_type>(random);
                }
            }
        }
        
        void sort_members_by_fitness_with_descending_order()
        {
            typedef typename members_with_fitnesses_type::value_type value_type;
            std::sort(std::begin(this->members_with_fitnesses()), std::end(this->members_with_fitnesses()), [](value_type&a, value_type&b) {
                return a.fitness > b.fitness;
            });
        }
        
        auto minmax_fitness_in_sorted_members_with_descending_order()
        ->  decltype(std::make_pair(this->members_with_fitnesses().back().fitness, this->members_with_fitnesses().front().fitness))
        {
            return std::make_pair(this->members_with_fitnesses().back().fitness, this->members_with_fitnesses().front().fitness);
        }
        
        void keep_best_for_ratio(real_type ratio)
        {
            auto n = static_cast<std::size_t>(this->members_with_fitnesses().size() * ratio);
            std::set<std::size_t> replace_points;
            while (replace_points.size() < n)
            {
                replace_points.insert(this->random()%this->members_with_fitnesses().size());
            }
            
            std::size_t i=0;
            for (auto it=std::begin(replace_points); it!=std::end(replace_points); it++,i++)
            {
                this->members_with_fitnesses_next()[*it] = this->members_with_fitnesses()[i];
            }
        }
    };

} // namespace algorithm {
