class Readonly {
private:
    unsigned short value;
public:
    Readonly(unsigned short);
    unsigned short getInstance() const;
};