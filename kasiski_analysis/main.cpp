#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <numeric>


using namespace std;

// ------------------------------------------------------------
// Function: clean_ciphertext
// Purpose : Remove spaces/special characters and normalize
//           ciphertext to uppercase A-Z.
// ------------------------------------------------------------
string clean_ciphertext(const string& ciphertext) {
    string cleaned;

    for (char c : ciphertext) {
        if (isalpha(static_cast<unsigned char>(c))) {
            cleaned += static_cast<char>(
                toupper(static_cast<unsigned char>(c))
            );
        }
    }

    return cleaned;
}

// ------------------------------------------------------------
// Function: find_repeated_patterns
// Purpose : Identify repeated sequences in ciphertext.
// ------------------------------------------------------------
map<string, vector<int>> find_repeated_patterns(
    const string& ciphertext,
    int patternLength = 3
) {
    map<string, vector<int>> occurrences;

    if (ciphertext.length() < static_cast<size_t>(patternLength)) {
        return occurrences;
    }

    for (size_t i = 0; i + patternLength <= ciphertext.length(); ++i) {
        string pattern = ciphertext.substr(i, patternLength);
        occurrences[pattern].push_back(static_cast<int>(i));
    }

    // Keep only patterns that occur more than once.
    for (auto it = occurrences.begin(); it != occurrences.end();) {
        if (it->second.size() < 2) {
            it = occurrences.erase(it);
        } else {
            ++it;
        }
    }

    return occurrences;
}

// ------------------------------------------------------------
// Function: calculate_distances
// Purpose : Find distances between repeated occurrences.
// ------------------------------------------------------------
vector<int> calculate_distances(
    const map<string, vector<int>>& repeatedPatterns
) {
    vector<int> distances;

    for (const auto& entry : repeatedPatterns) {
        const vector<int>& positions = entry.second;

        for (size_t i = 0; i < positions.size(); ++i) {
            for (size_t j = i + 1; j < positions.size(); ++j) {
                distances.push_back(positions[j] - positions[i]);
            }
        }
    }

    return distances;
}

// ------------------------------------------------------------
// Function: find_factors
// Purpose : Find factors of distances obtained from patterns.
// ------------------------------------------------------------
map<int, int> find_factors(
    const vector<int>& distances,
    int maxKeyLength = 20
) {
    map<int, int> factorCounts;

    for (int distance : distances) {
        for (int factor = 2; factor <= maxKeyLength; ++factor) {
            if (distance % factor == 0) {
                factorCounts[factor]++;
            }
        }
    }

    return factorCounts;
}

// ------------------------------------------------------------
// Function: calculate_ic
// Purpose : Calculate Index of Coincidence.
// ------------------------------------------------------------
double calculate_ic(const string& text) {
    if (text.length() < 2) {
        return 0.0;
    }

    int frequencies[26] = {0};

    for (char c : text) {
        if (c >= 'A' && c <= 'Z') {
            frequencies[c - 'A']++;
        }
    }

    long long numerator = 0;
    long long n = static_cast<long long>(text.length());

    for (int count : frequencies) {
        numerator += static_cast<long long>(count) * (count - 1);
    }

    return static_cast<double>(numerator) /
           static_cast<double>(n * (n - 1));
}

// ------------------------------------------------------------
// Function: split_into_groups
// Purpose : Divide ciphertext according to candidate key length.
// ------------------------------------------------------------
vector<string> split_into_groups(
    const string& ciphertext,
    int keyLength
) {
    vector<string> groups(keyLength);

    for (size_t i = 0; i < ciphertext.length(); ++i) {
        groups[i % keyLength] += ciphertext[i];
    }

    return groups;
}

// ------------------------------------------------------------
// Function: frequency_analysis
// Purpose : Calculate A-Z frequency for each group.
// ------------------------------------------------------------
vector<int> frequency_analysis(const string& group) {
    vector<int> frequency(26, 0);

    for (char c : group) {
        if (c >= 'A' && c <= 'Z') {
            frequency[c - 'A']++;
        }
    }

    return frequency;
}

// ------------------------------------------------------------
// English letter frequencies.
// ------------------------------------------------------------
const double ENGLISH_FREQ[26] = {
    0.08167, // A
    0.01492, // B
    0.02782, // C
    0.04253, // D
    0.12702, // E
    0.02228, // F
    0.02015, // G
    0.06094, // H
    0.06966, // I
    0.00153, // J
    0.00772, // K
    0.04025, // L
    0.02406, // M
    0.06749, // N
    0.07507, // O
    0.01929, // P
    0.00095, // Q
    0.05987, // R
    0.06327, // S
    0.09056, // T
    0.02758, // U
    0.00978, // V
    0.02360, // W
    0.00150, // X
    0.01974, // Y
    0.00074  // Z
};

// ------------------------------------------------------------
// Function: find_shift
// Purpose : Estimate Caesar shift for one group.
//
// Uses chi-squared statistical comparison against English
// letter frequencies.
// ------------------------------------------------------------
int find_shift(const string& group) {
    if (group.empty()) {
        return 0;
    }

    vector<int> frequency = frequency_analysis(group);

    int n = static_cast<int>(group.length());

    double bestScore = 1e100;
    int bestShift = 0;

    // Try all 26 possible Caesar shifts.
    for (int shift = 0; shift < 26; ++shift) {
        double chiSquare = 0.0;

        for (int cipherLetter = 0; cipherLetter < 26; ++cipherLetter) {
            // If ciphertext letter is C and shift is K,
            // plaintext = C - K.
            int plainLetter = (cipherLetter - shift + 26) % 26;

            double expected =
                ENGLISH_FREQ[plainLetter] * n;

            if (expected > 0) {
                chiSquare +=
                    pow(frequency[cipherLetter] - expected, 2) /
                    expected;
            }
        }

        if (chiSquare < bestScore) {
            bestScore = chiSquare;
            bestShift = shift;
        }
    }

    return bestShift;
}

// ------------------------------------------------------------
// Function: find_key
// Purpose : Combine shifts to obtain probable Vigenere key.
// ------------------------------------------------------------
string find_key(const vector<string>& groups) {
    string key;

    for (const string& group : groups) {
        int shift = find_shift(group);
        key += static_cast<char>('A' + shift);
    }

    return key;
}

// ------------------------------------------------------------
// Function: vigenere_decrypt
// Purpose : Decrypt ciphertext using recovered key.
// ------------------------------------------------------------
string vigenere_decrypt(
    const string& ciphertext,
    const string& key
) {
    string plaintext;

    if (key.empty()) {
        return plaintext;
    }

    for (size_t i = 0; i < ciphertext.length(); ++i) {
        int c = ciphertext[i] - 'A';
        int k = key[i % key.length()] - 'A';

        int p = (c - k + 26) % 26;

        plaintext += static_cast<char>('A' + p);
    }

    return plaintext;
}

// ------------------------------------------------------------
// Function: vigenere_encrypt
// Purpose : Re-encrypt plaintext for verification.
// ------------------------------------------------------------
string vigenere_encrypt(
    const string& plaintext,
    const string& key
) {
    string ciphertext;

    if (key.empty()) {
        return ciphertext;
    }

    for (size_t i = 0; i < plaintext.length(); ++i) {
        int p = plaintext[i] - 'A';
        int k = key[i % key.length()] - 'A';

        int c = (p + k) % 26;

        ciphertext += static_cast<char>('A' + c);
    }

    return ciphertext;
}

// ------------------------------------------------------------
// Function: verify
// Purpose : Check whether re-encryption produces original.
// ------------------------------------------------------------
bool verify(
    const string& originalCiphertext,
    const string& encryptedCiphertext
) {
    return originalCiphertext == encryptedCiphertext;
}

// ------------------------------------------------------------
// Function: kasiski_analysis
// Purpose : Use repeated patterns/distances to suggest
//           candidate key lengths.
// ------------------------------------------------------------
vector<int> kasiski_analysis(
    const string& ciphertext,
    int maxKeyLength = 20
) {
    map<int, int> totalFactorCounts;

    // Analyze trigrams, tetragrams and pentagrams.
    for (int patternLength = 3; patternLength <= 5; ++patternLength) {
        auto repeatedPatterns =
            find_repeated_patterns(ciphertext, patternLength);

        vector<int> distances =
            calculate_distances(repeatedPatterns);

        map<int, int> factors =
            find_factors(distances, maxKeyLength);

        for (const auto& entry : factors) {
            totalFactorCounts[entry.first] += entry.second;
        }
    }

    // Sort candidates according to factor frequency.
    vector<pair<int, int>> candidates(
        totalFactorCounts.begin(),
        totalFactorCounts.end()
    );

    sort(
        candidates.begin(),
        candidates.end(),
        [](const pair<int, int>& a,
           const pair<int, int>& b) {
            return a.second > b.second;
        }
    );

    vector<int> keyLengths;

    for (const auto& candidate : candidates) {
        keyLengths.push_back(candidate.first);
    }

    return keyLengths;
}

// ------------------------------------------------------------
// Print frequency table.
// ------------------------------------------------------------
void print_frequency_table(
    const vector<string>& groups
) {
    cout << "\n========== FREQUENCY TABLES ==========\n";

    for (size_t i = 0; i < groups.size(); ++i) {
        vector<int> frequencies =
            frequency_analysis(groups[i]);

        cout << "\nGroup " << i + 1
             << " (Key position " << i + 1 << ")\n";

        cout << "Text: " << groups[i] << "\n";

        cout << "Letter  Frequency\n";
        cout << "-----------------\n";

        for (int j = 0; j < 26; ++j) {
            cout << "  "
                 << static_cast<char>('A' + j)
                 << "       "
                 << frequencies[j]
                 << "\n";
        }
    }
}

// ------------------------------------------------------------
// Print repeated patterns.
// ------------------------------------------------------------
void print_repeated_patterns(
    const string& ciphertext
) {
    cout << "\n========== REPEATED PATTERNS ==========\n";

    for (int length = 3; length <= 5; ++length) {
        auto patterns =
            find_repeated_patterns(ciphertext, length);

        cout << "\nPatterns of length "
             << length << ":\n";

        for (const auto& entry : patterns) {
            cout << entry.first << " -> ";

            for (size_t i = 0; i < entry.second.size(); ++i) {
                cout << entry.second[i];

                if (i + 1 < entry.second.size()) {
                    cout << ", ";
                }
            }

            cout << "\n";
        }
    }
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main() {

    cout << "====================================================\n";
    cout << " VIGENERE CIPHER CRYPTANALYSIS\n";
    cout << " KASISKI EXAMINATION + FREQUENCY ANALYSIS\n";
    cout << "====================================================\n";

    // --------------------------------------------------------
    // Take ciphertext from a text file
    // --------------------------------------------------------
    string filename;

    cout << "\nEnter ciphertext file name: ";
    cin >> filename;

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file: "
             << filename << "\n";
        return 1;
    }

    string input;
    string line;

    while (getline(file, line)) {
        input += line;
        input += '\n';
    }

    file.close();

    // --------------------------------------------------------
    // 1. Preprocess ciphertext
    // --------------------------------------------------------
    string ciphertext = clean_ciphertext(input);

    if (ciphertext.empty()) {
        cerr << "Error: Ciphertext file is empty "
             << "or contains no alphabetic characters.\n";
        return 1;
    }

    cout << "\n========== PREPROCESSED CIPHERTEXT ==========\n";
    cout << ciphertext << "\n";

    cout << "\nCiphertext length: "
         << ciphertext.length() << "\n";

    // --------------------------------------------------------
    // 2. Kasiski examination
    // --------------------------------------------------------
    print_repeated_patterns(ciphertext);

    vector<int> candidates =
        kasiski_analysis(ciphertext, 20);

    cout << "\n========== KASISKI CANDIDATE KEY LENGTHS ==========\n";

    if (candidates.empty()) {
        cout << "No candidate key lengths found.\n";
        return 0;
    }

    for (size_t i = 0; i < candidates.size(); ++i) {
        cout << candidates[i];

        if (i + 1 < candidates.size()) {
            cout << ", ";
        }
    }

    cout << "\n";

    // --------------------------------------------------------
    // 3. Index of Coincidence
    // --------------------------------------------------------
    cout << "\n========== INDEX OF COINCIDENCE ==========\n";

    int bestKeyLength = candidates[0];
    double bestICDifference = 1e100;

    const double targetIC = 0.066;

    for (int keyLength : candidates) {

        if (keyLength <= 0 ||
            keyLength > static_cast<int>(ciphertext.length())) {
            continue;
        }

        vector<string> groups =
            split_into_groups(ciphertext, keyLength);

        double totalIC = 0.0;

        for (const string& group : groups) {
            totalIC += calculate_ic(group);
        }

        double averageIC =
            totalIC / groups.size();

        cout << "Key length "
             << keyLength
             << " -> Average IC = "
             << averageIC << "\n";

        double difference =
            abs(averageIC - targetIC);

        if (difference < bestICDifference) {
            bestICDifference = difference;
            bestKeyLength = keyLength;
        }
    }

    cout << "\nEstimated key length: "
         << bestKeyLength << "\n";

    // --------------------------------------------------------
    // 4. Divide ciphertext into groups
    // --------------------------------------------------------
    vector<string> groups =
        split_into_groups(ciphertext, bestKeyLength);

    // --------------------------------------------------------
    // 5. Frequency analysis
    // --------------------------------------------------------
    print_frequency_table(groups);

    // --------------------------------------------------------
    // 6. Determine probable key
    // --------------------------------------------------------
    string key = find_key(groups);

    cout << "\n========== RECOVERED KEY ==========\n";
    cout << key << "\n";

    // --------------------------------------------------------
    // 7. Decrypt ciphertext
    // --------------------------------------------------------
    string plaintext =
        vigenere_decrypt(ciphertext, key);

    cout << "\n========== RECOVERED PLAINTEXT ==========\n";

    for (size_t i = 0; i < plaintext.length(); i += 80) {
        cout << plaintext.substr(i, 80) << "\n";
    }

    // --------------------------------------------------------
    // 8. Re-encrypt plaintext
    // --------------------------------------------------------
    string encrypted =
        vigenere_encrypt(plaintext, key);

    // --------------------------------------------------------
    // 9. Verify
    // --------------------------------------------------------
    bool result =
        verify(ciphertext, encrypted);

    cout << "\n========== VERIFICATION ==========\n";

    if (result) {
        cout << "SUCCESS: Re-encrypted ciphertext matches "
             << "the original ciphertext.\n";
    } else {
        cout << "FAILED: Re-encrypted ciphertext does NOT "
             << "match the original ciphertext.\n";
    }

    cout << "\nOriginal ciphertext length : "
         << ciphertext.length() << "\n";

    cout << "Encrypted ciphertext length: "
         << encrypted.length() << "\n";

    return 0;
}

