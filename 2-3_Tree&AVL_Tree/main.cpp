// 10627130 林冠良 & 10627131 李峻瑋 // CodeBlocks 17.12
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <time.h>

using namespace std ;

typedef struct DataStruct {
    int index = 0 ;
    string schoolName = "\0" ;
    string departmentName = "\0" ;
    string dayOrNight = "\0" ;
    string graduateType = "\0" ;
    int student = 0 ;
    string whole = "\0" ;
} DataStruct ;

typedef struct PointerStruct {
    vector<vector<DataStruct> > nodeData ;
    PointerStruct* parent = NULL ;
    PointerStruct* leftChild = NULL ;
    PointerStruct* middleChild = NULL ;
    PointerStruct* rightChild = NULL ;
} PointerStruct;

static ifstream input ;
static ofstream output ;
static string FileNumber = "0" ;
static int Count = 1 ;
static int Command = 0 ;

class TwoThreeTree {
private:
    PointerStruct* root = NULL ;
public:
    int GetHeight( PointerStruct* root ) {
        if ( root == NULL ) return 0 ;
        else {
            int leftDepth = GetHeight( root->leftChild ) ;
            int rightDepth = GetHeight( root->rightChild ) ;
            if ( leftDepth > rightDepth ) return leftDepth+1 ;
            else return rightDepth+1 ;
        } //else
    } // Get tree height

    void GetData() {
        DataStruct tempData ;
        string temp = "\0" ;
        getline( input, temp ) ; // read the labels in the first three lines
        getline( input, temp ) ;
        getline( input, temp ) ;

        while ( getline( input, temp ) ) {
            tempData.whole = temp ;
            tempData.index = Count ;

            vector<string> tokens ;
            istringstream cutStream( temp ) ;

            while ( getline( cutStream, temp, '\t' ) ) // cut the token one by one
                tokens.push_back( temp ) ;

            tempData.schoolName = tokens[1] ;
            tempData.departmentName = tokens[3] ;
            tempData.dayOrNight = tokens[4] ;
            tempData.graduateType = tokens[5] ;
            tempData.student = atoi( tokens[6].c_str() ) ;
            InsertBySchoolName( tempData ) ; // Insert & Analyze
            Count++ ;
        } // get the whole file

        cout << "Tree Height = " << GetHeight( root ) << endl ; // print the tree height
        int n = 1 ;
        for ( int first = 0 ; first < root->nodeData.size() ; first++ ) {
            for ( int second = 0 ; second < root->nodeData[first].size() ; second++ ) {
                cout << n << ": [" << root->nodeData[first][second].index << "]" << ", " ;
                cout << root->nodeData[first][second].schoolName << ", " ;
                cout << root->nodeData[first][second].departmentName << ", " ;
                cout << root->nodeData[first][second].dayOrNight << ", " ;
                cout << root->nodeData[first][second].graduateType << ", " ;
                cout << root->nodeData[first][second].student << ", " << endl ;
                n++ ;
            } // the second layer
        } // print the first layer
        cout << endl ;
    } // Analyze the whole input file

    void InsertBySchoolName( DataStruct data ) {
        PointerStruct* walk = root ;
        vector<DataStruct> tempVector ;
        bool notYet = true ;

        if ( walk == NULL ) {
            walk = new PointerStruct ;
            walk->nodeData[0].push_back( data ) ;
            walk->parent = root ;
            root = walk ;
        } // first data

        else {
            while ( notYet == true ) {
                if ( walk->nodeData.size() == 1 ) {
                    if ( data.schoolName > walk->nodeData[0][0].schoolName ) {
                        if ( walk->rightChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.push_back( tempVector ) ;
                            notYet = false ;
                        } // if

                        else {
                            walk->parent = walk ;
                            walk = walk->rightChild ;
                        } // else
                    } // greater than

                    else if ( data.schoolName < walk->nodeData[0][0].schoolName ) {
                        if ( walk->leftChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.insert( walk->nodeData.begin(), tempVector ) ;
                            notYet = false ;
                        } // if

                        else {
                            walk->parent = walk ;
                            walk = walk->leftChild ;
                        } // else
                    } // smaller than

                    else if ( data.schoolName == walk->nodeData[0][0].schoolName ) {
                        tempVector.push_back( data ) ;
                        walk->nodeData[0].push_back( data ) ;
                        notYet = false ;
                    } // equal to
                } // size = 1

                else {
                    if ( data.schoolName == walk->nodeData[0][0].schoolName ) {
                        walk->nodeData[0].push_back( data ) ;
                        notYet = false ;
                    } // equal to the first data in walk

                    else if ( data.schoolName == walk->nodeData[1][0].schoolName ) {
                        walk->nodeData[1].push_back( data ) ;
                        notYet = false ;
                    } // equal to the second data in walk

                    else if ( data.schoolName > walk->nodeData[0][0].schoolName && data.schoolName < walk->nodeData[1][0].schoolName ) {
                        walk->parent = walk ;
                        walk = walk->middleChild ;
                        tempVector.push_back( data ) ;
                        walk->nodeData.insert( walk->nodeData.begin()+1, tempVector ) ;
                        notYet = false ;
                    } // in the middle of the two data in walk

                    else if ( data.schoolName < walk->nodeData[0][0].schoolName ) {
                        if ( walk->leftChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.insert( walk->nodeData.begin(), tempVector ) ;
                            notYet = false ;
                        } // if

                        else {
                            walk->parent = walk ;
                            walk = walk->leftChild ;
                        } // else
                    } // the smallest of two data in walk

                    else if ( data.schoolName > walk->nodeData[1][0].schoolName ) {
                        if ( walk->rightChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.push_back( tempVector ) ;
                            notYet = false ;
                        } // if

                        else {
                            walk->parent = walk ;
                            walk = walk->rightChild ;
                        } // else
                    } // the greatest of two data in walk
                } // size = 2
            } // while not found the right place

            if ( walk->nodeData.size() > 2 ) FixBySchoolName( walk->parent, walk ) ;
        } // not first data
    } // InsertBySchoolName( )

    void FixBySchoolName( PointerStruct* parent, PointerStruct* currentIssue ) {
        PointerStruct* newNode ;
        vector<DataStruct> tempNewVector ;

        if ( parent->nodeData.size() < 2 ) {
            if ( currentIssue->nodeData[1][0].schoolName < parent->nodeData[0][0].schoolName )
                parent->nodeData.insert( parent->nodeData.begin(), currentIssue->nodeData[1] ) ;
            else if ( currentIssue->nodeData[1][0].schoolName > parent->nodeData[0][0].schoolName )
                parent->nodeData.push_back( currentIssue->nodeData[1] ) ;
            currentIssue->nodeData.erase( currentIssue->nodeData.begin()+1 ) ;
        } // parent is not full

        else if (  )
    }
} ;

class AVLTree : public TwoThreeTree {
private:
public:
    void InsertByDepartmentName( DataStruct data ) {

    }

    void FixByDepartmentName() {

    }
} ;

int main() {
    bool continueOrNot = false ;
    TwoThreeTree twoThreeTree ;
    AVLTree avlTree ;

    do {
        cout << "**********************************************" << endl ; // welcome message
        cout << "*****         2-3 Tree & AVL Tree        *****" << endl ;
        cout << "***** 0 : Quit                           *****" << endl ;
        cout << "***** 1 : Build 2-3 Tree                 *****" << endl ;
        cout << "***** 2 : Build AVL Tree                 *****" << endl ;
        cout << "**********************************************" << endl ;
        cout << endl << "Please enter your choice:" << endl ;

        cin >> Command ; // read in user command
        cout << endl ;

        if ( Command == 0 ) { // bye :(((
            cout << "Bye :(((" << endl ;
            return 0 ;
        } // quit

        else if ( Command > 2 || Command < 0 ) {
            cout << "Error command! please enter an acceptable command:" << endl << endl ;
            continueOrNot = true ;
        } // wrong command

        else if ( Command == 1 ) { // read, count and build
            bool function1Confirm = false ;

            do {
                cout << "Please enter the file you want to build a 2-3 Tree or [0] to quit:" << endl ;
                cin >> FileNumber ;

                if ( FileNumber == "0" ) {
                    function1Confirm = true ;
                    continueOrNot = true ;
                } // quit

                else {
                    string fileName = "input" + FileNumber + ".txt" ;
                    input.open( fileName.c_str() ) ;
                    if ( input.is_open() ) {
                        twoThreeTree.GetData() ;
                        function1Confirm = true ;
                        continueOrNot = true ;
                    } // open successfully
                    else cout << "*****  " << fileName << " does not exist!  *****" << endl ;
                } // continue
            } while( ! function1Confirm ) ;

            Count = 1 ;
            FileNumber = "0" ;
            input.close() ;
            output.close() ;
        } // 2-3 tree

        else if ( Command == 2 ) {
            bool function2Confirm = false ;

            do {
                cout << "Please enter the file you want to build a AVL Tree or [0] to quit:" << endl ;
                cin >> FileNumber ;

                if ( FileNumber == "0" ) {
                    function2Confirm = true ;
                    continueOrNot = true ;
                } // quit

                else {
                    string fileName = "input" + FileNumber + ".txt" ;
                    input.open( fileName.c_str() ) ;
                    if ( input.is_open() ) {
                        avlTree.GetData() ;
                        function2Confirm = true ;
                        continueOrNot = true ;
                    } // open successfully
                    else cout << "*****  " << fileName << " does not exist!  *****" << endl ;
                } // open file and input data to BST
            } while ( ! function2Confirm ) ;

            Count = 0 ;
            FileNumber = "0" ;
            input.close() ;
            output.close() ;
        } // AVL Tree
    } while( continueOrNot ) ;
} // Main function
