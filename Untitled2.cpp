#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <map>
#include <stack>

using namespace std;

class parser{
	
	public:
		
		string** Rules;
		string ** Table;
		int RuleNumber;
		std::map<string,int> TerminalMap;
		std::map<string,int> NonTerminalMap;
		
		parser()
		{
			//open the file which consist of a grammer
			ifstream fin("grammer.txt");
		    if(!fin.is_open())
			{
		        cout<<"error while opening the file\n";
		        fin.close();
		    }	
		    
		    RuleNumber = 0;
		    while(!fin.eof())
			{
				string line;
				getline(fin, line); 
				RuleNumber++;
			}
			
			Rules = new string *[RuleNumber];
			for(int i=0; i<RuleNumber;i++)
			{
				Rules[i]=new string[2];
			}
			
			getRules();
			list<string> Terminal = GetTerminal();
			list<string> NonTerminal = GetNonTerminal();
			
			//creat LL1 Table
			Table = new string *[NonTerminal.size() + 1];
			for(int i=0; i<NonTerminal.size() + 1 ;i++)
			{
				Table[i]=new string[Terminal.size() + 1];
			}
			
		}
	    void getRules()
		{
			//open the file which consist of a grammer
			ifstream fin("grammer.txt");
		    if(!fin.is_open())
			{
		        cout<<"error while opening the file\n";
		        fin.close();
		    }	
		    
		    RuleNumber = 0;
		    while(!fin.eof())
			{
				string line;
				getline(fin, line); 
				
				Rules[RuleNumber][0] = line[0];
				for(int i = 5; i<line.length(); i++)
					Rules[RuleNumber][1] += line[i];
				
				RuleNumber ++;	
			}	
		    
		}
		
		list<string> first(string NonTerminal)
		{
			list<string> First;
			
			if(!(NonTerminal >= "A" && NonTerminal<="Z"))
			{
				First.push_back(NonTerminal);
				return First;
			}
			
			for(int i=0; i<RuleNumber; i++)
			{
				if(Rules[i][0] == NonTerminal)
				{
					string s;
					stringstream ss;
					ss <<Rules[i][1][0];
					ss >> s;
					
					if(Rules[i][1] == "epsilon")
					{	
				
					}
					else if(!(Rules[i][1][0] >= 'A' && Rules[i][1][0] <='Z'))
					{
						First.push_back(ch2str(Rules[i][1][0]));
					}
					else if(s != NonTerminal )
					{	
						list<string> Temp = first(ch2str(Rules[i][1][0]));
						First.insert(First.end(), Temp.begin(), Temp.end());
						
						
						for(int k = 0 ; k<Rules[i][1].length();k++)
						{
							if(isEpsilon(ch2str(Rules[i][1][k])))
							{							
								list<string> Temp = first(ch2str(Rules[i][1][k+1]));
								First.insert(First.end(), Temp.begin(), Temp.end());
							}
							else 
							{
								break;
							}
						}
					}
					
					else if(s == NonTerminal && isEpsilon(NonTerminal))
					{
						list<string> Temp = first(ch2str(Rules[i][1][1]));
						First.insert(First.end(), Temp.begin(), Temp.end());
					}
						
				}
			}
			
			First.sort();
			First.unique();
			return First;
		}
		
		list<string> follow(string NonTerminal)
		{
			list<string> Follow;
			
			for(int i=0; i<RuleNumber; i++)
			{
				
				for(int j=0; j< Rules[i][1].length(); j++)
				{
					string s;
					stringstream ss;
					ss <<Rules[i][1][j];
					ss >> s;
						
					if(s == NonTerminal)
					{

						if( j == Rules[i][1].length() -1) 
						{	
							if(Rules[i][0] != NonTerminal)
							{
								list<string> Temp = follow(Rules[i][0]);
								Follow.insert(Follow.end(), Temp.begin(), Temp.end());
							}		
						}
						
						else if(!(Rules[i][1][j+1] >= 'A' && Rules[i][1][j+1] <='Z'))
						{
							Follow.push_back(ch2str(Rules[i][1][j+1]));
						}						
						else
						{
							
							list<string> Temp = first(ch2str(Rules[i][1][j+1]));
							Follow.insert(Follow.end(), Temp.begin(), Temp.end());
							
							for(int k = j+1 ; k<Rules[i][1].length();k++)
							{
								if(isEpsilon(ch2str(Rules[i][1][k])))
								{	
									if( k == Rules[i][1].length() - 1)
									{
										list<string> Temp = follow(Rules[i][0]);
										Follow.insert(Follow.end(), Temp.begin(), Temp.end());
									}	
									else 
									{
										list<string> Temp = first(ch2str(Rules[i][1][k+1]));
										Follow.insert(Follow.end(), Temp.begin(), Temp.end());
									}					
								}
								else
								{
									break;
								}
							}
							
						}					
						
					}
				}
				
			}
			Follow.sort();
			Follow.unique();
			return Follow;
		}
		
		string ch2str(char ch)
		{
			string s;
			stringstream ss;
			
			ss << ch;
			ss >> s;
			
			return s;			
		}
		
		bool isEpsilon(string NonTerminal)
		{
			
			for(int i=0 ; i< RuleNumber ; i++)
			{
				if(NonTerminal == Rules[i][0])
				{
					if(Rules[i][1] == "epsilon")
					
						return true;
				}
			}
			
			return false;
		}		
		
		list<string> GetNonTerminal()
		{
			list<string> NonTerminal;
			
			for(int i=0; i<RuleNumber; i++)
			{
				NonTerminal.push_back(Rules[i][0]);
			}
			
			NonTerminal.sort();
			NonTerminal.unique();
			
			return NonTerminal;
		}
		
		list<string> GetTerminal()
		{
			list<string> Terminal;
			for(int i=0; i<RuleNumber; i++)
			{
				
				for(int j=0; j<Rules[i][1].length() ; j++)
				{
					if(Rules[i][1]!= "epsilon")
					{
						if(!(Rules[i][1][j] >= 'A' && Rules[i][1][j] <= 'Z'))
						Terminal.push_back(ch2str(Rules[i][1][j]));	
					}			
				}		
			}
			
			Terminal.sort();
			Terminal.unique();
			
			return Terminal;
		}
		
		void LL1Table()
		{	
			list<string> Terminal = GetTerminal();
			list<string> NonTerminal = GetNonTerminal();
			
			//Declare name of each column in Table
			list<string> ::iterator i;
			int index = 1;
			for (i=Terminal.begin(); i!= Terminal.end(); ++i)
			{
				Table[0][index] = *i;
				TerminalMap[*i] = index;
				index ++;
			}
			
			//Declare name of each row in Table
			index = 1;
			for (i = NonTerminal.begin(); i != NonTerminal.end(); ++i)
			{
				Table[index][0] = *i;
				NonTerminalMap[*i] = index;
				index ++;
			}
			
			//Fill Table 
			for(int i=1 ; i<NonTerminal.size() + 1; i++)
			{
				string  TableNonTerminal = Table[i][0];
				
				for(int row = 0 ; row < RuleNumber ; row ++)
				{
					if(Rules[row][0] == TableNonTerminal)
					{
						if(Rules[row][1] != "epsilon")
						{
							list<string> temp = first(ch2str(Rules[row][1][0]));
							
							for(int k = 0 ; k<Rules[row][1].length();k++)
							{
								if(isEpsilon(ch2str(Rules[row][1][k])))
								{							
									list<string> Temp = first(ch2str(Rules[row][1][k+1]));
									temp.insert(temp.end(), Temp.begin(), Temp.end());
								}
								else 
								{
									break;
								}
							}
							
							temp.sort();
							temp.unique();
											
							list<string> ::iterator it;
							for (it=temp.begin(); it!= temp.end(); ++it)
							{							
								if(*it != "epsilon")
								{
									int column = TerminalMap[*it] ;
									stringstream ss;
									string s;
									ss<<row;
									ss>>s;
									Table[i][column] = s;
								}
							}
						}
						
						if(Rules[row][1] == "epsilon")
						{
							list<string> temp = follow(Rules[row][0]);					
							list<string> ::iterator it;
							for (it=temp.begin(); it!= temp.end(); ++it)
							{	
								if(*it != "epsilon")
								{
									int column = TerminalMap[*it] ;
									stringstream ss;
									string s;
									ss<<row;
									ss>>s;
									Table[i][column] = s;
								}												
							}
						}
					}
				}
				
			}	
			
			cout<<"\n\n LL(1) TABLE \n\n";
			//Print Table
			for(int i=0 ; i<NonTerminal.size() + 1; i++)
			{
				for(int j=0 ; j<Terminal.size() + 1; j++)
					cout<<Table[i][j]<<"\t";
				cout<<endl;
			}
				 	
		}
		
		void Trace(string input)
		{
			stack<char> inputStack;
			stack<char> parseStack;
			
			//add $ at the end of input
			input = input + "$";
			
			//push input to input stack
			for(int i=input.length()-1 ; i>=0 ; i--)
			{
				inputStack.push(input[i]);
			}
			
			//push $ and S to parse stack
			parseStack.push('$');
			parseStack.push('S');
			
			cout<<"\n\nParse Stack \t Input Stack \n";
			showTrace(parseStack,inputStack);
			
			while(true)
			{
				char parseFront = parseStack.top();
				char inputFront = inputStack.top();
				
				if(parseFront != inputFront)
				{
					int row;
					int column;
					
					if(NonTerminalMap.count(ch2str(parseFront)) > 0 )
						 row = NonTerminalMap[ch2str(parseFront)];
					else 
					{
						cout<<"Reject";
						break;
					}	
					if( TerminalMap.count(ch2str(inputFront)) > 0)
						 column = TerminalMap[ch2str(inputFront)];
					else 
					{
						cout<<"Reject";
						break;
					}
					string  number = Table[row][column];
					
					stringstream ss;
					int Number;
					ss<<number;
					ss>>Number;
					string temp = Rules[Number][1];
					
					parseStack.pop();
					if(temp != "epsilon")
					{
						//push input to input stack
						for(int i=temp.length()-1 ; i>=0 ; i--)
						{
							parseStack.push(temp[i]);
						}	
					}		
				}
				else if(parseFront == inputFront)
				{
					if(parseFront == '$')
					{
						cout<<"Accept"<<endl;
						break;
					}
						
					parseStack.pop();
					inputStack.pop();
				}
				
				showTrace(parseStack,inputStack);				
			}
					
		}
		
		void showTrace(stack<char> parseStack,stack<char> inputStack)
		{
			while(!parseStack.empty())
			{
				cout << parseStack.top();
				parseStack.pop();
			}		
			cout<<"\t\t";
			
			while(!inputStack.empty())
			{
				cout<<inputStack.top();
				inputStack.pop();
			}
			cout<<endl;		
		}
		
		void showRules()
		{
			for(int i =0 ; i<RuleNumber ; i++)
			{
				cout<<i<<" : "<<Rules[i][0]<<" -> "<<Rules[i][1]<<endl;
			}
			cout<< endl;
		}
		
		void showFirst()
		{
			for(int j = 0; j<RuleNumber ; j++)
			{
				list<string> First = first(Rules[j][0]);
				list<string> ::iterator i;
				
				cout << "first("<< Rules[j][0] <<") = { ";
				 for (i = First.begin(); i != First.end(); ++i)
				 	cout << *i << ", ";	  
			     cout<<"}"<< endl; 
			}       
			       
			 cout<<endl;		
		}	
		
		void showFollow()
		{
			for(int j = 0; j<RuleNumber ; j++)
			{
				list<string> Follow = follow(Rules[j][0]);
				list<string> ::iterator i;
				
				cout << "follow("<< Rules[j][0] <<") = {";
				 for (i = Follow.begin(); i != Follow.end(); ++i)
				 		cout << *i << ", ";
			     cout<<"}"<< endl; 
			}
		}
		
		void parse(string input)
		{
			showRules();
			showFirst();
			showFollow();
			LL1Table();
			Trace(input);
			
		}
};

void lexer()
{	
								
	char ch;
	char token[80];
	int state = 0;
	int length = 0;
	bool isRead = true;
	char keywords[47][15] = { "and", "assert", "break", "class",
							"continue", "def", "del", "elif"	
							"else", "except", "exec", "finally",
							"for", "from", "global", "if",
							"import", "in", "is", "lambda",
							"not", "or","pass", "print",
							"raise", "return","try", "while",
							"Data", "Float","Int", "Numeric",
							"Oxphys", "array","close", "float",
							"int", "input","open", "range",
							"type", "write","zeros"};
	
	//open the file which consist of a sample C program
	ifstream fin("test.txt");
    if(!fin.is_open())
	{
        cout<<"error while opening the file\n";
        fin.close();
    }
    
    while(!fin.eof())
	{
			if(isRead)
			{
				//read one character from file 
				ch = fin.get();
				//cout<<ch<<endl;
			}
			//cout<<state<<endl;	
           	
           switch(state)
           {
        		case 0:
				{
					if((( ch <= 'z' && ch >= 'a' ) || ( ch <= 'Z' && ch >= 'A' ) || ch == '_'))		{state = 1;  token[length] = ch; length ++; isRead = true;}
					else if(ch <='9' && ch >= '0')													{state = 3;  token[length] = ch; length ++; isRead = true;}
					else if(ch == '\"')																{state = 7;  token[length] = ch; length ++; isRead = true;}
					else if(ch == '\'')																{state = 17; token[length] = ch; length ++; isRead = true;}
					else if(ch == '#')																{state = 27; token[length] = ch; length ++; isRead = true;}
					else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')			{state = 28; token[length] = ch; length ++; isRead = true;}
					else if(ch == '&' || ch == '|' || ch == '~' || ch == '^')						{state = 30; token[length] = ch; length ++; isRead = true;}
					else if(ch == '<')																{state = 32; token[length] = ch; length ++; isRead = true;}
					else if(ch == '>')																{state = 35; token[length] = ch; length ++; isRead = true;}
					else if(ch == '(' || ch == ')'  || ch == '{' || ch == '}' || 
							ch == '[' || ch == ']' )												{state = 38; token[length] = ch; length ++; isRead = true;}
					else 
						isRead = true;
           			break;	
				}
				case 1:
				{
					if(( ch <= 'z' && ch >= 'a' ) || ( ch <= 'Z' && ch >= 'A' ) || (ch <='9' && ch >= '0') || ch == '_') 
					{	
						state = 1; 
						token[length] = ch; 
						length++; 
						isRead = true;
					}
					else	{state = 2; isRead = false;}																									
					break;
				}
				case 2:
				{	
				bool isKey;
					
					for(int i=0 ; i<47 ; i++)
					{
						isKey = true;
						for(int j=0 ; j<length; j++)
						{
							if(token[j] != keywords[i][j])
							{
								isKey = false;
								break;
							}
						}
						if(isKey == true)
						{
							break;
						}
					}
					if(isKey == false)	
					{
						for(int i=0 ; i<length; i++)
						cout<<token[i];
						cout<<" is an identifier.\n";	
						length = 0;
						state = 0;	
						isRead = false;	
					}
					else 
					{
						for(int i=0 ; i<length; i++)
							cout<<token[i];
						cout<<" is a keyword.\n";	
						length = 0;
						state = 0;	
						isRead = false;	
					}	
					break;
				}
				case 3:
				{
					if(ch <= '9' && ch >= '0') 		{state = 3; token[length] = ch; length++; isRead = true;}
					else if(ch == '.')				{state = 5; token[length] = ch; length++; isRead = true;}
					else 							{state = 4;isRead = false;}
					break;
				}
				case 4:
				{
					for(int i=0 ; i<length; i++)
							cout<<token[i];
					cout<<" is an integer.\n";	
					length = 0;
					state = 0;
					isRead = false;
					break;	
				}
				case 5:
				{
					if(ch <= '9' && ch >= '0') 		{state = 5; token[length] = ch; length++; isRead = true;}
					else 							{state = 6;isRead = false;}
					break;
				}
				case 6:
				{
					for(int i=0 ; i<length; i++)
							cout<<token[i];
					cout<<" is a float.\n";	
					length = 0;
					state = 0;
					isRead = false;
					break;
				}
				case 7:
				{
					if(ch == '\\')					{state = 9; token[length] = ch; length++; isRead = true;}
					else if(ch == '"')				{state = 11; token[length] = ch; length++; isRead = true;}
					else 							{state = 8; token[length] = ch; length++; isRead = true;}
					break;
				}
				case 8:
				{	
					if(ch == '\\')					{state = 9; token[length] = ch; length++; isRead = true;}
					else if(ch == '"')				{state = 10; token[length] = ch; length++; isRead = false;}
					else 							{state = 8; token[length] = ch; length++; isRead = true;}
					break;
				}
				case 9:
				{
					if(ch == 'n' || ch == 't' || ch == '"' || ch == '\\')		{state = 8; token[length] = ch; length++; isRead = true;}
					break;			
				}
				case 10:
				{
					for(int i=0 ; i<length; i++)
							cout<<token[i];
					cout<<" is a string.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}	
				case 11:
				{
					if(ch == '"')	{state = 12; token[length] = ch; length++; isRead = true;}	
					break;
				}
				case 12:
				{
					if(ch == '"')	{state = 13; token[length] = ch; length++; isRead = true;}
					break;
				}
				case 13:
				{
					if(ch == '"')	{state = 14; token[length] = ch; length++; isRead = true;}
					else 			{state = 13; token[length] = ch; length++; isRead = true;}
					break;
				}
				case 14:
				{
					if(ch == '"')	{state = 15; token[length] = ch; length++; isRead = true;}
					break;
				}
				case 15:
				{
					if(ch == '"')	{state = 16; token[length] = ch; length++; isRead = false;}
					break;
				}
				case 16:			
				{	
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a multiline comment.\n";	
					length = 0;
					state = 0;
					isRead = true;								
					break;
				}	
				case 17:
				{
					if(ch == '\\')			{state = 19; token[length] = ch; length++; isRead = true;}
					else if(ch == '\'')		{state = 21; token[length] = ch; length++; isRead = true;}
					else 					{state = 18; token[length] = ch; length++; isRead = true;}
					break;
				}
				case 18:
				{
					if(ch == '\\')			{state = 19; token[length] = ch; length++; isRead = true;}
					else if(ch == '\'')		{state = 20; token[length] = ch; length++; isRead = false;}
					else 					{state = 18; token[length] = ch; length++; isRead = true;}		
					break;
				}
				case 19:
				{
					if(ch == 'n' || ch == 't' || ch == '\\' ||ch == '\'')    		{state = 17; token[length] = ch; length++; isRead = true;}							
					break;
				}	
				
				case 20:
				{		
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a string.\n";	
					length = 0;
					state = 0;
					isRead = false;
					break;
				}
				case 21:
				{
					if(ch == '\'')    		{state = 22; token[length] = ch; length++; isRead = true;}	
					break;	
				}
				case 22:
				{
					if(ch == '\'')    		{state = 23; token[length] = ch; length++; isRead = true;}	
					break;
				}	
				case 23:
				{
					if(ch == '\'')    		{state = 24; token[length] = ch; length++; isRead = true;}
					else    				{state = 23; token[length] = ch; length++; isRead = true;}		
					break;
				}
				
				case 24:
				{
					if(ch == '\'')    		{state = 25; token[length] = ch; length++; isRead = true;}	
					break;	
				}
				case 25:
				{
					if(ch == '\'')    		{state = 26; token[length] = ch; length++; isRead = false;}	
					break;	
				}
				case 26:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a multiline comment.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}
				case 27:
				{
					if((int)ch == 10)
					{
						for(int i=0 ; i<length; i++)
							cout<<token[i];
						cout<<" is a single comment.\n";	
						length = 0;
						state = 0;
						isRead = true;
					}
					else 		{state = 27; token[length] = ch; length++; isRead = true;}
					break;
				}	
				case 28:
				{
					if(ch == '=')				{state = 29; token[length] = ch; length++; isRead = false;}	
					else 
					{
						for(int i=0 ; i<length; i++)
							cout<<token[i];
						cout<<" is an arithmatic operator.\n";	
						length = 0;
						state = 0;
						isRead = false;
					}
					break;
				}
				case 29:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is an arithmatic assignment.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}	
				case 30:
				{
					if(ch == '=')				{state = 31; token[length] = ch; length++; isRead = false;}	
					else 
					{
						for(int i=0 ; i<length; i++)
							cout<<token[i];
						cout<<" is a bitwise operator.\n";	
						length = 0;
						state = 0;
						isRead = false;
					}
					break;
				}	
				case 31:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is an bitwise assignment.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}	
				case 32:
				{
					if(ch == '<')				{state = 33; token[length] = ch; length++; isRead = false;}
					else if(ch == '=')				{state = 34; token[length] = ch; length++; isRead = false;}
					else 
					{
						for(int i=0 ; i<length; i++)
							cout<<token[i];
						cout<<" is a relational operator.\n";	
						length = 0;
						state = 0;
						isRead = false;
					}
					break;
				}	
				case 33:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a bitwise operator.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}
				case 34:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a relational operator.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}
				case 35:
				{
					if(ch == '>')				{state = 33; token[length] = ch; length++; isRead = false;}
					else if(ch == '=')				{state = 34; token[length] = ch; length++; isRead = false;}
					else 
					{
						for(int i=0 ; i<length; i++)
							cout<<token[i];
						cout<<" is a relational operator.\n";	
						length = 0;
						state = 0;
						isRead = false;
					}
					break;
				}
				case 36:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a bitwise operator.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}
				case 37:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a relational operator.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}
				case 38:
				{
					for(int i=0 ; i<length; i++)
						cout<<token[i];
					cout<<" is a delimiter.\n";	
					length = 0;
					state = 0;
					isRead = true;
					break;
				}	
           }        
    }
    
    fin.close();	
}


int main()
{
	cout<<"Hello,please select what you want:\n";
	cout<<"\t1. lexical Analysis\n";
	cout<<"\t2. syntax analysis\n";
	int inputNumber;
	cin>>inputNumber;
	
	if(inputNumber == 1)
	{
		lexer();		
	}
	else if(inputNumber == 2)
	{
		cout<<"LL(1)\n";
		parser P;
		//P.parse("ddabbb");
		P.parse("dd");
	}
	
	return 0;
	
}
