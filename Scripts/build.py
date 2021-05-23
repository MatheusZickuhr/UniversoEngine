import sys
import os
import subprocess
import requests
import zipfile

REQUIRED_VULKAN_VERSION = "1.2.170.0"


def download_file(url, file_path, show_progress=True):
    file = open(file_path, 'wb')

    response = requests.get(url, stream=True)

    total_length = int(response.headers.get('content-length'))
    total_downlowded = 0
    for chunk in response.iter_content(chunk_size=1024):
        file.write(chunk)
        file.flush()
        
        if show_progress:
            total_downlowded += len(chunk)
            print(f"downloaded {total_downlowded // 1024 // 1024}MB from {total_length // 1024 // 1024}MB", end='\r')

    file.close()


def check_vulkan_sdk():
    # check vulkan sdk
    vulkan_sdk_path = os.environ.get('VULKAN_SDK')

    if vulkan_sdk_path is None:
        print("cannot find vulkan sdk, download the sdk with the required version" 
            f" ({REQUIRED_VULKAN_VERSION}) at https://www.lunarg.com/vulkan-sdk/")
        return False
    elif not REQUIRED_VULKAN_VERSION in vulkan_sdk_path:
        print(f"vulkan sdk found, but is not the required version ({REQUIRED_VULKAN_VERSION}),"
            " download the corrent one at https://www.lunarg.com/vulkan-sdk/")
        return False
    
    print(f"vulkan sdk found with the required version ({REQUIRED_VULKAN_VERSION}) at {vulkan_sdk_path}")
    return True

def run_premake():
    # run premake
    print("running premake")
     
    command = subprocess.run(["premake5", "vs2019"], capture_output=True) if sys.platform == 'win32' \
        else subprocess.run(["premake5", "gmake"], capture_output=True)
    
    sys.stdout.buffer.write(command.stdout)
    sys.stderr.buffer.write(command.stderr)


def download_vulkan_debug_libraries():
    if sys.platform != "win32":
        return

    # check if debug libs are downloaded
    required_libs_paths = [
        "Libraries/VulkanDebugSdk/Lib/shaderc_combinedd.lib",
        "Libraries/VulkanDebugSdk/Lib/spirv-cross-cored.lib",
        "Libraries/VulkanDebugSdk/Lib/spirv-cross-glsld.lib",
        "Libraries/VulkanDebugSdk/Lib/spirv-cross-hlsld.lib",
        "Libraries/VulkanDebugSdk/Lib/SPIRV-Toolsd.lib"
    ] 

    found_required_libs = True
    for lib_path in required_libs_paths:
        if not os.path.isfile(lib_path):
            print(f"cannot find {lib_path}")
            found_required_libs = False
            break

    if found_required_libs:
        print('found vulkan debug libraries')
        return

    # download debug libs
    print("downloading vulkan debug libraries")
    url = f"https://sdk.lunarg.com/sdk/download/{REQUIRED_VULKAN_VERSION}/windows/VulkanSDK-{REQUIRED_VULKAN_VERSION}-DebugLibs.zip"
    file_path = 'DebugLibs.zip'
    download_file(url, file_path)

    # extract download libs
    print("extracting vulkan debug libraries")
    debug_libs_zip = zipfile.ZipFile(file_path, 'r')
    debug_libs_zip.extractall("Libraries/VulkanDebugSdk")
    debug_libs_zip.close()

    # delete the zip file
    os.remove(file_path)


if __name__ == "__main__":
    os.chdir("../")

    vulkan_sdk_found = check_vulkan_sdk()
    
    if not vulkan_sdk_found:
        exit('bulid aborted')
    
    run_premake()
    download_vulkan_debug_libraries()

    print("build done")
