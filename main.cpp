#include <iostream>
#include <string>
using namespace std;

class personType{
    private:
        string firstName;
        string lastName;

    public:
        void setStudentName();
        void print();
};

class studentType : public personType{
    private:
        int numberOfCourses;
        courseType *course;//gradeyi burdan çekicez
        int studentId;
        int isTuitionPaid;
    
    public:
        void print();
        void print(int one);
        void print(int one, int two);
        void setName();
        void getName();
        void getHoursEnrolled();
        void getGpa();
        void billingAmount();
        studentType();
};

class courseType{
    private:
        string courseName;
        int courseNo;
        char courseGrade;//student sınıfı bunu kullanıcak özellikle
        char courseCredits;

    public:
        void setCourseInfo();
        void print(int one);
        void print(int one, int two);
        void getCredits();
        void getCourseNumber();
        void getGrade();
        courseType();

};

int main(){
    int number = 5;
    studentType students[number];
    return 0;
}