
namespace algorithm {
    
    template<typename Real=double, int N_time_over_seconds=60, int N_epoch_width=6, int N_precision=16>
    struct simple_report
    {
        typedef Real real_type;
        std::string message;
        std::size_t n_epoch = 0;
        std::pair<real_type, real_type> minmax;
        
        std::chrono::time_point<std::chrono::steady_clock> last_time = std::chrono::steady_clock::now();
        
        bool assign(real_type min, real_type max)
        {
            bool fitnessIncremented = max > this->minmax.second;
            bool timeOver = false;
            {
                auto current_time = std::chrono::steady_clock::now();
                auto duration_seconds = std::chrono::duration_cast<std::chrono::duration<int>>(current_time - last_time);
                timeOver = duration_seconds > std::chrono::seconds(N_time_over_seconds);
                if (timeOver || fitnessIncremented)
                    last_time = current_time;
            }
            
            if (fitnessIncremented || timeOver)
            {
                real_type deltaMin = min  - this->minmax.first;
                real_type deltaMax = max - this->minmax.second;
                this->minmax.first  = min;
                this->minmax.second = max;
                
                {
                    const size_t n = N_precision;
                    std::ostringstream oss;
                    oss << std::setprecision(n) << std::fixed;
                    oss << std::setw(14) << std::this_thread::get_id() << "[" << (timeOver?"T":" ") << "]";
                    oss << "[" << std::setw(N_epoch_width) << n_epoch << "]: {";
                    oss << std::setw(n) << std::noshowpos << this->minmax.first  << "(" << std::showpos << std::setw(n) << deltaMin << "), ";
                    oss << std::setw(n) << std::noshowpos << this->minmax.second << "(" << std::showpos << std::setw(n) << deltaMax << ")";
                    oss << "}";
                    message = oss.str();
                }
            }
            
            n_epoch ++;
            
            return fitnessIncremented || timeOver;
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
        typedef simple_team                                 this_type;
        typedef Solution                                    solution_type;
        typedef typename solution_type::real_type           real_type;
        typedef typename solution_type::random_engine_type  random_engine_type;
        typedef typename solution_type::member_type         member_type;
        
        struct MemberWithFitness
        {
            real_type   fitness;
            member_type member;
            friend bool operator > (const MemberWithFitness&a, const MemberWithFitness&b) { return a.fitness > b.fitness; }
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
        
        simple_team():random(rd()), result(*this), migrate(*this)
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
        
        void random_replace_member_next_with_range(typename members_with_fitnesses_type::iterator begin, typename members_with_fitnesses_type::iterator end)
        {
            size_t n = std::distance(begin, end);
            
            if (n <= 0)
                return;
            
            std::set<size_t> random_positions;
            util::random_positions_with_distinct<size_t>(random_positions, n, random, this->size());
            
            {
                auto it_position = std::begin(random_positions);
                auto n_position = *it_position;
                
                for (auto it=begin; it!=end; it++)
                {
                    auto it_next = std::begin(this->members_with_fitnesses_next());
                    std::advance(it_next, *it_position);
                    *it_next = *it;
                }
            }
        }
        
        class result_catetory
        {
            this_type&team;
        public:
            typedef std::set<MemberWithFitness, std::greater<MemberWithFitness>> history_best_in_descending_order_type;
            history_best_in_descending_order_type history_best_in_descending_order;
            explicit result_catetory(this_type&t):team(t) {}
            void sort_by_fitness_with_descending_order()
            {
                typedef typename members_with_fitnesses_type::value_type value_type;
                auto&ms_w_fs = team.members_with_fitnesses();
                std::sort(std::begin(ms_w_fs), std::end(ms_w_fs), [](value_type&a, value_type&b) {
                    return a.fitness > b.fitness;
                });
            }
            auto minmax_fitness()->std::pair<real_type, real_type>
            {
                return std::make_pair(team.members_with_fitnesses().back().fitness,
                                      team.members_with_fitnesses().front().fitness);
            }
            
            void keep_best_for_ratio(real_type ratio)
            {
                auto n = static_cast<std::size_t>(team.members_with_fitnesses().size() * ratio);
                // has been sorted by fitness with descending order
                auto&ms_w_fs = team.members_with_fitnesses();
                for (int i=0; i<n; i++)
                {
                    auto&h = ms_w_fs.at(i);
                    history_best_in_descending_order.insert(h);
                }
                auto num_will_be_erased = history_best_in_descending_order.size() - n;
                if (num_will_be_erased > 0)
                {
                    auto it_from = history_best_in_descending_order.end();
                    auto it_end = history_best_in_descending_order.end();
                    std::advance(it_from, -num_will_be_erased);
                    history_best_in_descending_order.erase(it_from, it_end);
                }
                
                auto begin = std::begin(team.members_with_fitnesses());
                auto end = begin; std::advance(end, n-1);
                
                team.random_replace_member_next_with_range(begin, end);
            }
        }result;
        
        class migrate_catetory
        {
            this_type&team;
            std::mutex mutex;
            members_with_fitnesses_type pool;
            explicit migrate_catetory(this_type&t):team(t){}
            friend this_type;
        public:
            void insert(typename members_with_fitnesses_type::const_iterator begin, typename members_with_fitnesses_type::const_iterator end, size_t nMaxInPool)
            {
                std::lock_guard<std::mutex> lock(mutex);
                assert(nMaxInPool < team.size());
                if (pool.size() >= nMaxInPool)
                    return;
                pool.insert(std::end(pool), begin, end);
            }
            void process()
            {
                std::lock_guard<std::mutex> lock(mutex);
                team.random_replace_member_next_with_range(std::begin(pool), std::end(pool));
                pool.clear();
            }
        }migrate;
    };
    
    template<typename T_CPU>
    class island_team
    {
        std::mutex report_mutex;
        std::vector<T_CPU> cpus;//(std::thread::hardware_concurrency()+2);
        std::vector<std::future<void>> futures;
        std::default_random_engine random;
        friend T_CPU;
    public:
        explicit island_team(size_t N_cpus):cpus(N_cpus){}
        void operator()()
        {
            for (auto i_cpu=0;i_cpu<cpus.size(); i_cpu++)
            {
                auto migrate_cpu = (i_cpu+1)%cpus.size();
                futures.push_back(std::async([this,migrate_cpu,i_cpu](){
                    auto&&cpu = cpus[i_cpu];
                    cpu(*this, migrate_cpu);
                }));
            }
            
            for (auto&&future:futures)
                future.wait();
        }
    };

} // namespace algorithm {
