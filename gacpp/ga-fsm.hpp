namespace finite_state_machine {
    
    typedef std::function<bool()> condition;
    typedef std::function<void()> action;
    
    struct transition
    {
        size_t condition;
        std::vector<size_t> actions;
        size_t target_state;
    };
    
    struct state
    {
        std::vector<size_t>     on_enter;
        std::vector<size_t>     on_leave;
        std::vector<transition> transitions;
    };
    
    struct machine
    {
        size_t                  initial_state = 0;
        size_t                  current_state = 0;
        std::vector<state>      states;
        std::vector<condition>  conditions;
        std::vector<action>     actions;
        
        void do_actions(const std::vector<size_t>&actions)
        {
            for (auto&&i:actions)
            {
                this->actions.at(i)();
            }
        }
        
        void start()
        {
            this->current_state = this->initial_state;
            auto&&state = states.at(this->current_state);
            do_actions(state.on_enter);
        }
        
        void update()
        {
            auto&&state = this->states.at(this->current_state);
            for (auto&&transition:state.transitions)
            {
                auto&&test = this->conditions.at(transition.condition);
                if (test())
                {
                    if (this->current_state == transition.target_state)
                    {
                        do_actions(transition.actions);
                    }
                    else
                    {
                        do_actions(state.on_leave);
                        do_actions(transition.actions);
                        this->current_state = transition.target_state;
                        do_actions(state.on_enter);
                    }
                    break;
                }
            }
        }
    };
}// namespace finite_state_machine
