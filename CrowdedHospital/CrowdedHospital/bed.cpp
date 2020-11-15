
#include "bed.hpp"


Bed::Bed() {
    _is_idle = true;
    _type = kAntiViralBed;
    _patient = NULL;
}


void Bed::Reset() {
    if (_patient != NULL) {
        _patient->bed = NULL;
    }
    _patient = NULL;
    _is_idle = true;
}


bool Bed::IsIdle() {
    return _is_idle;
}


bool Bed::IsAntiViral() {
    return _type == kAntiViralBed;
}


bool Bed::IsAntiBacterial() {
    return _type == kAntiBacterialBed;
}


char Bed::TypeDesc() {
    switch (_type) {
        case kAntiBacterialBed:
            return 'B';
        case kAntiViralBed:
            return 'V';
    }
}


void Bed::SetPosition(int row, int column) {
    _row = row;
    _column = column;
}


void Bed::set_type(BedType type) {
    _type = type;
}


void Bed::set_patient(Patient *patient) {
    if (patient != NULL) {
        _patient = patient;
        patient->bed = this;
        _is_idle = false;
    }
}


Patient * Bed::get_patient() {
    return _patient;
}


int Bed::get_row() {
    return _row;
}


int Bed::get_column() {
    return _column;
}
