#include <string>
#include <queue>
#include <iostream>

using namespace std;


struct singlyNode{
    string fileName; // Name of local file
    string fileVersion; // Name of file in .minigit folder
    singlyNode * next;

    //void incrementFileVersion();
};

struct doublyNode{
    int commitNumber;
    singlyNode * head;
    doublyNode * previous;
    doublyNode * next;


    //Add File functions
    void addSLLFile(string in);

    //Remove File functions
    void removeFile(string in);

    //Commit Functions
    void commit(doublyNode* curNode);
    bool checkMiniGit(string name);
    void addToMiniGit(singlyNode* node);
    void addToMiniGitNoChange(singlyNode* node);
    void addToRepo(singlyNode* node);
    bool compareVersion(string name);
    string findLatestVersion(string name);
    string incrementFileVersion(singlyNode* node);

    void addPrevCommitFiles();

    //Checkout Functions

    //Testing functions
    void printSLL();
};


