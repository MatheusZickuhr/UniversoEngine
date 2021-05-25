import os
from pathlib import Path
import shutil


def delete_build_files_from_dir(dir):
    build_files_extensions = [".make", ".sln", ".vcxproj", ".filters", ".user"]

    for file_path in os.listdir(dir):
        if Path(file_path).suffix in build_files_extensions:
            print(f"{file_path} deleted")
            os.remove(f"{dir}{os.sep}{file_path}")


def delete_folders(folders):
    for folder in folders:
        if os.path.exists(folder):
            print(f"{folder} folder deleted")
            shutil.rmtree(folder)


if __name__ == "__main__":
    os.chdir("../")

    # delete build files
    root_dir = os.curdir
    libraries_dir = f"{root_dir}{os.sep}Libraries"

    delete_build_files_from_dir(root_dir)
    delete_build_files_from_dir(libraries_dir)

    # delete build folders
    delete_folders(["shaderCache", "bin", "obj"])

    print("clean done")