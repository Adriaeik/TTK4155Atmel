import os
import zipfile

def zip_source_files(directory, zip_name):
    # Create a ZipFile object in write mode
    with zipfile.ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED) as zipf:
        # Walk through the directory and its subdirectories
        for root, _, files in os.walk(directory):
            for file in files:
                # Check if the file has the desired extension
                if file.endswith(('.c', '.h', '.md', '.png')):
                    # Get the full file path
                    full_path = os.path.join(root, file)
                    # Add the file to the ZIP archive with a relative path
                    zipf.write(full_path, os.path.relpath(full_path, directory))
    print(f'Files have been zipped into {zip_name}')

source_directory = '../TTK4155Atmel' 
output_zip = 'group_20_ttk4155_h23.zip'  
zip_source_files(source_directory, output_zip)
