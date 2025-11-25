/****************************************************************************
** Meta object code from reading C++ file 'ControlButton.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ControlButton.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControlButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13ControlButtonE_t {};
} // unnamed namespace

template <> constexpr inline auto ControlButton::qt_create_metaobjectdata<qt_meta_tag_ZN13ControlButtonE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ControlButton",
        "iconColorChanged",
        "",
        "iconScaleChanged",
        "setIconColor",
        "color",
        "getIconColor",
        "setIconScale",
        "scale",
        "getIconScale",
        "iconColor",
        "iconScale"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'iconColorChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'iconScaleChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setIconColor'
        QtMocHelpers::SlotData<void(const QColor &)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QColor, 5 },
        }}),
        // Slot 'getIconColor'
        QtMocHelpers::SlotData<QColor() const>(6, 2, QMC::AccessPrivate, QMetaType::QColor),
        // Slot 'setIconScale'
        QtMocHelpers::SlotData<void(qreal)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QReal, 8 },
        }}),
        // Slot 'getIconScale'
        QtMocHelpers::SlotData<qreal() const>(9, 2, QMC::AccessPrivate, QMetaType::QReal),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'iconColor'
        QtMocHelpers::PropertyData<QColor>(10, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'iconScale'
        QtMocHelpers::PropertyData<qreal>(11, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ControlButton, qt_meta_tag_ZN13ControlButtonE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ControlButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ControlButtonE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ControlButtonE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ControlButtonE_t>.metaTypes,
    nullptr
} };

void ControlButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ControlButton *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->iconColorChanged(); break;
        case 1: _t->iconScaleChanged(); break;
        case 2: _t->setIconColor((*reinterpret_cast< std::add_pointer_t<QColor>>(_a[1]))); break;
        case 3: { QColor _r = _t->getIconColor();
            if (_a[0]) *reinterpret_cast< QColor*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->setIconScale((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 5: { qreal _r = _t->getIconScale();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ControlButton::*)()>(_a, &ControlButton::iconColorChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlButton::*)()>(_a, &ControlButton::iconScaleChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QColor*>(_v) = _t->getIconColor(); break;
        case 1: *reinterpret_cast<qreal*>(_v) = _t->getIconScale(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setIconColor(*reinterpret_cast<QColor*>(_v)); break;
        case 1: _t->setIconScale(*reinterpret_cast<qreal*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ControlButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ControlButtonE_t>.strings))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int ControlButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ControlButton::iconColorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ControlButton::iconScaleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
