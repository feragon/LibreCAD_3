#include <gtest/gtest.h>
#include <cad/meta/class.h>

TEST(ClassTest, Construction1) {
    auto name = "ClassName";
    auto cppClass = "Plugin::Name::ClassName";
    auto applicationName = "LibreCAD";
    auto cl = new lc::Class(name, cppClass, applicationName, FLAG_CLASS_ALL_ALLOWED, 1, false, true);

    EXPECT_EQ(name, cl->className());
    EXPECT_EQ(cppClass, cl->cppClass());
    EXPECT_EQ(applicationName, cl->applicationName());

    EXPECT_EQ(FLAG_CLASS_ALL_ALLOWED, cl->flags());
    EXPECT_EQ(true, cl->cloningAllowed());
    EXPECT_EQ(true, cl->colorChangeAllowed());
    EXPECT_EQ(true, cl->eraseAllowed());
    EXPECT_EQ(true, cl->layerChangeAllowed());
    EXPECT_EQ(true, cl->linetypeChangeAllowed());
    EXPECT_EQ(true, cl->linetypeScaleChangeAllowed());
    EXPECT_EQ(true, cl->lineweightChangeAllowed());
    EXPECT_EQ(true, cl->plotStyleNameChangeAllowed());
    EXPECT_EQ(true, cl->transformAllowed());
    EXPECT_EQ(true, cl->visibilityChangeAllowed());
    EXPECT_EQ(false, cl->proxyWarningDialogDisabled());
    EXPECT_EQ(false, cl->r13FormatProxy());
}

TEST(ClassTest, Construction2) {
    auto name = "ClassName";
    auto cppClass = "Plugin::Name::ClassName";
    auto applicationName = "LibreCAD";
    auto cl = new lc::Class(name, cppClass, applicationName, 
                            FLAG_CLASS_DISABLE_PROXY_WARNING_DIALOG | FLAG_CLASS_R13_FORMAT_PROXY, 1, false, true);

    EXPECT_EQ(FLAG_CLASS_DISABLE_PROXY_WARNING_DIALOG | FLAG_CLASS_R13_FORMAT_PROXY, cl->flags());
    EXPECT_EQ(false, cl->cloningAllowed());
    EXPECT_EQ(false, cl->colorChangeAllowed());
    EXPECT_EQ(false, cl->eraseAllowed());
    EXPECT_EQ(false, cl->layerChangeAllowed());
    EXPECT_EQ(false, cl->linetypeChangeAllowed());
    EXPECT_EQ(false, cl->linetypeScaleChangeAllowed());
    EXPECT_EQ(false, cl->lineweightChangeAllowed());
    EXPECT_EQ(false, cl->plotStyleNameChangeAllowed());
    EXPECT_EQ(false, cl->transformAllowed());
    EXPECT_EQ(false, cl->visibilityChangeAllowed());
    EXPECT_EQ(true, cl->proxyWarningDialogDisabled());
    EXPECT_EQ(true, cl->r13FormatProxy());
}