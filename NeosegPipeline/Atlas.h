#ifndef DEF_Atlas
#define DEF_Atlas

#include <QString>
#include <QFile>

struct Atlas {
QString name;
QString T1;
QString T2;
QString mask;
bool probabilistic;
bool parc_ok;
bool roi_ok;
int roi_nbr;
QString seg; 
QString roi[10];
QString parc;
QString white;
QString gray;
QString csf; 
};

#endif
