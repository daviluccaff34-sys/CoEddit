#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

static std::string coeditText(char const* english, char const* portuguese) {
    auto language = Mod::get()->getSettingValue<std::string>("language");
    return language == "Português" ? portuguese : english;
}

// HOST is shown inside the level editor.
class $modify(CoEdditEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        auto menu = CCMenu::create();
        menu->setID("coedit-host-menu");

        auto hostLabel = coeditText("HOST", "CRIAR SALA");
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(hostLabel.c_str(), 45, true, "goldFont.fnt", "GJ_button_01.png", 30, 1),
            this,
            menu_selector(CoEdditEditorUI::onCoEdditHost)
        );

        auto size = this->getContentSize();
        button->setPosition({size.width - 48.f, size.height - 28.f});
        menu->addChild(button);
        menu->setPosition({0.f, 0.f});
        this->addChild(menu, 100);

        return true;
    }

    void onCoEdditHost(CCObject*) {
        auto title = coeditText("CoEddit - Host", "CoEddit - Criar sala");
        auto message = coeditText(
            "Room hosting is not connected yet. The next step is connecting the room server.",
            "A criação de salas ainda não está conectada. O próximo passo é ligar o servidor de salas."
        );
        FLAlertLayer::create(title, message, "OK")->show();
    }
};

// JOIN is shown in the level browser.
class $modify(CoEdditLevelBrowser, LevelBrowserLayer) {
    bool init(GJSearchObject* searchObject) {
        if (!LevelBrowserLayer::init(searchObject))
            return false;

        auto menu = CCMenu::create();
        menu->setID("coedit-join-menu");

        auto joinLabel = coeditText("JOIN", "ENTRAR");
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(joinLabel.c_str(), 45, true, "goldFont.fnt", "GJ_button_01.png", 30, 1),
            this,
            menu_selector(CoEdditLevelBrowser::onCoEdditJoin)
        );

        auto size = this->getContentSize();
        button->setPosition({size.width - 48.f, 28.f});
        menu->addChild(button);
        menu->setPosition({0.f, 0.f});
        this->addChild(menu, 100);

        return true;
    }

    void onCoEdditJoin(CCObject*) {
        auto title = coeditText("CoEddit - Join", "CoEddit - Entrar");
        auto message = coeditText(
            "Joining rooms is not connected yet. The next step is connecting the room server.",
            "A entrada em salas ainda não está conectada. O próximo passo é ligar o servidor de salas."
        );
        FLAlertLayer::create(title, message, "OK")->show();
    }
};

$on_mod(Loaded) {
    log::info("CoEddit carregado!");
}
