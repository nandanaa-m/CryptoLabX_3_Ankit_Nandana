import sys
from shift_cipher import encrypt, decrypt
from brute_force_dictionary import brute_force_dictionary
from attacks.shift_cipher_attack.src.chi_square_attack import chi_square_attack
import os

def get_text_input(prompt_message):
    """Checks if the user entered a file path. If yes, reads the file. If no, returns the raw text."""
    user_input = input(prompt_message).strip()
    
    # If the user typed a .txt file name and the file actually exists
    if user_input.endswith('.txt') and os.path.exists(user_input):
        with open(user_input, 'r', encoding='utf-8') as file:
            content = file.read()
            print(f"[*] Successfully loaded {len(content)} characters from {user_input}")
            return content
    
    # Otherwise, just treat it as normal typed text
    return user_input

def display_menu():
    print("\n==========================================")
    print("      SHIFT CIPHER CRYPTANALYSIS SUITE    ")
    print("==========================================")
    print("1. Encrypt Message (Shift Cipher)")
    print("2. Decrypt Message (Known Key)")
    print("3. Brute-Force Attack (Dictionary Scoring)")
    print("4. Chi-Square Cryptanalysis Attack")
    print("5. Exit")
    print("==========================================")

def main():
    while True:
        display_menu()
        choice = input("Enter choice (1-5): ").strip()

        if choice == '1':
            text = input("\nEnter plaintext: ")
            try:
                key = int(input("Enter key (0-25): ")) % 26
                ciphertext = encrypt(text, key)
                print(f"\n[+] Ciphertext: {ciphertext}")
            except ValueError:
                print("[!] Error: Key must be an integer.")

        elif choice == '2':
            ciphertext = input("\nEnter ciphertext: ")
            try:
                key = int(input("Enter key (0-25): ")) % 26
                plaintext = decrypt(ciphertext, key)
                print(f"\n[+] Decrypted Plaintext: {plaintext}")
            except ValueError:
                print("[!] Error: Key must be an integer.")

        elif choice == '3':
            ciphertext = input("\nEnter ciphertext to crack: ")
            print("\n[*] Running Brute-Force Dictionary Attack...")
            results = brute_force_attack(ciphertext)
            print("\n--- Top Results (Dictionary Matches) ---")
            for rank, item in enumerate(results[:5], 1):
                print(f"Rank {rank} | Key: {item['key']:2d} | Valid Words: {item['score']:2d} | Text: {item['plaintext']}")

        elif choice == '4':
            ciphertext = input("\nEnter ciphertext to crack: ")
            print("\n[*] Running Chi-Square Frequency Analysis...")
            results = chi_square_attack(ciphertext)
            print("\n--- Ranked Results (Lowest Chi-Square Score is Best) ---")
            for rank, item in enumerate(results[:5], 1):
                print(f"Rank {rank} | Key: {item['key']:2d} | Chi-Square: {item['score']:.4f} | Text: {item['plaintext']}")

        elif choice == '5':
            print("Exiting...")
            sys.exit(0)

        else:
            print("[!] Invalid selection. Please enter 1-5.")

if __name__ == "__main__":
    main()