"""
main.py
Driver script to demonstrate the Padding Oracle Attack on AES-CBC.
Coordinates encryption, oracle creation, attack execution, and analysis reporting.
"""

from padding_oracle import encrypt_message, make_padding_oracle
from attack import recover_plaintext


def display_analysis_and_recommendations(num_blocks, total_queries):
    """Display attack analysis and security recommendations."""

    avg_per_block = total_queries / num_blocks if num_blocks else 0
    avg_per_byte = total_queries / (num_blocks * 16) if num_blocks else 0

    print("\n" + "=" * 65)
    print("ANALYSIS & SECURITY REPORT")
    print("=" * 65)

    print("\n--- Task 1: Understand the Attack ---")

    print("1. AES-CBC Chaining:")
    print("   P_i = D_K(C_i) XOR C_(i-1)")
    print("   The IV is used as the previous block for the first block.")

    print("\n2. PKCS#7 Padding:")
    print("   Messages are padded to a multiple of 16 bytes.")
    print("   If N bytes are required, N bytes with value N are appended.")

    print("\n3. Role of the Padding Oracle:")
    print("   The oracle reports whether decrypted data has valid padding.")
    print("   By modifying the previous ciphertext block and observing")
    print("   oracle responses, an attacker can recover plaintext bytes")
    print("   without knowing the AES encryption key.")

    print("\n--- Task 3: Analyze the Attack ---")

    print("\n1. Query Statistics:")
    print(f"   Total oracle queries executed : {total_queries}")
    print(f"   Average queries per block     : {avg_per_block:.2f}")
    print(f"   Average queries per byte      : {avg_per_byte:.2f}")
    print("   Basic candidate-search bound  : 4096 queries per block")
    print("   Note: Additional validation queries may be required.")

    print("\n2. Why Modify the Previous Ciphertext Block?")
    print("   CBC decryption follows:")
    print("   P_i = D_K(C_i) XOR C_(i-1)")
    print("   Changing a byte in C_(i-1) changes the corresponding")
    print("   byte in P_i. The oracle's response helps identify")
    print("   candidate plaintext bytes.")

    print("\n--- Task 4: Security Recommendations ---")

    print("1. Prefer authenticated encryption such as AES-GCM.")
    print("2. If CBC is required, authenticate the IV and ciphertext")
    print("   with a secure MAC and verify it before decryption.")
    print("3. Avoid distinguishable padding and decryption errors.")
    print("4. Do not process decrypted data before authentication.")

    print("=" * 65)


def main():

    # Target message used for the local demonstration.
    secret_message = (
        "Confidential Data: AES-CBC padding oracle "
        "demonstration successfully executed!"
    )

    print("=" * 65)
    print("       AES-CBC PADDING ORACLE ATTACK DEMONSTRATION")
    print("=" * 65)

    # Display the original plaintext.
    print("\n[+] Original Plaintext:")
    print(secret_message)

    # Server-side setup: encrypt the message and create the oracle.
    # The key is not passed to the attack algorithm.
    key, iv, ciphertext = encrypt_message(secret_message)

    oracle = make_padding_oracle(key)

    num_blocks = len(ciphertext) // 16

    print("\n[+] Challenge Generated:")
    print(f"    IV (hex)           : {iv.hex()}")
    print(f"    Ciphertext (hex)   : {ciphertext.hex()}")
    print(f"    Ciphertext Length  : {len(ciphertext)} bytes")
    print(f"    Number of Blocks   : {num_blocks}")

    # Attacker-side execution: only IV, ciphertext and oracle are passed.
    print("\n[+] Launching Padding Oracle Attack...")
    print("[+] The attack function does not receive the AES key.")

    recovered_plaintext_bytes, total_queries = recover_plaintext(
        iv,
        ciphertext,
        oracle
    )

    # Display recovered plaintext.
    print("\n" + "=" * 65)
    print("DELIVERABLES")
    print("=" * 65)

    print("\n[+] Original Plaintext:")
    print(secret_message)

    print("\n[+] Recovered Plaintext (Raw Bytes):")
    print(recovered_plaintext_bytes)

    print("\n[+] Recovered Plaintext (Decoded):")
    recovered_text = recovered_plaintext_bytes.decode(
        "utf-8",
        errors="replace"
    )
    print(recovered_text)

    print("\n[+] Total Oracle Queries Executed:")
    print(total_queries)

    # Verify recovered plaintext against the original demonstration input.
    if recovered_plaintext_bytes == secret_message.encode("utf-8"):
        print("\n[+] Verification: SUCCESS")
        print("[+] Original and recovered plaintext match.")
    else:
        print("\n[-] Verification: FAILED")
        print("[-] Original and recovered plaintext do not match.")

    # Display attack analysis and security recommendations.
    display_analysis_and_recommendations(
        num_blocks,
        total_queries
    )


if __name__ == "__main__":
    main()
