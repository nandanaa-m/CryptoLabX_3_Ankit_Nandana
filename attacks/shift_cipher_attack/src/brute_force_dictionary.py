from shift_cipher import decrypt


def load_dictionary(filename):
    words = set()

    with open(filename, "r") as file:
        for line in file:
            word = line.strip().lower()

            if word:
                words.add(word)

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
    results = []

    for key in range(26):

        plaintext = decrypt(ciphertext, key)

        score = score_text(plaintext, dictionary)

        results.append({
            "key": key,
            "score": score,
            "plaintext": plaintext
        })

    # Highest dictionary score first
    results.sort(
        key=lambda x: x["score"],
        reverse=True
    )

    return results
