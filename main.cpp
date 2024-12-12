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

class studentType : public personType{
    private:
        int numberOfCourses;
        courseType *courses;//gradeyi burdan çekicez
        int studentId;
        char isTuitionPaid;
        int tuitionPerHours;
    
    public:
        void print(ofstream &outputFile);
        void setInfo(string firstName, string lastName, int id, char isTuitionPaid, int numberOfCourses, int tuition);
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
    isTuitionPaid = 'N';
}

void studentType::setInfo(string firstName, string lastName, int id, char isTuitionPaid, int numberOfCourses, int tuition){
    setStudentName(firstName, lastName);
    studentId = id;
    this->isTuitionPaid = isTuitionPaid;
    this->numberOfCourses = numberOfCourses;
    this->tuitionPerHours = tuition;
}

void studentType::setCourses(courseType *courses){
    this->courses = new courseType[numberOfCourses];
    for(int c = 0; c < numberOfCourses; c++){
        this->courses[c] = courses[c];
    }
}

int studentType::getHoursEnrolled(){
    int total = 0;
    for(int i = 0; i < numberOfCourses; i++){
        total += this->courses[i].getCourseCredit();
    }
    return total;
}

float studentType::getGPA(){
    float total = 0;

    for(int i = 0; i < numberOfCourses; i++){
        char check = this->courses[i].getCourseGrade();
        
        switch(check){
            case 'A':
                total += 4 * this->courses[i].getCourseCredit();
                break;
            case 'B':
                total += 3 * this->courses[i].getCourseCredit();
                break;
            case 'C':
                total += 2 * this->courses[i].getCourseCredit();
                break;
            case 'D':
                total += this->courses[i].getCourseCredit();
                break;
            default:
                break;
        }
    }
    total /= getHoursEnrolled();
    return total;
}

int studentType::bill(){
    return getHoursEnrolled() * tuitionPerHours;
}

void studentType::print(ofstream &outputFile){//setwleri düzenle ve kursları sıralı yazdırma ekle
    
    outputFile<< getStudentFirstName() << " " << getStudentLastName() << endl 
        << "id : " << studentId <<endl
        << "number of courses : " <<numberOfCourses << endl;
    
    if(isTuitionPaid == 'N'){
        outputFile << "Öğrenim ücreti ödenmemiştir.\nÜcret : " << bill() << "TL."<< endl;
    }else{
        outputFile<< "\nCourse Name "<< setw(10) << "Course No " << setw(10) << "Credits " << setw(10) << "Grade \n" << endl;
        for(int h = 0; h < numberOfCourses; h++){
            outputFile<< courses[h].getCourseName()
                << setw(10) << courses[h].getCoursNo()
                << setw(10) << courses[h].getCourseCredit()
                << setw(10) << courses[h].getCourseGrade() << endl;
        }
        outputFile<< "toplam kredi : " << getHoursEnrolled() << endl
            << "GPA : " << getGPA() << endl;
        }
        outputFile<< "................." << endl;
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
        students[i].setInfo(fname, lname, id, control, nOfcourses, tuitionPerHours);

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

    ofstream outputFile("output.txt");
    for(int i = 0; i < number; i++)students[i].print(outputFile );

    return 0;
}