#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <string.h>
using namespace std;
/*
* this is a connect four engine implementing the barebones of C4P.
* it makes moves at random, but is useful as a skeleton for more complex programs
*
*/

//keep track of game state
//1 represents first player, -1 second player
//rowsXcolumn
int* boardData;
int width = 7;
int height = 6;

int board(int h,int w){
    return boardData[h*height+w];
}

void board(int h,int w,int data){

    boardData[h*height+w]= data;
}

void shutDown(){
    exit(0);
}


//function for splitting strings. copied from https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

//checkWin
bool checkWin(int color){   

    for(int i = 0;i < height;i++){
        for(int j = 0;j<width;j++){
            if(j>=3){
                if((board(i,j)==color)&& (board(i,j-1)==color)&& (board(i,j-2)==color)&& (board(i,j-3)==color)) return true;
                if(i>=3){
                    if((board(i,j)==color)&& (board(i-1,j-1)==color)&& (board(i-2,j-2)==color)&& (board(i-3,j-3)==color)) return true;
                }
                if(i<height-3){
                   if((board(i,j)==color)&& (board(i+1,j-1)==color)&& (board(i+2,j-2)==color)&& (board(i+3,j-3)==color)) return true; 
                }
            }
            if(i>=3){
                if((board(i,j)==color)&& (board(i-1,j)==color)&& (board(i-2,j)==color)&& (board(i-3,j)==color)) return true;                
            }
        }
    }
    return false;
}

//to be called only after checkWin
bool checkTie(){
    for(int i=0;i<width;i++){
        if(board(0,i)==0)return false;
    }
    return true;
}

void initialHandshake(){
    string line;
    cin >> line;
    if(line.compare("C4P_begin")==0){
        //confirm we conform
        cout << "ready" << endl;
    }else{
        //terminate, as program can only be run with C4P
        cout << "shutting_down" << endl;
        shutDown();
    }
}

//clear the board
void clear(){
    for(int i = 0;i<height;i++){
        for(int j = 0;j<width;j++){
            board(i,j,0);
        }
    }
}

void init(){
    boardData = new int[height*width];
    clear();
}

//only call this function if game is not already over
bool makeMove(int color,int column){
    if((column >= width) || (column < 0)) return false;
    for(int i = height-1; i >= 0;i--){
        if(board(i,column) == 0){
            board(i,column,color);
            return true;
        }
    }
    return false;
}

//only call thus function if game is not already over
int getMove(int color, int time){
    while (true){
        int move = rand() % width;
        if(board(height-1,move)==0){
            //column isn't full
            makeMove(color,move);
            return move;
        }
    }
    return -1;
}

bool setBoardSize(int w,int h){
    if(w<4 || h<4) return false;
    width = w;
    height = h;
    free(boardData);
    boardData = new int[height*width];
    clear();
    return true;
}


void protocalLoop(){
    string line = "";
    
    do{
        getline(cin, line);
        
        //now see what command was inputed:
        
        if(strncmp(line.c_str(), "clear", 5)==0){
            clear();
            cout << "ready" << endl;
        }
        
        if(strncmp(line.c_str(), "kill", 4)==0){
            cout << "shutting_down" << endl;
            shutDown();
        }
        
              
        if(strncmp(line.c_str(), "boardsize", 9)==0){
            //for now, this engine supports arbitrary board size
            vector<string> tokens = split(line,string(" "));
            if(tokens.size() != 3){
                cout << "error: boardsize has two args" << endl;
                cout << "shutting_down" << endl;
                shutDown();
            }
            int w = stoi(tokens.at(1));
            int h = stoi(tokens.at(2));
            if(setBoardSize(w,h)){
                cout << "ready" << endl;
            }else{
                cout << "not_supported" << endl;
            }
        }
        
        
        
        
        if(strncmp(line.c_str(), "set", 3)==0){
            vector<string> tokens = split(line,string(" "));
            if(tokens.size() != 3){
                cout << "error: set has 2 args" << endl;
                cout << "shutting_down" << endl;
                shutDown();
            }
            
            int color = stoi(tokens.at(1));
            int column = stoi(tokens.at(2));
            
            //make sure game is not over
            if(checkWin(1)){
                cout << "game_result 1" << endl;
            }else if(checkWin(-1)){
                cout << "game_result -1" << endl;
            }else if (checkTie()){
                cout << "game_result tie" << endl;
            }else if(makeMove(color,column)){
                cout << "ready" << endl;
            }else{
                cout << "illegal_move" << endl;
            }
        }
        
        
        
        
        if(strncmp(line.c_str(), "get", 3)==0){
            vector<string> tokens = split(line,string(" "));
            if(tokens.size() != 3){
                cout << "error:get has 2 args" << endl;
                cout << "shutting_down" << endl;
                shutDown();
            }
            int color = stoi(tokens.at(1));
            int time = stoi(tokens.at(2));
            
            int moveCol = getMove(color,time);
            cout << "<"<< moveCol << ">"<<endl;

        }  

        if(strncmp(line.c_str(), "print", 5)==0){
            for(int i = 0;i<height;i++){
                cout <<"|";
                for(int j = 0;j<width;j++){
                    string s = " ";
                    if(board(i,j)==1) s="X";
                    if(board(i,j)==-1) s="O";

                    cout <<s;
                }
                cout <<"|"<< endl;
            }

        }  


        
    }while(true);
}

int main() 
{
    cout << "CONNECT-4 CMD PROMPT:\n";
    init();
    //set things up with GUI
    initialHandshake();
    protocalLoop();
    return 0;
}