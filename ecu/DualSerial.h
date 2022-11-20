class DualSerial: public Stream
{

public:
    int available() override {
        return Serial.available();
    }

    int read() override {
        return Serial.read();
    }

    size_t write(uint8_t t) override {
      return Serial1.write(t);
      }

      int peek() {
        return Serial.peek();
        }
};
