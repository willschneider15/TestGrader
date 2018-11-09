//* * * * * * * * * * * * * * * * * * * * * * * * * *
//
// program: TestGrader/Homework7
//
// Name : William Schneider
// Email : ws828417@ohio.edu
//
// Description: This application grades a class of student answer's on a multiple choice test,
//              you input the file with the answer key and student answers. It outputs to a
//              file or to a termianl window; printing organized test grades in an answer sheet.
//
// Date : Nov/9/2018
//
// * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <iomanip> //need this for formating setw
#include <iostream> //used for outputting to the terminal
#include <fstream> //needed so I can use files input output streams
#include <vector> //opted to use vectors instead of arrays
#include <cctype> //used this just to make sure everything was capitalized for checking answers

using namespace std; //used for cout
char getGrade(double score); //for changing percent grade to letter grade
void outputTerminal(string name, vector<char> student, vector<char> answers,double score); //to output to terminal
void outputFile(string name, vector<char> student, vector<char> answers,double score, ofstream &ofs); //to output to file
double studentTest(vector<char> answers, vector<char> &grades, ifstream &ins); //to test student and hold there answers

int main (int argc, char **argv){
    vector<char> answerKey; //variables used and distributed in the main scope
    vector<string> highestScoreStudents; //this is used just to hold students that achieve the highest scores
    string firstName, lastName, fullName; //to hold onto names
    double score, classAverage, totalScore = 0, totalStudents = 0, highestScore = 0; //keeping track of stats
    ifstream ins; //input and output streams
    ofstream ofs;
    char ch; //used to get the answer key

    if(argc == 2){ //opening all necesary files and making sure program can run
        ins.open(argv[1]);
        if(ins.fail()){
            cout << "There is no input file under this name" << endl;
            return 0;
        }
    }
    else if(argc == 3){ //this is for the option with the output file
         ins.open(argv[1]);
         if(ins.fail()){
             cout << "There is no input file under this name" << endl;
             return 0;
         }

         ofs.open(argv[2]);
         if(ofs.fail()){
            cout << "There is no output file under this name" << endl;
            return 0;
         }
    }
    else{ //there is no other option available to run this application
        cout << "Files not inputted correctly" << endl;
        return 0;
    }

    ins.get(ch); //sentinel loop to gather the answer key for testing the students
    while(ch != '\n'){
        answerKey.push_back(ch); //this becomes the answer key
        ins.get(ch);
    }
    vector<char> grades(answerKey.size()); //have to set it to a size because otherwise itll have sementation error
                                           //when trying to set indexs to values

    while(!ins.eof()){ //this loop is to get each students information and call to output all needed information
        ins >> firstName;
        ins >> lastName;
        fullName = lastName + ", " + firstName; // just used to format output
        score = studentTest(answerKey, grades, ins); //getting the student score on test and holding student answers

        totalStudents++; //using these in the calculation of class average
        totalScore += score;

        if(score > highestScore){ //testing if there is a new high score
                    highestScore = score; //changing high score
                    highestScoreStudents.clear(); //clearing array for new class leader
                    highestScoreStudents.push_back(firstName + " " + lastName); //adding them to new array
        }
        else if(score == highestScore){ //testing if there is a tie for high score
                 highestScoreStudents.push_back(firstName + " " + lastName); //adding student, a tie for class high score
        }

        if(argc == 2){  //test if should output to terminal or file
            outputTerminal(fullName, grades, answerKey, score);
        }
        else{ //to a file
            outputFile(fullName, grades, answerKey, score, ofs);
        }

    }
    //finished outputing student answer sheet, finishing by printing class statistics

    classAverage = totalScore/totalStudents; //calculation for class average;
    if(argc == 2){ //displaying class average + grade plus formatting for clean display
        cout << "\n" << "Class average: " << classAverage << "% " << getGrade(classAverage) << endl;
        cout << endl;
        cout << "Students with the highest grades (" << highestScore << "%):" << endl;  //displaying the highest scores
            for(size_t i = 0; i < highestScoreStudents.size(); i++){
                cout << highestScoreStudents[i] << endl;
            }
        cout << endl;
    }
    else{ //this outputs to a file
        ofs << "\n" << "Class average: " << classAverage << "% " << getGrade(classAverage) << endl;
        ofs << endl;
        ofs << "Students with the highest grades (" << highestScore << "%):" << endl; //displaying the highest scores
                    for(size_t i = 0; i < highestScoreStudents.size(); i++){
                        ofs << highestScoreStudents[i] << endl;
                    }
        ofs << endl;
    }



    ins.close(); //closing all files and ending the program
    ofs.close();
    return 0;
}

//*****************************************************************
// Function: studentTest
 //
 // Purpose: This is to get the student grade percentage and
 //          hold all of the test answers in a vector
 //
 // Parameters: The vector of answers to check students,
 //             the referenced vector for holding student answers,
 //             and the instream to read answers from
 //
 // Member/Global Variables: none
 //
 // Pre Conditions: The answer key must be inputted, size of vector needs to be set
 //                 and there must be a file open to read from
 //
 // Post Conditions: returns the score in a percentage out of 100 that the student had,
 //                  and references the the students answers into a vector.
//******************************************************************
double studentTest(vector<char> answers, vector<char> &grades, ifstream &ins){
    double totalCorrect = 0;    //used to calculate score on test
    double totalAnswers = double(answers.size());   //used to calculate score on test
    int index = 0;  //need to keep track of which problem comparing to answer key
    char ch; //to grab the student answer
    ins.get(ch);//skipping the space from name to student answers
    ins.get(ch);//holding the first student answer
    while(ch != '\n' && !ins.eof()){ //ending grading if reach end of line or file
        if(ch == ' '){ //changing this in the vector for cleaner output
            ch = '-';
        }
        grades[index] = ch; //always setting them to indexs between 0 and length of test
        if(char(toupper(grades[index])) == answers[index]){ //answers match test
            totalCorrect++; //answer is correct
        }
        ins.get(ch); //used for the sentinel loop to grab next student answer
        index++; //going to the next index
    }
    return (totalCorrect/totalAnswers)*100; //its a decimal for score, multiply by 100 to turn to percent

}

//*****************************************************************
// Function: outputTerminal
 //
 // Purpose: This prints all the student report cards and class stats
 //          using cout to the terminal
 //
 // Parameters: name - name of student
 //             student - the answers the student choose
 //             answers - the answers to the test
 //             score - the score teh student got
 //
 // Member/Global Variables: none
 //
 // Pre Conditions: all values are valid
 //
 // Post Conditions: prints the values in an organized fashion to the terminal
//******************************************************************
void outputTerminal(string name, vector<char> student, vector<char> answers,double score){
    int x = 0; //used to keep track of which problem on the test its looking at
    cout << "\n" << name;
    cout << setfill ('-') << setw(60) << endl;//for formating
    cout << "\n" << "Answers, correct answer in parenthesis" << endl;
    cout << endl;
    cout << setfill (' '); //so the fill still isnt '-'
    for(size_t i = 1; i <= answers.size()/5; i++){//this is for amount of rows which is dependent on total answers
        for(size_t r = 0; r < 5; r++){
            cout << setw(5) << x+1 << ": " << char(toupper(student[x])) << "(" << answers[x] << ")";
            x++;
        }
        cout << endl;
    }
    cout << setfill ('-') << setw(60) << endl;//for formating
    cout << setprecision(1) << fixed << showpoint; //so percentage is one decimal place
    cout << "\n" << "Score: " << score << "% " <<getGrade(score)<<endl; //printing the final score
}

//*****************************************************************
// Function: outputFile
 //
 // Purpose: This prints all the student report cards and class stats
 //          using output stream to a file
 //
 // Parameters: name - name of student
 //             student - the answers the student choose
 //             answers - the answers to the test
 //             score - the score teh student got
 //
 // Member/Global Variables: none
 //
 // Pre Conditions: all values are valid
 //
 // Post Conditions: prints the values in an organized fashion to a file
//******************************************************************
void outputFile(string name, vector<char> student, vector<char> answers,double score, ofstream &ofs){
    int x = 0; //used to keep track of which problem on the test its looking at
    ofs << "\n" << name; //displaying the name
    ofs << setfill ('-') << setw(60) << endl; //for formating
    ofs << "\n" << "Answers, correct answer in parenthesis" << endl;
    ofs << endl;
    ofs << setfill (' '); //so the fill still isnt '-'
    for(size_t i = 1; i <= answers.size()/5; i++){ //this is for amount of rows which is dependent on total answers
        for(size_t r = 0; r < 5; r++){
            ofs << setw(5) << x+1 << ": " << char(toupper(student[x])) << "(" << answers[x] << ")";
            x++;
        }
        ofs << endl;
    }
    ofs << setfill ('-') << setw(60) << endl; //for formating
    ofs << setprecision(1) << fixed << showpoint; //so the percentage is one decimal place
    ofs << "\n" << "Score: " << score << "% " <<getGrade(score)<<endl; //the score on the test
}

//*****************************************************************
// Function: getGrade
 //
 // Purpose: to turn a percent grade into a letter grade
 //
 // Parameters: score-ther percent grade of the student
 //
 // Member/Global Variables: none
 //
 // Pre Conditions: score is valid
 //
 // Post Conditions: returns the letter grade equivalent
//******************************************************************
char getGrade(double score){
    if(score <= 100 && score >= 90){ //simply going thru the score possibilities
        return 'A';
    }
    else if(score < 90 && score >= 80){
        return 'B';
    }
    else if(score < 80 && score >= 70){
        return 'C';
    }
    else if(score < 70 && score >= 60){
        return 'D';
    }
    else{
        return 'F';
    }
}
