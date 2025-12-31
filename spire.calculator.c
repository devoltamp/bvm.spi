// Relative SPI Calculator for BVM Engineering College (Class Average Based)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define MAX_SEMESTERS 8
#define MAX_SUBJECTS 10
#define MAX_NAME_LENGTH 50
#define MAX_STUDENTS 80

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"
#define UNRL    "\033[4m"

//bvm college animation
void printHeader() {
    FILE *file = fopen("banner.txt", "r");
    if (!file) {
        printf("\n%sBVM ENGINEERING COLLEGE%s\n", CYAN, RESET);
        printf("%sRELATIVE GRADING SPI CALCULATOR%s\n\n", BLUE, RESET);
        return;
    }

    printf("\033[1;36m");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        for (int i = 0; line[i] != '\0'; i++) {
            putchar(line[i]);
            fflush(stdout);
            usleep(3000);
        }
    }
    printf("\033[0m");
    fclose(file);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to simulate class average and standard deviation
void generateClassStats(float *average, float *stdDev) {
    *average = 70.0 + (rand() % 15);
    *stdDev = 5.0 + (rand() % 7);
}

// to define the score/grade
int getRelativeGradePoints(float studentMark, float classAvg, float stdDev) {
    float zscore = (studentMark - classAvg) / stdDev;

    if (zscore >= 1.5) return 10;
    if (zscore >= 1.0) return 9;
    if (zscore >= 0.5) return 8;
    if (zscore >= 0.0) return 7;
    if (zscore >= -0.5) return 6;
    if (zscore >= -1.0) return 5;
    if (zscore >= -1.5) return 4;
    return 0;
}

float calculateSPI(int subjects) {

    char subjectNames[MAX_SUBJECTS][MAX_NAME_LENGTH];
    int credits[MAX_SUBJECTS];
    float totalGradePoints = 0, totalCredits = 0;
    float marks[MAX_SUBJECTS];
    float classAverages[MAX_SUBJECTS];
    float stdDeviations[MAX_SUBJECTS];

    //subject marks for each
    for (int i = 0; i < subjects; i++) {
        generateClassStats(&classAverages[i], &stdDeviations[i]);
    }

    //student marks
    for (int i = 0; i < subjects; i++) {
        float mid1, mid2, finalExam, otherMarks, attendance;
        int credit;

        printf("\n%sEnter details for Subject %d%s\n", BOLD, i + 1, RESET);
        printf("Subject name: ");
        scanf("%s", subjectNames[i]);
        printf("Credits: ");
        scanf("%d", &credit);
        printf("Mid 1 (out of 30): ");
        scanf("%f", &mid1);
        printf("Mid 2 (out of 30): ");
        scanf("%f", &mid2);
        printf("Final Exam (out of 60): ");
        scanf("%f", &finalExam);
        printf("Attendance (out of 10): ");
        scanf("%f", &attendance);
        printf("Practicals/Assignments (out of 50): ");
        scanf("%f", &otherMarks);

        //total marks out of 150
        marks[i] = (mid1 + mid2) * 0.5 + finalExam + attendance + otherMarks;
        credits[i] = credit;
    }

    // Display results
    printf(BOLD CYAN "\n--- SUBJECT WISE PERFORMANCE ---" RESET "\n");
    printf("%-15s %8s %10s %10s %12s %12s\n",
           "Subject", "Marks", "Class Avg", "Std Dev", "Z-Score", "Grade Points");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < subjects; i++) {

        float zscore = (marks[i] - classAverages[i]) / stdDeviations[i];

        int gradePoints = getRelativeGradePoints(marks[i], classAverages[i], stdDeviations[i]);

        printf("%-15s %8.2f %10.2f %10.2f %12.2f %12d\n",
               subjectNames[i], marks[i], classAverages[i],
               stdDeviations[i], zscore, gradePoints);

        totalGradePoints = totalGradePoints + (gradePoints * credits[i]);
        totalCredits = totalCredits + credits[i];
    }

    if (totalCredits > 0) {
        return totalGradePoints / totalCredits;
    }
    return 0.0;
}

int main() {
    int numSemesters;
    float spi[MAX_SEMESTERS], totalSPI = 0;

    clearScreen();
    printHeader();

    printf("\n%s%sCAUTION!!%s\n\n", RED, BOLD, RESET);
    printf("1. This calculator uses relative grading based on class averages\n");
    printf("2. Class statistics are simulated for demonstration\n");
    printf("3. Actual grading may vary - consult official sources\n\n");

    while (1) {
        printf("Enter number of semesters completed (1-%d): ", MAX_SEMESTERS);
        scanf("%d", &numSemesters);
        if (numSemesters > 0 && numSemesters <= MAX_SEMESTERS) break;
        printf(RED "Invalid input. Please try again.\n" RESET);
    }

    // for each semester
    for (int i = 0; i < numSemesters; i++) {
        int subjects;
        printf(CYAN BOLD "\n--- SEMESTER %d ---\n" RESET, i + 1);

        while (1) {
            printf("Enter number of subjects (1-%d): ", MAX_SUBJECTS);
            scanf("%d", &subjects);
            if (subjects > 0 && subjects <= MAX_SUBJECTS) break;
            printf(RED "Invalid input. Please try again.\n" RESET);
        }

        spi[i] = calculateSPI(subjects);
        printf(GREEN "\nSPI for Semester %d: " RESET, i + 1);
        printf(CYAN BOLD "%.2f" RESET "\n", spi[i]);
        totalSPI += spi[i];
    }

    //CGPA
    if (numSemesters > 0) {
        float CGPA = totalSPI / numSemesters;
        printf(GREEN "\nFinal CGPA after %d semesters: " RESET, numSemesters);
        printf(CYAN BOLD "%.2f" RESET "\n", CGPA);

        printf("\n%sPerformance Analysis:%s\n", BOLD, RESET);
        printf("AA (10): Top 7%% of class\n");
        printf("AB (9): Next 10%%\n");
        printf("BB (8): Next 15%%\n");
        printf("BC (7): Next 20%%\n");
        printf("CC (6): Next 20%%\n");
        printf("CD (5): Next 15%%\n");
        printf("DD (4): Next 10%%\n");
        printf("FF (0): Bottom 3%%\n");
    }

    printf("\n");
    return 0;
}
