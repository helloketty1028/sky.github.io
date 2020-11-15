#ifndef patientQueue_hpp
#define patientQueue_hpp

#include <stdio.h>
#include "patient.hpp"


// Patient queue (circular queue, first in first out, traversable)
class PatientQueue {
public:
    PatientQueue(int capacity);
    
    // Determine whether the queue is empty
    bool IsEmpty();
    // Determine whether the queue is full
    bool IsFull();
    // Join the queue
    void Push(Patient *patient);
    // Leave the team
    Patient * Pop();
    // Queue length
    int Size();
    // Queue capacity
    int Capacity();
    // Start iteration
    void Begin();
    // Iterate the next element
    Patient * Next();
    
    ~PatientQueue();
    
private:
    // Queue storage object
    Patient **_queue;
    
    // Total queue capacity
    const int _capacity;
    
    // Head of queue pointer
    int _front;
    // Tail pointer
    int _rear;
    
    // Iterator pointer
    int _cursor;
};


#endif /* patientQueue_hpp */
