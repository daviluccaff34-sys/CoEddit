#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <functional>

using namespace geode::prelude;

static constexpr char API[] = "https://coeddit-api.daviluccaff34.workers.dev";

static std::string coeditText(char const* english, char const* portuguese) {
    auto language = Mod::get()->getSettingValue<std::string>("language");
    return language == "Português" ? portuguese : english;
}

static std::string jsonEscape(std::string const& value) {
    std::string out;
    for (char c : value) {
        if (c == '"' || c == '\\') out += '\\';
        if (c == '\n' || c == '\r') continue;
        out += c;
    }
    return out;
}

static std::string getAuthKey() {
    return Mod::get()->getSavedValue<std::string>("authorization-key", "");
}

static void showResult(std::string const& title, web::WebResponse response) {
    auto body = response.string().unwrapOr("Sem resposta do servidor.");
    auto text = response.ok() ? body : body;
    FLAlertLayer::create(title, text, "OK")->show();
}

static void apiPost(std::string const& path, std::string const& body, std::string const& key, std::string const& title) {
    auto request = web::WebRequest();
    request.header("Content-Type", "application/json");
    request.header("X-CoEddit-Key", key);
    request.bodyString(body);
    async::spawn(request.post(std::string(API) + path), [title](web::WebResponse response) {
        showResult(title, std::move(response));
    });
}

class CoEdditInputPopup : public Popup, public TextInputDelegate {
public:
    using Callback = std::function<void(std::string)>;

    static CoEdditInputPopup* create(std::string title, std::string placeholder, Callback callback) {
        auto popup = new CoEdditInputPopup();
        if (popup->init(std::move(title), std::move(placeholder), std::move(callback))) {
            popup->autorelease();
            return popup;
        }
        delete popup;
        return nullptr;
    }

    void textChanged(CCTextInputNode*) override {}

protected:
    CCTextInputNode* m_input = nullptr;
    Callback m_callback;

    bool init(std::string title, std::string placeholder, Callback callback) {
        if (!Popup::init(320.f, 170.f))
            return false;

        m_callback = std::move(callback);
        this->setTitle(title.c_str());

        m_input = CCTextInputNode::create(260.f, 30.f, placeholder.c_str(), "bigFont.fnt");
        m_input->setPosition({160.f, 92.f});
        m_input->setDelegate(this);
        this->addChild(m_input);

        auto menu = CCMenu::create();
        menu->setPosition({160.f, 38.f});
        auto confirm = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(coeditText("CONTINUE", "CONTINUAR").c_str(), 90, true, "goldFont.fnt", "GJ_button_01.png", 30, 1),
            this, menu_selector(CoEdditInputPopup::onConfirm)
        );
        menu->addChild(confirm);
        this->addChild(menu);
        return true;
    }

    void onConfirm(CCObject*) {
        auto value = m_input ? m_input->getString() : "";
        if (value.empty()) {
            FLAlertLayer::create("CoEddit", coeditText("Enter a value first.", "Digite um valor primeiro."), "OK")->show();
            return;
        }
        auto callback = m_callback;
        this->onClose();
        if (callback) callback(value);
    }
};

static void requestAuthorization(std::string key) {
    auto request = web::WebRequest();
    request.header("X-CoEddit-Key", key);
    request.header("Content-Type", "application/json");
    request.bodyString("{}");
    async::spawn(request.post(std::string(API) + "/auth/authorize"), [key](web::WebResponse response) {
        if (!response.ok()) {
            showResult("CoEddit - Login", std::move(response));
            return;
        }
        Mod::get()->setSavedValue("authorization-key", key);
        FLAlertLayer::create("CoEddit", coeditText("Authorization complete! You can now create and join rooms.", "Autorização concluída! Agora você pode criar e entrar em salas."), "OK")->show();
    });
}

static void ensureAuthorized(std::function<void(std::string)> callback) {
    auto key = getAuthKey();
    if (!key.empty()) {
        callback(key);
        return;
    }
    CoEdditInputPopup::create(
        coeditText("CoEddit Authorization", "Autorização do CoEddit"),
        coeditText("Enter the authorization key", "Digite a chave de autorização"),
        [callback](std::string entered) {
            if (entered != "AUTH000100300401036393972737383737") {
                FLAlertLayer::create("CoEddit", coeditText("Invalid authorization key.", "Chave de autorização inválida."), "OK")->show();
                return;
            }
            auto request = web::WebRequest();
            request.header("X-CoEddit-Key", entered);
            request.header("Content-Type", "application/json");
            request.bodyString("{}");
            async::spawn(request.post(std::string(API) + "/auth/authorize"), [entered, callback](web::WebResponse response) {
                if (!response.ok()) {
                    showResult("CoEddit - Login", std::move(response));
                    return;
                }
                Mod::get()->setSavedValue("authorization-key", entered);
                FLAlertLayer::create("CoEddit", coeditText("Login successful!", "Login concluído!"), "OK")->show();
                callback(entered);
            });
        }
    )->show();
}

// HOST appears in the editor.
class $modify(CoEdditEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        auto menu = CCMenu::create();
        menu->setID("coedit-host-menu");
        auto label = coeditText("HOST", "CRIAR SALA");
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(label.c_str(), 45, true, "goldFont.fnt", "GJ_button_01.png", 30, 1),
            this, menu_selector(CoEdditEditorUI::onCoEdditHost)
        );
        auto size = this->getContentSize();
        button->setPosition({size.width - 48.f, size.height - 28.f});
        menu->addChild(button);
        menu->setPosition({0.f, 0.f});
        this->addChild(menu, 100);
        return true;
    }

    void onCoEdditHost(CCObject*) {
        ensureAuthorized([](std::string key) {
            CoEdditInputPopup::create(
                coeditText("Create room", "Criar sala"),
                coeditText("Room name", "Nome da sala"),
                [key](std::string name) {
                    apiPost("/rooms", "{\"name\":\"" + jsonEscape(name) + "\"}", key,
                        coeditText("CoEddit - Room created", "CoEddit - Sala criada"));
                }
            )->show();
        });
    }
};

// JOIN appears in the level browser.
class $modify(CoEdditLevelBrowser, LevelBrowserLayer) {
    bool init(GJSearchObject* searchObject) {
        if (!LevelBrowserLayer::init(searchObject))
            return false;

        auto menu = CCMenu::create();
        menu->setID("coedit-join-menu");
        auto label = coeditText("JOIN", "ENTRAR");
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(label.c_str(), 45, true, "goldFont.fnt", "GJ_button_01.png", 30, 1),
            this, menu_selector(CoEdditLevelBrowser::onCoEdditJoin)
        );
        auto size = this->getContentSize();
        button->setPosition({size.width - 48.f, 28.f});
        menu->addChild(button);
        menu->setPosition({0.f, 0.f});
        this->addChild(menu, 100);
        return true;
    }

    void onCoEdditJoin(CCObject*) {
        ensureAuthorized([](std::string key) {
            CoEdditInputPopup::create(
                coeditText("Join room", "Entrar na sala"),
                coeditText("Enter room code", "Digite o código da sala"),
                [key](std::string code) {
                    apiPost("/rooms/join", "{\"code\":\"" + jsonEscape(code) + "\"}", key,
                        coeditText("CoEddit - Join result", "CoEddit - Resultado da entrada"));
                }
            )->show();
        });
    }
};

$on_mod(Loaded) {
    log::info("CoEddit carregado!");
}