class Engine {
private:
    int controlPin;
    int forward;
    int backward;
public:
    Engine(int p1, int p2, int p3)
    {
        this->controlPin = p1;
        this->forward = p2;
        this->backward = p3;
        pinMode(controlPin, OUTPUT);
        digitalWrite(controlPin, LOW);
        pinMode(forward, OUTPUT);
        pinMode(backward, OUTPUT);
    }

    void stop() {
      digitalWrite(controlPin, LOW);
    }

    void moveforward(bool forwards = true) {
      digitalWrite(forward, forwards);
      digitalWrite(backward, !forwards);
      digitalWrite(controlPin, HIGH);
    }
};
