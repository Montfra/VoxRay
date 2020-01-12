#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "Core.h"

// For visualStudio
#pragma warning(disable : 4996)

using namespace std::chrono;
using namespace std;

// CONSTANT
const string programPath = "opencl_kernel.cl";

int size = 0; // sphere size
int speed = 4; // speed of animation


void executeProgram() {
    // SET VARIABLE
    clSetKernelArg(kernel, 2, sizeof(int), &size);

    // EXECUTE PROGRAM
    clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(commands);

    // GET RESULT
    clEnqueueReadBuffer(commands, pix, CL_TRUE, 0, sizeof(CL_UNSIGNED_INT8) * image_width * image_height, cpu_pix, 0, NULL, NULL);
}

int main(int argc, char* argv[]) {

    // GET PROGRAM
    string source;
    ifstream file(programPath);
    if (!file){
        cout << "\nNo OpenCL file found!" << endl << "Exiting..." << endl;
        system("PAUSE");
        exit(1);
    }
    while (!file.eof()){
        char line[256];
        file.getline(line, 255);
        source += line;
    }
    const char *kernelSource = source.c_str();


    // INIT LIBRARIES
    initLibs(kernelSource);


    // MAIN LOOP
    while (1)
    {
        auto start = high_resolution_clock::now(); // To compute frameRate


        // Catch exit
        eventCatch();


        // EXECUTE PROGRAM
        executeProgram();


        // PRINT TO SCREEN
        printToScreen();


        // CHANGE ANIMATION
        size += speed;
        if (size > 400 || size < 0) {
            speed *= -1;
        }


        // COMPUTE FRAMERATE
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        // cout << duration.count() << endl;
        long int time = static_cast<long int>(duration.count());

        if (time < 20) {
            SDL_Delay(20 - time);
        }
        else {
            cout << "LAG" << endl;
        }

    }

    return 0;
}
