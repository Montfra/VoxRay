//
// Created by Valentin MUNIER on 12/01/2020.
//

#ifndef RAY_CORE_H
#define RAY_CORE_H

#include <CL/opencl.h>
#include <SDL.h>
#include <iostream>
#include "Menu.h"

// CONSTANT
const int image_width = 1280;
const int image_height = 720;
const int deviceChoice = 1;



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
    // Init var
    cl_uint num_devices, i;

    // get number devices
    clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);

    // Get all devices
    cl_device_id* devices = static_cast<cl_device_id *>(calloc(sizeof(cl_device_id), num_devices));
    clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);

    char buf[128];
    for (i = 0; i < num_devices; i++) {
        if (i == deviceChoice) {std::cout << "Your choice this device -> ";}
        clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 128, buf, NULL);
        fprintf(stdout, "Device %s supports ", buf);

        clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, 128, buf, NULL);
        fprintf(stdout, "%s\n", buf);
    }

    clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, deviceChoice, device_id, NULL);

    free(devices);
}

void initOpenCL(const char *KernelSource) {
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
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

void eventCatch(Menu& menu) {
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
    }
}
#endif //RAY_CORE_H