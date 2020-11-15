
#include "hospital.hpp"
#include <iostream>

using namespace std;

Hospital::Hospital(int rows, int columns): rows(rows), columns(columns) {
    _beds = new Bed *[rows];
    for (int row = 0; row < rows; row++) {
        _beds[row] = new Bed [columns];
        for (int column = 0; column < columns; column++) {
            _beds[row][column].SetPosition(row, column);
        }
    }
    
    int max_patient_num = rows * columns;
    _covid_patients = new PatientQueue(max_patient_num);
    _measles_patients = new PatientQueue(max_patient_num);
    
    _rule = new Rule(this);
}


bool Hospital::AcceptPatient(Patient *patient) {
    if (patient->IsCovidPatient()) {
        // For every 3rd Covid patient that enters, 1 is discharged (resetting the counter)
        if (_rule->CheckForCovidPatientCount(patient)) {
            return true;
        }
    } else { // IsMeaslesPatient
        // For every 5th Measles patients that enters, 2 are discharged (resetting the counter)
        if (_rule->CheckForMeaslesPatientCount(patient)) {
            return true;
        }
        
        // 如果是 Measles patients，优先使用 anti-bacterial 床位
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                Bed &bed = GetBed(row, column);
                if (bed.IsAntiBacterial() && bed.IsIdle() && _rule->IsSuitableBed(&bed, patient, true)) {
                    bed.set_patient(patient);
                    return true;
                }
            }
        }
    }
    
    
    // Find a suitable bed among vacant beds
    Bed *bed = FindBed(patient);
    if (bed != NULL) {
        bed->set_patient(patient);
        return true;
    }
    
    // Find the right bed by changing the bed
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            Bed &bed = GetBed(row, column);
            // Only check the beds that are used and suitable for new patients
            if (bed.IsIdle() || !_rule->IsSuitableBed(&bed, patient, true)) {
                continue;
            }
            
            Patient *old_patient = bed.get_patient();
            Bed *tmp_bed = FindBed(old_patient);
            if (tmp_bed != NULL) {
                ChangeBed(&bed, tmp_bed);
                bed.set_patient(patient);
                return true;
            }
        }
    }
    
    return false;
}


void Hospital::AddPatient(Patient *patient) {
    if (patient->IsCovidPatient()) {
        _covid_patients->Push(patient);
    } else {
        _measles_patients->Push(patient);
    }
}

void Hospital::AcceptPatientFailed(Patient *patient) {
    _rule->FixCount(patient);
}


void Hospital::DischargePatient(Patient *patient) {
    cout << "* 病人(" << patient->num << ")出院" << endl;
    patient->bed->Reset();
    delete patient;
}


void Hospital::DischargeAndAcceptPatient(Patient *old_patient, Patient *new_patient) {
    if (old_patient != NULL && new_patient != NULL) {
        auto bed = old_patient->bed;
        DischargePatient(old_patient);
        bed->set_patient(new_patient);
    }
}


void Hospital::ChangeBed(Bed *old_bed, Bed *new_bed) {
    Patient *patient = old_bed->get_patient();
    old_bed->Reset();
    new_bed->set_patient(patient);
    cout << "* 病人(" << patient->num << ")从床("
         << old_bed->get_row() << ',' << old_bed->get_column() << ")移到("
         << new_bed->get_row() << ',' << new_bed->get_column() << ")"
         << endl;
}


Bed * Hospital::FindBed(Patient *patient) {
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            Bed &bed = GetBed(row, column);
            if (bed.IsIdle() && _rule->IsSuitableBed(&bed, patient, true)) {
                return &bed;
            }
        }
    }
    return NULL;
}


Bed & Hospital::GetBed(int row, int column) {
    return _beds[row][column];
}


void Hospital::PrintBedLayout() {
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            Bed &bed = GetBed(row, column);
            cout << bed.TypeDesc() << ' ';
        }
        cout << endl;
    }
}


void Hospital::PrintBedUsed() {
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            Bed &bed = _beds[row][column];
            if (bed.IsIdle()) {
                cout << '-' << ' ';
            } else {
                cout << bed.get_patient()->TypeDesc() << ' ';
            }
        }
        cout << endl;
    }
}


Hospital::~Hospital() {
    for (int i = 0; i < rows; i++) {
        delete [] _beds[i];
    }
    delete [] _beds;
    
    delete _covid_patients;
    delete _measles_patients;
    
    delete _rule;
}
