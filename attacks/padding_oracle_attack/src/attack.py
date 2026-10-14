# Padding Oracle Attack Implementation

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad


BLOCK_SIZE = AES.block_size  # 16 bytes


def recover_plaintext(iv, ciphertext, oracle):
    """
    Recover plaintext using a padding oracle attack.

    Arguments:
        iv         : Original initialization vector.
        ciphertext : AES-CBC ciphertext.
        oracle     : Function returning True/False for padding validity.

    Returns:
        recovered_plaintext : Recovered plaintext as bytes.
        query_count         : Total number of oracle queries.
    """

    if len(iv) != BLOCK_SIZE:
        raise ValueError("IV must be exactly 16 bytes.")

    if not ciphertext or len(ciphertext) % BLOCK_SIZE != 0:
        raise ValueError("Ciphertext must contain complete AES blocks.")

    query_count = 0

    def query(test_iv, test_ciphertext):
        nonlocal query_count
        query_count += 1
        return oracle(test_iv, test_ciphertext)

    def recover_block(previous_block, target_block):
        """
        Recover one plaintext block from right to left.
        """

        intermediate = bytearray(BLOCK_SIZE)
        crafted = bytearray(BLOCK_SIZE)

        for position in range(BLOCK_SIZE - 1, -1, -1):

            padding_value = BLOCK_SIZE - position

            # Force already-recovered bytes to the desired padding.
            for j in range(position + 1, BLOCK_SIZE):
                crafted[j] = intermediate[j] ^ padding_value

            found = False

            for guess in range(256):
                crafted[position] = guess

                if not query(bytes(crafted), target_block):
                    continue

                # Special check for a possible accidental longer
                # padding when testing a one-byte padding.
                if position == BLOCK_SIZE - 1 and position > 0:
                    check = bytearray(crafted)
                    check[position - 1] ^= 1

                    if query(bytes(check), target_block):
                        # Padding remains valid after changing the
                        # preceding byte, confirming one-byte padding.
                        pass
                    else:
                        # The response was caused by a longer padding.
                        continue

                intermediate[position] = guess ^ padding_value
                found = True
                break

            if not found:
                raise RuntimeError(
                    f"Could not recover byte at position {position}."
                )

        # P_i = D_K(C_i) XOR C_(i-1)
        plaintext_block = bytes(
            intermediate[i] ^ previous_block[i]
            for i in range(BLOCK_SIZE)
        )

        return plaintext_block

    ciphertext_blocks = [
        ciphertext[i:i + BLOCK_SIZE]
        for i in range(0, len(ciphertext), BLOCK_SIZE)
    ]

    recovered = bytearray()

    previous_block = iv

    for block_number, target_block in enumerate(ciphertext_blocks, start=1):
        print(f"Recovering block {block_number}/{len(ciphertext_blocks)}...")

        plaintext_block = recover_block(previous_block, target_block)
        recovered.extend(plaintext_block)

        previous_block = target_block

    # Remove the original PKCS#7 padding after recovering every block.
    try:
        plaintext = unpad(bytes(recovered), BLOCK_SIZE)
    except ValueError as exc:
        raise RuntimeError(
            "Recovered data has invalid PKCS#7 padding."
        ) from exc

    return plaintext, query_count
