#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class personType{
    private:
        string firstName;
        string lastName;

    public:
        void setStudentName(string firstName, string lastName);
        void getStudentName();
};

void personType::setStudentName(string firstName, string lastName){
    this->firstName = firstName;
    this->lastName = lastName;
}

void personType::getStudentName(){
    cout << firstName << "||" << lastName << "||";
}

class courseType{
    private:
        string courseName;
        int courseNo;
        char courseGrade;//student sınıfı bunu kullanıcak özellikle
        char courseCredits;

    public:
        // void setCourseInfo();
        // void print(int one);
        // void print(int one, int two);
        // void getCredits();
        // void getCourseNumber();
        // void getGrade();
        // courseType();

};

class studentType : public personType{
    private:
        int numberOfCourses;
        courseType *course;//gradeyi burdan çekicez
        string studentId;
        char isTuitionPaid;
    
    public:
        void print();
        // void print(int one);
        // void print(int one, int two);
        void setInfo(string firstName, string lastName, string id, char isTuitionPaid, int numberOfCourses);
        // void getHoursEnrolled();
        // void getGpa();
        // void billingAmount();
        studentType();
};

studentType::studentType(){
    numberOfCourses = 0;
    course = NULL;
    isTuitionPaid = 'N';
}

void studentType::setInfo(string firstName, string lastName, string id, char isTuitionPaid, int numberOfCourses){
    setStudentName(firstName, lastName);
    studentId = id;
    this->isTuitionPaid = isTuitionPaid;
    this->numberOfCourses = numberOfCourses;
}

void studentType::print(){
    getStudentName();
    cout << studentId << "||" << isTuitionPaid << "||" << numberOfCourses << endl;
}

int main(){
    int number, tuitionPerHours;

    fstream file("input.txt", ios::in | ios::out | ios::app);
    file >> number;

    studentType *students = new studentType[number];

    file >> tuitionPerHours;

    string fname, lname, id, line;
    int nOfcourses;
    char control;
    for(int i = 0; i < number; i++){
        file >> fname >> lname >> id >> control >> nOfcourses;
        students[i].setInfo(fname, lname, id, control, nOfcourses);
        while(getline(file, line)){
            
        }
    }

    students[0].print();

    return 0;
}