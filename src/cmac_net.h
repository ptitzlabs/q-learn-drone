#ifndef _CMAC_NET_H_
#define _CMAC_NET_H_
#include "RL_headers.h"
#include "tiles.h"


class cmac_net {
   public:
       cmac_net();
    cmac_net(int num_inputs, double* tile_dimension, int tile_resolution = 8,
             int memory_size = 3000, int num_tilings = 10, int num_hashings = 3,
             double alpha = 0.5, double gamma = 1, double lambda = 0.9);

    
    ~cmac_net();

    void parm_init(int num_inputs, double* tile_dimension, int tile_resolution,
             int memory_size, int num_tilings, int num_hashings,
             double alpha , double gamma , double lambda );

    void clear_traces();
    void clear_weights();
    void drop_traces();
    void update_traces(int hash);
    void generate_tiles(double* input);
    void return_value(double* output, int hash);
    void quick_update(double delta);
    void update(double* state, double target, int hash);
    void report();
    int get_num_hashings();
    double * get_weights();
    double * get_tile_sub_dimension();
    int get_memory_size();
    int get_num_tilings();

    void read_weights(char* filename);
    void write_weights(char* filename);
    int id;
    

   private:
    int _memory_size;
    int _num_tilings;
    int _num_hashings;
    int _num_inputs;
    double _alpha;
    double _gamma;
    double _lambda;
    double* _tile_dimension;
    double* _tile_sub_dimension;
    double* _weights;
    double* _traces;
    int** _hashings;
    int _tile_resolution;

    double* _outputs_tmp;
};
#endif
