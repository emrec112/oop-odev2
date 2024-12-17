#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

class personType{
    private:
        string firstName;//sonra bunları birleştir
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
        int courseCredits;

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

        void setCourseName(string newName){
            courseName = newName;
        }

        void setCourseNo(string newNo){
            courseNo = newNo;
        }

        void setCourseGrade(char newGrade){
            courseGrade = newGrade;
        }

        void setCourseCredits(int newCredit){
            courseCredits = newCredit;
        }

};

class studentType : public personType {
    private:
        int numberOfCourses;
        courseType *courses;//gradeyi burdan çekicez
        int studentId;
        bool isTuitionPaid;
        int tuitionPerHours;
    
    public:
        void print(ofstream &outputFile);
        void setInfo(string firstName, string lastName, int id, bool isTuitionPaid, int numberOfCourses, int tuition);
        void setCourses(courseType *courses);
        int getHoursEnrolled();
        float getGPA();
        int bill();
        //set info ekle
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

void studentType::setInfo(string firstName, string lastName, int id, bool isTuitionPaid, int numberOfCourses, int tuition){
    setStudentName(firstName, lastName);
    studentId = id;
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

void studentType::print(ofstream &outputFile){//setwleri düzenle ve kursları sıralı yazdırma ekle
    
    outputFile
        << "Student Name: " << getStudentFirstName() << " " << getStudentLastName() << endl 
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
                << setw(10) << courses[h].getCoursNo()
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
    int studentCount, tuitionPerHours;

    ifstream file("input.txt");
    file >> studentCount;

    studentType *students = new studentType[studentCount];

    file >> tuitionPerHours;

    string fname, lname, skip;
    int courseCount, id;
    char tuitionChar;
    for(int i = 0; i < studentCount; i++){
        file >> fname >> lname >> id >> tuitionChar >> courseCount;
        students[i].setInfo(
            fname, lname, id,
            tuitionChar == 'Y',
            courseCount, tuitionPerHours);

        courseType courses[courseCount];
        for(int j = 0; j < courseCount; j++){
            string courseName, courseNumber;
            int courseCredits;
            char courseGrade;

            file >> courseName >> courseNumber >> courseCredits >> courseGrade;
            courses[j] = courseType(courseName, courseNumber, courseCredits, courseGrade);
        }
        students[i].setCourses(courses);
        // ...... gerçekten olacak ise geri ekleyebiliriz
        //getline(file, skip);
        //getline(file, skip);
    }

    // TODO: Çıkış seçeneği ekle (argv?)
    ofstream outputFile("output.txt"); 
    for(int i = 0; i < studentCount; i++)students[i].print(outputFile );

    return 0;
}
