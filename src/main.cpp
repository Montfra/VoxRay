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

int sphereSize = 100; // sphere size
int speed = -1; // speed of animation

float mdl[image_width * image_height];


void executeProgram() {
    clEnqueueWriteBuffer(commands, model, CL_TRUE, 0, sizeof(float) * image_height * image_width, mdl, 0, NULL, NULL);

    // SET VARIABLE
    clSetKernelArg(kernel, 2, sizeof(int), &sphereSize);

    // EXECUTE PROGRAM
    clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(commands);

    // GET RESULT
    clEnqueueReadBuffer(commands, pix, CL_TRUE, 0, sizeof(CL_UNSIGNED_INT8) * image_width * image_height, cpu_pix, 0, NULL, NULL);
}

int main(int argc, char* argv[]) {

    // FRONT
    mdl[0] = 0.0f;
    mdl[1] = -0.6f;
    mdl[2] = 25.0f;

    mdl[3] = -0.5f;
    mdl[4] = 0.0f;
    mdl[5] = 0.0f;
    // Normal (0.0f, 1.0f, 0.0f)

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


	SDL_Surface* image = SDL_LoadBMP("background.bmp");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_Rect dstrect = { 5, 5, 320, 240 };

    // MAIN LOOP
    while (1)
    {
        auto start = high_resolution_clock::now(); // To compute frameRate

        // Catch exit
        eventCatch();


        // EXECUTE PROGRAM
        executeProgram();

        // PRINT TO SCREEN
		//printToScreen();
		SDL_RenderClear(renderer);
		SDL_UpdateTexture(screen_texture, NULL, cpu_pix, image_width * 4);
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);

        // CHANGE ANIMATION
        sphereSize += speed;
        if (sphereSize > 100 || sphereSize < -40) {
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

    free(mdl);
    return 0;
}