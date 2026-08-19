import sys
import os

from shift_cipher import encrypt, decrypt
from brute_force_dictionary import brute_force_dictionary, load_dictionary
from chi_square_attack import chi_square_attack

LOG_FILE = "../outputs/execution_log.txt"

def log_interaction(menu_choice, user_input_desc, output_result):
    """Appends the user choice, input, and output cleanly to the outputs log file."""
    os.makedirs(os.path.dirname(LOG_FILE), exist_ok=True)
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(f"--- Menu Choice: {menu_choice} ---\n")
        f.write(f"Input: {user_input_desc}\n")
        f.write(f"Output:\n{output_result}\n")
        f.write("=" * 50 + "\n\n")

def get_text_input(prompt_message):
    """
    Checks if the user entered a .txt file path.
    If the file exists, reads and returns its contents.
    Otherwise, treats the input as normal text.
    """
    user_input = input(prompt_message).strip()

    if user_input.endswith('.txt') and os.path.exists(user_input):
        with open(user_input, 'r', encoding='utf-8') as file:
            content = file.read()

        print(
            f"[*] Successfully loaded "
            f"{len(content)} characters from {user_input}"
        )

        return content

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

        # ----------------------------------
        # 1. ENCRYPTION
        # ----------------------------------
        if choice == '1':

            text, src_desc = get_text_input("\nEnter plaintext (or path to .txt file): ")
            try:
                key = int(input("Enter key (0-25): ")) % 26
                ciphertext = encrypt(text, key)
                print(f"\n[+] Ciphertext: {ciphertext}")
                
                # Combine the original text, key, and output into one clean log block
                log_details = f"Input Text:\n{text}\n\nKey: {key}\n\nEncrypted Output:\n{ciphertext}"
                log_interaction(choice, f"Source: {src_desc}", log_details)
                
            except ValueError:
                print("[!] Error: Key must be an integer.")

        # ----------------------------------
        # 2. DECRYPTION
        # ----------------------------------
        elif choice == '2':

            ciphertext, src_desc = get_text_input("\nEnter ciphertext (or path to .txt file): ")
            try:
                key = int(input("Enter key (0-25): ")) % 26
                plaintext = decrypt(ciphertext, key)
                print(f"\n[+] Decrypted Plaintext: {plaintext}")
                
                # Combine the original text, key, and output into one clean log block
                log_details = f"Input Ciphertext:\n{ciphertext}\n\nKey: {key}\n\nDecrypted Output:\n{plaintext}"
                log_interaction(choice, f"Source: {src_desc}", log_details)
                
            except ValueError:
                print("[!] Error: Key must be an integer.")

        # ----------------------------------
        # 3. BRUTE FORCE + DICTIONARY
        # ----------------------------------
        elif choice == '3':

            ciphertext = get_text_input(
                "\nEnter ciphertext (or path to .txt file): "
            )

            print(
                "\n[*] Running Brute-Force Dictionary Attack..."
            )

            try:
                dictionary = load_dictionary(
                    "../dictionary/english_words.txt"
                )

                results = brute_force_dictionary(
                    ciphertext,
                    dictionary
                )

                print(
                    "\n--- Top Results (Dictionary Matches) ---"
                )

                for rank, item in enumerate(
                    results[:5], 1
                ):

                    print(
                        f"Rank {rank} | "
                        f"Key: {item['key']:2d} | "
                        f"Valid Words: {item['score']:2d} | "
                        f"Text: {item['plaintext']}"
                    )

            except FileNotFoundError:
                print(
                    "[!] Error: Dictionary file not found."
                )

            except Exception as e:
                print(
                    f"[!] Error during brute-force attack: {e}"
                )

        # ----------------------------------
        # 4. CHI-SQUARE ATTACK
        # ----------------------------------
        elif choice == '4':

            ciphertext = get_text_input(
                "\nEnter ciphertext (or path to .txt file): "
            )

            print(
                "\n[*] Running Chi-Square Frequency Analysis..."
            )

            try:
                results = chi_square_attack(ciphertext)

                print(
                    "\n--- Ranked Results "
                    "(Lowest Chi-Square Score is Best) ---"
                )

                for rank, item in enumerate(
                    results[:5], 1
                ):

                    print(
                        f"Rank {rank} | "
                        f"Key: {item['key']:2d} | "
                        f"Chi-Square: {item['score']:.4f} | "
                        f"Text: {item['plaintext']}"
                    )

            except Exception as e:
                print(
                    f"[!] Error during Chi-Square attack: {e}"
                )

        # ----------------------------------
        # 5. EXIT
        # ----------------------------------
        elif choice == '5':

            print("Exiting...")
            sys.exit(0)

        # ----------------------------------
        # INVALID CHOICE
        # ----------------------------------
        else:

            print(
                "[!] Invalid selection. "
                "Please enter 1-5."
            )


if __name__ == "__main__":
    main()
