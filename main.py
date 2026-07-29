def display_menu():
    print("\n========== CryptoLabX ==========")
    print("1. Encrypt")
    print("2. Decrypt")
    print("3. Attack")
    print("4. Analyze")
    print("5. Exit")
    print("================================")


def main():
    while True:
        display_menu()

        choice = input("Enter your choice (1-5): ")

        if choice == "1":
            print("\nEncrypt - Coming Soon!")

        elif choice == "2":
            print("\nDecrypt - Coming Soon!")

        elif choice == "3":
            print("\nAttack - Coming Soon!")

        elif choice == "4":
            print("\nAnalyze - Coming Soon!")

        elif choice == "5":
            print("\nThank you for using CryptoLabX.")
            break

        else:
            print("\nInvalid choice! Please enter a number between 1 and 5.")


if __name__ == "__main__":
    main()