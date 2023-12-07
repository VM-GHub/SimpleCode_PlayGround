#include <iostream>
#include <string> //to use getline()
#include "encryption.h"

using namespace std;

//Simple program that encrypt lower and upper case letters
//using Caeser Shift Cipher technique: 
//sample text = abc, shift key = 3, then encrypt to = def; 
//decode by shift key = -3, from def back to abc
//For fun, not for practical usage
int main(){

    //file name we try to read
    string filename; //same as std::string filename;
    
    //Mode is either encrypt mode or decrypt mode
    char mode;

    //Loop flag
    char play;
    bool end{false};

    while(end!=true){
    //Begin with getting file name from the user
    cout << "Enter the input file name : ";
    //Get more than one line and avoid wide space
    getline(cin >> ws, filename);//ws is same as std::ws

    //Prompt to choose a mode
    cout << "Encrypt (e) or Decrypt (d)? ";
    cin >> mode;

    //Check if mode is legit
    if((mode != 'e') & (mode != 'E') & (mode != 'd') & (mode != 'D')){
        cout << "Invalid mode, please enter a correct mode.\n";
        cout << "Use 'e' for encryption or 'd' for decryption.  ";
    } 
    else if((mode == 'e') || (mode == 'E')){//Encrypt mode is true
        if(encryptFile(filename, true)){
            cout << "Encryption completed successfully. \n";
        }else{
            cerr << "Error : Unable to perform encryption. \n";
        }
    } 
    else if((mode == 'd') || (mode == 'D')){
        if(encryptFile(filename, false)){
            cout << "Decryption completed successfully. \n";
        }else{
            cerr << "Error : Unable to perform decryption. \n";
        }
    }

    cout << "Continue to cipher? Yes (y) or No (n)";
    cin >> play;
    end = (play == 'y'|| play == 'Y')? false : true;
    }
    

    return 0;
}
