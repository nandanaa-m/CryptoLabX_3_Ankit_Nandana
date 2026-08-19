import string
from shift_cipher import decrypt

# Standard English letter frequencies (in percentage)
ENGLISH_FREQ = {
    'A': 8.167, 'B': 1.492, 'C': 2.782, 'D': 4.253, 'E': 12.702,
    'F': 2.228, 'G': 2.015, 'H': 6.094, 'I': 6.966, 'J': 0.153,
    'K': 0.772, 'L': 4.025, 'M': 2.406, 'N': 6.749, 'O': 7.507,
    'P': 1.929, 'Q': 0.095, 'R': 5.987, 'S': 6.327, 'T': 9.056,
    'U': 2.758, 'V': 0.978, 'W': 2.360, 'X': 0.150, 'Y': 1.974,
    'Z': 0.074
}

def calculate_chi_square(text: str) -> float:
    """Calculates the Chi-Square statistic of a given plaintext against English frequencies."""
    text_upper = [c for c in text.upper() if c in string.ascii_uppercase]
    total_letters = len(text_upper)
    
    if total_letters == 0:
        return float('inf')

    # Count observed frequencies
    observed_counts = {c: 0 for c in string.ascii_uppercase}
    for c in text_upper:
        observed_counts[c] += 1

    # Compute Chi-Square score
    chi_square = 0.0
    for char, expected_pct in ENGLISH_FREQ.items():
        expected_count = total_letters * (expected_pct / 100.0)
        observed_count = observed_counts[char]
        chi_square += ((observed_count - expected_count) ** 2) / expected_count

    return chi_square

def chi_square_attack(ciphertext: str):
    """
    Tests all 26 possible shift keys and returns a list of candidate plaintexts
    ranked by ascending Chi-Square score (lowest is best).
    """
    results = []
    
    for key in range(26):
        decrypted_text = decrypt(ciphertext, key)
        score = calculate_chi_square(decrypted_text)
        results.append({
            'key': key,
            'score': score,
            'plaintext': decrypted_text
        })
    
    # Sort results so the lowest Chi-Square score comes first
    results.sort(key=lambda x: x['score'])
    return results