"""
Randomness analysis engine for the CryptoLabX random-generation assignment.

This module intentionally contains reusable mathematics/statistics functions only.
The notebook (report.ipynb) should handle:
    - reading .bin files
    - calling these functions
    - plotting
    - explaining/interpreting results

Expected input:
    bytes, bytearray, or a NumPy uint8 array.
"""

from __future__ import annotations

from collections import Counter
from math import erfc, log2, sqrt
from typing import Dict, Iterable, Tuple

import numpy as np
from scipy.stats import chi2


def _as_uint8_array(data: bytes | bytearray | np.ndarray) -> np.ndarray:
    """Convert input data to a one-dimensional uint8 NumPy array."""
    if isinstance(data, (bytes, bytearray)):
        return np.frombuffer(data, dtype=np.uint8)

    arr = np.asarray(data, dtype=np.uint8)
    return arr.reshape(-1)


def byte_frequencies(data: bytes | bytearray | np.ndarray) -> np.ndarray:
    """Return counts for byte values 0..255."""
    arr = _as_uint8_array(data)
    return np.bincount(arr, minlength=256)


def byte_probabilities(data: bytes | bytearray | np.ndarray) -> np.ndarray:
    """Return empirical probability of each byte value 0..255."""
    counts = byte_frequencies(data)
    return counts / counts.sum()


def byte_statistics(data: bytes | bytearray | np.ndarray) -> Dict[str, float]:
    """Return basic statistics for the byte values."""
    arr = _as_uint8_array(data)

    if arr.size == 0:
        raise ValueError("Input data is empty.")

    return {
        "size_bytes": int(arr.size),
        "min": int(arr.min()),
        "max": int(arr.max()),
        "mean": float(arr.mean()),
        "std": float(arr.std()),
    }


def bit_distribution(data: bytes | bytearray | np.ndarray) -> Dict[str, float | int]:
    """Count 0/1 bits and return their proportions."""
    arr = _as_uint8_array(data)

    if arr.size == 0:
        raise ValueError("Input data is empty.")

    # np.unpackbits gives 8 bits per byte.
    bits = np.unpackbits(arr)
    ones = int(bits.sum())
    total = int(bits.size)
    zeros = total - ones

    return {
        "total_bits": total,
        "ones": ones,
        "zeros": zeros,
        "ones_proportion": ones / total,
        "zeros_proportion": zeros / total,
    }


def shannon_entropy(data: bytes | bytearray | np.ndarray) -> float:
    """Calculate Shannon entropy in bits per byte."""
    probabilities = byte_probabilities(data)
    nonzero = probabilities[probabilities > 0]
    return float(-np.sum(nonzero * np.log2(nonzero)))


def chi_square_test(
    data: bytes | bytearray | np.ndarray,
) -> Dict[str, float | int | bool]:
    """
    Pearson chi-square test for a uniform byte distribution.

    H0: all 256 byte values are equally likely.
    H1: the byte distribution is not uniform.
    """
    counts = byte_frequencies(data)
    total = int(counts.sum())

    if total == 0:
        raise ValueError("Input data is empty.")

    expected = np.full(256, total / 256.0)
    statistic = float(np.sum((counts - expected) ** 2 / expected))
    degrees_of_freedom = 255
    p_value = float(chi2.sf(statistic, degrees_of_freedom))

    return {
        "chi_square": statistic,
        "degrees_of_freedom": degrees_of_freedom,
        "p_value": p_value,
        "reject_at_0.05": bool(p_value < 0.05),
    }


def bit_sequence(data: bytes | bytearray | np.ndarray) -> np.ndarray:
    """Return the complete input as a 0/1 NumPy array."""
    arr = _as_uint8_array(data)

    if arr.size == 0:
        raise ValueError("Input data is empty.")

    return np.unpackbits(arr)


def runs_test(
    data: bytes | bytearray | np.ndarray,
) -> Dict[str, float | int | bool]:
    """
    Wald-Wolfowitz-style runs test for a binary sequence.

    The test uses the normal approximation for the number of runs.

    H0: the binary sequence is consistent with random ordering.
    H1: the ordering shows evidence against randomness.
    """
    bits = bit_sequence(data)
    n = int(bits.size)

    n1 = int(bits.sum())
    n0 = n - n1

    if n < 2:
        raise ValueError("At least two bits are required.")

    if n0 == 0 or n1 == 0:
        return {
            "n_bits": n,
            "zeros": n0,
            "ones": n1,
            "runs": 1,
            "expected_runs": 1.0,
            "z_score": float("inf"),
            "p_value": 0.0,
            "reject_at_0.05": True,
        }

    runs = int(1 + np.count_nonzero(bits[1:] != bits[:-1]))

    expected = 1.0 + (2.0 * n0 * n1) / n
    variance = (
        2.0
        * n0
        * n1
        * (2.0 * n0 * n1 - n)
        / (n**2 * (n - 1))
    )

    if variance <= 0:
        raise ValueError("Runs-test variance is not positive.")

    z = (runs - expected) / sqrt(variance)

    # Two-sided normal p-value.
    p_value = erfc(abs(z) / sqrt(2.0))

    return {
        "n_bits": n,
        "zeros": n0,
        "ones": n1,
        "runs": runs,
        "expected_runs": expected,
        "z_score": float(z),
        "p_value": float(p_value),
        "reject_at_0.05": bool(p_value < 0.05),
    }


def serial_correlation(
    data: bytes | bytearray | np.ndarray,
    lag: int = 1,
) -> float:
    """
    Pearson serial correlation between x[i] and x[i+lag].

    lag=1 tests adjacent bytes.
    """
    arr = _as_uint8_array(data).astype(np.float64)

    if arr.size <= lag:
        raise ValueError("Input data is too short for the requested lag.")

    if lag <= 0:
        raise ValueError("lag must be a positive integer.")

    x = arr[:-lag]
    y = arr[lag:]

    x_std = x.std()
    y_std = y.std()

    if x_std == 0 or y_std == 0:
        return float("nan")

    return float(np.corrcoef(x, y)[0, 1])


def serial_correlations(
    data: bytes | bytearray | np.ndarray,
    lags: Iterable[int] = (1, 2, 3, 4, 5, 10),
) -> Dict[int, float]:
    """Calculate serial correlations for multiple positive lags."""
    return {int(lag): serial_correlation(data, int(lag)) for lag in lags}


def repeated_patterns(
    data: bytes | bytearray | np.ndarray,
    pattern_length: int = 2,
    top_n: int = 10,
) -> list[Tuple[bytes, int]]:
    """
    Return the most frequent non-overlapping-independent n-byte patterns.

    Patterns are counted at every position, so overlapping occurrences count.
    """
    raw = _as_uint8_array(data).tobytes()

    if pattern_length <= 0:
        raise ValueError("pattern_length must be positive.")

    if top_n <= 0:
        raise ValueError("top_n must be positive.")

    if len(raw) < pattern_length:
        return []

    counter = Counter(
        raw[i : i + pattern_length]
        for i in range(len(raw) - pattern_length + 1)
    )

    return counter.most_common(top_n)


def longest_constant_run(
    data: bytes | bytearray | np.ndarray,
) -> Dict[str, int | bytes]:
    """Find the longest run of an identical byte value."""
    arr = _as_uint8_array(data)

    if arr.size == 0:
        raise ValueError("Input data is empty.")

    best_value = int(arr[0])
    best_length = 1
    current_value = int(arr[0])
    current_length = 1

    for value in arr[1:]:
        value = int(value)

        if value == current_value:
            current_length += 1
        else:
            if current_length > best_length:
                best_length = current_length
                best_value = current_value

            current_value = value
            current_length = 1

    if current_length > best_length:
        best_length = current_length
        best_value = current_value

    return {
        "byte_value": best_value,
        "run_length": best_length,
        "byte_hex": f"{best_value:02X}",
    }


def analyze_all(
    data: bytes | bytearray | np.ndarray,
) -> Dict[str, object]:
    """Run the main analyses and return one dictionary."""
    return {
        "basic_statistics": byte_statistics(data),
        "byte_frequencies": byte_frequencies(data),
        "bit_distribution": bit_distribution(data),
        "entropy_bits_per_byte": shannon_entropy(data),
        "chi_square": chi_square_test(data),
        "runs_test": runs_test(data),
        "serial_correlations": serial_correlations(data),
        "longest_constant_run": longest_constant_run(data),
        "top_2_byte_patterns": repeated_patterns(data, 2, 10),
        "top_3_byte_patterns": repeated_patterns(data, 3, 10),
        "top_4_byte_patterns": repeated_patterns(data, 4, 10),
    }


if __name__ == "__main__":
    print("analysis_engine.py loaded successfully.")
    print("Use the functions from report.ipynb.")

