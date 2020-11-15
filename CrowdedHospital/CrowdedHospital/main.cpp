#include <iostream>
#include <iomanip>

#include "hospital.hpp"

using namespace std;


Hospital* CreateHospital();
void InitBedLayout(Hospital *hospital);
PatientType InputPatientType();
void CinClear();


int main(int argc, const char * argv[]) {
    auto *hospital = CreateHospital();
    InitBedLayout(hospital);
    hospital->PrintBedLayout();
    
    bool flag_covid = false;
    bool flag_measles = false;

    while (true) {
        cout << endl;
        
        PatientType type = InputPatientType();
        cout << (type == kCovidPatient ? "Covid patient" : "Measles patient") << endl;
        
        Patient *patient = new Patient(type);
        if (hospital->AcceptPatient(patient)) {
            hospital->AddPatient(patient);
            cout << "* patient(" << patient->num << ") Check in to bed ("
                 << patient->bed->get_row() << ',' << patient->bed->get_column() << ")"
                 << endl;
            cout <<  "----- flag_covid="<< flag_covid <<  "， flag_measles=" <<flag_measles << endl;
            flag_covid = false;
            flag_measles = false;
        } else {
            hospital->AcceptPatientFailed(patient);
            cout << "* No suitable bed can be allocated to this new patient(" << patient->num << ")"
                 << endl;
            if (patient->IsCovidPatient()) {
                flag_covid = true;
            } else if (patient->IsMeaslesPatient()) {
                flag_measles = true;
            }
            cout <<  "flag_covid="<< flag_covid <<  "， flag_measles=" <<flag_measles << endl;
            if (flag_covid && flag_measles) {
                cout << endl << "Exit the program!!!" << endl;
                break;
            }
        }
        
        hospital->PrintBedUsed();
    }

    delete hospital;
    return 0;
}


// Create hospital object
Hospital* CreateHospital() {
    int columns;
    int rows;
    while (true) {
        cout << "Please enter the number of rows and columns of beds, format：Rows Cols (which can be any number between 2 and 1000):" << endl;
        cout << "> ";
        cin >> rows >> columns;
        CinClear();
        
        if (rows >= 2 && rows <= 1000 && columns >= 2 && columns <= 1000) {
            cout << "rows: " << rows << ", columns: " << columns << endl << endl;
            break;
        }
        
        cout << "Input errors, please re-enter!" << endl << endl;
    }
    
    return new Hospital(rows, columns);
}


// Set the bed layout
void InitBedLayout(Hospital *hospital) {
    int rows = hospital->rows;
    int columns = hospital->columns;
    cout << "Please enter the bed layout，" << columns << " beds per row，" << rows << " row in total, V ： anti-viral，B ： anti-bacterial:" << endl;
    
    int width = rows < 10 ? 1 : (rows < 100 ? 2 : (rows < 1000 ? 3 : 4));
    for (int row = 0; row < rows; row++) {
        cout << "row #" << setw(width) << setfill('0') << row + 1 << "> ";
        
        bool is_valid = true;
        for (int column = 0; column < columns; column++) {
            char ch;
            cin >> ch;
            
            Bed &bed = hospital->GetBed(row, column);
            if (ch == 'v' || ch == 'V') {
                bed.set_type(kAntiViralBed);
            } else if (ch == 'b' || ch == 'B') {
                bed.set_type(kAntiBacterialBed);
            } else {
                is_valid = false;
                break;
            }
        }
        CinClear();
        
        if (!is_valid) {
            row--;
            cout << "Input errors, please re-enter！" << endl;
        }
    }
}


PatientType InputPatientType() {
    while (true) {
        cout << "Please enter the type of inpatient: Covid (C) patients and Measles (M) patients:" << endl;
        cout << "#> ";

        char ch;
        cin >> ch;

        if (ch == 'c' || ch == 'C') {
            return kCovidPatient;
        } else if (ch == 'm' || ch == 'M') {
            return kMeaslesPatient;
        }
        cout << "Input errors, please re-enter！" << endl << endl;
    }
}


// Clear the input buffer
void CinClear() {
    cin.clear();
    string junk;
    getline(cin, junk);
//    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
