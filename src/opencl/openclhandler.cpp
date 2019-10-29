#include "openclhandler.h"

#define CL_TARGET_OPENCL_VERSION 120

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


OpenCLHandler::OpenCLHandler()
{
    context = nullptr;
    commandQueue = nullptr;
    cl_int errNum = cl::Platform::get(&platforms);
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
        errNum = printDeviceInfo(gpu_device);
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

OpenCLHandler::~OpenCLHandler() = default;

cl_int OpenCLHandler::printDeviceInfo(const cl::Device& device) {
    cl_int errNum;
    cl_ulong value;
    std::string vendor, name, version;

    errNum = device.getInfo(CL_DEVICE_VENDOR, &vendor);
    std::cout << "Device vendor: " << vendor << std::endl;

    errNum |= device.getInfo(CL_DEVICE_NAME, &name);
    std::cout << "Device Name: " << name << std::endl;

    errNum |= device.getInfo(CL_DEVICE_VERSION, &version);
    std::cout << "Supported OpenCL version: " << version << std::endl;

    errNum |= device.getInfo(CL_DRIVER_VERSION, &version);
    std::cout << "Driver version: " << version << std::endl;

    errNum |= device.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &value);
    std::cout << "Max comute units: " << (int)value << std::endl;

    errNum |= device.getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &value);
    std::cout << "Max work item dimensions: " << (int)value << std::endl;

    errNum |= device.getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &value);
    std::cout << "Max clock frequency: " << (int)value << " MHz" << std::endl;

    errNum |= device.getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &value);
    std::cout << "Max memory for mem.object allocation: " << (int)(value/1048576) << " MiByte" << std::endl;

    errNum |= device.getInfo(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, &value);
    std::cout << "Global memory cache size: " << (int)(value/1024) << " KiByte" << std::endl;

    errNum |= device.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &value);
    std::cout << "Global device memory size: " << (int)(value/1048576) << " MiByte" << std::endl;

    errNum |= device.getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &value);
    std::cout << "Local memory size: " << (int)(value/1024) << " KiByte" << std::endl;
    return errNum;
}

cl_int OpenCLHandler::interatePlatforms(){
    cl_int errNum = CL_SUCCESS;
    std::string vendor, name, version, profile;

    for(unsigned long i = 0; i < platforms.size(); i++){
        cl::Platform platform = platforms.at(i);
        std::cout << "Platform: " << i << std::endl;
        errNum |= platform.getInfo(CL_PLATFORM_VENDOR, &vendor);
        errNum |= platform.getInfo(CL_PLATFORM_NAME, &name);
        errNum |= platform.getInfo(CL_PLATFORM_VERSION, &version);
        errNum |= platform.getInfo(CL_PLATFORM_PROFILE, &profile);

        std::cout << "Platform vendor: " << vendor << std::endl
                  << "Platform name: " << name << std::endl
                  << "Supported OpenCL version: " << version << std::endl
                  << "Platform profile: " << profile << std::endl << std::endl;
    }
    return errNum;
}

cl_int OpenCLHandler::clGetGPUDevice() {
    cl_int errNum = CL_SUCCESS;
    std::cout << platforms.size() << " Platforms" << std::endl;
    for (const cl::Platform& platform : platforms) {
        std::vector<cl::Device> gpuDevices;
        printf("\n\nGetting GPU device IDs\n");
        errNum = platform.getDevices(CL_DEVICE_TYPE_GPU, &gpuDevices);
        if(gpuDevices.empty()) {
            continue;
        }
        printf("\n\nQuery 1st GPU Device\n");
        gpu_device = gpuDevices.at(0);
        printf("\n\nSelected Device:\n");
        printDeviceInfo(gpu_device);
    }
    return errNum;
}

cl_int OpenCLHandler::createContext(){
    cl_int errNum;
    context = cl::Context(gpu_device, nullptr, nullptr, nullptr, &errNum);
    if(errNum != CL_SUCCESS)
        std::cerr << "Failed to create context for platform";
    return errNum;
}

cl_int OpenCLHandler::createCommandQueue(){
    cl_int errNum;
    size_t deviceBufferSize;
    // Get the size of the device buffer
    errNum = context.getInfo(CL_CONTEXT_DEVICES, &deviceBufferSize);
    if(errNum != CL_SUCCESS){
        std::cerr << "Failed to call clGetContextInfo(context, CL_CONTEXT_DEVICES, ...)";
        return errNum;
    }
    if(deviceBufferSize < 1){
        std::cerr << "No devices available";
        return errNum;
    }
    // ToDo: Check if device is the same as selected in platform selection
    // Allocate memory for the device buffer
    std::vector<cl::Device> devices;
    errNum = context.getInfo(CL_CONTEXT_DEVICES, &devices);
    if(errNum != CL_SUCCESS){
        std::cerr << "Failed to get device IDs";
        return errNum;
    }
    commandQueue = cl::CommandQueue(context, gpu_device, 0, &errNum);
    if(errNum != CL_SUCCESS){
        std::cerr << "Failed to create commandQueue for first GPU device";
    }
    return errNum;
}

cl_int OpenCLHandler::compileKernelFromFile(const std::string& filename){
    cl_int errNum = CL_BUILD_ERROR;
    std::ifstream kernelFile(filename);
    if(!kernelFile.is_open()){
        std::cerr << "Failed to open source file: "
                  << filename << std::endl;
        return errNum;
    }
    std::string kernelString((std::istreambuf_iterator<char>(kernelFile)),
            std::istreambuf_iterator<char>());
    programs.emplace_back(context, kernelString, false, &errNum);
    if(errNum != CL_SUCCESS){
        std::cerr << "Failed to create program from source: "
                  << filename << std::endl;
        return errNum;
    }

    errNum = programs.back().build("-cl-no-signed-zeros");
    if(errNum != CL_SUCCESS){
        // Read compiler log for determning the error
        std::string buildLog;
        errNum = programs.back().getBuildInfo(gpu_device, CL_PROGRAM_BUILD_LOG, &buildLog);
        std::cerr << "Error in kernel " << filename << ":" << std::endl;
        std::cerr << buildLog;
        programs.pop_back();
        return errNum;
    }
    // Create kernel objects from program objects
    size_t kernelNameSize = filename.length() - 2;
    std::string kernelName = filename.substr(0, kernelNameSize - 1);
    kernels.emplace_back(programs.back(), kernelName.c_str(), &errNum);
    if(errNum != CL_SUCCESS){
        cl::Program currentProgram = programs.back();
        programs.pop_back();
        cl::Kernel failedKernel = kernels.back();
        kernels.pop_back();
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
        return errNum;
    }
    std::cout << filename << " successfully compiled" << std::endl;
    kernelArgErrNrs.push_back(CL_INVALID_KERNEL);
    return CL_SUCCESS;
}

void OpenCLHandler::setKernelArg(size_t kernel_index, cl_uint arg_index, size_t arg_size, const void *arg_value) {
    cl::Kernel krn = kernels.at(kernel_index);
    cl_int errNum = krn.setArg(arg_index, arg_size, arg_value);
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
}

void OpenCLHandler::setKernelArgWithMemObj(size_t kernel_index, cl_uint arg_index, size_t memObj_index) {
    cl::Kernel krn = kernels[kernel_index];
    cl::Memory memObj = memoryObjects.at(memObj_index);
    cl_int errNum = krn.setArg(arg_index, sizeof(cl_mem), &memObj);
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
}

bool OpenCLHandler::validateKernelArgs(size_t kernel_index){
    return kernelArgErrNrs.at(kernel_index) == CL_SUCCESS;
}
cl_int OpenCLHandler::enqueueKernel(size_t kernel_index,
                                    cl_uint work_dim,
                                    const size_t *global_work_size,
                                    const size_t *local_work_size) {
    cl_int errNum = CL_FALSE;
    cl::NDRange globalOffset;
    cl::NDRange globalWorkSize;
    cl::NDRange localWorkSize;
    switch (work_dim) {
        case 2:
            globalOffset = cl::NDRange(0, 0);
            globalWorkSize = cl::NDRange(global_work_size[0], global_work_size[1]);
            localWorkSize = cl::NDRange(local_work_size[0], local_work_size[1]);
            break;
        case 3:
            globalOffset = cl::NDRange(0, 0, 0);
            globalWorkSize = cl::NDRange(global_work_size[0], global_work_size[1], global_work_size[2]);
            localWorkSize = cl::NDRange(local_work_size[0], local_work_size[1], local_work_size[2]);
            break;
        case 1:
        default:
            globalOffset = cl::NDRange(0);
            globalWorkSize = cl::NDRange(global_work_size[0]);
            localWorkSize = cl::NDRange(local_work_size[0]);
            break;
    }
    if (kernel_index < kernels.size() && kernelArgErrNrs.at(kernel_index) == CL_SUCCESS) {
        errNum = commandQueue.enqueueNDRangeKernel(kernels[kernel_index], globalOffset, globalWorkSize, localWorkSize);
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

cl_int OpenCLHandler::create2DImageARGB(const size_t width, const size_t height, void *host_ptr) {
    cl_int errNum;
    const cl::ImageFormat imgFormat(CL_ARGB, CL_UNSIGNED_INT8);
    images.emplace_back(context, CL_MEM_WRITE_ONLY, imgFormat, width, height, 0, host_ptr, &errNum);
    return errNum;
}

cl_int OpenCLHandler::create2DImageA(size_t width, size_t height, void *host_ptr) {
    cl_int errNum;
    cl::ImageFormat imgFormat(CL_A, CL_FLOAT);
    images.emplace_back(context, CL_MEM_READ_WRITE, imgFormat, width, height, 0, host_ptr, &errNum);
    return errNum;
}

cl_int OpenCLHandler::createMemObj(size_t arg_size, void *arg_value, cl_mem_flags flags){
    cl_int errNum;
    memoryObjects.push_back(cl::Buffer(context, flags, arg_size, arg_value, &errNum));
    if(errNum != CL_SUCCESS) { // Release and delete Buffer object from list
        cl::Memory memObj = memoryObjects.back();
        memoryObjects.pop_back();
    }
    return errNum;
}

bool OpenCLHandler::overwriteMemObj(size_t index, size_t arg_size, void *arg_value, cl_mem_flags flags){
    if(index >= memoryObjects.size()) return false;
    cl_int errNum;
    // Create new memory object at the same index
    memoryObjects[index] = cl::Buffer(context, flags, arg_size, arg_value, &errNum);
    return errNum == CL_SUCCESS;
}

cl_int OpenCLHandler::getImageFromDevice(size_t idx, cl::size_t<3> region, void *dest) {
    const cl::size_t<3> origin;
    cl_int errNum = commandQueue.enqueueReadImage(images[idx], CL_TRUE, origin, region, 0, 0, dest, nullptr);
    if(errNum != CL_SUCCESS) {
        std::cerr << "Failed to read image from device" << std::endl;
    }
    return errNum;
}
