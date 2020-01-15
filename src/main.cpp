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

	/*for (int i = 0; i < 1; i++) {
		tmp = model.point(i);
		mdl[i].x = tmp.x / 50;
		mdl[i].y = tmp.y / 50;
		mdl[i].z = tmp.z / 50;
	}*/
	/*int face = 0;
	for (int i = 0; i < 1; i += 9) {
		auto p0 = model.point(model.vert(face, 0));
		auto p1 = model.point(model.vert(face, 1));
		auto p2 = model.point(model.vert(face, 2));

		mdl[i] = p0.x;
		mdl[i + 1] = p0.y;
		mdl[i + 2] = p0.z;

		mdl[i + 3] = p0.x;
		mdl[i + 4] = p0.y;
		mdl[i + 5] = p0.z;

		mdl[i + 6] = p0.x;
		mdl[i + 7] = p0.y;
		mdl[i + 8] = p0.z;

		face++;
	}*/

	mdl[0] = -0.5f;
	mdl[1] = 0.3f;
	mdl[2] = 2.0f;

	mdl[3] = 0.5f;
	mdl[4] = 0.3f;
	mdl[5] = 2.0f;

	mdl[6] = -0.5f;
	mdl[7] = -0.2f;
	mdl[8] = 2.0f;

	mdl[9] = 0.5f;
	mdl[10] = 0.3f;
	mdl[11] = 2.0f;
		
	mdl[12] = 0.5f;
	mdl[13] = -0.2f;
	mdl[14] = 2.0f;
		
	mdl[15] = -0.5f;
	mdl[16] = -0.2f;
	mdl[17] = 2.0f;

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
