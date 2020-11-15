
#include "patientQueue.hpp"

PatientQueue::PatientQueue(int capacity): _capacity(capacity + 1) {
    _rear = 0;
    _front = 0;
    _queue = new Patient *[capacity];
}


bool PatientQueue::IsEmpty() {
    return _front == _rear;
}


bool PatientQueue::IsFull() {
    return (_rear + 1) % _capacity == _front;
}


void PatientQueue::Push(Patient *patient) {
    if (!IsFull()) {
        _rear = (_rear + 1) % _capacity;
        _queue[_rear] = patient;
    }
}

Patient * PatientQueue::Pop() {
    if (IsEmpty()) {
        return NULL;
    }
    
    _front = (_front + 1) % _capacity;
    return _queue[_front];
}


int PatientQueue::Size() {
    return _rear >= _front ? _rear - _front : _rear + _capacity - _front;
}


int PatientQueue::Capacity() {
    return _capacity - 1;
}


void PatientQueue::Begin() {
    _cursor = _front;
}


Patient * PatientQueue::Next() {
    if (IsEmpty() || _cursor == _rear) {
        return NULL;
    }
    _cursor = (_cursor + 1) % _capacity;
    
    return _queue[_cursor];
}


PatientQueue::~PatientQueue() {
    Patient *item;
    while ((item = Pop()) != NULL) {
        delete item;
    }
}
