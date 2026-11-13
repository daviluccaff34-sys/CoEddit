#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

// The first UI milestone: Host is available from the editor,
// while Join is available from the level browser (My Levels).
// The room/network actions are placeholders until the room system is added.

class $modify(CoEdditEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        auto menu = CCMenu::create();
        menu->setID("coedit-host-menu");

        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("HOST", 45, true, "goldFont.fnt", "GJ_button_01.png", 30, 1),
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
        FLAlertLayer::create(
            "CoEddit - Host",
            "Room hosting is coming soon! This button is placed inside the level editor.",
            "OK"
        )->show();
    }
};

class $modify(CoEdditLevelBrowser, LevelBrowserLayer) {
    bool init(GJSearchObject* searchObject) {
        if (!LevelBrowserLayer::init(searchObject))
            return false;

        auto menu = CCMenu::create();
        menu->setID("coedit-join-menu");

        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("JOIN", 45, true, "goldFont.fnt", "GJ_button_01.png", 30, 1),
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
        FLAlertLayer::create(
            "CoEddit - Join",
            "Joining a room is coming soon! This button is placed in the level browser.",
            "OK"
        )->show();
    }
};

$on_mod(Loaded) {
    log::info("CoEddit carregado!");
}
