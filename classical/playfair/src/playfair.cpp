#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>

using namespace std;

// Generate 5x5 Playfair Key Matrix
vector<vector<char>> generate_key_matrix(string keyword)
{
    vector<vector<char>> matrix(5, vector<char>(5));
    vector<bool> used(26, false);
    string sequence = "";

    for (char ch : keyword)
    {
        if (isalpha(ch))
        {
            ch = toupper(ch);

            if (ch == 'J')
                ch = 'I';

            int index = ch - 'A';

            if (!used[index])
            {
                used[index] = true;
                sequence += ch;
            }
        }
    }

    used['J' - 'A'] = true;

    for (char ch = 'A'; ch <= 'Z'; ch++)
    {
        if (ch == 'J')
            continue;

        int index = ch - 'A';

        if (!used[index])
        {
            used[index] = true;
            sequence += ch;
        }
    }

    int k = 0;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            matrix[i][j] = sequence[k++];
        }
    }

    return matrix;
}

// Prepare Plaintext
string prepare_plaintext(string plaintext)
{
    string result = "";

    for (char ch : plaintext)
    {
        if (isalpha(ch))
        {
            ch = toupper(ch);

            if (ch == 'J')
                ch = 'I';

            result += ch;
        }
    }

    return result;
}

// Create Digraphs
vector<string> create_digraphs(string plaintext)
{
    vector<string> digraphs;
    int i = 0;

    while (i < plaintext.length())
    {
        char first = plaintext[i];

        if (i + 1 >= plaintext.length())
        {
            digraphs.push_back(string(1, first) + "X");
            i++;
        }
        else
        {
            char second = plaintext[i + 1];

            if (first == second)
            {
                digraphs.push_back(string(1, first) + "X");
                i++;
            }
            else
            {
                digraphs.push_back(
                    string(1, first) + string(1, second)
                );
                i += 2;
            }
        }
    }

    return digraphs;
}

// Find position in matrix
void find_position(
    const vector<vector<char>>& matrix,
    char ch,
    int& row,
    int& col)
{
    if (ch == 'J')
        ch = 'I';

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (matrix[i][j] == ch)
            {
                row = i;
                col = j;
                return;
            }
        }
    }
}

// Playfair Encryption
string playfair_encrypt(
    string plaintext,
    const vector<vector<char>>& matrix)
{
    string prepared = prepare_plaintext(plaintext);
    vector<string> digraphs = create_digraphs(prepared);

    string ciphertext = "";

    for (const string& pair : digraphs)
    {
        char first = pair[0];
        char second = pair[1];

        int row1, col1, row2, col2;

        find_position(matrix, first, row1, col1);
        find_position(matrix, second, row2, col2);

        // Same row
        if (row1 == row2)
        {
            ciphertext += matrix[row1][(col1 + 1) % 5];
            ciphertext += matrix[row2][(col2 + 1) % 5];
        }

        // Same column
        else if (col1 == col2)
        {
            ciphertext += matrix[(row1 + 1) % 5][col1];
            ciphertext += matrix[(row2 + 1) % 5][col2];
        }

        // Rectangle rule
        else
        {
            ciphertext += matrix[row1][col2];
            ciphertext += matrix[row2][col1];
        }
    }

    return ciphertext;
}

// Playfair Decryption
string playfair_decrypt(
    string ciphertext,
    const vector<vector<char>>& matrix)
{
    string decrypted = "";

    for (size_t i = 0; i + 1 < ciphertext.length(); i += 2)
    {
        char first = ciphertext[i];
        char second = ciphertext[i + 1];

        int row1, col1, row2, col2;

        find_position(matrix, first, row1, col1);
        find_position(matrix, second, row2, col2);

        // Same row
        if (row1 == row2)
        {
            decrypted += matrix[row1][(col1 + 4) % 5];
            decrypted += matrix[row2][(col2 + 4) % 5];
        }

        // Same column
        else if (col1 == col2)
        {
            decrypted += matrix[(row1 + 4) % 5][col1];
            decrypted += matrix[(row2 + 4) % 5][col2];
        }

        // Rectangle rule
        else
        {
            decrypted += matrix[row1][col2];
            decrypted += matrix[row2][col1];
        }
    }

    return decrypted;
}

// Digraph Frequency Analysis
void digraph_frequency(string ciphertext)
{
    map<string, int> frequency;

    for (size_t i = 0; i + 1 < ciphertext.length(); i += 2)
    {
        string digraph = ciphertext.substr(i, 2);
        frequency[digraph]++;
    }

    cout << "Digraph Frequencies:" << endl;

    for (const auto& pair : frequency)
    {
        cout << pair.first << ": "
             << pair.second << endl;
    }
}

// Verification
void verify(
    string ciphertext,
    const vector<vector<char>>& matrix)
{
    string decrypted = playfair_decrypt(ciphertext, matrix);
    string re_encrypted = playfair_encrypt(decrypted, matrix);

    cout << "Verification: ";

    if (re_encrypted == ciphertext)
        cout << "SUCCESS" << endl;
    else
        cout << "FAILED" << endl;
}

// Print Matrix
void print_matrix(const vector<vector<char>>& matrix)
{
    cout << "Key Matrix:" << endl;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << matrix[i][j];

            if (j < 4)
                cout << " ";
        }

        cout << endl;
    }
}

// Main
int main()
{
    string keyword;
    string plaintext;

    cout << "Keyword: ";
    getline(cin, keyword);

    cout << "Plaintext: ";
    getline(cin, plaintext);

    cout << endl;

    vector<vector<char>> matrix =
        generate_key_matrix(keyword);

    print_matrix(matrix);

    cout << endl;

    string prepared =
        prepare_plaintext(plaintext);

    vector<string> digraphs =
        create_digraphs(prepared);

    cout << "Prepared Digraphs: ";

    for (size_t i = 0; i < digraphs.size(); i++)
    {
        cout << digraphs[i];

        if (i < digraphs.size() - 1)
            cout << " ";
    }

    cout << endl;

    string ciphertext =
        playfair_encrypt(plaintext, matrix);

    cout << "Ciphertext: "
         << ciphertext << endl;

    string decrypted =
        playfair_decrypt(ciphertext, matrix);

    cout << "Decrypted Text: "
         << decrypted << endl;

    cout << endl;

    digraph_frequency(ciphertext);

    cout << endl;

    verify(ciphertext, matrix);

    return 0;
}