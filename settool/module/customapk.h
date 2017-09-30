#ifndef CUSTOMAPK_H
#define CUSTOMAPK_H

#include "manager/apk.h"
#include "define.h"


class CCustomapk : public CApk
{

public:
    CCustomapk();
    ~CCustomapk();

public:
protected:
    int doModule();
};

EXPORT_DEF(CCustomapk);

extern void* getinstanceCCustomapk();

#endif // CUSTOMAPK_H
