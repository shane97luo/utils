#ifndef CUPS_H
#define CUPS_H

#include <QObject>

namespace ultracore {

class Cups : public QObject
{
    Q_OBJECT
public:
    explicit Cups(){};

    enum C1upType{
        QW,
        EE,
    };
    Q_ENUM(C1upType)


//protected slots:
//    void Park(int a);

//signals:

//    void TakeIt(QString str);

};

}

#endif // CUPS_H
