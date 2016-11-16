namespace selection {
    
    template<typename ForwardIterator, typename Real, typename Fitness /* std::function<Real(ForwardIterator)> */>
    ForwardIterator roulette_one(ForwardIterator begin, ForwardIterator end, Real fSlice, Fitness&&fitness)
    {
        auto fFitnessSoFar = Real(0.0);
        
        for (auto it=begin; it!=end; it++)
        {
            fFitnessSoFar += fitness(it);
            if (fFitnessSoFar >= fSlice)
            {
                return it;
            }
        }

        return begin;
    }

}// namespace selection
