import sys
import os
import subprocess
from download_vulkan_debug_libraries import check_vulkan_sdk, download_vulkan_debug_libraries


def run_premake():
    # run premake
    print("running premake")
     
    command = subprocess.run(["premake5", "vs2022"], capture_output=True) if sys.platform == 'win32' \
        else subprocess.run(["premake5", "gmake"], capture_output=True)
    
    sys.stdout.buffer.write(command.stdout)
    sys.stderr.buffer.write(command.stderr)


if __name__ == "__main__":
    os.chdir("../")

    vulkan_sdk_found = check_vulkan_sdk()
    
    if not vulkan_sdk_found:
        exit('bulid aborted')
    
    run_premake()

    if sys.platform == "win32":
        download_vulkan_debug_libraries()

    print("build done")
