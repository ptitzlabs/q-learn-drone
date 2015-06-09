/****************************************************************************
** Meta object code from reading C++ file 'qt_interface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt_interface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qt_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qt_interface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      26,   13,   13,   13, 0x0a,
      37,   13,   13,   13, 0x0a,
      54,   49,   13,   13, 0x0a,
      94,   13,   13,   13, 0x0a,
     110,   13,   13,   13, 0x0a,
     123,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_qt_interface[] = {
    "qt_interface\0\0test_cmac()\0lineplot()\0"
    "shadeplot()\0,,,,\0"
    "contourf(PLFLT*,PLFLT*,PLFLT**,int,int)\0"
    "test_contourf()\0plotCurves()\0interactive()\0"
};

void qt_interface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        qt_interface *_t = static_cast<qt_interface *>(_o);
        switch (_id) {
        case 0: _t->test_cmac(); break;
        case 1: _t->lineplot(); break;
        case 2: _t->shadeplot(); break;
        case 3: _t->contourf((*reinterpret_cast< PLFLT*(*)>(_a[1])),(*reinterpret_cast< PLFLT*(*)>(_a[2])),(*reinterpret_cast< PLFLT**(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 4: _t->test_contourf(); break;
        case 5: _t->plotCurves(); break;
        case 6: _t->interactive(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qt_interface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qt_interface::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_qt_interface,
      qt_meta_data_qt_interface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qt_interface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qt_interface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qt_interface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qt_interface))
        return static_cast<void*>(const_cast< qt_interface*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int qt_interface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
