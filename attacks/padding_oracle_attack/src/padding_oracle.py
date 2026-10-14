# AES-CBC Padding Oracle

from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad, unpad


BLOCK_SIZE = AES.block_size  # 16 bytes


def encrypt_message(plaintext, key=None):
    """
    Encrypt plaintext using AES-CBC.

    Returns:
        key: AES key (used only by the local demo setup)
        iv: Initialization vector
        ciphertext: Encrypted message
    """
    if isinstance(plaintext, str):
        plaintext = plaintext.encode("utf-8")

    if key is None:
        key = get_random_bytes(BLOCK_SIZE)

    iv = get_random_bytes(BLOCK_SIZE)

    cipher = AES.new(key, AES.MODE_CBC, iv)
    ciphertext = cipher.encrypt(pad(plaintext, BLOCK_SIZE))

    return key, iv, ciphertext


def decrypt_message(key, iv, ciphertext):
    """
    Normal AES-CBC decryption.
    Used only to verify the demonstration.
    """
    cipher = AES.new(key, AES.MODE_CBC, iv)
    padded_plaintext = cipher.decrypt(ciphertext)

    return unpad(padded_plaintext, BLOCK_SIZE)


def make_padding_oracle(key):
    """
    Create a padding oracle.

    The oracle accepts an IV and ciphertext and returns:
        True  -> valid PKCS#7 padding
        False -> invalid PKCS#7 padding

    The attack receives only this oracle function,
    not the AES key.
    """

    def padding_oracle(iv, ciphertext):
        if len(iv) != BLOCK_SIZE:
            return False

        if (
            not ciphertext
            or len(ciphertext) % BLOCK_SIZE != 0
        ):
            return False

        try:
            cipher = AES.new(key, AES.MODE_CBC, iv)
            padded_plaintext = cipher.decrypt(ciphertext)

            # Validate PKCS#7 padding.
            unpad(padded_plaintext, BLOCK_SIZE)

            return True

        except ValueError:
            return False

    return padding_oracle
