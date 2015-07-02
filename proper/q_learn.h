#define Q_LEARN_H
#ifndef RL_HEADERS_H
#include "RL_headers.h"
#endif
#ifndef CMAC_NET_H
#include "cmac_net.h"
#endif
#ifndef MODEL_H
#include "model.h"
#endif

class q_learn {
    public:
        q_learn(cmac_net * net, model * m, float epsilon = 0.0, int max_steps = 10000);
        ~q_learn();

        void set_goal(float * goal);
        int find_max();
        void calc_q();
        bool with_probability(float p);
        bool goal_reached();
        void run_episode();

    private:
        int _max_steps;
        float _epsilon;
        int _size_q;
        float *_q;
        cmac_net *_net;
        model * _m;

};


