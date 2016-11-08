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
    
}// namespace mutation
