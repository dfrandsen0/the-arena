import os
import sys
import re

def camel_to_upper_underscore(name: str) -> str:
    # Remove extension
    base = os.path.splitext(name)[0]
    # Insert underscores before uppercase letters (except the first)
    base = re.sub(r'(?<!^)(?=[A-Z])', '_', base)
    # Convert to uppercase
    return base.upper()

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <folder> <output_file>")
        sys.exit(1)

    folder = sys.argv[1]
    output_file = sys.argv[2]

    if not os.path.isdir(folder):
        print(f"Error: '{folder}' is not a valid folder.")
        sys.exit(1)

    files = [f for f in os.listdir(folder) if os.path.isfile(os.path.join(folder, f))]

    with open(output_file, "w", encoding="utf-8") as out:
        for filename in files:
            name_upper = camel_to_upper_underscore(filename)
            out.write(f'{{{name_upper}, "{filename}"}}\n')

if __name__ == "__main__":
    main()
