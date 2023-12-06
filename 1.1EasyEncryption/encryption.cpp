#include "encryption.h"
#include <fstream> //for file input/output
#include <cctype> //for checking upper or lower case alphabets

using namespace std;

bool performCaesarCipher(string& content, bool encrypt){
    //hard coded shift key
    int shift = encrypt? 3 : -3; //encrypt for 3 and decrypt for -3

    for(char& ch:content){
        if(isalpha(ch)){
            char base = isupper(ch)? 'A' : 'a';
            ch = static_cast<char>((ch - base + shift + 26) % 26 + base);
        }
    }

    return true;
}

bool encryptFile(const string& filename, bool encrypt){
    
    //Open input file with an input file handler
    ifstream inFile(filename);

    //if there's a file issue
    if(!inFile){
        return false;
    }

    //Read the content of the file: 
    //for every char, read it as a string and put it into inFile
    string content((istreambuf_iterator<char>(inFile)), {});
    //Close file
    inFile.close();

    //Perform Caesar Cipher function
    if(performCaesarCipher(content, encrypt)){
        //Create an output file and writing the modified content (either encrypt or decrypt)
        ofstream outFile(encrypt? "encrypted_" + filename : "decrypted_" + filename);

        //if output file is not created, return false
        if(!outFile) {
            return false;
        }

        //Write all content to output file
        outFile << content; //content will modified by reference in performCaesarCipher function

        //Close output file
        outFile.close();

    }

        //Cipher success
        return true;
}