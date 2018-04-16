/* CSI 3334
* Filename: main.cpp
* Student name: Ian Laird
* Project 4 - Balanced tree-based encryption and decryption
* Due date: 10/20/17
* Version: 1.0
*
* This file contains the driver for the encryption and decryption using an AVL tree.
*/
const int STRING_STARTING_POS = 0,
        SPACE_AND_QUOTATION_MARK = 2;
#include "Red-Black-Implementation.h"

/**
 * main
 *
 * This function includes the interface through which data can be encrypted or decrypted
 * using an AVL tree. It reads in data from standard in, and outputs data to standard out.
 *
 * Parameters:
 *     argc: the number of c-strings passed to the function
 *         This is by default 1
 *     argv: an array of c-style strings which are the function arguments.
 *
 * Return:
 *     0 indicates a successful execution
 */
int main() {
    char commandLetter;
    string word, word2;
    string const * decrypted;
    queue<string> toPrint;
    RBTree<string> tree;

    while(cin >> commandLetter && commandLetter != 'q'){
        if(commandLetter == 'i' || commandLetter == 'r'){
            cin >> word;
            if(commandLetter == 'i')
                tree.insert(word);
            else
                tree.remove(word);
        }
        else if(commandLetter == 'e' || commandLetter == 'd'){
            getline(cin, word);
            //Remove the leading space and apostrophe from the word
            word.erase(STRING_STARTING_POS, SPACE_AND_QUOTATION_MARK);
            // Remove the ' at the end of the string
            word.pop_back();

            for(int i = 0; i < word.length(); i++ ){
                if(!isspace(word[i])){
                    word2.push_back(word[i]);
                }
                if(isspace(word[i]) || (i == word.length() - 1)){
                    if(commandLetter == 'e')
                        //toPrint.push(tree.encrypt(word2));
                    //else {
                        //decrypted = tree.decrypt(word2);
                        //if (decrypted != NULL)
                          //  toPrint.push(*decrypted);
                        //else toPrint.push("?");
                    //}
                    word2 = "";
                }
            }
            while(!toPrint.empty()){
                cout << toPrint.front();
                toPrint.pop();
                if(!toPrint.empty())
                    cout << " ";
            }
            cout << endl;
        }
        else if( commandLetter == 'p')
            tree.printPreorder();
        //else if(commandLetter == 'l')
            //tree.printLevelOrder();
    }
    return 0;
}