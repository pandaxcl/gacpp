namespace mutate {
    
    template<typename ForwardIterator, typename MutateFunction /*void(Member&)*/>
    void at_single_point(ForwardIterator begin, ForwardIterator end,
                         typename std::iterator_traits<ForwardIterator>::difference_type i,
                         MutateFunction&&mutate)
    {
        assert(i >= 0);
        assert(i < std::distance(begin, end));
        
        auto it = begin; std::advance(it, i);
        mutate(*it);
    }
    
    template<typename Value, typename Random, typename Real>
    void for_real_value_clamped_in_range(Value&&x, Random&&random, Real step, Real from, Real to)
    {
        auto t = gacpp::util::random_0_1<Real>(random);
        auto&&sign = gacpp::util::random_sign<Real>(random);
        x = gacpp::util::value_clamped_in_range(from, to, x+sign*t*step);
    }
    
    
}// namespace mutation
