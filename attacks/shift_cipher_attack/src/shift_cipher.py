def encrypt(text, key):
    result = ""

    for ch in text:
        if ch.isalpha():
            base = ord('A') if ch.isupper() else ord('a')
            result += chr((ord(ch) - base + key) % 26 + base)
        else:
            result += ch

    return result


def decrypt(text, key):
    return encrypt(text, -key)


if __name__ == "__main__":
    plaintext = input("Enter plaintext: ")
    key = int(input("Enter key (0-25): "))

    ciphertext = encrypt(plaintext, key)

    print("Ciphertext:", ciphertext)
    print("Decrypted:", decrypt(ciphertext, key))
