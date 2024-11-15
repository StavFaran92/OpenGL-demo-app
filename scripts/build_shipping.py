import os
import shutil
import sys
import subprocess

def copy_folder_contents(folder_to_game, output_folder):
    """
    Copies all contents from folder_to_game to output_folder.
    Creates the output_folder if it does not exist.
    """
    # Ensure output_folder exists, create it if necessary
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
        print(f"Created output folder: {output_folder}")

    # Copy all contents from folder_to_game to output_folder
    try:
        # Iterate through all files and subfolders in folder_to_game
        for item in os.listdir(folder_to_game):
            src_path = os.path.join(folder_to_game, item)
            dest_path = os.path.join(output_folder, item)

            # Check if the item is a file or directory
            if os.path.isdir(src_path):
                # Recursively copy directories
                shutil.copytree(src_path, dest_path)
            else:
                # Copy files
                shutil.copy2(src_path, dest_path)

        print(f"All contents copied from {folder_to_game} to {output_folder}")
    except Exception as e:
        print(f"Error while copying: {e}")


def build_project(msbuild_path, project_path, folder_to_game):
    """
    Builds the project using MSBuild and defines the GAME_FOLDER preprocessor macro.
    """
    build_command = [
        msbuild_path, project_path, "/p:Configuration=Release"
    ]

    # Print the build command for debugging purposes
    print(f"Executing: {' '.join(build_command)}")

    try:
        subprocess.check_call(build_command)
        print("Build successful!")
    except subprocess.CalledProcessError:
        print("Build failed.")
        sys.exit(1)


# Ensure the script receives a folder_to_game argument
if len(sys.argv) < 2:
    print("Error: folder_to_game path not provided.")
    sys.exit(1)

# Capture folder_to_game from the arguments
folder_to_game = sys.argv[1]
output_folder = os.path.join(os.getcwd(), "../Release/data")

# Define the MSBuild path and project path
msbuild_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
project_path = r"../Game/Game.vcxproj"

# Step 1: Build the project
build_project(msbuild_path, project_path, folder_to_game)

# Step 2: Copy the contents after build
copy_folder_contents(folder_to_game, output_folder)