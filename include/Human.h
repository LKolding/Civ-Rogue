
class Human {
    
public:
    Human(int intelligence, int strength, int speed) {
        this->intelligence = intelligence;
        this->strength = strength;
        this->speed = speed;
    };
    int getIntelligence() { return this->intelligence; };
    int getStrength() { return this->strength; };
    int getSpeed() { return this->speed; }

protected:
    int intelligence;  // Gained by spending time in educational facility
    int strength;      // Gained by spending time in gym facility
    int speed;
};