/**
 * \brief The following program reads from the CLI a list of students. Each student has a first name and grades average.
 * Then, the program outputs the student that has the highest grades average. If there're more than one student, it'll
 * output the first one.
 */
#include <iostream>
#include <sstream>
#include "VLVector.hpp"

/** \brief The vector static _capacity. */
#define STATIC_CAPACITY 16

// region The student class implementation

/**
 * \brief Describes a student.
 */
class Student {
public:
    /**
     * Default constructor (used with std::cin).
     */
    Student() : _name(""), _average(0.0) { };

    /**
     * Initializes a new student object.
     * @param name The student full name.
     * @param average The student grades average.
     */
    Student(std::string const& name, double average) : _name(name), _average(average) { };

    /**
     * Gets the student name.
     * @return The student name.
     */
    std::string getName() const { return _name; }

    /**
     * Gets the student grades average.
     * @return The student average.
     */
    double getAverage() const { return _average; }

    friend std::istream &operator>>(std::istream &is, Student& s);
    friend std::ostream &operator<<(std::ostream &os, Student const& s);
private:
    /** \brief The student full name. */
    std::string _name;

    /** \brief The student grades average. */
    double _average;
};

/**
 * Reads a student object from the given stream.
 * @param is The stream object.
 * @param s The output student instance.
 * @return The modified input stream.
 */
std::istream& operator>>(std::istream& is, Student& s) {
    std::string name;
    double average;

    is >> name >> average;

    s = Student(name, average);
    return is;
}

/**
 * Fills the given output stream with the information about the given student.
 * @param os The output stream.
 * @param s The student to write.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, Student const& s) {
    return os << s.getName() << " (average: " << s.getAverage() << ")";
}

// endregion

/**
 * @brief The main function that runs the program.
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated
 * multi-byte strings that represent the arguments passed to the program from the execution
 * environment. The value of argv[argc] is guaranteed to be a null pointer.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has execution errors.
 */
int main(int argc, const char* argv[]) {
    /* Setup */
    VLVector<Student, STATIC_CAPACITY> students;
    std::string line;

    /* Read until the line is empty */
    while (true) {
        /* Read the entire line */
        std::cout << "Enter a student in the format \"<name> <average>\" or an empty string to stop:" << std::endl;
        std::getline(std::cin, line);
        if (line.empty()) {
            break;
        }

        /* Convert to std::stringstream (string-stream) so that we can activate operator>>. This will be the same
         * operator>> that we saw in std::cin. As "Student" implements an override of operator>>,
         * it will get called. Yes. It will get called for std::stringstream as it would be called for std::cin. Why? */
        std::stringstream ss(line);
        Student s;
        ss >> s;

        /* Add to the VLVector */
        students.push_back(s);
    }

    /* Find the student with the highest grade */
    Student highestGradeStudent;
    for (const auto& student : students) {
        if (student.getAverage() > highestGradeStudent.getAverage()) {
            highestGradeStudent = student;
        }
    }

    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "Total Students: " << students.size() << std::endl;
    std::cout << "Student with highest grade: " << highestGradeStudent << std::endl;

    return EXIT_SUCCESS;
}