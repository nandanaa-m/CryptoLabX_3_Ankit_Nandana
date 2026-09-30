#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;


// ------------------------------------------------------------
// Function 1: Generate 5x5 Playfair Key Matrix
// ------------------------------------------------------------
vector<vector<char>> generate_key_matrix(string keyword)
{
    vector<vector<char>> matrix(5, vector<char>(5));
    vector<bool> used(26, false);

    string key;

    // Convert keyword to uppercase
    for (char ch : keyword)
    {
        if (isalpha(ch))
        {
            ch = toupper(ch);

            // Playfair treats I and J as same
            if (ch == 'J')
                ch = 'I';

            key += ch;
        }
    }

    string sequence = "";

    // Add unique characters from keyword
    for (char ch : key)
    {
        int index = ch - 'A';

        if (!used[index])
        {
            used[index] = true;
            sequence += ch;
        }
    }

    // J is excluded because I/J are combined
    used['J' - 'A'] = true;

    // Add remaining alphabet characters
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

    // Fill 5x5 matrix
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


// ------------------------------------------------------------
// Function 2: Prepare Plaintext
// ------------------------------------------------------------
string prepare_plaintext(string plaintext)
{
    string result = "";

    for (char ch : plaintext)
    {
        if (isalpha(ch))
        {
            ch = toupper(ch);

            // Combine J with I
            if (ch == 'J')
                ch = 'I';

            result += ch;
        }
    }

    return result;
}


// ------------------------------------------------------------
// Function 3: Create Digraphs
// ------------------------------------------------------------
vector<string> create_digraphs(string plaintext)
{
    vector<string> digraphs;

    int i = 0;

    while (i < plaintext.length())
    {
        char first = plaintext[i];

        // If only one character remains
        if (i + 1 >= plaintext.length())
        {
            digraphs.push_back(string(1, first) + "X");
            i++;
        }

        else
        {
            char second = plaintext[i + 1];

            // Repeated characters
            if (first == second)
            {
                digraphs.push_back(string(1, first) + "X");

                // Only move one position
                i++;
            }

            else
            {
                digraphs.push_back(string(1, first) +
                                   string(1, second));

                i += 2;
            }
        }
    }

    return digraphs;
}


// ------------------------------------------------------------
// Find position of character in matrix
// ------------------------------------------------------------
void find_position(const vector<vector<char>>& matrix,
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


// ------------------------------------------------------------
// Function 4: Playfair Encryption
// ------------------------------------------------------------
string playfair_encrypt(string plaintext,
                        const vector<vector<char>>& matrix)
{
    string prepared = prepare_plaintext(plaintext);

    vector<string> digraphs = create_digraphs(prepared);

    string ciphertext = "";

    for (string pair : digraphs)
    {
        char first = pair[0];
        char second = pair[1];

        int row1, col1;
        int row2, col2;

        find_position(matrix, first, row1, col1);
        find_position(matrix, second, row2, col2);

        // ----------------------------------------------------
        // Rule 1: Same Row
        // Move each character one position to the right
        // ----------------------------------------------------
        if (row1 == row2)
        {
            char encrypted1 =
                matrix[row1][(col1 + 1) % 5];

            char encrypted2 =
                matrix[row2][(col2 + 1) % 5];

            ciphertext += encrypted1;
            ciphertext += encrypted2;
        }

        // ----------------------------------------------------
        // Rule 2: Same Column
        // Move each character one position downward
        // ----------------------------------------------------
        else if (col1 == col2)
        {
            char encrypted1 =
                matrix[(row1 + 1) % 5][col1];

            char encrypted2 =
                matrix[(row2 + 1) % 5][col2];

            ciphertext += encrypted1;
            ciphertext += encrypted2;
        }

        // ----------------------------------------------------
        // Rule 3: Rectangle Rule
        // Take the other corners of the rectangle
        // ----------------------------------------------------
        else
        {
            char encrypted1 =
                matrix[row1][col2];

            char encrypted2 =
                matrix[row2][col1];

            ciphertext += encrypted1;
            ciphertext += encrypted2;
        }
    }

    return ciphertext;
}


// ------------------------------------------------------------
// Print Key Matrix
// ------------------------------------------------------------
void print_matrix(const vector<vector<char>>& matrix)
{
    cout << "Key Matrix:" << endl;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << matrix[i][j] << " ";
        }

        cout << endl;
    }
}


// ------------------------------------------------------------
// Main Function
// ------------------------------------------------------------
int main()
{
    string keyword;
    string plaintext;

    cout << "Enter keyword: ";
    getline(cin, keyword);

    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    // Generate matrix
    vector<vector<char>> matrix =
        generate_key_matrix(keyword);

    cout << endl;

    print_matrix(matrix);

    // Prepare plaintext
    string prepared =
        prepare_plaintext(plaintext);

    cout << endl;
    cout << "Prepared Plaintext: "
         << prepared << endl;

    // Create digraphs
    vector<string> digraphs =
        create_digraphs(prepared);

    cout << "Prepared Digraphs: ";

    for (string pair : digraphs)
    {
        cout << pair << " ";
    }

    cout << endl;

    // Encrypt
    string ciphertext =
        playfair_encrypt(plaintext, matrix);

    cout << "Ciphertext: "
         << ciphertext << endl;

    return 0;
}