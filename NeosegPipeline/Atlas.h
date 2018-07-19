#ifndef DEF_Atlas
#define DEF_Atlas

#include <QString>
#include <QFile>

struct Atlas {
QString name;
QString T1;
QString T2;
QString mask;
QString seg;
bool roi_ok[10]; 
QString roi[10];
bool parc_ok;

bool probabilistic;
QString parc;
QString white;
QString gray;
QString csf; 
};

#endif
