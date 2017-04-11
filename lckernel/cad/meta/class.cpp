#include "class.h"

using namespace lc;

Class::Class(const std::string& name, const std::string& cppClass, const std::string& applicationName,
             const int flags, const int instanceCount, bool wasAProxy, bool entityFlag) :
    _name(name),
    _cppClass(cppClass),
    _applicationName(applicationName),
    _flags(flags),
    _instanceCount(instanceCount),
    _wasAProxy(wasAProxy),
    _entityFlag(entityFlag) {
}

const std::string Class::id() const {
    return LCMETANAME() + "_" + _cppClass;
}

const std::string& Class::className() const {
    return _name;
}

const std::string& Class::cppClass() const {
    return _cppClass;
}

const std::string& Class::applicationName() const {
    return _applicationName;
}

int Class::flags() const {
    return _flags;
}

const int Class::instanceCount() const {
    return _instanceCount;
}

bool Class::wasAProxy() const {
    return _wasAProxy;
}

bool Class::entityFlag() const {
    return _entityFlag;
}

bool Class::eraseAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_ERASE_ALLOWED);
}

bool Class::transformAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_TRANSFORM_ALLOWED);
}

bool Class::colorChangeAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_COLOR_CHANGE_ALLOWED);
}

bool Class::layerChangeAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_LAYER_CHANGE_ALLOWED);
}

bool Class::linetypeChangeAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_LINETYPE_CHANGE_ALLOWED);
}

bool Class::linetypeScaleChangeAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_LINETYPE_SCALE_CHANGE_ALLOWED);
}

bool Class::visibilityChangeAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_VISIBILITY_CHANGE_ALLOWED);
}

bool Class::cloningAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_CLONING_ALLOWED);
}

bool Class::lineweightChangeAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_LINEWEIGHT_CHANGE_ALLOWED);
}

bool Class::plotStyleNameChangeAllowed() {
    return static_cast<bool>(_flags & FLAG_CLASS_PLOT_STYLE_NAME_CHANGE_ALLOWED);
}

bool Class::proxyWarningDialogDisabled() {
    return static_cast<bool>(_flags & FLAG_CLASS_DISABLE_PROXY_WARNING_DIALOG);
}

bool Class::r13FormatProxy() {
    return static_cast<bool>(_flags & FLAG_CLASS_R13_FORMAT_PROXY);
}

const std::string Class::name() const {
    return _cppClass;
}

std::string Class::LCMETANAME() {
    return "_CLASS";
}