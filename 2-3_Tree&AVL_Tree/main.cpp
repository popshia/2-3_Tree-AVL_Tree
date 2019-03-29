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
    string whole = "\0" ;
} DataStruct ;

typedef struct VectorStuct {
    vector<vector<DataStruct>> nodeData ;
    VectorStuct* parent = NULL ;
    VectorStuct* leftChild = NULL ;
    VectorStuct* rightChild = NULL ;
} VectorStuct;

static ifstream input ;
static ofstream output ;
static string FileNumber = "0" ;
static int Count = 1 ;

class TwoThreeTree {
private:
    VectorStuct* root = NULL ;
public:
    int GetHeight( VectorStuct* root ) {
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
            tempData.departmentName = tokens[4] ;
            // InsertBySchoolName( tempdata ) ; // Insert & Analyze
            Count++ ;
        } // get the whole file
        
        cout << "Tree Height: " << GetHeight( root ) << endl << endl ; // print the tree height
        for ( int first = 0 ; first < root->nodeData.size() ; first++ ) {
            for ( int second = 0 ; second < root->nodeData[first].size() ; second++ )
                cout << root->nodeData[first][second].whole << endl ;
        } // print the root
    } // Analyze the whole input file
    
    void InsertBySchoolName( DataStruct data ) {
        
    }
    
    void AnalyzeBySchoolName() {
        
    }
} ;

class AVLTree : public TwoThreeTree {
private:
public:
    void InsertByDepartmentName( DataStruct data ) {
        
    }
    
    void AnalyzeByDepartmentName() {
        
    }
} ;

int main() {
    int command = 0 ;
    bool continueOrNot = false ;
    TwoThreeTree twoThreeTree ;
    AVLTree avlTree ;
    
    do {
        cout << "********************************************************" << endl ; // welcome message
        cout << "*****              2-3 Tree & AVL Tree             *****" << endl ;
        cout << "***** 0 : Quit                                     *****" << endl ;
        cout << "***** 1 : 2-3 Tree                                 *****" << endl ;
        cout << "***** 2 : AVL Tree                                 *****" << endl ;
        cout << "********************************************************" << endl ;
        cout << endl << "Please enter your choice:" << endl ;
        
        cin >> command ; // read in user command
        cout << endl ;
        
        if ( command == 0 ) { // bye :(((
            cout << "Bye :(((" << endl ;
            return 0 ;
        } // quit
        
        else if ( command > 2 || command < 0 ) {
            cout << "Error command! please enter an acceptable command:" << endl << endl ;
            continueOrNot = true ;
        } // wrong command
        
        else if ( command == 1 ) { // read, count and build
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
        
        else if ( command == 2 ) {
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
