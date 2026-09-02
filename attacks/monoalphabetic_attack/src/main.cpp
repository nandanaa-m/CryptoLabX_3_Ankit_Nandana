#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include<bits/stdc++.H>
using namespace std;

string current_ciphertext = "";
string current_partial_plaintext = "";
unordered_map<char, char> active_substitution;

// --- UTILITY FUNCTIONS ---
void init_substitution() {
    active_substitution.clear();
    for (char c = 'A'; c <= 'Z'; c++) 
        active_substitution[c] = '_';
}

string read_file(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) 
        return "";

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 0. ENCRYPTION MODULE (To generate ciphertext from the book excerpt)
string generate_random_cipher(const string& plaintext) {
    vector<char> alphabet;
    for(char c = 'A'; c <= 'Z'; c++) 
        alphabet.push_back(c);
    
    vector<char> shuffled = alphabet;
    srand(time(0));
    random_shuffle(shuffled.begin(), shuffled.end());
    
    unordered_map<char, char> secret_key;
    for(size_t i = 0; i < alphabet.size(); i++) 
        secret_key[alphabet[i]] = shuffled[i];
    
    string ciphertext = "";
    for(char c : plaintext) {
        if(isalpha(c)) 
            ciphertext += secret_key[toupper(c)];
        else 
            ciphertext += c;
    }
    return ciphertext;
}

// 1. FREQUENCY ANALYSIS
void frequency_analysis(const string& ciphertext) {
    unordered_map<char, int> freq_map;
    int total = 0;
    for (char c : ciphertext) 
        if (isalpha(c)) { freq_map[toupper(c)]++; total++; }

    vector<pair<char, int>> freq_vec(freq_map.begin(), freq_map.end());
    sort(freq_vec.begin(), freq_vec.end(), [](const auto& a, const auto& b) { return b.second < a.second; });

    cout << "\n--- 1. Letter Frequency Analysis ---" << endl;
    cout << setw(10) << "Letter" << setw(10) << "Count" << setw(15) << "Percentage" << endl;
    for (const auto& p : freq_vec) {
        double pct = total > 0 ? (double)p.second / total * 100.0 : 0.0;
        cout << setw(10) << p.first << setw(10) << p.second << setw(14) << fixed << setprecision(2) << pct << "%" << endl;
    }
}

// 2. WORD FREQUENCY
void word_frequency_analysis(const string& ciphertext) {
    stringstream ss(ciphertext);
    string word;
    unordered_map<string, int> one_letter, two_letter, three_letter;

    while (ss >> word) {
        string clean = "";
        for (char c : word){
            if (isalpha(c)) 
                clean += toupper(c);
        }
        
        if (clean.length() == 1) 
            one_letter[clean]++;
        else if (clean.length() == 2) 
            two_letter[clean]++;
        else if (clean.length() == 3) 
            three_letter[clean]++;
    }

    cout << "\n--- 2. Word Frequency ---" << endl;
    cout << "[*] Top 1-Letter Words: ";
    for (auto& p : one_letter) {
        if (p.second > 1) 
            cout << p.first << "(" << p.second << ") ";
    }

    cout << "\n[*] Top 2-Letter Words: ";
    for (auto& p : two_letter) {
        if (p.second > 2) 
            cout << p.first << "(" << p.second << ") ";
    
    }

    cout << "\n[*] Top 3-Letter Words: ";
    for (auto& p : three_letter){
        if (p.second > 2) 
            cout << p.first << "(" << p.second << ") ";
    }
    cout << endl;
}

// 3. PATTERN ANALYSIS
void pattern_analysis(const string& ciphertext) {
    cout << "\n--- 3. Repeated Letter Patterns ---" << endl;
    stringstream ss(ciphertext);
    string word;
    bool found_any = false;

    while (ss >> word) {
        string clean = "";
        for (char c : word) if (isalpha(c)) clean += toupper(c);
        
        if (clean.length() < 2) continue;

        // Check for double letters (e.g., 'SS' in 'MESSAGE')
        for (size_t i = 0; i < clean.length() - 1; i++) {
            if (clean[i] == clean[i+1]) {
                cout << "  [Double Letter] found in word: " << clean << " (Pattern: " << clean[i] << clean[i+1] << ")" << endl;
                found_any = true;
                break; // Move to the next word once we find a double letter
            }
        }

        // Check for ABBA pattern (4 letters)
        if (clean.length() == 4 && clean[0] == clean[3] && clean[1] == clean[2] && clean[0] != clean[1]) {
            cout << "  [ABBA] Pattern found in word: " << clean << endl;
            found_any = true;
        }
    }

    if (!found_any) {
        cout << "  [!] No common repeating patterns (Double letters or ABBA) found." << endl;
    }
}

// 4. APPLY SUBSTITUTION
void apply_substitution(char cipher_char, char plain_char) {
    active_substitution[toupper(cipher_char)] = toupper(plain_char);
    cout << "[+] Mapped '" << toupper(cipher_char) << "' -> '" << toupper(plain_char) << "'" << endl;
}

// 5. DISPLAY PARTIAL PLAINTEXT
void display_partial_plaintext(const string& ciphertext) {
    current_partial_plaintext = "";
    for (char c : ciphertext) {
        if (isalpha(c)) {
            char upper = toupper(c);
            char mapped = active_substitution[upper];
            current_partial_plaintext += (mapped != '_') ? (islower(c) ? tolower(mapped) : mapped) : '_';
        } 
        else 
            current_partial_plaintext += c;
    }
    cout << "\n--- Current Text State ---\n" << current_partial_plaintext << "\n--------------------------\n";
}

// 6. VERIFY SOLUTION
void verify_solution() {
    cout << "\n--- 6. Solution Verification ---" << endl;
    int mapped_count = 0;
    
    cout << "Current Key Mapping:" << endl;
    for (char c = 'A'; c <= 'Z'; c++) {
        if (active_substitution[c] != '_') {
            cout << c << " -> " << active_substitution[c] << "   ";
            mapped_count++;
            if (mapped_count % 6 == 0) cout << endl; // Just for neat formatting
        }
    }
    
    cout << "\n\nTotal letters mapped: " << mapped_count << " / 26" << endl;
    
    if (mapped_count == 26) {
        cout << "[+] Full substitution key unlocked successfully! Decryption complete." << endl;
    } else {
        cout << "[!] Key is incomplete. " << (26 - mapped_count) << " letters remaining." << endl;
    }
    cout << "--------------------------------\n";
}

int main() {
    init_substitution();
    int choice;
    
    do {
        cout << "\n=== MONOALPHABETIC CRYPTANALYSIS ===" << endl;
        cout << "1. Load Plaintext from File & Generate Ciphertext" << endl;
        cout << "2. Run Letter Frequency Analysis" << endl;
        cout << "3. Run Word Frequency Analysis" << endl;
        cout << "4. Run Pattern Analysis" << endl;
        cout << "5. Apply Substitution Hypothesis" << endl;
        cout << "6. View Partial Plaintext" << endl;
        cout << "7. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string path;
            cout << "Enter path to textbook excerpt (e.g., ../testcases/sample.txt): ";
            cin >> path;
            string plaintext = read_file(path);
            if(plaintext.empty()) {
                cout << "[!] Could not read file. Check the path." << endl;
            } else {
                current_ciphertext = generate_random_cipher(plaintext);
                cout << "[+] Ciphertext generated successfully! Ready for analysis." << endl;
            }
        }
        else if (choice == 2) frequency_analysis(current_ciphertext);
        else if (choice == 3) word_frequency_analysis(current_ciphertext);
        else if (choice == 4) pattern_analysis(current_ciphertext);
        else if (choice == 5) {
            char c, p;
            cout << "Cipher letter: "; cin >> c;
            cout << "Plain letter: "; cin >> p;
            apply_substitution(c, p);
        }
        else if (choice == 6) display_partial_plaintext(current_ciphertext);
        
    } while (choice != 7);

return 0;
}