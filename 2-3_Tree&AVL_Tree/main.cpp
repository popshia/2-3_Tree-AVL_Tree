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
    PointerStruct* theExtra = NULL ;
} PointerStruct;

static ifstream input ;
static ofstream output ;
static string FileNumber = "0" ;
static int Count = 1 ;
static int Command = 0 ;

class TwoThreeTree {
private:
    vector<DataStruct> DATABASE ;
    PointerStruct* root = NULL ;
    PointerStruct* splitMiddle = NULL ;
    PointerStruct* splitLeft = NULL ;
    PointerStruct* splitRight = NULL ;
public:
    void debugPrintTree( PointerStruct* root ) {
        if ( !root ) return ;
        else {
            for ( int first = 0 ; first < root->nodeData.size() ; first++ ) {
                for ( int second = 0 ; second < root->nodeData[first].size() ; second++ ) {
                    cout << root->nodeData[first][second].schoolName << endl ;
                } // the second layer
            } // print the first layer
            debugPrintTree( root->leftChild ) ;
            debugPrintTree( root->middleChild ) ;
            debugPrintTree( root->rightChild ) ;
        }
    }
    
    int GetHeight( PointerStruct* root ) {
        if ( root == NULL ) return 0 ;
        else {
            int leftDepth = GetHeight( root->leftChild ) ;
            int rightDepth = GetHeight( root->rightChild ) ;
            int middleDepth = GetHeight( root->middleChild ) ;
            if ( leftDepth > rightDepth && leftDepth > middleDepth ) return leftDepth+1 ;
            else if ( rightDepth > leftDepth && rightDepth > middleDepth ) return rightDepth+1 ;
            else return middleDepth+1 ;
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
            DATABASE.push_back( tempData ) ;
            Count++ ;
        } // get the whole file
        
        for ( int i = 0 ; i < DATABASE.size() ; i++ ) {
            InsertBySchoolName( DATABASE[i] ) ;
            cout << i << endl ;
            debugPrintTree( root ) ;
            cout << endl ;
        } // for()
        
        PrintBTree() ;
    } // Analyze the whole input file
    
    void InsertBySchoolName( DataStruct data ) {
        PointerStruct* walk = root ;
        vector<DataStruct> tempVector ;
        bool insertComplete = false ;
        
        if ( root == NULL ) {
            walk = new PointerStruct ;
            tempVector.push_back( data ) ;
            walk->nodeData.push_back( tempVector ) ;
            root = walk ;
        } // first data
        
        else {
            while ( insertComplete == false ) {
                if ( walk->nodeData.size() == 1 ) {
                    if ( data.schoolName > walk->nodeData[0][0].schoolName ) {
                        if ( walk->rightChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.push_back( tempVector ) ;
                            insertComplete = true ;
                        } // if
                        
                        else walk = walk->rightChild ;
                    } // greater than
                    
                    else if ( data.schoolName < walk->nodeData[0][0].schoolName ) {
                        if ( walk->leftChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.insert( walk->nodeData.begin(), tempVector ) ;
                            insertComplete = true ;
                        } // if
                        
                        else walk = walk->leftChild ;
                    } // smaller than
                    
                    else if ( data.schoolName == walk->nodeData[0][0].schoolName ) {
                        tempVector.push_back( data ) ;
                        walk->nodeData[0].push_back( data ) ;
                        insertComplete = true ;
                    } // equal to
                } // size = 1
                
                else {
                    if ( data.schoolName == walk->nodeData[0][0].schoolName ) {
                        walk->nodeData[0].push_back( data ) ;
                        insertComplete = true ;
                    } // equal to the first data in walk
                    
                    else if ( data.schoolName == walk->nodeData[1][0].schoolName ) {
                        walk->nodeData[1].push_back( data ) ;
                        insertComplete = true ;
                    } // equal to the second data in walk
                    
                    else if ( data.schoolName > walk->nodeData[0][0].schoolName && data.schoolName < walk->nodeData[1][0].schoolName ) {
                        if ( walk->middleChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.insert( walk->nodeData.begin()+1, tempVector ) ;
                            insertComplete = true ;
                        } //if
                        
                        else walk = walk->middleChild ;
                    } // in the middle of the two data in walk
                    
                    else if ( data.schoolName < walk->nodeData[0][0].schoolName ) {
                        if ( walk->leftChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.insert( walk->nodeData.begin(), tempVector ) ;
                            insertComplete = true ;
                        } // if
                        
                        else walk = walk->leftChild ;
                    } // the smallest of two data in walk
                    
                    else if ( data.schoolName > walk->nodeData[1][0].schoolName ) {
                        if ( walk->rightChild == NULL ) {
                            tempVector.push_back( data ) ;
                            walk->nodeData.push_back( tempVector ) ;
                            insertComplete = true ;
                        } // if
                        
                        else walk = walk->rightChild ;
                    } // the greatest of two data in walk
                } // size = 2
            } // while haven't insert the data yet
            
            vector<DataStruct>().swap( tempVector ) ;
            if ( walk->nodeData.size() > 2 ) FixBySchoolName( walk->parent, walk ) ;
        } // not first data
    } // InsertBySchoolName( )
    
    void FixBySchoolName( PointerStruct* currentParent, PointerStruct* currentIssue ) {
        splitLeft = new PointerStruct ;
        splitRight = new PointerStruct ;
        splitMiddle = new PointerStruct ;
        
        do {
            if ( currentParent || currentParent == root ) cout << "currentIssue: " << currentIssue->nodeData[1][0].schoolName << " currentParent: " << currentParent->nodeData[0][0].schoolName << endl ;
            splitMiddle = new PointerStruct ;
            splitMiddle->nodeData.clear() ;
            //splitMiddle->leftChild = NULL ;
            //splitMiddle->rightChild = NULL ;
            //splitMiddle->middleChild = NULL ;
            splitLeft = new PointerStruct ;
            splitLeft->nodeData.clear() ;
            //splitLeft->leftChild = NULL ;
            //splitLeft->rightChild = NULL ;
            //splitLeft->middleChild = NULL ;
            splitRight = new PointerStruct ;
            splitRight->nodeData.clear() ;
            //splitRight->leftChild = NULL ;
            //splitRight->rightChild = NULL ;
            //splitRight->middleChild = NULL ;
            splitMiddle->nodeData.push_back( currentIssue->nodeData[1] ) ;
            splitLeft->nodeData.push_back( currentIssue->nodeData[0] ) ;
            splitRight->nodeData.push_back( currentIssue->nodeData[2] ) ; // split and push the three data
            
            if ( ( !currentIssue->leftChild || !currentIssue->rightChild ) && ( !currentParent ) ) {
                root = splitMiddle ;
                splitMiddle->parent = root ;
                splitMiddle->leftChild = splitLeft ;
                splitMiddle->rightChild = splitRight ;
                splitLeft->parent = splitMiddle ;
                splitRight->parent = splitMiddle ;
                cout << "first case" << endl ;
                return ;
            } // first fix situation
                
            else if ( ( !currentIssue->leftChild || !currentIssue->rightChild ) && ( currentParent ) ) {
                if ( currentParent->nodeData.size() == 2 ) {
                    if ( currentIssue->nodeData[1][0].schoolName > currentParent->nodeData[1][0].schoolName )
                        currentParent->nodeData.push_back( currentIssue->nodeData[1] ) ;
                    
                    else if ( currentIssue->nodeData[1][0].schoolName < currentParent->nodeData[1][0].schoolName &&
                              currentIssue->nodeData[1][0].schoolName > currentParent->nodeData[0][0].schoolName )
                        currentParent->nodeData.insert( currentParent->nodeData.begin()+1, currentIssue->nodeData[1] ) ;
                
                    else if ( currentIssue->nodeData[1][0].schoolName < currentParent->nodeData[0][0].schoolName )
                        currentParent->nodeData.insert( currentParent->nodeData.begin(), currentIssue->nodeData[1] ) ; // split and push the middle data into the parent
                } // 2 data
                else {
                    if ( currentIssue->nodeData[1][0].schoolName > currentParent->nodeData[0][0].schoolName )
                        currentParent->nodeData.push_back( currentIssue->nodeData[1] ) ;
                    else
                        currentParent->nodeData.insert( currentParent->nodeData.begin(), currentIssue->nodeData[1] ) ;
                } // 1 data
                
                //cout << currentIssue << " " << currentParent->rightChild << endl ;
                if ( currentIssue == currentParent->rightChild ) {
                    if ( currentParent->middleChild ) {
                        currentParent->rightChild = splitLeft ;
                        splitLeft->parent = currentParent ;
                        currentParent->theExtra = splitRight ;
                        splitRight->parent = currentParent ;
                    } // the four
                    else {
                        currentParent->middleChild = splitLeft ;
                        splitLeft->parent = currentParent ;
                        currentParent->rightChild = splitRight ;
                        splitRight->parent = currentParent ;
                    } // normal
                } // rightChild
                
                else if ( currentIssue == currentParent->leftChild ) {
                    if ( currentParent->middleChild ) {
                        currentParent->leftChild = splitRight ;
                        splitRight->parent = currentParent ;
                        currentParent->theExtra = splitLeft ;
                        splitLeft->parent = currentParent ;
                    } // the four
                    else {
                        cout << splitLeft->nodeData[0][0].schoolName << " " << splitRight->nodeData[0][0].schoolName << endl ;
                        currentParent->middleChild = splitRight ;
                        splitRight->parent = currentParent ;
                        currentParent->leftChild = splitLeft ;
                        splitLeft->parent = currentParent ;
                    } // normal
                } // leftChild
                
                else if ( currentIssue == currentParent->middleChild ) {
                    currentParent->middleChild = splitLeft ;
                    splitLeft->parent = currentParent ;
                    currentParent->theExtra = splitRight ;
                    splitRight->parent = currentParent ;
                } // middle child
                
                // cout << currentParent->leftChild->nodeData[0][0].schoolName << " " << currentParent->middleChild->nodeData[0][0].schoolName << " " << currentParent->rightChild->nodeData[0][0].schoolName << endl ;
                
                currentIssue = currentParent ;
                currentParent = currentParent->parent ;
                cout << "is a leaf, with a parent case" << endl ;
            } // is a leaf, and with a parent
                
            else if ( ( currentIssue->leftChild || currentIssue->rightChild ) && currentIssue == root ) {
                root = splitMiddle ;
                splitMiddle->parent = root ;
                splitMiddle->leftChild = splitLeft ;
                splitLeft->parent = splitMiddle ;
                splitMiddle->rightChild = splitRight ;
                splitRight->parent = splitMiddle ; // set the currentIssue's three data
                
                if ( currentIssue->theExtra->nodeData[0][0].schoolName < currentIssue->leftChild->nodeData[0][0].schoolName ) {
                    splitLeft->leftChild = currentIssue->theExtra ;
                    currentIssue->theExtra->parent = splitLeft ;
                    splitLeft->rightChild = currentIssue->leftChild ;
                    currentIssue->leftChild->parent = splitLeft ;
                    splitRight->leftChild = currentIssue->middleChild ;
                    currentIssue->middleChild->parent = splitRight ;
                    splitRight->rightChild = currentIssue->rightChild ;
                    currentIssue->rightChild->parent = splitRight ;
                    cout << "left case: " << currentIssue->theExtra->nodeData[0][0].schoolName << endl ;
                } // left extra
                
                else if ( currentIssue->theExtra->nodeData[0][0].schoolName > currentIssue->middleChild->nodeData[0][0].schoolName &&
                          currentIssue->theExtra->nodeData[0][0].schoolName < currentIssue->rightChild->nodeData[0][0].schoolName ) {
                    splitLeft->leftChild = currentIssue->leftChild ;
                    currentIssue->leftChild->parent = splitLeft ;
                    splitLeft->rightChild = currentIssue->middleChild ;
                    currentIssue->middleChild->parent = splitLeft ;
                    splitRight->leftChild = currentIssue->theExtra ;
                    currentIssue->theExtra->parent = splitRight ;
                    splitRight->rightChild = currentIssue->rightChild ;
                    currentIssue->rightChild->parent = splitRight ;
                    cout << "middle case: " << currentIssue->theExtra->nodeData[0][0].schoolName << endl ;
                } // middle extra
                
                else if ( currentIssue->theExtra->nodeData[0][0].schoolName > currentIssue->rightChild->nodeData[0][0].schoolName ) {
                    //cout << currentIssue->nodeData[0][0].schoolName << " " << splitRight->nodeData[0][0].schoolName << endl ;
                    splitLeft->leftChild = currentIssue->leftChild ;
                    currentIssue->leftChild->parent = splitLeft ;
                    splitLeft->rightChild = currentIssue->middleChild ;
                    currentIssue->middleChild->parent = splitLeft ;

                    splitRight->leftChild = currentIssue->rightChild ;
                    currentIssue->rightChild->parent = splitRight ;
                    splitRight->rightChild = currentIssue->theExtra ;
                    currentIssue->theExtra->parent = splitRight ;
                    //cout << currentIssue->rightChild->rightChild << " " << currentIssue->theExtra->rightChild << endl;
                    
                    /*
                    splitRight->leftChild->leftChild = 0 ;
                    splitRight->leftChild->rightChild = 0 ;
                    splitRight->rightChild->leftChild = 0 ;
                    splitRight->rightChild->rightChild = 0 ;
                    */
                    cout << "right case: " << currentIssue->theExtra->nodeData[0][0].schoolName << endl ;
                } // right extra
                
                cout << "not a leaf, without a parent case ( root )" << endl ;
                return ;
            } // not a leaf, without a parent ( root )
                
            else if ( ( currentIssue->leftChild || currentIssue->rightChild ) && currentIssue != root ) {
                if ( currentParent->nodeData.size() == 2 ) {
                    if ( currentIssue->nodeData[1][0].schoolName > currentParent->nodeData[1][0].schoolName )
                        currentParent->nodeData.push_back( currentIssue->nodeData[1] ) ;
                    
                    else if ( currentIssue->nodeData[1][0].schoolName < currentParent->nodeData[1][0].schoolName &&
                             currentIssue->nodeData[1][0].schoolName > currentParent->nodeData[0][0].schoolName )
                        currentParent->nodeData.insert( currentParent->nodeData.begin()+1, currentIssue->nodeData[1] ) ;
                    
                    else if ( currentIssue->nodeData[1][0].schoolName < currentParent->nodeData[0][0].schoolName )
                        currentParent->nodeData.insert( currentParent->nodeData.begin(), currentIssue->nodeData[1] ) ; // split and push the middle data into the parent
                } // 2 data
                else {
                    if ( currentIssue->nodeData[1][0].schoolName > currentParent->nodeData[0][0].schoolName )
                        currentParent->nodeData.push_back( currentIssue->nodeData[1] ) ;
                    else
                        currentParent->nodeData.insert( currentParent->nodeData.begin(), currentIssue->nodeData[1] ) ;
                } // 1 data
                
                /*
                splitMiddle->leftChild = splitLeft ;
                splitLeft->parent = splitMiddle ;
                splitMiddle->rightChild = splitRight ;
                splitRight->parent = splitMiddle ; // set the currentIssue's three data
                */
                 
                if ( currentIssue == currentParent->rightChild ) {
                    if ( currentParent->middleChild ) {
                        currentParent->rightChild = splitLeft ;
                        splitLeft->parent = currentParent ;
                        currentParent->theExtra = splitRight ;
                        splitRight->parent = currentParent ;
                    } // the four
                    else {
                        currentParent->middleChild = splitLeft ;
                        splitLeft->parent = currentParent ;
                        currentParent->rightChild = splitRight ;
                        splitRight->parent = currentParent ;
                    } // normal
                } // rightChild
                
                else if ( currentIssue == currentParent->leftChild ) {
                    if ( currentParent->middleChild ) {
                        currentParent->leftChild = splitRight ;
                        splitRight->parent = currentParent ;
                        currentParent->theExtra = splitLeft ;
                        splitLeft->parent = currentParent ;
                    } // the four
                    else {
                        currentParent->middleChild = splitRight ;
                        splitRight->parent = currentParent ;
                        currentParent->rightChild = splitLeft ;
                        splitLeft->parent = currentParent ;
                    } // normal
                } // leftChild
                
                else if ( currentIssue == currentParent->middleChild ) {
                    currentParent->middleChild = splitLeft ;
                    splitLeft->parent = currentParent ;
                    currentParent->theExtra = splitRight ;
                    splitRight->parent = currentParent ;
                } // middle child
                

                
                if ( currentIssue->theExtra->nodeData[0][0].schoolName < currentIssue->leftChild->nodeData[0][0].schoolName ) {
                    splitLeft->leftChild = currentIssue->theExtra ;
                    currentIssue->theExtra->parent = splitLeft ;
                    splitLeft->rightChild = currentIssue->leftChild ;
                    currentIssue->leftChild->parent = splitLeft ;
                    splitRight->leftChild = currentIssue->middleChild ;
                    currentIssue->middleChild->parent = splitRight ;
                    splitRight->rightChild = currentIssue->rightChild ;
                    currentIssue->rightChild->parent = splitRight ;
                } // left extra
                
                else if ( currentIssue->theExtra->nodeData[0][0].schoolName > currentIssue->middleChild->nodeData[0][0].schoolName &&
                         currentIssue->theExtra->nodeData[0][0].schoolName < currentIssue->rightChild->nodeData[0][0].schoolName ) {
                    splitLeft->leftChild = currentIssue->leftChild ;
                    currentIssue->leftChild->parent = splitLeft ;
                    splitLeft->rightChild = currentIssue->middleChild ;
                    currentIssue->middleChild->parent = splitLeft ;
                    splitRight->leftChild = currentIssue->theExtra ;
                    currentIssue->theExtra->parent = splitRight ;
                    splitRight->rightChild = currentIssue->rightChild ;
                    currentIssue->rightChild->parent = splitRight ;
                } // middle extra
                
                else if ( currentIssue->theExtra->nodeData[0][0].schoolName > currentIssue->rightChild->nodeData[0][0].schoolName ) {
                    splitLeft->leftChild = currentIssue->leftChild ;
                    currentIssue->leftChild->parent = splitLeft ;
                    splitLeft->rightChild = currentIssue->middleChild ;
                    currentIssue->middleChild->parent = splitLeft ;
                    splitRight->leftChild = currentIssue->rightChild ;
                    currentIssue->rightChild->parent = splitRight ;
                    splitRight->rightChild = currentIssue->theExtra ;
                    currentIssue->theExtra->parent = splitRight ;
                } // right extra
                
                currentIssue = currentParent ;
                cout << "not a leaf, with a parent case" << endl ;
            } // not a leaf, but with a parent ( in the middle of the tree )*/
            if ( currentIssue->nodeData.size() != 3 ) break ; // break situation
        } while ( currentParent != NULL ) ;
    } // FixBySchoolName()
    
    void PrintBTree() {
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
    } // PrintBTree()
} ;

class AVLTree : public TwoThreeTree {
private:
    PointerStruct* AvlRoot = NULL ;
    PointerStruct* walker = NULL ;
    PointerStruct* newNode = NULL;
public:
    vector<DataStruct> database ;
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
            database.push_back( tempData ) ;
            Count++ ;
        } // get the whole file
        
        for ( int i = 0 ; i < database.size() ; i++ ) {
            InsertByDepartmentName( database[i] ) ;
            //debugPrintTree( root ) ;
            cout << endl ;
        } // for()
        
    } // Analyze the whole input file
    
    void InsertByDepartmentName( DataStruct data ) {
        bool NotYet = true ;
        vector<DataStruct> temp;
          newNode = new PointerStruct ;
          temp.push_back( data );
          newNode->nodeData.push_back( temp );
        
          if ( AvlRoot == NULL ) {
              AvlRoot = newNode ;
              newNode->parent = AvlRoot ;
              newNode = NULL ;
              walker = AvlRoot ;
              //cout << walker << endl;
          } // if
        
          else {
              do {
                  
                  if ( walker->nodeData[0][0].departmentName == newNode->nodeData[0][0].departmentName  ) {
                      walker->nodeData[0].push_back( newNode->nodeData[0][0] );
                      NotYet = false ;
                      // cout << "same" << endl;
                  } // if
                
                  else if ( walker->nodeData[0][0].departmentName < newNode->nodeData[0][0].departmentName  ) {
                      if ( walker->rightChild == NULL ) {
                          newNode->parent = walker ;
                          walker->rightChild = newNode ;
                          NotYet = false ;
                          // cout << "larger" << endl;
                      } // if
                    
                      else walker = walker->rightChild ;
                  } // if
                
                  else if ( walker->nodeData[0][0].departmentName > newNode->nodeData[0][0].departmentName  ) {
                      if ( walker->leftChild == NULL ) {
                          newNode->parent = walker ;
                          walker->leftChild = newNode ;
                          NotYet = false ;
                          // cout << "smaller" << endl;
                      } // if
                    
                      else walker = walker->leftChild ;
                  } // if
              } while ( NotYet ) ;
          } // else

        
        //cout << walker << endl;
        FixByDepartmentName( walker ) ;
        PointerStruct * depth = AvlRoot;
        cout << "AVL tree height:" << GetHeight( depth ) << endl;
        //for( int i = 0 ; i < AvlRoot->nodeData[0].size() ; i ++ )
            //cout << AvlRoot->nodeData[0][i].whole << endl;

    } //InsertByDepartmentName()
    
    int AnalyzeByDepartmentName( PointerStruct* walker ){
        PointerStruct * leftwalker = walker->leftChild ;
        PointerStruct * rightwalker = walker->rightChild;
        int leftHeight = GetHeight( leftwalker ) ;
        int rightHeight = GetHeight( rightwalker ) ;
        return leftHeight - rightHeight ;
    } //AnalyzeByDepartmentName()
    
    PointerStruct* RotateLL( PointerStruct * x ) {
        PointerStruct * y = x->leftChild;
        x->leftChild = y->rightChild;
        y->rightChild->parent = x->leftChild;
        y->rightChild = x;
        x->parent = y->rightChild;
        return y;
    } // RotateLL()
    
    PointerStruct* RotateRR( PointerStruct * x ) {
        PointerStruct * y = x->rightChild;
        x->rightChild = y->leftChild;
        y->leftChild->parent = x->rightChild ;
        y->leftChild = x ;
        x->parent = y->leftChild ;
        return y;
    } // RotateRR()
    
    void FixByDepartmentName( PointerStruct * walker ) {
        while( ( AnalyzeByDepartmentName( walker ) <= 1 || AnalyzeByDepartmentName( walker ) >= -1 )&& walker != AvlRoot )
            walker = walker->parent;
        
            if( AnalyzeByDepartmentName( walker ) > 1 ) {
                if( walker->parent ) walker = walker->parent;
                  if( AnalyzeByDepartmentName( walker->leftChild ) < 0 ){
                    walker = RotateLL( walker );
                    walker = RotateRR( walker );
                  } // LR situation
                  else walker = RotateLL( walker ); // LL situation
            } // left
        
            else if ( AnalyzeByDepartmentName( walker ) < -1 && walker->parent ){
                if( walker->parent ) walker = walker->parent;
                if( AnalyzeByDepartmentName( walker->rightChild ) > 0 ){
                    walker = RotateRR( walker->parent );
                    walker = RotateLL( walker->parent );
                } // RL situation
                else walker = RotateRR( walker->parent ); //RR situation
                
            } // right

    } //FixByDepartmentName()
    
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
} //
