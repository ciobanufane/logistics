#include "logistic.h"

std::string Logistic::toString() const
{
    char* buf = new char[1000];
    sprintf(buf, "Chapter: %s\nManpower: %0.1f\nRations: %0.1f\n"
                        "Ammo: %0.1f\nParts: %0.1f\nMinutes: %d\n",
                        chapter.c_str(), manpower, rations,
                        ammo, parts, minutes);
    std::string logString(buf);
    delete[] buf;
    return logString;
}

LogisticGroup::LogisticGroup()
{

}

LogisticGroup::LogisticGroup(const std::vector<Logistic>& logs)
{
    group = logs;
}

std::vector<Logistic>::iterator LogisticGroup::begin()
{
    return group.begin();
}

std::vector<Logistic>::iterator LogisticGroup::end()
{
    return group.end();
}

std::vector<Logistic>::const_iterator LogisticGroup::begin() const
{
    return group.begin();
}

std::vector<Logistic>::const_iterator LogisticGroup::end() const
{
    return group.end();
}

void LogisticGroup::add(const Logistic& log)
{
    group.push_back(log);
}

double LogisticGroup::getResource(int resource) const
{
    double total = 0;
    for (const Logistic& log : group) {
        switch(resource) {
        case Logistic::Manpower:
            total += log.manpower;
            break;
        case Logistic::Ammo:
            total += log.ammo;
            break;
        case Logistic::Rations:
            total += log.rations;
            break;
        case Logistic::Parts:
            total += log.parts;
            break;
        default:
            return 0;
        }
    }
    return total;
}

void LogisticGroup::print() const
{
    for (const Logistic& log : group)
    {
        std::cout << "Chapter : " << log.chapter  << '\n';
        std::cout << "Manpower: " << log.manpower << '\n';
        std::cout << "Ammo    : " << log.ammo     << '\n';
        std::cout << "Rations : " << log.rations  << '\n';
        std::cout << "Parts   : " << log.parts    << '\n';
        std::cout << "Minutes : " << log.minutes  << '\n';
    }
}

Logistic& LogisticGroup::operator[](int index)
{
    return group[index];
}

LogisticGroup operator+(const LogisticGroup& group1, const LogisticGroup& group2)
{
    LogisticGroup tmp = group1;
    for (const Logistic& log : group2) {
        tmp.add(log);
    }
    return tmp;
}

LogisticGroup operator+(const LogisticGroup& group, const Logistic& log)
{
    LogisticGroup tmp = group;
    tmp.add(log);
    return tmp;
}

LogisticGroup operator+(const Logistic& log, const LogisticGroup& group)
{
    return group+log;
}
