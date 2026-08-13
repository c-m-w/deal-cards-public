/// button.hpp

class button {
private:

    int _pin;

public:

    button(int pin);
    ~button() = default;

    bool pressed();

    operator bool();
};
