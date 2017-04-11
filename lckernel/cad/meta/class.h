#pragma once

#include <ostream>
#include "cad/interface/metatype.h"

#define FLAG_CLASS_NO_OPERATION 0x0
#define FLAG_CLASS_ERASE_ALLOWED 0x1
#define FLAG_CLASS_TRANSFORM_ALLOWED 0x2
#define FLAG_CLASS_COLOR_CHANGE_ALLOWED 0x4
#define FLAG_CLASS_LAYER_CHANGE_ALLOWED 0x8
#define FLAG_CLASS_LINETYPE_CHANGE_ALLOWED 0x10
#define FLAG_CLASS_LINETYPE_SCALE_CHANGE_ALLOWED 0x20
#define FLAG_CLASS_VISIBILITY_CHANGE_ALLOWED 0x40
#define FLAG_CLASS_CLONING_ALLOWED 0x80
#define FLAG_CLASS_LINEWEIGHT_CHANGE_ALLOWED 0x100
#define FLAG_CLASS_PLOT_STYLE_NAME_CHANGE_ALLOWED 0x200
#define FLAG_CLASS_ALL_EXCEPT_CLONING_ALLOWED 0x37F
#define FLAG_CLASS_ALL_ALLOWED 0x3FF
#define FLAG_CLASS_DISABLE_PROXY_WARNING_DIALOG 0x400
#define FLAG_CLASS_R13_FORMAT_PROXY 0x8000

namespace lc {
    class Class : public DocumentMetaType {
        public:
            Class(const std::string& name, const std::string& cppClass, const std::string& applicationName,
                  const int flags, const int instanceCount, bool wasAProxy, bool entityFlag);

            const std::string id() const override;

            const std::string name() const override;

            static std::string LCMETANAME();

            const std::string& className() const;
            const std::string& cppClass() const;
            const std::string& applicationName() const;
            int flags() const;
            const int instanceCount() const;
            bool wasAProxy() const;
            bool entityFlag() const;

            bool eraseAllowed();
            bool transformAllowed();
            bool colorChangeAllowed();
            bool layerChangeAllowed();
            bool linetypeChangeAllowed();
            bool linetypeScaleChangeAllowed();
            bool visibilityChangeAllowed();
            bool cloningAllowed();
            bool lineweightChangeAllowed();
            bool plotStyleNameChangeAllowed();
            bool proxyWarningDialogDisabled();
            bool r13FormatProxy();

        private:
            std::string _name;
            std::string _cppClass;
            std::string _applicationName;
            int _flags;
            const int _instanceCount;
            bool _wasAProxy;
            bool _entityFlag;
    };

    using Class_SPtr = std::shared_ptr<Class>;
    using Class_CSPtr = std::shared_ptr<const Class>;
}