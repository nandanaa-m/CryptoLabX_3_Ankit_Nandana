# Shift Cipher Cryptanalysis Suite

A Python-based cryptographic tool designed to encrypt, decrypt, and automatically crack shift ciphers. This project demonstrates foundational cryptanalysis techniques by implementing both dictionary-based brute-force scoring and statistical frequency analysis.

## Features

* **Encryption & Decryption:** Standard shift cipher (Caesar cipher) implementation supporting keys from 0 to 25.
* **Smart Input Handling:** Input text directly via the terminal or pass relative file paths (e.g., `../testcases/tc_02_long.txt`) for rapid batch testing.
* **Dictionary Brute-Force Attack:** Systematically tests all 26 possible shifts and ranks the results based on the number of valid English words found using a local dictionary lookup.
* **Chi-Square Frequency Analysis:** Computes the $\chi^2$ statistic against standard English letter frequencies to programmatically identify the most probable key without relying on whole-word dictionaries.

## Repository Structure

```text
attacks/shift_cipher_attack/
├── src/
│   ├── shift_cipher.py             # Core encryption/decryption logic
│   ├── brute_force_dictionary.py   # Dictionary scoring module
│   ├── chi_square_attack.py        # Statistical frequency analysis module
│   └── main.py                     # Interactive CLI menu
├── dictionary/
│   └── english_words.txt           # Wordlist for brute-force scoring
├── testcases/                      # Sample plaintexts and ciphertexts
├── outputs/                        # Terminal logs and execution results
├── screenshots/                    # Visual proof of successful attacks
└── reports/                        # Final assignment PDF documentation