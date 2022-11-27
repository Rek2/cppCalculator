#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
#include <cmath>
#include <sstream>
using namespace std;

class Calculator //Class calculator
{
public:
    char c = ' '; //Char the tracks the entered keyboard input
    char op = '?'; //Char that tracks what operator is being applied
    string calcDisplay = ""; //String that is put onto the calculators display
    string currentCalc = ""; //String to keep track of working equation
    vector<string> sVector; //Vector to hold each equation as a string
    map<string, string> myMap; //Map to store memory

    //Memory function, adds given key value pair into calculator map.
    string memoryFunction(char storeRetrieve, char memKey, string memVal)
    {
        string key = "M"; 
        key = key + memKey; //Add number entered into key string

        switch (storeRetrieve)
        {
        case 'M': //If user entered char M (for storing to memory)
            myMap[key] = memVal; //Set key memKey to memVal
            return memVal; //Return memVal (this is so that our calculator display doesn't change when you store a value to memory)

        case 'R': //If user entered char R (for retrieving from memory)
            return myMap[key]; //Return value in key memKey

        default: //If neither work report error
            cout << "Memory function Error" << endl; 
            return "";
        }


    }

    //Function that controls how the calculator display reacts to key presses. Takes char as keypress
    void displaySequence(char c) 
    {
        c = (toupper(c)); //Convert lowercase inputs to uppercase
        if (c == '\t' || c == '\n' || c == '\r' || c == ' ') return; //Continue if any whitespace is entered
        if (c == '\b') //If backspace is entered remove last char in string calcDisplay
        {
            if (calcDisplay == "") return; //Avoid crash if s is already empty
            calcDisplay.pop_back(); //Remove last char in string s and continue if backspace is entered
            op = '?';
        }

        if ((c == '+' || c == '-' || c == '*' || c == '/' || c == 'M') && calcDisplay != "") //Turn operator display into user entered operator by changing char op into entered char
        {
            op = c;
        }

        if (c == 'R')  //If user entered R (access memory), we have to handle if they also are adding the number in memory to the number on the display
        { 
            switch (op)
            {
            case '?': case 'M': //If operator is ? (blank) or M (save to memory)
                op = c; //Set operator to 'R'
                break;

            default: //If operator is anything else (+ - / *)
                currentCalc += (calcDisplay + op); //Add display number and correspoding operator to our currentCalc string
                op = c; //Set our operator to R
                calcDisplay = ""; //Wipe display num
                break;

            }
             
        }
        if (c == 'C') { currentCalc = ""; calcDisplay = ""; } //If c is entered, clear display and the calculation that has been building
        if (c == 'K') { calcDisplay = ""; } //If k is entered, only clear the display
        if (c == 'S' && calcDisplay != "")calcDisplay = to_string(sqrt(stod(calcDisplay))); //Square root method !note i probably should make this 2dp, but i also think it's worth keeping more precision for sqrt
        if (c == '!' && calcDisplay != "")calcDisplay = to_string(stod(calcDisplay) * -1); //Negate method !same as above but i am lazy

        if ((op == 'M' || op == 'R') && (c >= '0' && c <= '9')) //If M or R (memory and retrive from memory) are the current operator, AND the next key pressed is a number
        {
            calcDisplay = memoryFunction(op, c, calcDisplay); //Send the operator (m or r), the char (0-9) and the current calue of display to memoryFunction()
            op = '?'; //Set operator back to ?
            c = ' '; //Set char entered to ' '
        }

        //Adds user entered number into display
        if (c >= '0' && c <= '9' || c == '.') //If user inputs a number (or decimal)
        {
            if (op != '?' && op != 'M' && op != 'R') //If operator isn't ? M or R
            {
                currentCalc += (calcDisplay + op); //Add numbers on display and the operator into currentCalc string
                op = '?';
                calcDisplay = "";
            }

            int displayLen = calcDisplay.length(); //Set displaylength to equal length of calcDispaly string as int
            if (displayLen <= 9) calcDisplay = calcDisplay + c;  //Append entered char into string if the displaylength doesn't exceed 9 characters
        
        }

        //If user enters =, calculate the working equation and add it to display
        if (c == '=' && op == '?' && calcDisplay != "") 
        {
            currentCalc += (calcDisplay + c); //Add display into working equation
            sVector.push_back(currentCalc); //Store string of working equation into a new element in sVector
            currentCalc = ""; //Reset the working calculation

            ostringstream streamObj3; //Create a stream object
            streamObj3 << std::fixed; //Set precision to fixed
            streamObj3 << std::setprecision(2); //Set precision to 2DP
            streamObj3 << calcSequence(sVector); //Send vector of strings to calculation function. Feed input into streamObj
            string strObj3 = streamObj3.str(); //Declare new string as streamObj3 converted to string

            calcDisplay = strObj3; //Set calculator display as answer from calculation with 2DP
        }

        //Display formatting

        //If user has stored anything, display storage
        if (!myMap.empty())
        {
            cout << "Memory Values" << endl;
            cout << "*-----------------*" << endl;

            map<string, string>::iterator iter;  //Construct an iterator
            for (iter = myMap.begin(); iter != myMap.end(); ++iter) //Loop through each key-value pair in map
            {
                cout << "| " << iter->first << " | " << setw(10) << iter->second << " | " << '\n';
                cout << "*-----------------*" << endl;
            }
     
        }

        //Show user display and working calculations and operator
        cout << currentCalc << endl;
        cout << "______________________" << endl;
        cout << "|" << setw(20) << calcDisplay << "|" << " (" << op << ")" << " <- Operator" << endl;
        cout << "----------------------" << endl;
        cout << endl;
    }

    //Function for running the caluclator using a given vector of strings
    void inputSequence(vector<string> sequence)
    {
        //For each line, for each character in that line, run calculator inputting that char
        for (int i = 0; i < sequence.size(); i++)
        {
            for (char const& c : sequence[i])
            {
                if (c == ';') return; //if ; is entered then return
                cout << "Key entered: [" << c << "]" << endl;
                cout << endl;
                displaySequence(c);
            }
        }
    }

    //Function for running the calculator using user inputs
    void createSequence()
    {

        //Show instance of display so user knows they can begin
        cout << "______________________" << endl;
        cout << "|" << setw(20) << "|" << " (" << '?' << ")" << " <- Operator" << endl;
        cout << "----------------------" << endl;
        cout << endl;

        while (c != ';') //Exit on ;
        {
            c = _getch(); //Store keystroke into c
            displaySequence(c); //Send keystroke into calculator function
        }

        return;
    }

    //Function that takes a vector of strings, where each element is a string representing an equation. Returns the result of running all equations line by line (this allows run on equations)
    double calcSequence(vector<string> sequence)
    {
        double result = 0;
        vector<double> sequenceResult; //Vector to store result of each line

        //First, format vector of strings, where each element in the vector represents a mathematical expression
        for (int i = 0; i < sequence.size(); i++) //For each equation in our sequence
        {
            vector<double> terms; //Vector of terms
            vector<double> multiplier; //Vector of multipliers
            vector<char> operations; //vector of operations
            string tempNum = ""; //String to hold temp numbers
            int o = -1; //iterator counter
            for (char const& c : sequence[i])
            {
                o++;

                //If the first char in the string is -, add that to operation list and continue
                if (c == '-' && o == 0)
                {
                    operations.push_back(c);
                    continue;
                }

                if (c != '+' && o == 0) { operations.push_back('+'); } //If first char is + add it to operation list (doesn't continue)

                if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') //If any operator is pressed
                {
                    terms.push_back(stod(tempNum)); //Push tempNum into vector of terms
                    if (c != '=') operations.push_back(c); //Push our operator into vector of operator (unless it's =)
                    tempNum = ""; //Reset temp num
                    continue;
                }

                //Adds char to tempNum to build a number longer than 1 character
                if (c >= '0' && c <= '9' || c == '.') //if c is a number)
                {
                    tempNum += c;
                    continue;
                }
                else //If character isn't accepted previously to this than it must be invalid for our program
                {
                    cout << "invalid character has been found: " << c << " ";
                    break;
                }

            }

            //Second, take a line and find out it's result.
            vector<double> newTerms; //Vector to store new terms, any terms that come after a multiplier will NOT be stored into this vector

            //Iterates through each element (number) in vector of terms, if the corrosponding operation for that number is a + or - then add it into list of new terms
            //then add a multiplier of 1 for that term into our vector of multipliers
            //If the corresponding oepration is * or / then we take that term, DON'T add it to new terms, but instead multiply the corresponding multiplier in our multiplier vector by that term
            //This thing is maybe stupid and complicated but it means i follow bodmas!!!!!
            for (int a = 0; a < terms.size(); a++)
            {
                if (operations[a] == '+')
                {
                    multiplier.push_back(1);
                    newTerms.push_back(terms[a]);
                }

                if (operations[a] == '-')
                {
                    multiplier.push_back(-1);
                    newTerms.push_back(terms[a]);
                }

                if (operations[a] == '*')
                {
                    multiplier.back() = multiplier.back() * terms[a];
                }

                if (operations[a] == '/')
                {
                    multiplier.back() = multiplier.back() / terms[a];
                }

            }

            sequenceResult.push_back(0); //Create a new element in sequence of result starting at 0
            for (int z = 0; z < newTerms.size(); z++) //For each term in newTerms
            {
                sequenceResult[i] += newTerms[z] * multiplier[z]; //Take the term, multiply it by it's multilier (meaning we follow bodmas!) then add it our curret result element
            }
            
        }

        result = sequenceResult[sequenceResult.size() - 1]; //result = the result of the last sequence
        
        return result;

    }

    //Reads a txt file and returns vector of strings where each line is an equation
    vector<string> readSequence(string fileName)
    {
        vector<string>sequence;
        fstream myFile; //Declare variable as fstream

        myFile.open(fileName, ios::in); //Open file in read mode
        if (myFile.is_open())
        {
            string line; //Store each line into a string
            while (getline(myFile, line))
            {
                sequence.push_back(line); //Add that line into new element in vector
            }
        myFile.close();
        }
        else
        {
            cout << "Failed to open file!" << endl;
            cout << endl;
        }

        return sequence; //Return vector of lines
    }
};

class Directory
{
public:
    //File handling code from module
#define DEFDIR "E:\\A_ST2\\ST2_2022\\Assignment2\\"
    string dir = DEFDIR;// 
    string filename;
    string fullFileName;

    bool getFileName()
    {
        cout << "enter filename:";
        cin >> filename;
        cout << endl;
        fullFileName = dir + filename;
        cout << "Full FileName is:'" << fullFileName << "'" << endl;
        if (fileExists(fullFileName)) return true;
        cout << "** ERROR ** File '" << fullFileName << "'Does not exist";
        return false;
    }

    bool fileExists(string fullFileName)
    {
        fstream myFile;
        myFile.open(fullFileName, ios::in);
        if (myFile.is_open())
        {
            myFile.close();
            return true;
        }
       
    return false;
 
    }

    void do_S()
    {
        cout << "enter directory:";
        cin >> dir;
        cout << endl;
        cout << "\nCurrent Directory :" << dir << endl;
    }
    void do_R()
    {
        cout << "\nCurrent Directory :" << dir << endl;
        if (!getFileName()) return;
    }
};

int main()
{
    char select = '0';
    vector<string> sequence;
    Calculator calc;
    Directory dir;

    while (select != 'E' && select != 'e')
    {
        //Reset calculator
        calc.c = ' ';
        calc.op = '?';
        calc.calcDisplay = "";
        calc.currentCalc = "";
        calc.sVector.clear();
        calc.myMap.clear();

        cout << "Select one of the following options:" << endl;
        cout << "E - Exit \nS - Set Directory \n4 - Run 4 function calculator using keyboard input \nR - Read a test file and run test" << endl;
        cin >> select;

        switch (select) {
        case '4':
            calc.createSequence();
            break;

        case 'R':
            dir.do_R();
            calc.inputSequence(calc.readSequence(dir.fullFileName));
            break;

        case 'E': case 'e':
            return 0;

        case 'C': case 'c': //might add a controls menu?
            break;

        case 'S': case 's':
            dir.do_S();
            break;

        default:
            break;
        }

    }
    return 0;
}
