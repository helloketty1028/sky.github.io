#ifndef rule_hpp
#define rule_hpp

#include <stdio.h>

#include "hospital.hpp"

class Hospital;

class Rule {
public:
    Rule(Hospital *hospital);
    
    // For every 3rd Covid patient that enters, 1 is discharged (resetting the counter)
    bool CheckForCovidPatientCount(Patient *patient);
    
    // For every 5th Measles patients that enters, 2 are discharged (resetting the counter)
    bool CheckForMeaslesPatientCount(Patient *patient);
    
    // Check whether the bed is suitable for the patient
    bool IsSuitableBed(Bed *bed, Patient *patient, bool check_arround);

    void FixCount(Patient *patient);
    
private:
    Hospital *_hospital;
    
    // Covid patients counter
    int _covid_count;
    // Measles patients counter
    int _measles_count;
    
    // There are measles patients in the neighbors
    bool _NeighbourIsMeaslesPatient(Bed *bed);
    // Surrounded by covid patients
    bool _SurroundedByCovidPatients(Bed *bed);
    
    // Get the bed on the left
    Bed * GetLeftBed(Bed *bed);
    // Get the bed on the right
    Bed * GetRightBed(Bed *bed);
    // Get the bed above
    Bed * GetTopBed(Bed *bed);
    // Get the bed below
    Bed * GetBottomBed(Bed *bed);
    // Get the bed in the upper left corner
    Bed * GetTopLeftBed(Bed *bed);
    // Get the bed in the upper right corner
    Bed * GetTopRightBed(Bed *bed);
    // Get the bed in the bottom left corner
    Bed * GetBottomLeftBed(Bed *bed);
    // Get the bed in the lower right corner
    Bed * GetBottomRightBed(Bed *bed);
};

#endif /* rule_hpp */
