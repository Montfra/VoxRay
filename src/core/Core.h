#ifndef RAY_CORE_H
#define RAY_CORE_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <SDL.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include "../game_interfaces/Menu.h"
#include "../game_interfaces/hud.h"

// CONSTANT
const int image_width = 1280;
const int image_height = 720;

// PROGRAM
cl_mem pix;
cl_mem model;
unsigned int* cpu_pix;
int err; // error catch
size_t local;
size_t global = image_width * image_height;

// OPENCL
cl_device_id device_id;
cl_context context;
cl_command_queue commands;
cl_program program;
cl_kernel kernel;

// SDL2
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* screen_texture;
SDL_Event event;



//**  OPENCL  **//
void selectDevice(cl_device_id* device_id) {
    // Passing platform=NULL to clGetDeviceIDs is only "implementation-defined"
    // per the OpenCL spec: it happens to work when a single ICD (vendor
    // OpenCL driver) is registered on the system (e.g. an NVIDIA-only Linux
    // box), but once more than one is registered -- e.g. a Windows PC with
    // both an Intel CPU/iGPU OpenCL runtime and an NVIDIA GPU driver -- the
    // ICD loader has to arbitrarily pick one or fail, which can hand back an
    // invalid/mismatched device handle. That's what produced the garbled
    // "Device <garbage> supports <garbage>" output: clGetDeviceInfo() failed
    // silently on that bad handle and printf'd an uninitialized buffer.
    // Fix: always go through clGetPlatformIDs() and query each platform
    // explicitly instead of relying on NULL.
    cl_uint num_platforms = 0;
    clGetPlatformIDs(0, NULL, &num_platforms);
    if (num_platforms == 0) {
        std::cerr << "No OpenCL platform found. Exiting..." << std::endl;
        exit(1);
    }

    std::vector<cl_platform_id> platforms(num_platforms);
    clGetPlatformIDs(num_platforms, platforms.data(), NULL);

    std::vector<cl_device_id> gpuDevices;
    std::vector<cl_device_id> cpuDevices;

    char buf[128];
    for (cl_uint p = 0; p < num_platforms; p++) {
        cl_uint num_devices = 0;
        clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
        if (num_devices == 0) {
            continue;
        }

        std::vector<cl_device_id> devices(num_devices);
        clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_ALL, num_devices, devices.data(), NULL);

        for (cl_uint i = 0; i < num_devices; i++) {
            buf[0] = '\0';
            clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buf), buf, NULL);
            fprintf(stdout, "Device %s supports ", buf);

            buf[0] = '\0';
            clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buf), buf, NULL);
            fprintf(stdout, "%s\n", buf);

            cl_device_type type = 0;
            clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(type), &type, NULL);
            if (type & CL_DEVICE_TYPE_GPU) {
                gpuDevices.push_back(devices[i]);
            } else if (type & CL_DEVICE_TYPE_CPU) {
                cpuDevices.push_back(devices[i]);
            }
        }
    }

    // Prefer a GPU device. Fall back to a CPU OpenCL device (e.g. a VM
    // without GPU passthrough, or a machine with no dedicated GPU) so the
    // game still runs -- just slower -- instead of silently handing
    // clCreateContext an uninitialized device_id.
    if (!gpuDevices.empty()) {
        *device_id = gpuDevices.front();
    } else if (!cpuDevices.empty()) {
        std::cout << "No GPU OpenCL device found, falling back to CPU (this will be slow)." << std::endl;
        *device_id = cpuDevices.front();
    } else {
        std::cerr << "No OpenCL device (GPU or CPU) found. Exiting..." << std::endl;
        exit(1);
    }
}

void initOpenCL(const char *KernelSource) {
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);

    if (clBuildProgram(program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS) {
        size_t logSize;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
        std::vector<char> buildLog(logSize + 1);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, logSize, buildLog.data(), NULL);
        buildLog[logSize] = '\0';
        std::cerr << "\nOpenCL kernel build failed:\n" << buildLog.data() << std::endl;
        exit(1);
    }

    kernel = clCreateKernel(program, "render_kernel", &err);
}

void initStaticVariable() {
    pix = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(CL_UNORM_INT8) * image_width * image_height, NULL, NULL);
    model = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(CL_UNORM_INT8) * image_width * image_height, NULL, NULL);

    cpu_pix = (unsigned int*)malloc(image_width * image_height * 4);

    // SET STATIC CONTEXT
    clSetKernelArg(kernel, 0, sizeof(int), &image_width);
    clSetKernelArg(kernel, 1, sizeof(int), &image_height);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &pix);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &model);

    clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
}




//**  SDL2  **//
void initSDL2() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height,SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_PRESENTVSYNC);

    SDL_SetWindowMinimumSize(window, image_width, image_height);

    SDL_RenderSetLogicalSize(renderer, image_width, image_height);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    screen_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);
}

void printToScreen() {
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(screen_texture, NULL, cpu_pix, image_width * 4);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}



//**  GENERAL  **//
void initLibs(const char * KernelSource) {
    // SELECT DEVICE
    selectDevice(&device_id);

    // INNIT OPENCL
    initOpenCL(KernelSource);

    // INIT SDL2
    initSDL2();


    // INIT VARIABLES
    initStaticVariable();
}

void cleanAll() {
    free(cpu_pix);
    clReleaseMemObject(pix);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
}

void eventCatch(Menu& menu, hud& hud, float& middle, float& left, float& right, int& score) {
	bool a = false;
	bool z = false;
	bool e = false;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            cleanAll();
            exit(0);
        }
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			menu.setInactive();
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_a) {
				a = true;
				hud.setA();
			}
			if (event.key.keysym.sym == SDLK_z) {
				z = true;
				hud.setZ();
			}
			if (event.key.keysym.sym == SDLK_e) {
				e = true;
				hud.setE();
			}
		}
		else if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_a) {
				hud.dsetA();
			}
			if (event.key.keysym.sym == SDLK_z) {
				hud.dsetZ();
			}
			if (event.key.keysym.sym == SDLK_e) {
				hud.dsetE();
			}
		}
    }

	if ( middle == 0.0f && a && e && !z) {
		score++;
	}
	if ( left == 0.0f && z && e && !a) {
		score++;
	}
	if ( right == 0.0f && a && z && !e) {
		score++;
	}
	hud.setScore(score);
}
#endif //RAY_CORE_H