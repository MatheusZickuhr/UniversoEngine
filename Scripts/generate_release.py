import os
import sys
from pathlib import Path
import shutil

os.chdir("../")

release_folder = f"{os.getcwd()}/Release Binaries/"

def delete_cpp_files_from_dir(dir):
    if os.path.isdir(dir):

        for file in os.listdir(dir):
            full_file_path = dir + os.sep + file
            delete_cpp_files_from_dir(full_file_path)

    else:
        if Path(dir).suffix == ".cpp":
            os.remove(dir)
        
        return


def copy_bin_files_windows():
    # copy the binary files to the release folder
    print("Coping binary files")
    os.mkdir(f"{release_folder}/Bin/")

    shutil.copyfile(
        src=f"{os.getcwd()}/bin/Debug/UniversoEngine.lib",
        dst=f"{release_folder}/Bin/UniversoEngined.lib")

    shutil.copyfile(
        src=f"{os.getcwd()}/bin/Release/UniversoEngine.lib",
        dst=f"{release_folder}/Bin/UniversoEngine.lib")

    vulkan_debug_sdk_bin_folder = f"{os.getcwd()}/Libraries/VulkanDebugSdk/Bin/"
    for file in os.listdir(vulkan_debug_sdk_bin_folder):
        shutil.copyfile(
            src=f"{vulkan_debug_sdk_bin_folder}/{file}",
            dst=f"{release_folder}/Bin/{file}")


def create_zip_file():
    print("Generating Release.zip in the root directory")
    shutil.make_archive("Release", "zip", release_folder)


def copy_header_files():
    # move all the required header files to the release folder
    print("Coping header files")

    engine_src_folder = f"{os.getcwd()}/UniversoEngine/src/"
    entt_include_dir = f"{os.getcwd()}/Libraries/entt/include/entt/" 
    glm_include_dir = f"{os.getcwd()}/Libraries/glm/glm/" 
    reactphysics_include_dir = f"{os.getcwd()}/Libraries/reactphysics3d/Include/reactphysics3d/" 

    release_binaries_include_dir = f"{release_folder}/Include"

    shutil.copytree(src=engine_src_folder, dst=release_binaries_include_dir)
    shutil.copytree(src=entt_include_dir, dst=f"{release_binaries_include_dir}/entt")
    shutil.copytree(src=glm_include_dir, dst=f"{release_binaries_include_dir}/glm")
    shutil.copytree(src=reactphysics_include_dir, dst=f"{release_binaries_include_dir}/reactphysics3d")

    # delete all cpp files from the release folder (cpp files are no required)
    delete_cpp_files_from_dir(release_binaries_include_dir)


def copy_resource_files():
    print("Coping resource files")

    os.mkdir(f"{release_folder}/UniversoEngine/")

    shutil.copytree(
        src=f"{os.getcwd()}/UniversoEngine/resources/",
        dst=f"{release_folder}/UniversoEngine/resources/")


if __name__ == "__main__":
    if os.path.exists(release_folder):
        shutil.rmtree(release_folder)

    os.mkdir(release_folder)

    copy_header_files()

    if sys.platform == "win32":
        copy_bin_files_windows()

    else:
        # linux implementation
        pass
    
    copy_resource_files()

    create_zip_file()

    shutil.rmtree(release_folder)
