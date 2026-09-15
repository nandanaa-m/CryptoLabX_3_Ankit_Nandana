# CryptoLabX

## Overview

CryptoLabX is a modular cryptography toolkit developed as part of the Cryptography Laboratory course (22CPP307). The project is continuously expanding into a complete cryptanalysis framework supporting classical and modern cryptographic algorithms, statistical attacks, and analysis tools.

## Team Members

- Ankit Verma (2024UCP1794)
- Nandana Manoj (2024UCP1909)

---

## Folder Structure

```text
CryptoLabX/
│
├── analysis/            # File and frequency analysis tools
├── attacks/             # Cryptanalysis algorithms (includes screenshots/)
├── classical/           # Classical cipher implementations
├── datasets/            # Sample text files and documentation
├── docs/                # Project documentation
├── kasiski_analysis/    # Vigenère cipher breaking using Kasiski & Frequency Analysis
├── math/                # Mathematical utilities
├── modern/              # Modern cryptographic algorithms
├── outputs/             # Generated execution logs and output files
├── secure_application/  # Shift cipher attack and application modules
├── testdata/            # Sample datasets for testing algorithms
├── tests/               # Unit tests
├── utils/               # Helper functions
├── main.py              # Main Python application
├── requirements.txt     # Python dependencies
└── .gitignore           # Ignored files

Current Features
Project Infrastructure: Modular folder structure, command-line interface, and execution logging.
Classical Attacks:
Monoalphabetic Substitution Cryptanalysis (Frequency & Pattern Analysis)
Vigenère Cipher Cryptanalysis (Kasiski Examination & Chi-Squared Frequency Analysis)
Shift Cipher Attacks
Analysis Tools: File processing, text sanitization, and automated execution logging.
Datasets: Integrated sample text files for testing algorithms.

Technologies & Tools Used
C++: High-performance cryptanalysis algorithms (Monoalphabetic, Kasiski).
Python 3: Main application and utility scripts.
Git & GitHub: Version control, collaboration, and repository management.