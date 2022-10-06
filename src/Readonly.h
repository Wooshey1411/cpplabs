class Readonly {
private:
    Readonly();
    Readonly(const Readonly&);
    Readonly& operator=(Readonly&);
    unsigned short value;
public:
    Readonly(unsigned short);
    unsigned short getInstance();
};