from shift_cipher import decrypt


def load_dictionary(filename):
    words = set()

    with open(filename, "r") as file:
        for line in file:
            words.add(line.strip().lower())

    return words


def score_text(text, dictionary):
    words = text.lower().split()

    score = 0

    for word in words:
        word = word.strip(".,!?;:")

        if word in dictionary:
            score += 1

    return score


def brute_force_dictionary(ciphertext, dictionary):
    best_key = None
    best_plaintext = None
    best_score = -1

    for key in range(26):

        plaintext = decrypt(ciphertext, key)

        score = score_text(plaintext, dictionary)

        print(
            f"Key {key:2}: "
            f"Score = {score:2} | "
            f"{plaintext}"
        )

        if score > best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext

    print("\nPredicted Key:", best_key)
    print("Predicted Plaintext:", best_plaintext)
    print("Dictionary Score:", best_score)


if __name__ == "__main__":

    ciphertext = input("Enter ciphertext: ")

    dictionary = load_dictionary(
        "dictionary/english_words.txt"
    )

    brute_force_dictionary(ciphertext, dictionary)
