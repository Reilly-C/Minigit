#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "MiniGit.hpp"

using namespace std;
namespace fs = std::filesystem;

//Add File Functions
void doublyNode::addSLLFile(string in){

    singlyNode* temp = new singlyNode; // Temp is node to be added
    singlyNode* crawler = head;
    temp->fileName = in;
    temp->fileVersion = "00";
    temp->next = NULL;

    bool fileExists = false;

    string path = "Files";

    while(!fileExists){
        for (const auto & entry : fs::directory_iterator(path)){ //Checks to see if the fileName given exists in the directory of files
            if("Files\\" + in == entry.path()){
                fileExists = true;
            }
        }

        if(!fileExists){
            cout << endl << "File Not Found. Enter new file name (or \"quit\"): " << endl;
            getline(cin, in);
            temp->fileName = in;

            if(in == "quit"){
                break;
            }
        }
    }

    if(fileExists){ //Adds file to SLL node
    bool toBeAdded = true;
        if(crawler == NULL){
            head = temp;
            cout << endl << "File added" << endl;
        }
        else if(head->fileName == in){
            cout << endl << "File already added" << endl;
            toBeAdded = false;
        }
        else{
            while(crawler->next != NULL){
                if(crawler->fileName == in){
                    cout << endl << "File already added" << endl;
                    toBeAdded = false;
                }
                crawler = crawler->next;
            }
            if(crawler->fileName == in){ //Checks last node in chain
                cout << endl << "File already added" << endl;
                toBeAdded = false;
            }  

            if(toBeAdded){  //Adding file to chain
                crawler->next = temp;
                cout << endl << "File added" << endl; 
            } 
        }  
    }
    else{
        cout << endl << "No File Added" << endl;
    }   
}

//Remove File Functions
void doublyNode::removeFile(string in){

    singlyNode* prev = head;
    singlyNode* temp = head;

    //No Nodes
    if(temp == nullptr){
        cout << endl << "Error:: File list empty" << endl;
        return;
    }
    //Head node
    if(temp->fileName == in){
        head = head->next;
        delete temp;
        cout << endl << "File deleted" << endl;
        return;
    }
    //Midlle Node
    
    else{
        while(temp != nullptr){
            if(temp->fileName == in){
    // Checking If Last Node
                if(temp->next == nullptr){      
                    prev->next = nullptr;
                }
                else{
                    prev->next = temp->next;
                }
                delete temp;
                cout << endl << "File deleted" << endl;
                return;
            }
            prev = temp;
            temp = temp->next;
        }

        
    }
    cout << endl << "File Not Found" << endl;
    
}

//Commit Functions
void doublyNode::commit(doublyNode* curNode){
    /*
        TO-DO
        *For every node in current commit SLL
            -> Check to see if matching fileversion exists in .minigit/                     (DONE)
                -> If NO, add file to directory with its contents                           (DONE)
                -> If YES, check each line of file to see if everything matches             (DONE)
                    -> If NO, do nothing                                                    (DONE)
                    -> If YES, update fileversion and add to .minigit/                      (X)
    */

    //Current Commit
    singlyNode* crawlerSLL = head; //Head of current commit SLL

    //Function variables
    fs::create_directories(".minigit\\" + to_string(commitNumber)); //Creates commit version sub directory

    while(crawlerSLL != nullptr){
        if(!(checkMiniGit(crawlerSLL->fileName))){ //File does not exist in minigit directory, gets added
            addToMiniGit(crawlerSLL);
            addToRepo(crawlerSLL);
        }
        else{   //File already exists, should be compared
            if(!compareVersion(crawlerSLL->fileName)){ //Files was changed

                //update version number
                //incrementFileVersion(crawlerSLL)Takes in base input of _00 and updates commited file name to the latest version... Hopefully

                addToMiniGit(crawlerSLL);
                addToRepo(crawlerSLL);
                
            }
            else{
                cout << "No Changes Made To " << crawlerSLL->fileName << endl;
                addToMiniGitNoChange(crawlerSLL);
            }
        }
        crawlerSLL = crawlerSLL->next;
    }

    doublyNode* newDLLNode = new doublyNode; //Node replacing current commit (returned?)
    newDLLNode->head = NULL;
    newDLLNode->next = NULL;
    newDLLNode->previous = curNode;
    newDLLNode->commitNumber = curNode->commitNumber + 1;
    curNode->next = newDLLNode;
    
}

bool doublyNode::checkMiniGit(string name){
    string path = ".minigit\\";
    for (const auto & entry : fs::directory_iterator(path)){ //Checks to see if the fileName given exists in the directory of files
        if(path + name == entry.path()){
            return true;
        }
    }
    return false;
}

void doublyNode::addToMiniGit(singlyNode* node){ //Adds file to current commit file
    ofstream out;
    string outFileName;
    ifstream in;
    string inFileName;
    string line; //Used to copy over contents of new files

    inFileName = "Files\\" + node->fileName.substr(0, node->fileName.length()-7) + "_00" + node->fileName.substr(node->fileName.length()-4);
    
    while(checkMiniGit(node->fileName)){
        node->fileName = incrementFileVersion(node);
    }
    
    in.open(inFileName);

    if(in.fail()){
        cout << "Failed to open file" << endl;
        return;
    }

    outFileName = ".minigit\\" + to_string(commitNumber) + "\\" + node->fileName;
    cout << endl << "Adding Commit File Path: " << outFileName << endl;

    out.open(outFileName);
     
    //For each line in the file, copy from original version and add to new copy
    while(getline(in, line)){
        out << line << endl;
    }

    in.close();
    out.close();
}

void doublyNode::addToMiniGitNoChange(singlyNode* node){ //Adds file to current commit file
    ofstream out;
    string outFileName;
    ifstream in;
    string inFileName;
    string line; //Used to copy over contents of new files

    inFileName = "Files\\" + node->fileName.substr(0, node->fileName.length()-7) + "_00" + node->fileName.substr(node->fileName.length()-4);
    
    node->fileName = findLatestVersion(node->fileName);
    
    in.open(inFileName);

    if(in.fail()){
        cout << "Failed to open file" << endl;
        return;
    }

    outFileName = ".minigit\\" + to_string(commitNumber) + "\\" + node->fileName;
    cout << endl << "Adding Commit File Path: " << outFileName << endl;

    out.open(outFileName);
     
    //For each line in the file, copy from original version and add to new copy
    while(getline(in, line)){
        out << line << endl;
    }

    in.close();
    out.close();
}

void doublyNode::addToRepo(singlyNode* node){
    ofstream out;
    string outFileName;
    ifstream in;
    string inFileName;
    string line; //Used to copy over contents of new files

    inFileName = "Files\\" + node->fileName.substr(0, node->fileName.length()-7) + "_00" + node->fileName.substr(node->fileName.length()-4);

    while(checkMiniGit(node->fileName)){
        node->fileName = incrementFileVersion(node);
    }

    in.open(inFileName);

    if(in.fail()){
        cout << "Failed to open file" << endl;
        return;
    }

    outFileName = ".minigit\\" + node->fileName;
    out.open(outFileName);
     
    //For each line in the file, copy from original version and add to new copy
    while(getline(in, line)){
        out << line << endl;
    }

    cout << endl << "Adding Repo Path: " << outFileName << endl;
    in.close();
    out.close();
}

bool doublyNode::compareVersion(string name){ //False if files arent the same 
    ifstream compareFile;
    string compareFileName = ".minigit\\" + findLatestVersion(name);

    compareFile.open(compareFileName);

    ifstream newFile;
    string newFileName = "Files\\" + name;
    newFile.open(newFileName);

    //Temporary string holders
    string lineNew;
    string lineCompare;

    if(compareFile.fail() || newFile.fail()){
        cout << "File Not Found" << endl;
        return false;
    }
    else{
        while(getline(compareFile, lineCompare)){ //ERROR HERE

             getline(newFile, lineNew);

            if(!(lineNew == lineCompare)){ //Compares each line
                cout << endl << "Change Found" << endl;
                return false;
            }
        }
        if(getline(newFile, lineNew)){
            cout << endl << "Change Found" << endl;
                return false;
        }
    }
    return true;
}

string doublyNode::findLatestVersion(string name){ //Finds the most recent copy of "name" in the .minigit
    string versionNumber = name.substr(name.length()- 6, name.length() - 4);
    int versionNum = stoi(versionNumber); //Gets version numbers

    string temp = name;
    string latest;
    while(checkMiniGit(temp)){
        latest = temp;
        versionNum++;

        if(versionNum < 10){
            temp = name.substr(0, name.length()-6) + "_0" + to_string(versionNum);
        }
        else{
            temp = name.substr(0, name.length()-6) + "_" + to_string(versionNum);
        }
    }    
    return latest;
}

string doublyNode::incrementFileVersion(singlyNode* node){
    string versionNumber = node->fileName.substr(node->fileName.length()- 6, node->fileName.length() - 4);
    int versionNum = stoi(versionNumber); //Gets version numbers

    string temp = node->fileName;
    string latest;

    while(checkMiniGit(temp)){
        latest = temp;
        versionNum++;

        if(versionNum < 10){
            temp = node->fileName.substr(0, node->fileName.length()-7) + "_0" + to_string(versionNum) + node->fileName.substr(node->fileName.length()-4);
        }
        else{
            node->fileName = node->fileName.substr(0, node->fileName.length()-7) + "_" + to_string(versionNum) + node->fileName.substr(node->fileName.length()-4);
        }
    }    
    return temp;
}

void doublyNode::addPrevCommitFiles(){
    singlyNode* temp = previous->head;
    
    if(temp == nullptr){
        return;
    }

    singlyNode* newCrawler = head;

    while(temp != nullptr){
        singlyNode* curSLLNode = new singlyNode;
        curSLLNode->fileName = temp->fileName;
        curSLLNode->fileVersion = temp->fileVersion;
        curSLLNode->next = NULL;
        if(head == nullptr){
            head = curSLLNode;
        }
        else{
            while(newCrawler->next != nullptr){
                newCrawler = newCrawler->next;
            }
            newCrawler->next = curSLLNode;
        }
        newCrawler = head;
        temp = temp->next;
    }
}

//Checkout Functions

//Testing functions
void doublyNode::printSLL(){
    singlyNode* temp = head;

    cout << endl << "Current Commit Version: " << commitNumber << endl;
    while(temp != nullptr){
        cout << temp->fileName;
        if(temp->next != nullptr){
            cout << " -> ";
        }
        temp = temp->next;
    }
    cout << endl;
}


