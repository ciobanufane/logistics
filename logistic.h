#ifndef LOGISTIC_H
#define LOGISTIC_H

#include <vector>
#include <iostream>
#include <string>

struct Logistic
{

    enum Value
    {
        Chapter,
        Manpower,
        Ammo,
        Rations,
        Parts,
        Minutes
    };

    std::string chapter;
    double manpower;
    double ammo;
    double rations;
    double parts;
    int minutes;
    std::string toString() const;
};

class LogisticGroup
{
public:
    LogisticGroup();
    LogisticGroup(const std::vector<Logistic>& logs);

    std::vector<Logistic>::iterator begin();
    std::vector<Logistic>::iterator end();
    std::vector<Logistic>::const_iterator begin() const;
    std::vector<Logistic>::const_iterator end() const;

    void add(const Logistic& log);
    double getResource(int resource) const;
    void print() const;

    Logistic& operator[](int index);
    friend LogisticGroup operator+(const LogisticGroup& group1, const LogisticGroup& group2);
    friend LogisticGroup operator+(const LogisticGroup& group, const Logistic& log);
    friend LogisticGroup operator+(const Logistic& log, const LogisticGroup& group);
private:
    std::vector<Logistic> group;
};

#endif
