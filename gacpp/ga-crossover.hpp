namespace crossover {
    
    template<typename ForwardIterator, typename SwapFunction>
    void with_single_point(ForwardIterator begin1, ForwardIterator end1,
                           ForwardIterator begin2, ForwardIterator end2,
                           typename std::iterator_traits<ForwardIterator>::difference_type i,
                           SwapFunction&&swap)
    {
        assert(std::distance(begin1, end1) == std::distance(begin2, end2));
        assert(i > 0);
        assert(i < std::distance(begin1, end1));
        assert(i < std::distance(begin2, end2));
        
        auto begin1_cross = begin1; std::advance(begin1_cross, i);
        auto begin2_cross = begin2; std::advance(begin2_cross, i);
        
        for (auto it=begin1_cross; it!=end1; it++)
        {
            swap(*begin1_cross, *begin2_cross);
            begin1_cross++;
            begin2_cross++;
        }
    }
    
    template<typename ForwardIterator>
    void with_single_point(ForwardIterator begin1, ForwardIterator end1,
                           ForwardIterator begin2, ForwardIterator end2,
                           typename std::iterator_traits<ForwardIterator>::difference_type i)
    {
        typedef typename std::iterator_traits<ForwardIterator>::reference reference;
        with_single_point(begin1, end1, begin2, end2, i, [](reference A, reference B){
            std::swap(A, B);
        });
    }
    
    template<typename Value, typename Real>
    void with_linear_interpolation(Value && A, Value && B, Real A_ratio_0_1, Real B_ratio_0_1)
    {
        A = util::value_in_range_with_ratio(A, B, A_ratio_0_1);
        B = util::value_in_range_with_ratio(A, B, B_ratio_0_1);
    }
    
    template<typename Real, typename Value, typename Random>
    void with_linear_interpolation(Value && A, Value && B, Random&&random)
    {
        auto&&A_ratio_0_1 = static_cast<Real>(random())/random.max();
        auto&&B_ratio_0_1 = static_cast<Real>(random())/random.max();
        with_linear_interpolation(A, B, A_ratio_0_1, B_ratio_0_1);
    }
    
} //namespace crossover {
