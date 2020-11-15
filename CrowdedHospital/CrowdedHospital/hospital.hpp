#ifndef hospital_hpp
#define hospital_hpp

#include <stdio.h>

#include "bed.hpp"
#include "patientQueue.hpp"
#include "rule.hpp"

class Rule;

// Hospital class
class Hospital {
public:
    friend Rule;
    
    // bed lines
    const int rows;
    // bed columns
    const int columns;
    
    Hospital(int rows, int columns);
    ~Hospital();
    
    // The patient is admitted to the hospital, return whether the admission was successful
    bool AcceptPatient(Patient *patient);
    
    // Add to patient list after successful admission
    void AddPatient(Patient *patient);
    
    void AcceptPatientFailed(Patient *patient);
    
    // Patient discharged
    void DischargePatient(Patient *patient);
    
    // The old patient was discharged from the hospital and the bed was given to the new patient
    void DischargeAndAcceptPatient(Patient *old_patient, Patient *new_patient);
    
    // Change bed
    void ChangeBed(Bed *old_bed, Bed *new_bed);
    
    // Find the right bed
    Bed * FindBed(Patient *patient);
    
    // Get the specified location
    Bed & GetBed(int row, int column);
    
    // Print bed layout
    void PrintBedLayout();
    
    // Print bed usage
    void PrintBedUsed();
    
    
private:
    // Bed data
    Bed **_beds;
    
    // Covid patients
    PatientQueue *_covid_patients;
    // Measles patients
    PatientQueue *_measles_patients;
    
    // 规则
    Rule *_rule;
};

#endif /* hospital_hpp */
