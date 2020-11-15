
#ifndef patient_hpp
#define patient_hpp

#include <stdio.h>
#include "bed.hpp"

class Bed;

enum PatientType { kCovidPatient, kMeaslesPatient };

class Patient {
public:
    int num;
    const PatientType type;
    Bed *bed;
    
    Patient(PatientType type);
    
    // is it Covid patients
    bool IsCovidPatient();
    // is it Measles patients
    bool IsMeaslesPatient();
    
    char TypeDesc();
};

#endif /* patient_hpp */
