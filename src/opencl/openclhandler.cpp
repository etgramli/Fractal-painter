#include "openclhandler.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <CL/cl.h>

OpenCLHandler::OpenCLHandler()
{
    cl_int errNum;
    context = NULL;
    commandQueue = NULL;
    errNum = clGetPlatforms();
    std::cout << "Finished: Getting platforms" << std::endl;
    if(errNum == CL_SUCCESS) {
        errNum = interatePlatforms();
        std::cout << "Finished: Iterating platforms" << std::endl;
    }
    if(errNum == CL_SUCCESS) {
        errNum = clGetGPUDevice();
        std::cout << "Finished: Getting GPU device" << std::endl;
    }
    if(errNum == CL_SUCCESS) {
        errNum = printDeviceInfo(GPUDevice);
        std::cout << "Finished: printing device info" << std::endl;
    }
    if(errNum == CL_SUCCESS) {
        errNum = createContext();
        std::cout << "Finished: Getting context" << std::endl;
    }
    if(errNum == CL_SUCCESS) {
        errNum = createCommandQueue();
        std::cout << "Finished: Getting command queue" << std::endl;
    }
    if(errNum != CL_SUCCESS) {
        throw "Failed to create object of OpenCLHandler!";
    }
}

OpenCLHandler::~OpenCLHandler() {
    // Release OpenCL-Memory-Objects
    for(size_t l = 0; l < memoryObjects.size(); l++) {
        clReleaseMemObject(memoryObjects.at(l));
    }
    memoryObjects.clear();
    // Release OpenCL-Kernel-Objects
    for(size_t l = 0; l < kernels.size(); l++) {
        clReleaseKernel(kernels.at(l));
    }
    kernels.clear();
    // Release OpenCL-Program-Objects
    for(size_t l = 0; l < programs.size(); l++) {
        clReleaseProgram(programs.at(l));
    }
    programs.clear();
    clReleaseCommandQueue(commandQueue);
    clReleaseContext(context);
    delete platforms;
}

cl_int OpenCLHandler::printDeviceInfo(cl_device_id device) {
    cl_int errNum;
    size_t size;
    cl_ulong value;
    char *str;

    errNum = clGetDeviceInfo(device, CL_DEVICE_VENDOR,
                    0, NULL, &size);
    str = (char *) malloc(sizeof(char) * size);
    errNum = clGetDeviceInfo(device, CL_DEVICE_VENDOR,
                    size, str, NULL);
    std::cout << "Device vendor: " << str << std::endl;
    free(str);

    errNum = clGetDeviceInfo(device, CL_DEVICE_NAME,
                    0, NULL, &size);
    errNum = clGetDeviceInfo(device, CL_DEVICE_NAME,
                    size, str, NULL);
    std::cout << "Device Name: " << str << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_VERSION,
                    0, NULL, &size);
    errNum = clGetDeviceInfo(device, CL_DEVICE_VERSION,
                    size, str, NULL);
    std::cout << "Supported OpenCL version: " << str << std::endl;

    errNum = clGetDeviceInfo(device, CL_DRIVER_VERSION,
                    0, NULL, &size);
    errNum = clGetDeviceInfo(device, CL_DRIVER_VERSION,
                    size, str, NULL);
    std::cout << "Driver version: " << str << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS,
                    sizeof(cl_uint), &value, NULL);
    std::cout << "Max comute units: " << (int)value << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                    sizeof(cl_uint), &value, NULL);
    std::cout << "Max work item dimensions: " << (int)value << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY,
                    sizeof(cl_uint), &value, NULL);
    std::cout << "Max clock frequency: " << (int)value << " MHz" << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                    sizeof(cl_ulong), &value, NULL);
    std::cout << "Max memory for mem.object allocation: " << (int)(value/1048576) << " MiByte" << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                    sizeof(cl_ulong), &value, NULL);
    std::cout << "Global memory cache size: " << (int)(value/1024) << " KiByte" << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE,
                    sizeof(cl_ulong), &value, NULL);
    std::cout << "Global device memory size: " << (int)(value/1048576) << " MiByte" << std::endl;

    errNum = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE,
                    sizeof(cl_ulong), &value, NULL);
    std::cout << "Local memory size: " << (int)(value/1024) << " KiByte" << std::endl;
    return errNum;
}

cl_int OpenCLHandler::interatePlatforms(){
    cl_int errNum = CL_SUCCESS;
    size_t size;
    char *vendor;
    char *name;
    char *version;
    char *profile;
    for(cl_uint i = 0; i < numPlatforms; i++){
        std::cout << "Platform: " << i << std::endl;
        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 0, NULL, &size);
        vendor = (char *) malloc(size * sizeof(char));
        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, size, vendor, NULL);

        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, NULL, &size);
        name = (char *) malloc(size * sizeof(char));
        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, size, name, NULL);

        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 0, NULL, &size);
        version = (char *) malloc(size * sizeof(char));
        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, size, version, NULL);

        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 0, NULL, &size);
        profile = (char *) malloc(size * sizeof(char));
        errNum = clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, size, profile, NULL);

        std::cout << "Platform vendor: " << vendor << std::endl
                  << "Platform name: " << name << std::endl
                  << "Supported OpenCL version: " << version << std::endl
                  << "Platform profile: " << profile << std::endl << std::endl;
        free(vendor);
        free(name);
        free(version);
        free(profile);
    }
    return errNum;
}

cl_int OpenCLHandler::clGetPlatforms() {
    cl_int errNum;
    // Determine number of platforms
    if((errNum = clGetPlatformIDs(0, NULL, &numPlatforms)) != CL_SUCCESS) {
        std::cerr << "No Platform found\n";
    } else {
        printf("%d Platform(s) found\n\n", numPlatforms);
        // Get List of platforms
        platforms = (cl_platform_id *)  malloc(sizeof(cl_platform_id) * numPlatforms);
        errNum = clGetPlatformIDs(numPlatforms, platforms, NULL);
    }
    return errNum;
}

cl_int OpenCLHandler::clGetGPUDevice() {
    GPUDevice = NULL;
    cl_int errNum = CL_SUCCESS;
    cl_uint numDevices;
    std::cout << numPlatforms << " Platforms" << std::endl;
    for(cl_uint l = 0; l < numPlatforms; l++){
        numDevices = 0;
        printf("\n\nGetting GPU device IDs\n");
        errNum = clGetDeviceIDs(platforms[l], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
        if(numDevices < 1) {
            continue;
        }
        printf("\n\nQuery 1st GPU Device\n");
        errNum = clGetDeviceIDs(platforms[l],
                                CL_DEVICE_TYPE_GPU,
                                1, &GPUDevice, NULL);
        printf("\n\nSelected Device:\n");
        printTmpDeviceInfo(GPUDevice);

        if (errNum == CL_SUCCESS) {
            usedPlatform = l;
            return errNum;
        }
    }
    if (GPUDevice == NULL) {
        std::cerr << "No GPU found\n";
    }
    return errNum;
}

cl_int OpenCLHandler::createContext(){
    cl_int errNum;
    cl_context_properties contextProperties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platforms[usedPlatform],
        0
    };
    context = clCreateContextFromType(contextProperties,
                                      CL_DEVICE_TYPE_GPU,
                                      NULL, NULL, &errNum);
    if(errNum != CL_SUCCESS)
        std::cerr << "Failed to create context for platform";
    return errNum;
}

cl_int OpenCLHandler::createCommandQueue(){
    cl_int errNum;
    cl_device_id *devices;
    size_t deviceBufferSize = -1;
    // Get the size of the device buffer
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
    if(errNum != CL_SUCCESS){
        std::cerr << "Failed to call clGetContextInfo(context, CL_CONTEXT_DEVICES, ...)";
        return errNum;
    }
    if(deviceBufferSize <= 0){
        std::cerr << "No devices available";
        return errNum;
    }
    // Allocate memory for the device buffer
    devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES,
                              deviceBufferSize, devices, NULL);
    if(errNum != CL_SUCCESS){
        std::cerr << "Failed to get device IDs";
        return errNum;
    }
    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
    if(commandQueue == NULL){
        std::cerr << "Failed to create commandQueue for first GPU device";
        return errNum;
    }
    GPUDevice = devices[0];
    delete []devices;
    return errNum;
}

cl_int OpenCLHandler::compileKernelFromFile(const char *filename){
    cl_int errNum = CL_BUILD_ERROR;
    std::ifstream kernelFile(filename, std::ios::in);
    if(!kernelFile.is_open()){
        std::cerr << "Failed to open source file: "
                  << filename << std::endl;
        return errNum;
    }
    std::ostringstream oss;
    oss << kernelFile.rdbuf();
    std::string srcStdStr = oss.str();
    const char *srcStr = srcStdStr.c_str();
    programs.push_back(clCreateProgramWithSource(context,
                                 1, (const char**) &srcStr,
                                 NULL, NULL));
    if(programs.at(programs.size()-1) == NULL ){
        std::cerr << "Failed to create program from source: "
                  << filename << std::endl;
        return CL_BUILD_ERROR;
    }
    errNum = clBuildProgram(programs.at(programs.size()-1),
                            1, &GPUDevice, "-cl-no-signed-zeros",
                            NULL, NULL);
    if(errNum != CL_SUCCESS){
        // Read compiler log for determning the error
        char buildLog[16384];
        clGetProgramBuildInfo(programs.at(programs.size()-1),
                              GPUDevice, CL_PROGRAM_BUILD_LOG,
                              sizeof(buildLog), buildLog, NULL);
        std::cerr << "Error in kernel " << filename << ":" << std::endl;
        std::cerr << buildLog;
        cl_program currentProgram = programs.back();
        programs.pop_back();
        clReleaseProgram(currentProgram);
        return errNum;
    }
    // Create kernel objects from program objects
    size_t kernelNameSize = strlen(filename) - 2;
    char *kernelName = (char*) malloc(sizeof(char) * kernelNameSize);
    strncpy(kernelName, filename, kernelNameSize - 1);
    kernelName[kernelNameSize-1]='\0';
    kernels.push_back( clCreateKernel(programs.back(), kernelName, &errNum));
    if(errNum != CL_SUCCESS){
        cl_program currentProgram = programs.back();
        programs.pop_back();
        clReleaseProgram(currentProgram);
        cl_kernel failedKernel = kernels.back();
        kernels.pop_back();
        clReleaseKernel(failedKernel);
        std::cerr << "Failed to create kernel: ";
        switch (errNum) {
        case CL_INVALID_PROGRAM_EXECUTABLE:
            std::cerr << "Invalid program executable"  << std::endl;
            break;
        case CL_INVALID_KERNEL_NAME:
            std::cerr << "Invalid kernel name: "  << kernelName << std::endl;
            break;
        case CL_INVALID_KERNEL_DEFINITION:
            std::cerr << "Invalid kernel definition"  << std::endl;
            break;
        default:
            break;
        }
        free(kernelName);
        return errNum;
    }
    free(kernelName);
    std::cout << filename << " successfully compiled" << std::endl;
    kernelArgErrNrs.push_back(CL_INVALID_KERNEL);
    return CL_SUCCESS;
}

size_t OpenCLHandler::getNumKernels(){
    return kernels.size();
}

void OpenCLHandler::setKernelArg(size_t kernel_index, cl_uint arg_index,
                                 size_t arg_size, const void *arg_value)
{
    cl_int errNum = CL_SUCCESS;
    cl_kernel krn = kernels.at(kernel_index);
    errNum = clSetKernelArg(krn, arg_index, arg_size, arg_value);
    switch (errNum) {
    case CL_SUCCESS:
        break;
    case CL_INVALID_KERNEL:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Kernel" << std::endl;
        break;
    case CL_INVALID_HOST_PTR:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Host Pointer" << std::endl;
        break;
    case CL_INVALID_MEM_OBJECT:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Memory Object" << std::endl;
        break;
    case CL_INVALID_ARG_INDEX:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Argument Index" << std::endl;
        break;
    case CL_INVALID_ARG_SIZE:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Argument Size" << std::endl;
        break;
    case CL_INVALID_ARG_VALUE:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Argument Value" << std::endl;
        break;
    case CL_INVALID_KERNEL_ARGS:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Kernel Args" << std::endl;
        break;
    default:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: " << kernelArgErrNrs[kernel_index] << std::endl;
        break;
    }
    kernelArgErrNrs[kernel_index] = errNum;
    return;
}

void OpenCLHandler::setKernelArgWithMemObj(size_t kernel_index,
                                           cl_uint arg_index,
                                           size_t memObj_index)
{
    cl_int errNum = CL_SUCCESS;
    cl_kernel krn = kernels[kernel_index];
    cl_mem memObj = memoryObjects.at(memObj_index);
    errNum = clSetKernelArg(krn, arg_index, sizeof(cl_mem), &memObj);
    switch (errNum) {
    case CL_SUCCESS:
        break;
    case CL_INVALID_KERNEL:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Kernel" << std::endl;
        break;
    case CL_INVALID_HOST_PTR:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Host Pointer" << std::endl;
        break;
    case CL_INVALID_MEM_OBJECT:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Memory Object" << std::endl;
        break;
    case CL_INVALID_ARG_INDEX:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Argument Index" << std::endl;
        break;
    case CL_INVALID_ARG_SIZE:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Argument Size" << std::endl;
        break;
    case CL_INVALID_ARG_VALUE:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Argument Value" << std::endl;
        break;
    case CL_INVALID_KERNEL_ARGS:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: Invalid Kernel Args" << std::endl;
        break;
    default:
        std::cerr << "Error with Kernel: " << kernel_index << "; Argument: " << arg_index << "; ErrNum: " << kernelArgErrNrs[kernel_index] << std::endl;
        break;
    }
    kernelArgErrNrs[kernel_index] = errNum;
    return;
}

bool OpenCLHandler::validateKernelArgs(size_t kernel_index){
    return (kernelArgErrNrs.at(kernel_index) == CL_SUCCESS) ? true : false;
}
cl_int OpenCLHandler::enqueueKernel(size_t kernel_index, cl_uint work_dim,
                                    const size_t *global_work_size,
                                    const size_t *local_work_size){
    cl_int errNum = CL_FALSE;
    if(kernel_index < kernels.size() && kernelArgErrNrs.at(kernel_index)==CL_SUCCESS){
        errNum = clEnqueueNDRangeKernel(commandQueue, kernels[kernel_index],
                                        work_dim, NULL, global_work_size,
                                        local_work_size, 0, NULL, NULL);
        if(errNum != CL_SUCCESS) {
            std::cerr << "Error enqueueing kernel: " << kernel_index << std::endl;
            switch (errNum) {
            case CL_INVALID_KERNEL_ARGS:
                std::cerr << "Invalid kernel argument(s)" << std::endl;
                break;
            case CL_INVALID_WORK_DIMENSION:
                std::cerr << "Invalid work dimension" << std::endl;
                break;
            case CL_INVALID_WORK_GROUP_SIZE:
                std::cerr << "Invalid work group size" << std::endl;
                break;
            case CL_INVALID_WORK_ITEM_SIZE:
                std::cerr << "Invalid work item size" << std::endl;
                break;
            case CL_INVALID_OPERATION:
                std::cerr << "Cause an invalid opeation" << std::endl;
                break;
            default:
                std::cerr << "Error Number: " << errNum << std::endl;
                break;
            }
        }
    }
    //std::cout << "Enqueued kernel " << kernel_index << std::endl;
    return errNum;
}

cl_int OpenCLHandler::create2DImageARGB(size_t width, size_t height,
                                        void *host_ptr)
{
    cl_int errNum;
    cl_image_format imgFormat;
    imgFormat.image_channel_order = CL_ARGB;
    imgFormat.image_channel_data_type = CL_UNSIGNED_INT8;
    memoryObjects.push_back( clCreateImage2D(context,
                                             CL_MEM_WRITE_ONLY, &imgFormat,
                                             width, height, 0,
                                             host_ptr, &errNum) );
    return errNum;
}

cl_int OpenCLHandler::create2DImageA(size_t width, size_t height,
                                     void *host_ptr)
{
    cl_int errNum;
    cl_image_format imgFormat;
    imgFormat.image_channel_order = CL_A;
    imgFormat.image_channel_data_type = CL_FLOAT;
    memoryObjects.push_back( clCreateImage2D(context,
                                             CL_MEM_READ_WRITE, &imgFormat,
                                             width, height, 0,
                                             host_ptr, &errNum) );
    return errNum;
}

cl_int OpenCLHandler::createMemObj(size_t arg_size, void *arg_value,
                                   cl_mem_flags flags){
    cl_int errNum;
    memoryObjects.push_back( clCreateBuffer(context, flags, arg_size,
                                            arg_value, &errNum) );
    if(errNum != CL_SUCCESS){ // Release and delete Buffer object from list
        cl_mem memObj = memoryObjects.back();
        memoryObjects.pop_back();
        clReleaseMemObject(memObj);
    }
    return errNum;
}

bool OpenCLHandler::overwriteMemObj(size_t index, size_t arg_size,
                                    void *arg_value, cl_mem_flags flags){
    if(index >= memoryObjects.size())
        return false;
    cl_int errNum;
    // Release old memory object
    clReleaseMemObject(memoryObjects[index]);
    // Create new memory object at the same index
    memoryObjects[index] = clCreateBuffer(context, flags, arg_size, arg_value, &errNum);
    if(errNum != CL_SUCCESS) {
        return false;
    } else {
        return true;
    }
}

cl_mem OpenCLHandler::getMemObj(size_t idx){
    return memoryObjects.at(idx);
}

cl_int OpenCLHandler::getImageFromDevice(size_t idx, size_t region[3],
                                         void *dest){
    cl_int errNum;
    size_t origin[3] = {0, 0, 0};
    errNum = clEnqueueReadImage(commandQueue, memoryObjects[idx], CL_TRUE,
                                origin, region, 0, 0, dest,
                                0, NULL, NULL);
    if(errNum != CL_SUCCESS) {
        std::cerr << "Failed to read image from device" << std::endl;
    }
    return errNum;
}
