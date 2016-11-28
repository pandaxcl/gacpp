namespace util {
    
    template<typename Value, typename Real>
    Value&& value_in_range_with_ratio(const Value&A, const Value&B, Real t)
    {
        return std::move(A + (B - A) * t);
    }
    
    template<typename Value, typename Random>
    Value&&random_sign(Random&&random)
    {
        return std::move(Value((0 == random()%2)?+1:-1));
    }
    
    
    template<typename Value>
    Value&& value_clamped_in_range(const Value&A, const Value&B, Value&&x)
    {
        auto minmax = std::minmax(A, B);
        if (x < minmax.first)
            return std::move(Value(minmax.first));
        else if(x > minmax.second)
            return std::move(Value(minmax.second));
        
        return std::move(x);
    }
    
    template<typename Real, typename Random>
    typename std::enable_if<std::is_floating_point<Real>::value, Real>::type
    random_0_1(Random&&random)
    {
        return static_cast<Real>(random())/random.max();
    }
    
    template<typename Integer, typename RandomNumber>
    typename std::enable_if<std::is_integral<Integer>::value && std::is_integral<RandomNumber>::value, Integer>::type
    random_index_for_size(Integer N, RandomNumber random)
    {
        assert(0 != N);
        assert(N > 0);
        if (1 == N) return 0;
        return random%N;
    }
    
    template<typename Integer, typename Random, typename Set>
    void random_positions_with_distinct(Set&&positions, Integer n, Random&&random, Integer size)
    {
        std::uniform_int_distribution<Integer> generator(0, size);
        while(positions.size() < n)
        {
            positions.insert(generator(random));
        }
    }
    
}// namespace util {
