
#ifndef bed_hpp
#define bed_hpp

#include <stdio.h>

#include "patient.hpp"

class Patient;


// bed type
enum BedType { kAntiViralBed, kAntiBacterialBed };


// bed class
class Bed {
public:
    Bed();
    
    // Reset bed status（Patient discharged）
    void Reset();

    // Whether the bed is free
    bool IsIdle();
    // Is it an anti-bacterial bed
    bool IsAntiViral();
    // Is it an anti-viral(V) bed
    bool IsAntiBacterial();
    // Returns the character representation of the bed type
    char TypeDesc();
    
    // Set the bed position
    void SetPosition(int row, int column);
    
    // Set the bed type
    void set_type(BedType type);
    // Use bed
    void set_patient(Patient *patient);
    // Acquire patients using beds
    Patient * get_patient();
    // Get the bed row
    int get_row();
    // Get the column of the bed
    int get_column();
    
private:
    // Bed status
    bool _is_idle;
    
    // Bed type
    BedType _type;
    
    // Bed line
    int _row;
    // Bed column
    int _column;
    
    // Patients using this bed
    Patient *_patient;
};

#endif /* bed_hpp */
