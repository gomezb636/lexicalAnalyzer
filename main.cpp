#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<iomanip>

using namespace std;
char ch, buffer[15], operatorBuffer[4], operators[] = "+-><*/%=()#^~|&?!,:;.[]{}", terminalOperators[] = "})]";
string constBuffer = "";
int i, j=0, k=0;
string filename;
ofstream outputfile;

int isKeyword(char buffer[]){
    char keywords[33][10] = {"auto","break","case","char","const","continue","default",
                            "do","double","else","enum","extern","float","for","goto",
                            "if","int","long","register","return","short","signed",
                            "sizeof","static","struct","switch","typedef","union",
                            "unsigned","void","volatile","while", "std"};

    int i, flag = 0;
    for(i = 0; i < 33; ++i){
        if(strcmp(keywords[i], buffer) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}

bool isTerminalOperator(char ch){
  bool isTerminal = false;
  for(i = 0; i < sizeof(operators); ++i){
    if(ch == terminalOperators[i]){
      isTerminal = true;
      break;
    }
  }
  return isTerminal;
}

void outputOperatorToken(){
  string str = "";
  if (k != 0){
    operatorBuffer[k] = '\0';
    outputfile << left << setw(10) << operatorBuffer << right << setw(25) << " (operator)\n";
    k = 0;
  }
}

void outputIdentifierToken(){
  if(j != 0){
    // to make it a string add null at the end
    buffer[j] = '\0';
    outputfile << left << setw(10) << buffer << right << setw(25) << " (identifier)\n";
    j = 0;
  }
}

void outputConstToken(){
  if(constBuffer != ""){
    outputfile << left << setw(10) << constBuffer << right << setw(25) << " (constant)\n";
    constBuffer = "";
  }
}

int main(){
    bool bConstString = false;
    string strConst;

    outputfile.open("output.txt");

    cout << "Please enter filename: ";
    cin >> filename;

    ifstream fin(filename);
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
            outputfile << "\"" << strConst << left << setw(15) << "\"" << left << " (string literal)\n";
            strConst = "";
          }
          continue;
        }

        if (bConstString == true){
          strConst += ch;
          continue;
        }

        // operators
        for(i = 0; i < sizeof(operators); ++i){
            if(ch == operators[i]){
              outputIdentifierToken();
              outputConstToken();
              if(isTerminalOperator(ch)){
                outputOperatorToken();
                outputfile << left << setw(10) << ch << right << setw(25) << " (operator)\n";
              }
              else{
                operatorBuffer[k] = ch;
                k++;
              }
              break;
            }
        }

        if(isalpha(ch)){
            buffer[j++] = ch;
            outputOperatorToken();
        }

        if(isdigit(ch)){
            // it's not beginning with a number
            if (j != 0){
              buffer[j++] = ch;
            }
            else{
              constBuffer += ch;
            }
            outputOperatorToken();
        }
        else if((ch == ' ' || ch == '\n') && (j != 0)){
            buffer[j] = '\0';
            j = 0;

            if(isKeyword(buffer) == 1)
                outputfile << left << setw(10) << buffer << right << setw(25) << " (keyword)\n";
            else
                outputfile << left << setw(10) << buffer << right << setw(25) << " (identifier) \n";

        }

    }
    fin.close();
    outputfile.close();
    return 0;
}