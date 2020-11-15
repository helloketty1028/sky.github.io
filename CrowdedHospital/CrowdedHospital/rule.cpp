#include "rule.hpp"


Rule::Rule(Hospital *hospital): _hospital(hospital) {
    _covid_count = 0;
    _measles_count = 0;
}


bool Rule::CheckForCovidPatientCount(Patient *patient) {
    _covid_count++;
    if (_covid_count == 3) {
        _covid_count = 0;
        auto p = _hospital->_covid_patients->Pop();
        _hospital->DischargeAndAcceptPatient(p, patient);
        return true;
    }
    return false;
}


bool Rule::CheckForMeaslesPatientCount(Patient *patient) {
    _measles_count++;
    if (_measles_count == 5) {
        _measles_count = 0;
        auto p = _hospital->_measles_patients->Pop();
        _hospital->DischargeAndAcceptPatient(p, patient);
        // Another patient discharged
        p = _hospital->_measles_patients->Pop();
        _hospital->DischargePatient(p);
        return true;
    }
    return false;
}


bool Rule::IsSuitableBed(Bed *bed, Patient *patient, bool check_arround) {
    if (bed == NULL) {
        return false;
    }
    
    if (patient->IsCovidPatient()) {
        if (!bed->IsAntiViral()) {
            // Covid patients must only sleep on anti-viral beds
            return false;
        }
    } else { // IsMeaslesPatient
        // No two measles patients can be next to one another
        if (_NeighbourIsMeaslesPatient(bed)) {
            return false;
        }
        // No measles patients can be surrounded on all sides by covid patients
        if (_SurroundedByCovidPatients(bed)) {
            return false;
        }
    }
    
    if (patient->IsCovidPatient() && check_arround) {
        Patient *old_patient = bed->get_patient();
        bed->set_patient(patient);
        
        Bed *list[] = {
            GetLeftBed(bed), GetRightBed(bed), GetTopBed(bed), GetBottomBed(bed),
            GetTopLeftBed(bed), GetTopRightBed(bed), GetBottomLeftBed(bed), GetBottomRightBed(bed)
        };
        bool result = true;
        for (int i = 0; i < 8; i++) {
            Bed *item = list[i];
            if (item != NULL && !item->IsIdle() && !IsSuitableBed(item, item->get_patient(), false)) {
                result = false;
                break;
            }
        }
        
        bed->Reset();
        bed->set_patient(old_patient);
        return result;
    }
    
    return true;
}


void Rule::FixCount(Patient *patient) {
    if (patient->IsCovidPatient()) {
        _covid_count--;
    } else {
        _measles_count--;
    }
}


bool Rule::_NeighbourIsMeaslesPatient(Bed *bed) {
    Bed *list[] = {
        GetLeftBed(bed), GetRightBed(bed), GetTopBed(bed), GetBottomBed(bed)
    };
    for (int i = 0; i < 4; i++) {
        Bed *item = list[i];
        if (item != NULL && !item->IsIdle() && item->get_patient()->IsMeaslesPatient()) {
            return true;
        }
    }
    return false;
}


bool Rule::_SurroundedByCovidPatients(Bed *bed) {
    Bed *list[] = {
        GetLeftBed(bed), GetRightBed(bed), GetTopBed(bed), GetBottomBed(bed),
        GetTopLeftBed(bed), GetTopRightBed(bed), GetBottomLeftBed(bed), GetBottomRightBed(bed)
    };
    int count = 0;
    for (int i = 0; i < 8; i++) {
        Bed *item = list[i];
        if (item != NULL && !item->IsIdle() && item->get_patient()->IsCovidPatient()) {
            count++;
        }
    }
    return count == 8;
}


Bed * Rule::GetLeftBed(Bed *bed) {
    if (bed->get_column() == 0) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row(), bed->get_column() - 1);
}


Bed * Rule::GetRightBed(Bed *bed) {
    if (bed->get_column() >= _hospital->columns - 1) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row(), bed->get_column() + 1);
}


Bed * Rule::GetTopBed(Bed *bed) {
    if (bed->get_row() == 0) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row() - 1, bed->get_column());
}


Bed * Rule::GetBottomBed(Bed *bed) {
    if (bed->get_row() <= _hospital->rows - 1) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row() + 1, bed->get_column());
}


Bed * Rule::GetTopLeftBed(Bed *bed) {
    if (bed->get_row() == 0 || bed->get_column() == 0) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row() - 1, bed->get_column() - 1);
}


Bed * Rule::GetTopRightBed(Bed *bed) {
    if (bed->get_row() == 0 || bed->get_column() >= _hospital->columns - 1) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row() - 1, bed->get_column() + 1);
}


Bed * Rule::GetBottomLeftBed(Bed *bed) {
    if (bed->get_row() <= _hospital->rows - 1 || bed->get_column() == 0) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row() + 1, bed->get_column() - 1);
}


Bed * Rule::GetBottomRightBed(Bed *bed) {
    if (bed->get_row() <= _hospital->rows - 1 || bed->get_column() >= _hospital->columns - 1) {
        return NULL;
    }
    return &_hospital->GetBed(bed->get_row() + 1, bed->get_column() + 1);
}
