/* Header of my little OpenCL library.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

class OpenCLHandler
{
private:
    cl_uint numPlatforms;
    cl_platform_id *platforms;
    cl_uint usedPlatform;
    cl_device_id GPUDevice;
    cl_context context;
    cl_command_queue commandQueue;
    std::vector<cl_program> programs;
    std::vector<cl_kernel> kernels;
    // Error numbers set by paasing the arguments to the according kernel
    std::vector<cl_int> kernelArgErrNrs;
    std::vector<cl_mem> memoryObjects;

    cl_int interatePlatforms();
    cl_int clGetPlatforms();
    cl_int clGetGPUDevice();
    cl_int createContext();
    cl_int createCommandQueue();

public:
    OpenCLHandler() throw();
    ~OpenCLHandler();

    // Prints information about the GPU device
    cl_int printDeviceInfo();
    /* Compiles the kernel in the file specified by filename,
     * creates a program object and a kernel object.
     * The filename msut have the extension .cl and has to
     * be in the directory of the executable.
     */
    cl_int compileKernelFromFile(const char *filename);
    /* Set the argument (beginning from 0) of the kernel with the
     * index kernel_index (beginning from 0) with arg_value.
     */
    void setKernelArg(size_t kernel_index, cl_uint arg_index,
                      size_t arg_size, const void *arg_value);
    // Set Kernel argument with an existing mempry object
    void setKernelArgWithMemObj(size_t kernel_index, cl_uint arg_index,
                                size_t memObj_index);
    // Returns true if all Arguments for the kernel were set properly
    //  (CL_SUCCESS for each)
    bool validateKernelArgs(size_t kernel_index);
    cl_int enqueueKernel(size_t kernel_index, cl_uint work_dim,
                         const size_t *global_work_size,
                         const size_t *local_work_size);
    size_t getNumKernels();
    // Store images (write-only) in RGBA format with an value from 0 to 255.
    cl_int create2DImageARGB(size_t width, size_t height, void *host_ptr);
    // Store images (write-only) in A format with an single precision value.
    cl_int create2DImageA(size_t width, size_t height, void *host_ptr);

    cl_int createMemObj(size_t arg_size, void *arg_value, cl_mem_flags flags);
    bool overwriteMemObj(size_t index, size_t arg_size, void *arg_value,
                         cl_mem_flags flags);
    cl_mem getMemObj(size_t idx);
    cl_int getImageFromDevice(size_t idx, size_t region[3], void *dest);
};

#endif // OPENCLHANDLER_H
