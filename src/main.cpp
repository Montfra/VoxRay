#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "core/Core.h"
#include "game_interfaces/hud.h"

// For visualStudio
#pragma warning(disable : 4996)

using namespace std::chrono;
using namespace std;

// CONSTANT
const string programPath = "opencl_kernel.cl";

float sphereSize = 100; // sphere size
float speed = -0.5; // speed of animation
int score = 0;

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

	SDL_SetRenderDrawColor(renderer, 44, 62, 80, 255);

	hud hud(renderer);
	Menu menu(renderer);

	int alea1 = rand() % 3;
	int alea2 = rand() % 3;
	int alea3 = rand() % 3;
	int alea4 = rand() % 3;
	int alea5 = rand() % 3;
	int alea6 = rand() % 3;

	int random = 0;
	int correct1 = 18;
	int correct2 = 36;

	float middle = 0.0f;
	float left = -4.0f;
	float right = 4.0f;

	if (alea1 == 0)
	{
		middle = 0.0f;
		left = -4.0f;
		right = 4.0f;
	}
	else if (alea1 == 1)
	{
		middle = -4.0f;
		left = 0.0f;
		right = 4.0f;
	}
	else {
		middle = 4.0f;
		left = -4.0f;
		right = 0.0f;
	}

    // MAIN LOOP
    while (1)
    {
		// MIDDLE
		mdl[random] = middle;
		mdl[random + 1] = -0.5f;
		mdl[random + 2] = sphereSize * 20.0f;
		if (alea1 == 0)
		{
			mdl[random + 2] = -10000000.0f;
		}

		mdl[random + 3] = middle + 1.0f;
		mdl[random + 4] = 0.0f;
		mdl[random + 5] = sphereSize * 20;
		if (alea2 == 0)
		{
			mdl[random + 5] = -10000000.0f;
		}

		mdl[random + 6] = middle - 1.0f;
		mdl[random + 7] = 0.0f;
		mdl[random + 8] = sphereSize * 20;
		if (alea3 == 0)
		{
			mdl[random + 8] = -10000000.0f;
		}

		mdl[random + 9] = middle;
		mdl[random + 10] = 0.0f;
		mdl[random + 11] = sphereSize * 20;
		if (alea4 == 0)
		{
			mdl[random + 11] = -10000000.0f;
		}

		mdl[random + 12] = middle + 1.0f;
		mdl[random + 13] = -0.5f;
		mdl[random + 14] = sphereSize * 20;
		if (alea5 == 0)
		{
			mdl[random + 14] = -10000000.0f;
		}

		mdl[random + 15] = middle - 1.0f;
		mdl[random + 16] = -0.5f;
		mdl[random + 17] = sphereSize * 20;
		if (alea6== 0)
		{
			mdl[random + 17] = -10000000.0f;
		}

		// RIGHT
		mdl[correct1] = right;
		mdl[correct1 + 1] = -0.5f;
		mdl[correct1 + 2] = sphereSize * 20.0f;
		if (alea1 == 1)
		{
			mdl[correct1 + 2] = -10000000.0f;
		}

		mdl[correct1 + 3] = right + 1.0f;
		mdl[correct1 + 4] = 0.0f;
		mdl[correct1 + 5] = sphereSize * 20;
		if (alea2 == 1)
		{
			mdl[correct1 + 5] = -10000000.0f;
		}

		mdl[correct1 + 6] = right - 1.0f;
		mdl[correct1 + 7] = 0.0f;
		mdl[correct1 + 8] = sphereSize * 20;
		if (alea3 == 1)
		{
			mdl[correct1 + 8] = -10000000.0f;
		}

		mdl[correct1 + 9] = right;
		mdl[correct1 + 10] = 0.0f;
		mdl[correct1 + 11] = sphereSize * 20;
		if (alea4 == 1)
		{
			mdl[correct1 + 11] = -10000000.0f;
		}

		mdl[correct1 + 12] = right + 1.0f;
		mdl[correct1 + 13] = -0.5f;
		mdl[correct1 + 14] = sphereSize * 20;
		if (alea5 == 1)
		{
			mdl[correct1 + 14] = -10000000.0f;
		}

		mdl[correct1 + 15] = right - 1.0f;
		mdl[correct1 + 16] = -0.5f;
		mdl[correct1 + 17] = sphereSize * 20;
		if (alea6 == 1)
		{
			mdl[correct1 + 17] = -10000000.0f;
		}

		// LEFT
		mdl[correct2] = left;
		mdl[correct2 + 1] = -0.5f;
		mdl[correct2 + 2] = sphereSize * 20.0f;
		if (alea1 != 1)
		{
			mdl[correct2 + 2] = -10000000.0f;
		}

		mdl[correct2 + 3] = left + 1.0f;
		mdl[correct2 + 4] = 0.0f;
		mdl[correct2 + 5] = sphereSize * 20;
		if (alea2 != 1)
		{
			mdl[correct2 + 5] = -10000000.0f;
		}

		mdl[correct2 + 6] = left - 1.0f;
		mdl[correct2 + 7] = 0.0f;
		mdl[correct2 + 8] = sphereSize * 20;
		if (alea3 != 1)
		{
			mdl[correct2 + 8] = -10000000.0f;
		}

		mdl[correct2 + 9] = left;
		mdl[correct2 + 10] = 0.0f;
		mdl[correct2 + 11] = sphereSize * 20;
		if (alea4 != 1)
		{
			mdl[correct2 + 11] = -10000000.0f;
		}

		mdl[correct2 + 12] = left + 1.0f;
		mdl[correct2 + 13] = -0.5f;
		mdl[correct2 + 14] = sphereSize * 20;
		if (alea5 != 1)
		{
			mdl[correct2 + 14] = -10000000.0f;
		}

		mdl[correct2 + 15] = left - 1.0f;
		mdl[correct2 + 16] = -0.5f;
		mdl[correct2 + 17] = sphereSize * 20;
		if (alea6 != 1)
		{
			mdl[correct2 + 17] = -10000000.0f;
		}

		
		// Normal (0.0f, 1.0f, 0.0f)
        auto start = high_resolution_clock::now(); // To compute frameRate

        // Catch exit
        eventCatch(menu, hud, middle, left, right, score);

        // EXECUTE PROGRAM
        executeProgram();

        // PRINT TO SCREEN
		//printToScreen();
		SDL_RenderClear(renderer);

		if (menu.getActive())
		{
			SDL_RenderFillRect(renderer, NULL);
			menu.print(renderer);
		}
		else
		{
			SDL_UpdateTexture(screen_texture, NULL, cpu_pix, image_width * 4);
			SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

			hud.print(renderer);



			sphereSize += speed;

		}

		SDL_RenderPresent(renderer);

        // CHANGE ANIMATION
        
		 // sphereSize = 50.0f;
		if (sphereSize > 100 || sphereSize < -1) {
			speed -= 0.05;
			sphereSize = 100;
			alea1 = rand() % 3;
			alea2 = rand() % 3;
			alea3 = rand() % 3;
			alea4 = rand() % 3;
			alea5 = rand() % 3;
			alea6 = rand() % 3;

			if (alea1 == 0)
			{
				middle = 0.0f;
				left = -4.0f;
				right = 4.0f;
			}
			else if (alea1 == 1)
			{
				middle = -4.0f;
				left = 0.0f;
				right = 4.0f;
			}
			else {
				middle = 4.0f;
				left = -4.0f;
				right = 0.0f;
			}
		}					
        // Calea6 = rand() % 20;OMPUTE FRAMERATE
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