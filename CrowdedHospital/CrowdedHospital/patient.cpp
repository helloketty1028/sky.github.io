#include "patient.hpp"


Patient::Patient(PatientType type): type(type) {
    static int _num = 0;
    num = ++_num;
    bed = NULL;
}


bool Patient::IsCovidPatient() {
    return type == kCovidPatient;
}


bool Patient::IsMeaslesPatient() {
    return type == kMeaslesPatient;
}


char Patient::TypeDesc() {
    return type == kCovidPatient ? 'C' : 'M';
}
