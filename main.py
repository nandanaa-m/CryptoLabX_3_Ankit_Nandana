import os
import datetime
from collections import Counter

def log_execution(action):
    """
    Task 5: Maintains a log file recording date, time, and selected menu option.
    """
    # Create the outputs directory if it doesn't already exist
    os.makedirs("outputs", exist_ok=True)
    log_file_path = os.path.join("outputs", "execution.log")
    
    # Format the current date and time
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    # Append the log entry
    with open(log_file_path, 'a', encoding='utf-8') as log_file:
        log_file.write(f"[{current_time}] {action}\n")

def analyze_file():
    """
    Task 4: Reads a text file from the datasets folder and displays statistics.
    """
    print("\n--- Analyze Module ---")
    filename = input("Enter the filename from the datasets folder (e.g., sample.txt): ")
    filepath = os.path.join("datasets", filename)
    
    if not os.path.exists(filepath):
        print(f"\nError: Could not find '{filepath}'.")
        print("Please ensure the 'datasets' folder exists and contains your text file.")
        return

    # Read file contents
    with open(filepath, 'r', encoding='utf-8') as file:
        lines = file.readlines()

    # Calculate metrics
    line_count = len(lines)
    word_count = sum(len(line.split()) for line in lines)
    text = "".join(lines)
    char_count = len(text)
    unique_chars = set(text)
    
    # Calculate letter frequency (A-Z only, case-insensitive)
    letters_only = [char.lower() for char in text if char.isalpha()]
    letter_freq = Counter(letters_only)

    # Display results
    print("\n--- File Analysis Report ---")
    print(f"Total Lines: {line_count}")
    print(f"Total Words: {word_count}")
    print(f"Total Characters: {char_count}")
    print(f"Unique Characters: {len(unique_chars)}")
    
    print("\nLetter Frequency:")
    for letter, freq in sorted(letter_freq.items()):
        print(f"{letter}: {freq}")

def main():
    """
    Task 3: Menu-driven command-line interface.
    """
    log_execution("Program Started")
    
    while True:
        print("\n=== CryptoLabX Main Menu ===")
        print("1. Encrypt")
        print("2. Decrypt")
        print("3. Attack")
        print("4. Analyze")
        print("5. Exit")
        
        choice = input("Select an option (1-5): ")
        
        if choice == '1':
            log_execution("Selected Menu Option: Encrypt")
            print("\nEncrypt: 'Coming Soon'")
        elif choice == '2':
            log_execution("Selected Menu Option: Decrypt")
            print("\nDecrypt: 'Coming Soon'")
        elif choice == '3':
            log_execution("Selected Menu Option: Attack")
            print("\nAttack: 'Coming Soon'")
        elif choice == '4':
            log_execution("Selected Menu Option: Analyze")
            analyze_file()
        elif choice == '5':
            log_execution("Selected Menu Option: Exit")
            print("\nExiting CryptoLabX. Goodbye!")
            break
        else:
            log_execution(f"Invalid Option Attempted: {choice}")
            print("\nInvalid choice. Please select a number between 1 and 5.")

if __name__ == "__main__":
    main()