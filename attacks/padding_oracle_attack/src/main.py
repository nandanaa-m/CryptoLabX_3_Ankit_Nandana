# Padding Oracle Attack - Main Program
"""
main.py
Driver script to demonstrate the Padding Oracle Attack on AES-CBC.
Coordinates encryption, oracle creation, attack execution, and analysis reporting.
"""

from padding_oracle import encrypt_message, make_padding_oracle
from attack import recover_plaintext


def display_analysis_and_recommendations(num_blocks, total_queries):
    """Prints the analytical evaluation and security recommendations required by Tasks 1, 3, and 4."""
    avg_per_block = total_queries / num_blocks if num_blocks else 0
    avg_per_byte = total_queries / (num_blocks * 16) if num_blocks else 0

    print("\n" + "=" * 65)
    print("ANALYSIS & SECURITY REPORT")
    print("=" * 65)

    print("\n--- Task 1: Understand the Attack ---")
    print("1. AES-CBC Chaining: Decryption computes P_i = D_K(C_i) ^ C_{i-1}.")
    print("   The plaintext is revealed only after XORing the block cipher's")
    print("   raw decryption with the preceding ciphertext block (or IV for block 1).")
    print("2. PKCS#7 Padding: Messages are padded to a multiple of 16 bytes.")
    print("   If N bytes (1 <= N <= 16) are needed, N bytes of value 0xN are appended.")
    print("3. Role of the Oracle: Leaking whether decrypted data possesses valid PKCS#7")
    print("   padding creates a side-channel that lets an attacker deduce intermediate")
    print("   decryption states (D_K(C_i)) one byte at a time.")

    print("\n--- Task 3: Analyze the Attack ---")
    print(f"1. Query Statistics:")
    print(f"   - Total oracle queries executed : {total_queries}")
    print(f"   - Average queries per block     : {avg_per_block:.2f} (Theoretical max: 4096)")
    print(f"   - Average queries per byte      : {avg_per_byte:.2f} (Theoretical expected: ~128)")
    print("2. Mechanism of Modification:")
    print("   Because P_i[j] = D_K(C_i)[j] ^ C_{i-1}[j], modifying byte j of C_{i-1}")
    print("   directly alters byte j of P_i bit-for-bit during CBC decryption.")
    print("   This modification does NOT distort subsequent ciphertext blocks (C_{i+1}, etc.),")
    print("   enabling targeted, isolated byte manipulation.")

    print("\n--- Task 4: Security Recommendations ---")
    print("1. Authenticated Encryption with Associated Data (AEAD):")
    print("   Transition from CBC mode to modern AEAD modes like AES-GCM or")
    print("   ChaCha20-Poly1305 to ensure simultaneous confidentiality and integrity.")
    print("2. Encrypt-then-MAC:")
    print("   If CBC is mandatory, calculate a cryptographic MAC (e.g., HMAC-SHA256)")
    print("   over (IV || Ciphertext) and verify the MAC before initiating decryption.")
    print("3. Constant-Time Uniform Error Handling:")
    print("   Never differentiate between padding errors and decryption/integrity errors.")
    print("=" * 65 + "\n")


def main():
    # Target message for the demonstration
    secret_message = (
        "Confidential Data: AES-CBC padding oracle demonstration successfully executed!"
    )

    print("=================================================================")
    print("           AES-CBC PADDING ORACLE ATTACK DEMONSTRATION           ")
    print("=================================================================")

    # 1. Server-side setup: Encrypt message and generate padding oracle
    # Note: 'key' remains on the simulated server side and is NEVER passed to the attack
    key, iv, ciphertext = encrypt_message(secret_message)
    oracle = make_padding_oracle(key)

    num_blocks = len(ciphertext) // 16

    print("\n[+] Challenge Generated:")
    print(f"    IV (hex)          : {iv.hex()}")
    print(f"    Ciphertext (hex)   : {ciphertext.hex()}")
    print(f"    Ciphertext Length : {len(ciphertext)} bytes ({num_blocks} AES blocks)")

    # 2. Attacker-side execution: Recover plaintext using ONLY iv, ciphertext, and oracle
    print("\n[+] Launching Padding Oracle Attack (Zero knowledge of secret key)...")
    recovered_plaintext_bytes, total_queries = recover_plaintext(iv, ciphertext, oracle)

    # 3. Deliverables output
    print("\n" + "=" * 65)
    print("DELIVERABLES")
    print("=" * 65)
    print(f"[+] Recovered Plaintext (Raw Bytes) : {recovered_plaintext_bytes}")
    print(f"[+] Recovered Plaintext (Decoded)   : {recovered_plaintext_bytes.decode('utf-8', errors='replace')}")
    print(f"[+] Total Oracle Queries Executed   : {total_queries}")

    # 4. Display theoretical and security analysis
    display_analysis_and_recommendations(num_blocks, total_queries)


if __name__ == "__main__":
    main()