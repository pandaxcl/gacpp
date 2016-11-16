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
    
}// namespace util {
