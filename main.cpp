#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

class personType{
    private:
        string firstName;
        string lastName;

    public:
        void setName(string firstName, string lastName);

        string getName() const{
          return firstName + " " + lastName;
        }
};

void personType::setName(string firstName, string lastName){
    this->firstName = firstName;
    this->lastName = lastName;
}

class courseType{
    private:
        string courseName, courseNo;
        char courseGrade;
        int courseCredits;

    public:
        courseType(string name = "", string number = "", int credits = 0, char gr = 'F')
        : courseName(name), courseNo(number), courseCredits(credits), courseGrade(gr) {

        }
        void setCourseInfo(string courseName, string courseNo, char courseGrade, int courseCredits);

        string getCourseName()const;
        string getCourseNo()const;
        int getCourseCredit()const;
        char getCourseGrade()const;

        bool operator<(const courseType &other)const;

};

// kursları tek bir fonksiyonda değiştir
void courseType::setCourseInfo(string courseName, string courseNo, char courseGrade, int courseCredits){
            this->courseName = courseName;
            this->courseNo = courseNo;
            this->courseGrade = courseGrade;
            this->courseCredits = courseCredits;
}

string courseType::getCourseName()const{
    return courseName;
}

string courseType::getCourseNo()const{
    return courseNo;
}

int courseType::getCourseCredit() const{
    return courseCredits; 
}

char courseType::getCourseGrade() const{
    return courseGrade; 
}

// std::sort için küçüktür operatörünü overload ediyoruz
// Sıralama kriteri, kurs numarasının alfabetik değeri
bool courseType::operator< (const courseType& other)const{

  int minLength = min(other.getCourseNo().length(), getCourseNo().length());

  for(int i = 0; i < minLength; i++){
    if (getCourseNo()[i] < other.getCourseNo()[i]) return true;
    if (getCourseNo()[i] > other.getCourseNo()[i]) return false;
  }

  return getCourseNo().length() < other.getCourseNo().length();
}

class studentType : public personType {
    private:
        int numberOfCourses;
        courseType *courses;
        int studentId;
        bool isTuitionPaid;
        int tuitionPerHours;
    
    public:
        void print(ofstream &outputFile);
        void setInfo(string firstName, string lastName, int studentId, bool isTuitionPaid, int numberOfCourses, int tuition);
        void setCourses(courseType *courses);
        int getHoursEnrolled();
        float getGPA();
        int bill();
        studentType();
        ~studentType(){
            delete[] courses;
        }
};

studentType::studentType(){
    numberOfCourses = 0;
    courses = NULL;
    isTuitionPaid = false;
}

void studentType::setInfo(string firstName, string lastName, int studentId, bool isTuitionPaid, int numberOfCourses, int tuition){
    setName(firstName, lastName);
    this->studentId = studentId;
    this->isTuitionPaid = isTuitionPaid;
    this->numberOfCourses = numberOfCourses;
    this->tuitionPerHours = tuition;
}

void studentType::setCourses(courseType courses[]){
    this->courses = new courseType[numberOfCourses];
    for(int c = 0; c < numberOfCourses; c++){
        this->courses[c] = courses[c];
    }
}

int studentType::getHoursEnrolled(){
    int sum = 0;
    for(int i = 0; i < numberOfCourses; i++){
        sum += this->courses[i].getCourseCredit();
    }
    return sum;
}

float studentType::getGPA(){
    float gpa = 0;

    for(int i = 0; i < numberOfCourses; i++){
        char check = this->courses[i].getCourseGrade();
        
        switch(check){
            case 'A':
                gpa += 4 * this->courses[i].getCourseCredit();
                break;
            case 'B':
                gpa += 3 * this->courses[i].getCourseCredit();
                break;
            case 'C':
                gpa += 2 * this->courses[i].getCourseCredit();
                break;
            case 'D':
                gpa += this->courses[i].getCourseCredit();
                break;
            default:
                break;
        }
    }
    gpa /= getHoursEnrolled();
    return gpa;
}

int studentType::bill(){
    return getHoursEnrolled() * tuitionPerHours;
}

void studentType::print(ofstream &outputFile){
    
    sort(courses, courses + numberOfCourses);
    
    outputFile
        << "Student Name: " << getName() << endl 
        << "Student ID : " << studentId << endl
        << "Number of courses enrolled: " << numberOfCourses << endl
        << endl;
    
    if (!isTuitionPaid) {
        outputFile 
            << "Öğrenim ücreti ödenmemiştir." << endl
            << "Ücret : " << bill() << "TL."<< endl;
    }
    else {
        outputFile
          << left
          << setw(20) << "Course Name" 
          << right
          << setw(10) << "Course No" 
          << setw(10) << "Credits" 
          << setw(10) << "Grade" 
          << endl << endl;
        for(int h = 0; h < numberOfCourses; h++){
            outputFile
                << left 
                << setw(20) << courses[h].getCourseName()
                << right 
                << setw(10) << courses[h].getCourseNo()
                << setw(10) << courses[h].getCourseCredit()
                << setw(10) << courses[h].getCourseGrade() << endl;
        }
        outputFile
          << "Total number of credits: " 
            << getHoursEnrolled() << endl
          << "Mid-Semester GPA : " 
            << fixed << setprecision(2) << getGPA() << endl;
        }
        outputFile
          << endl 
          << "----------------------------" 
          << endl << endl;
}

int main(int argc, char** argv){
    string inputFileName = "input.txt",
           outputFileName = "output.txt";

    // ./program <input> <output> 
    // şeklinde çağırmaya izin ver
    if (argc == 3) {
      inputFileName = argv[1];
      outputFileName = argv[2];
    }


    ifstream file(inputFileName);


    int studentCount, tuitionPerHours;
    file >> studentCount >> tuitionPerHours;

    studentType *students = new studentType[studentCount];

    string fname, lname, skip;
    int courseCount, id;
    char tuitionChar;
    for (int i = 0; i < studentCount; i++) {
        file >> fname >> lname >> id >> tuitionChar >> courseCount;
        students[i].setInfo(
            fname, lname, id,
            tuitionChar == 'Y',
            courseCount, tuitionPerHours);

        courseType courses[courseCount];
        for (int j = 0; j < courseCount; j++) {
            string courseName, courseNumber;
            int courseCredits;
            char courseGrade;

            file >> courseName >> courseNumber >> courseCredits >> courseGrade;
            // Olası grade değerleri yalnızca A,B,C,D,F
            if ( courseGrade < 'A' 
              || courseGrade > 'F' 
              || courseGrade == 'E' ) {
                cerr << "HATA: " << id << " numaralı öğrencide yanlış grade değeri: " << courseGrade << endl;
                exit(0);
            }
            courses[j] = courseType(courseName, courseNumber, courseCredits, courseGrade);
        }
        students[i].setCourses(courses);
    }

    ofstream outputFile(outputFileName); 

    for (int i = 0; i < studentCount; i++) {
        students[i].print(outputFile);
    }

    return 0;
}
