#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

using namespace std;
char ch, buffer[15], operatorBuffer[4], operators[] = "+-><*/%=()#^~|&?!,:;.[]{}";
int i, j=0, k=0;
string filename;

int isKeyword(char buffer[]){
    char keywords[32][10] = {"auto","break","case","char","const","continue","default",
                            "do","double","else","enum","extern","float","for","goto",
                            "if","int","long","register","return","short","signed",
                            "sizeof","static","struct","switch","typedef","union",
                            "unsigned","void","volatile","while"};

    int i, flag = 0;
    for(i = 0; i < 32; ++i){
        if(strcmp(keywords[i], buffer) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}

void outputOperatorToken(){
  if (k != 0){
    operatorBuffer[k] = '\0';
    cout << operatorBuffer << '\n';
    k = 0;
  }
}

void outputIdentifierToken(){
  if(j != 0){
    // to make it a string add null at the end
    buffer[j] = '\0';
    cout << buffer << '\n';
    j = 0;
  }
}

int main(){
    bool bConstString = false;
    string strConst;

    ifstream fin("program.txt");
    if(!fin.is_open()){
        cout<<"error while opening the file\n";
        exit(0);
    }
    while(!fin.eof()){
        ch = fin.get();

        //for strings
        if(ch == '\"'){
          outputOperatorToken();
          outputIdentifierToken();
          bConstString = !bConstString;
          if (bConstString == false && strConst.length() != 0){
            cout << "\"" << strConst << "\"" << "\n";
            strConst = "";
          }
          continue;
        }

        if (bConstString == true){
          strConst += ch;
          //cout << "strConst now: " << strConst << "\n";
          continue;
        }

        // operators
        for(i = 0; i < sizeof(operators); ++i){
            if(ch == operators[i]){
              outputIdentifierToken();
              operatorBuffer[k] = ch;
              k++;
            }
        }

        if(isalnum(ch)){
            buffer[j++] = ch;
            outputOperatorToken();
        }
        else if((ch == ' ' || ch == '\n') && (j != 0)){
            buffer[j] = '\0';
            j = 0;

            if(isKeyword(buffer) == 1)
                cout << buffer << '\n';
            else
                cout << buffer << '\n';

        }

    }
    fin.close();
    return 0;
}