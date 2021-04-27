#include <iomanip>
#include <iostream>
#include <filesystem>
#include <queue>
#include "MiniGit.hpp"

using namespace std;
namespace fs = std::filesystem;

int main(){
    string line;
    string input;
    doublyNode* Dtemp; //DLL Crawler
    singlyNode* Stemp; //SLL Crawler

    doublyNode* commit; //Creating the intial commit
    commit = new doublyNode;
    commit->commitNumber = 0;
    commit->head = NULL;
    commit->next = NULL;
    commit->previous = NULL;

//////////////////////////////////////////// PROGRAM START ////////////////////////////////////////////////////////
    cout << "Would you like to create a new repository? (Y/N)" << endl;
    getline(cin, line);
    if(line == "Y" || line == "y"){
        fs::remove_all(".minigit");         //Wipes existing .minigit and recreates it
        fs::create_directory(".minigit"); 
    }
    else{
        // loadCurrentMiniGit()
    }

    line = "0";
    while(stoi(line) != 5){
        cout << endl << "*/---------------------------------------/*" << endl;
        cout << "=========  MENU  =========" << endl;
        cout << "1) Add a file" << endl << endl;
        cout << "2) Remove a file" << endl << endl;
        cout << "3) Commit Changes" << endl << endl;
        cout << "4) Check Commit Number" << endl << endl;
        cout << "5) Quit" << endl;

        getline(cin, line);

        if(!((int)line[0] > 48 && (int)line[0] < 54)){ //Checking valid input
            cout << endl << "INVALID" << endl;
            line = "0";
        }
        else{

            switch(stoi(line)){ //User has entered a valid input

                case 1: //Adding a file                     
                    cout << endl << "Enter the file name: " << endl;
                    getline(cin, input); //input = filename

                    commit->addSLLFile(input);

                    commit->printSLL();
                    break;
 
                case 2: //Removing a file
                    cout << endl << "Enter the file name: " << endl;
                    getline(cin, input); //input = filename

                    commit->removeFile(input);

                    commit->printSLL();
                    break;


                case 3: //Committing changes
                    commit->commit(commit);
                    commit = commit->next;
                    break;


                case 4: //Checking a commit #
                    break;


                case 5: //Quit
                    break;


                default:
                    break;
            }
        }
    }
}