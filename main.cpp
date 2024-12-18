#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

class personType{
    private:
        string name;//sonra bunları birleştir

    public:
        void setStudentName(string name);

        string getStudentName() const{
            return name;
        }
};

void personType::setStudentName(string name){
    this->name = name;
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
        void setCourseInfo(string courseName, string courseNo, char courseGrade, int courseCredits);

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

void courseType::setCourseInfo(string courseName, string courseNo, char courseGrade, int courseCredits){//kursları tek bir fonksiyonda değiştir
            this->courseName = courseName;
            this->courseNo = courseNo;
            this->courseGrade = courseGrade;
            this->courseCredits = courseCredits;
}

class studentType : public personType {
    private:
        int numberOfCourses;
        courseType *courses;//gradeyi burdan çekicez
        int studentId;
        bool isTuitionPaid;
        int tuitionPerHours;
    
    public:
        void print(ofstream &outputFile);
        void setInfo(string name, int id, bool isTuitionPaid, int numberOfCourses, int tuition);
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

void studentType::setInfo(string name, int studentId, bool isTuitionPaid, int numberOfCourses, int tuition){
    setStudentName(name);
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

void studentType::print(ofstream &outputFile){//setwleri düzenle ve kursları sıralı yazdırma ekle
    
    outputFile
        << "Student Name: " << getStudentName() << endl 
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

    string inputFileName = "input.txt",
           outputFileName = "output.txt";

    if (argc == 3) {
      inputFileName = argv[1];
      outputFileName = argv[2];
    }


    ifstream file(inputFileName);
    file >> studentCount;

    studentType *students = new studentType[studentCount];

    file >> tuitionPerHours;

    string fname, lname, skip;
    int courseCount, id;
    char tuitionChar;
    for(int i = 0; i < studentCount; i++){
        // TODO: fname lname diye alma, ismin hepsini birden al. (regex?)
        file >> fname >> lname >> id >> tuitionChar >> courseCount;
        students[i].setInfo(
            fname + " " + lname, id,
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

    ofstream outputFile(outputFileName); 
    for(int i = 0; i < studentCount; i++)students[i].print(outputFile );

    return 0;
}
