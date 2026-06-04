import os
import sys

def to_camel_case(filename_parts):
    """
    Converts a list of filename parts (split by spaces) into a single camelCase string.
    Example: ['File', 'With', 'Name'] -> 'fileWithName'
    """
    if not filename_parts:
        return ""

    # Start with the first part, converted to lowercase
    camel_name = filename_parts[0].lower()

    # Iterate through the remaining parts
    for part in filename_parts[1:]:
        # Capitalize the first letter of subsequent parts and append them
        # .capitalize() ensures only the first letter is upper-cased
        camel_name += part.capitalize()

    return camel_name

def rename_files_to_camel_case(directory_path):
    """
    Processes all files in the given directory and renames them to camelCase.
    """
    print(f"--- Starting rename process in directory: {directory_path} ---")

    # Check if the directory exists
    if not os.path.isdir(directory_path):
        print(f"Error: Directory not found at '{directory_path}'")
        return

    try:
        # Iterate over all entries in the directory
        for filename in os.listdir(directory_path):
            # We only care about files, not subdirectories
            full_old_path = os.path.join(directory_path, filename)

            if os.path.isfile(full_old_path):
                # 1. Separate filename from extension
                name_base, extension = os.path.splitext(filename)

                # 2. Split the name by spaces
                name_parts = name_base.split(' ')

                # 3. Convert the parts to camelCase
                new_name_base = to_camel_case(name_parts)

                # 4. Combine with the original extension
                new_filename = new_name_base + extension
                full_new_path = os.path.join(directory_path, new_filename)

                # Avoid renaming if the name is already correct (e.g., if there were no spaces)
                if filename != new_filename:
                    # 5. Perform the rename operation
                    os.rename(full_old_path, full_new_path)
                    print(f"Renamed: '{filename}' -> '{new_filename}'")
                else:
                    print(f"Skipped: '{filename}' (already in target format)")

            else:
                # If it's a directory or a link, skip it
                print(f"Skipped: '{filename}' (is a directory or other)")

        print("--- Rename process complete. ---")

    except OSError as e:
        print(f"An operating system error occurred: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    # The script expects the directory path as the first command-line argument
    if len(sys.argv) < 2:
        print("Usage: python rename_files.py <path_to_folder>")
        # Example for testing if no argument is provided:
        # print("Using default path for example (change this line for production use).")
        # default_path = "./test_folder" # Uncomment and change for a default path
        # rename_files_to_camel_case(default_path)
    else:
        # Get the directory path from the command line
        target_directory = sys.argv[1]
        rename_files_to_camel_case(target_directory)
