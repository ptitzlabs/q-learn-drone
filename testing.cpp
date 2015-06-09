#include "testing.h"
#include "cmac_net.h"

void CMAC_testing(PLFLT* xg, PLFLT* yg, PLFLT ** zg, int nx,int ny) {
    printf("doing stuff...\n");
    const int k_num_inputs = 2;
    const float k_range_inputs[][2] = {{0.0, 2.0}, {0.0, 2.0}};
    const int k_num_tilings = 32;
    const int k_tile_resolution = 8;
    const float k_alpha = 0.5;
    const int k_memory_size = 3000;
    const int k_hash = 0;

    float** range_inputs = new float*[k_num_inputs];
    for (int i = 0; i<k_num_inputs; i++) range_inputs[i] = new float[2];
    for (int i = 0; i<k_num_inputs; i++){
        range_inputs[i][0] = k_range_inputs[i][0];
        range_inputs[i][1] = k_range_inputs[i][1];
    }
    CMAC_net net(k_num_inputs, range_inputs,
                 k_num_tilings, k_tile_resolution, k_alpha, k_memory_size,
                 k_hash);
    net.report();

    float inputs[][2] = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    float targets[] = {1, 0, 1, 0};

    for (int j = 0; j<3; j++){
    for (int i = 0; i < 4; i++) {
        net.train(inputs[i], targets[i]);
    }
    }




    for (int i = 0; i < nx; i++) {
        xg[i] = (float)i/((float)nx-1)*2.0;
    }
    for (int i = 0; i < ny; i++) {
        yg[i] = (float)i/((float)ny-1)*2.0;
    }

    float input[2];
    float output;

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            input[0] = xg[i];
            input[1] = yg[j];
            //printf("(%.2f,%.2f)\n",input[0],input[1]);
            net.sim(&output,input);
            //printf("%.2f\n",output);
            //zg[i][j] = xg[i] * 2 * yg[j];
            zg[i][j] = output;
        }
    }

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            //printf("%.2f ", zg[i][j]);
        }

        //printf("\n");
    }
    

    // qt_interface::contourf(xg,yg,zg,nx,ny);



    // Python_interface python("python_fun");
    printf("doing more stuff...\n");
}
