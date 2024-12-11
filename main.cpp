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

        string getStudentFirstName() const{
            return firstName;
        }

        string getStudentLastName() const{
            return lastName;
        }
};

void personType::setStudentName(string firstName, string lastName){
    this->firstName = firstName;
    this->lastName = lastName;
}

class courseType{
    private:
        string courseName, courseNo;
        char courseGrade;//student sınıfı bunu kullanıcak özellikle
        char courseCredits;

    public:
        courseType(string name = "", string number = "", int credits = 0, char gr = 'F')
        : courseName(name), courseNo(number), courseCredits(credits), courseGrade(gr) {

        }

        string getCourseName()const{
            return courseName;
        }

        string getCoursNo()const{
            return courseNo;
        }

        int getCourseCredit() const{
            return courseCredits; 
        }
        char getCourseGrade() const{
        return courseGrade; 
        }

};

class studentType : public personType{
    private:
        int numberOfCourses;
        courseType *courses;//gradeyi burdan çekicez
        int studentId;
        char isTuitionPaid;
    
    public:
        void print();
        void setInfo(string firstName, string lastName, int id, char isTuitionPaid, int numberOfCourses);
        void setCourses(courseType *courses);
        studentType();
        ~studentType(){
            delete[] courses;
        }
};

studentType::studentType(){
    numberOfCourses = 0;
    courses = NULL;
    isTuitionPaid = 'N';
}

void studentType::setInfo(string firstName, string lastName, int id, char isTuitionPaid, int numberOfCourses){
    setStudentName(firstName, lastName);
    studentId = id;
    this->isTuitionPaid = isTuitionPaid;
    this->numberOfCourses = numberOfCourses;
}

void studentType::setCourses(courseType *courses){
    this->courses = new courseType[numberOfCourses];
    for(int c = 0; c < numberOfCourses; c++){
        this->courses[c] = courses[c];
    }
}

void studentType::print(){
    
    cout << getStudentFirstName() 
        << " " <<getStudentLastName()
        << " " << studentId << " " 
        << isTuitionPaid << " " 
        << numberOfCourses << endl;

    for(int h = 0; h < numberOfCourses; h++){
        cout << courses[h].getCourseName()
            << " " << courses[h].getCoursNo()
            << " " << courses[h].getCourseCredit()
            << " " << courses[h].getCourseGrade() << endl;
    }
    cout << "..." << endl;
}

int main(){
    int number, tuitionPerHours;

    fstream file("input.txt", ios::in | ios::out | ios::app);
    file >> number;

    studentType *students = new studentType[number];

    file >> tuitionPerHours;

    string fname, lname, skip;
    int nOfcourses, id;
    char control;
    for(int i = 0; i < number; i++){
        file >> fname >> lname >> id >> control >> nOfcourses;
        students[i].setInfo(fname, lname, id, control, nOfcourses);

        courseType *courses = new courseType[nOfcourses];
        for(int j = 0; j < nOfcourses; j++){
            string courseName, courseNumber;
            int courseCredits;
            char courseGrade;

            file >> courseName >> courseNumber >> courseCredits >> courseGrade;
            courses[j] = courseType(courseName, courseNumber, courseCredits, courseGrade);
        }
        students[i].setCourses(courses);
        getline(file, skip);
        getline(file, skip);
        delete[] courses;
    }

    for(int i = 0; i < number; i++)students[i].print();

    return 0;
}