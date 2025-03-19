#include "GUI.h"
#include <iostream>
#include <GL/gl.h>
#include <chrono>
#include <glm/ext/vector_float4.hpp>

// Static initialization flags and settings
static bool is_init = false;
static bool do_draw = true;
static bool huddraw = true;

// Fonts
static ImFont* arialFont = nullptr;
static ImFont* bigFont = nullptr;

// GUI options (enabled or disabled states for features)
bool GUI::xrayEnabled = false;
bool GUI::esp2dEnabled = false;
bool GUI::tracersEnabled = false;
bool GUI::nametagsEnabled = false;
bool GUI::chinaHatEnabled = false;
bool GUI::triggerbotEnabled = false;
bool GUI::aimbotEnabled = false;
bool GUI::aimCircleEnabled = false;
bool GUI::aimOnlyClick = false;
bool GUI::leftclickEnabled = false;
bool GUI::rightclickEnabled = false;
bool GUI::playerScaleEnabled = false;
bool GUI::chamsEnabled = false;
bool GUI::triggerToggle = false;
bool GUI::wallHackEnabled = false;
bool GUI::nameTagsEnabled = false;
bool GUI::chestespEnabled = false;
bool GUI::itemScaleEnabled = false;
bool GUI::itemespEnabled = false;
bool GUI::playerwallhackEnabled = false;
bool GUI::chestwallhackEnabled = false;
bool GUI::itemwallhackEnabled = false;
bool GUI::chamsChest = false;
bool GUI::chamsPlayer = false;
bool GUI::chamsItem = false;

int espselected = (GUI::espModeSelected == 1.0f) ? 1 : 0;
const char* espOptions[] = { "3D", "2D" };

float GUI::playerx = 1.0f;
float GUI::playery = 1.0f;
float GUI::playerz = 1.0f;

float GUI::itemx = 1.0f;
float GUI::itemy = 1.0f;
float GUI::itemz = 1.0f;

float GUI::tagsscale = 1.0f;

float GUI::triggerBotRange = 5.0f;
float GUI::triggetbotmincps = 9;
float GUI::triggetbotmaxcps = 11;


float GUI::aimbotRange = 5.0f;
float GUI::aimbotSpeed = 0.08f;

float GUI::leftclickermincps = 9;
float GUI::rightclickermincps = 9;
float GUI::leftclickermaxcps = 11;
float GUI::rightclickermaxcps = 11;
float minLeftCps = 1.0f;
float maxLeftCps = 20.0f;
float minRightCps = 1.0f;
float maxRightCps = 20.0f;

bool GUI::chinaRainbowEnabled = false;
float GUI::chinaRainbowSpeed = 1.0f;
float GUI::chinaR = 255;
float GUI::chinaG = 255;
float GUI::chinaB = 255;

bool GUI::espRainbow = false;
float GUI::espModeSelected = 1.0f;
float GUI::espRainbowSpeed = 1.0f;
float GUI::espR = 255;
float GUI::espG = 255;
float GUI::espB = 255;

float GUI::chestR = 1.0f;     // Kýrmýzý
float GUI::chestG = 0.0f;     // Yeþil
float GUI::chestB = 0.0f;     // Mavi

float GUI::itemR = 1.0f;     // Kýrmýzý
float GUI::itemG = 0.0f;     // Yeþil
float GUI::itemB = 0.0f;     // Mavi

float GUI::largeChestR = 0.0f;
float GUI::largeChestG = 1.0f;
float GUI::largeChestB = 0.0f;

bool GUI::chamsRainbowEnabled = false;
float GUI::chamsRainbowSpeed = 1.0f;
float GUI::chamsR = 255;
float GUI::chamsG = 255;
float GUI::chamsB = 255;

bool GUI::tracerRainbow = false;
float GUI::tracerColorR = 255;
float GUI::tracerColorG = 255;
float GUI::tracerColorB = 255;
float GUI::tracerOpacity = 100;
float GUI::tracerRainbowSpeed = 1.0;
float GUI::tracerColorThickness = 1.0f;

float GUI::targetespColorR = 255;
float GUI::targetespColorG = 255;
float GUI::targetespColorB = 255;

int currentCategory = 1;
int GUI::currentBindKey = -1;
bool GUI::isBindingKey = false;
bool* GUI::currentModuleToggle = nullptr;

float titleHeight = 20.0f;
float windowRounding = 0.0f;

std::unordered_map<int, bool*> GUI::keyBinds;

bool GUI::init(HWND wnd_handle)
{
    if (is_init)
        return false;

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    

    if (!io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f))
    {
        std::cerr << "Failed to load font." << std::endl;
        return false;
    }
    arialFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);

    if (!io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 19.0f))
    {
        std::cerr << "Failed to load big font." << std::endl;
        return false;
    }
    bigFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 19.0f);

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(wnd_handle);
    ImGui_ImplOpenGL3_Init("#version 130");

    is_init = true;
    return true;
}

void GUI::shutdown()
{
    if (!is_init)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    is_init = false;
}


void GUI::draw()
{
    if (!do_draw || !is_init)
        return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 windowBgColor(1.0f, 1.0f, 1.0f, 0.5f); // Beyaz arka plan
    ImVec4 contentBgColor(0.9f, 0.9f, 0.9f, 0.5f); // Açýk gri içerik arka planý
    ImVec4 windowBorderColor(0.8f, 0.8f, 0.8f, 0.5); // Açýk gri pencere kenarlýðý
    ImVec4 titleBgColor(0.7f, 0.7f, 0.9f, 1.0f); // Mavi tonlu baþlýk arka planý
    ImVec4 guicolor = ImVec4(0.8f, 0.8f, 0.95f, 1.0f); // GUI için genel renk
    ImVec4 buttonNormalColor = ImVec4(0.9f, 0.9f, 0.9f, 0.5f); // Mavi-mor tonlu buton
    ImVec4 buttonHoveredColor = ImVec4(1.0f, 1.0f, 1.0f, 0.7f); // Daha açýk mavi-mor
    ImVec4 buttonActiveColor = ImVec4(0.7f, 0.7f, 0.8f, 0.9f); // Daha koyu mavi-mor
    ImVec4 tracersColor = ImVec4(tracerColorR / 255.0f, tracerColorG / 255.0f, tracerColorB / 255.0f, 1.0f);
    ImVec4 targetespcolor = ImVec4(targetespColorR / 255.0f, targetespColorG / 255.0f, targetespColorB / 255.0f, 1.0f);
    ImVec4 chinaColor = ImVec4(tracerColorR / 255.0f, tracerColorG / 255.0f, tracerColorB / 255.0f, 1.0f);
    ImVec4 chamsColor = ImVec4(chamsR / 255.0f, chamsG / 255.0f, chamsB / 255.0f, 1.0f);
    ImVec4 espColor = ImVec4(espR / 255.0f, espG / 255.0f, espB / 255.0f, 1.0f);

    style.WindowRounding = 10.0f;
    style.ChildRounding = 10.0f;
    style.PopupRounding = 10.0f;
    style.FrameRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.ItemSpacing = ImVec2(3.5, 3.5);

    style.GrabMinSize = 10.0f;
    style.Colors[ImGuiCol_WindowBg] = windowBgColor;
    style.Colors[ImGuiCol_ChildBg] = contentBgColor;
    style.Colors[ImGuiCol_Border] = windowBorderColor;
    style.Colors[ImGuiCol_TitleBg] = titleBgColor;
    style.Colors[ImGuiCol_TitleBgActive] = titleBgColor;
    style.Colors[ImGuiCol_TitleBgCollapsed] = titleBgColor;

    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);   
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.8f, 0.8f, 1.0f, 1.0f); 
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.7f, 0.7f, 1.0f, 1.0f);  

    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6f, 0.6f, 0.9f, 1.0f);     // Slider tutma alaný
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5f, 0.5f, 1.0f, 1.0f); // Slider aktifken
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.3f, 0.3f, 1.0f, 1.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);          // Ana metin rengi (açýk gri)
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);    // Pasif metin rengi
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.6f, 0.8f, 1.0f, 1.0f);  // Seçili metnin arka planý (modern mavi)

    style.Colors[ImGuiCol_Button] = buttonNormalColor;
    style.Colors[ImGuiCol_ButtonHovered] = buttonHoveredColor;
    style.Colors[ImGuiCol_ButtonActive] = buttonActiveColor;

    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);      // Kaydýrma çubuðu arka planý
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);    // Kaydýrma çubuðu tutamaðý
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.65f, 0.85f, 1.0f); // Üzerine gelindiðinde
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.55f, 0.85f, 1.0f);  // Týklandýðýnda

    ImGui::PushStyleColor(ImGuiCol_WindowBg, windowBgColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, titleBgColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, titleBgColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, titleBgColor);

    

    ImVec2 windowSize = ImVec2(200, 600);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

    ImGui::PushFont(bigFont);

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 titlePos = ImVec2(windowPos.x + 10.0f, windowPos.y + 5.0f);



    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    
    ImGui::Begin("COMBAT", nullptr, ImGuiWindowFlags_NoResize);

    if (aimbotEnabled) {
        
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); 
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); 
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("Aimbot", ImVec2(200, 40))) currentCategory = 1;   
    ImGui::PopStyleVar();
    if (aimbotEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) aimbotEnabled = !aimbotEnabled; 

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        
        ImGui::OpenPopup("Aimbot Settings");
    }
    if (ImGui::BeginPopupModal("Aimbot Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));

        // Popup içerik renkleri
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
        ImGui::BeginChild("AimContainer", ImVec2(400, 300), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();

        ImGui::Checkbox("Aimbot", &aimbotEnabled);
        ImGui::SliderFloat("Speed", &GUI::aimbotSpeed, 1.0, 10.0, "%.0f");
        ImGui::SliderFloat("Range", &GUI::aimbotRange, 3.0, 7.0, "%.1f");
        ImGui::Checkbox("Only While Clicking", &aimOnlyClick);
        ImGui::Checkbox("Target ESP", &aimCircleEnabled);
        if (ImGui::ColorEdit3("", (float*)&targetespcolor, ImGuiColorEditFlags_NoAlpha)) {
            targetespColorR = targetespcolor.x * 255.0f;
            targetespColorG = targetespcolor.y * 255.0f;
            targetespColorB = targetespcolor.z * 255.0f;
        }
        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (triggerbotEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered 
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("TriggerBot", ImVec2(200, 40))) currentCategory = 2;
    ImGui::PopStyleVar();
    if (triggerbotEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) triggerbotEnabled = !triggerbotEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("TriggerBot Settings");
    }
    if (ImGui::BeginPopupModal("TriggerBot Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));

        // Popup içerik renkleri
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
        ImGui::BeginChild("TriggerConatiner", ImVec2(320, 190), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();

        ImGui::Checkbox("TriggerBot", &triggerbotEnabled);
        ImGui::SliderFloat("CPS", &GUI::triggetbotmaxcps, 1.0, 15.0, "%.0f");
        ImGui::SliderFloat("Range", &GUI::triggerBotRange, 3.0, 7.0, "%.1f");
        ImGui::Checkbox("Only While Pressing", &triggerToggle);

        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));


    if (leftclickEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); 
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); 
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("LeftClicker", ImVec2(200, 40))) currentCategory = 3;
    ImGui::PopStyleVar();
    if (leftclickEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) leftclickEnabled = !leftclickEnabled; 

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("LeftClicker Settings");
    }
    if (ImGui::BeginPopupModal("LeftClicker Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));

        // Popup içerik renkleri
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
        ImGui::BeginChild("ClickersContainer", ImVec2(320, 190), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();

        ImGui::Checkbox("Left Clicker", &leftclickEnabled);
        ImGui::SliderFloat("Max CPS", &GUI::leftclickermaxcps, minLeftCps, maxLeftCps, "%.0f");
        ImGui::SliderFloat("Min CPS", &GUI::leftclickermincps, minLeftCps, maxLeftCps, "%.0f");

        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (rightclickEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); 
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); 
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("RightClicker", ImVec2(200, 40))) currentCategory = 4;
    ImGui::PopStyleVar();
    if (rightclickEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) rightclickEnabled = !rightclickEnabled; 

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("RightClicker Settings");
    }
    if (ImGui::BeginPopupModal("RightClicker Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));

        // Popup içerik renkleri
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
        ImGui::BeginChild("ClickersContainer", ImVec2(320, 190), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();

        ImGui::Checkbox("Right Clicker", &rightclickEnabled);
        ImGui::SliderFloat("Max CPS", &GUI::rightclickermaxcps, minRightCps, maxRightCps, "%.0f");
        ImGui::SliderFloat("Min CPS", &GUI::leftclickermincps, minRightCps, maxRightCps, "%.0f");

        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("AutoRod", ImVec2(200, 40))) currentCategory = 5;
    ImGui::PopStyleVar();
    ImGui::End();
    
    
    

    
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
    ImGui::Begin("RENDER", nullptr, ImGuiWindowFlags_NoResize);

    if (esp2dEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); 
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); 
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("PlayerESP", ImVec2(200, 40))) currentCategory = 6;
    ImGui::PopStyleVar();
    if (esp2dEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) esp2dEnabled = !esp2dEnabled; 

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("PlayerESP Settings");
    }
    if (ImGui::BeginPopupModal("PlayerESP Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));

        // Popup içerik renkleri
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
        ImGui::BeginChild("PlayerESPContainer", ImVec2(190, 200), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("ESP", &esp2dEnabled);
        if (ImGui::ColorEdit3("", (float*)&espColor, ImGuiColorEditFlags_NoAlpha)) {
            espR = espColor.x * 255.0f;
            espG = espColor.y * 255.0f;
            espB = espColor.z * 255.0f;
        }

        ImGui::Combo("Mode", &espselected, espOptions, IM_ARRAYSIZE(espOptions));

        GUI::espModeSelected = (espselected == 1) ? 1.0f : 0.0f;

        ImGui::Checkbox("Rainbow", &espRainbow);
        ImGui::SliderFloat("Speed", &espRainbowSpeed, 0.1f, 3.0f, "%.1f");

        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));


    if (chestespEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); 
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); 
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("ChestESP", ImVec2(200, 40))) currentCategory = 7;
    ImGui::PopStyleVar();
    if (chestespEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) chestespEnabled = !chestespEnabled; 

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("ChestESP Settings");
    }
    if (ImGui::BeginPopupModal("ChestESP Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("ChestESPContainer", ImVec2(190, 200), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox(" Chest ESP", &chestespEnabled);
        if (ImGui::ColorEdit3("", (float*)&espColor, ImGuiColorEditFlags_NoAlpha)) {
            espR = espColor.x * 255.0f;
            espG = espColor.y * 255.0f;
            espB = espColor.z * 255.0f;
        }



        GUI::espModeSelected = (espselected == 1) ? 1.0f : 0.0f;

        ImGui::Checkbox("Rainbow", &espRainbow);
        ImGui::SliderFloat("Speed", &espRainbowSpeed, 0.1f, 3.0f, "%.1f");

        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));


    if (itemespEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("ItemESP", ImVec2(200, 40))) currentCategory = 8;
    ImGui::PopStyleVar();
    if (itemespEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) itemespEnabled = !itemespEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("ItemESP Settings");
    }
    if (ImGui::BeginPopupModal("ItemESP Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("ItemESPContainer", ImVec2(190, 200), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("Item ESP", &chestespEnabled);
        if (ImGui::ColorEdit3("", (float*)&espColor, ImGuiColorEditFlags_NoAlpha)) {
            espR = espColor.x * 255.0f;
            espG = espColor.y * 255.0f;
            espB = espColor.z * 255.0f;
        }



        GUI::espModeSelected = (espselected == 1) ? 1.0f : 0.0f;

        ImGui::Checkbox("Rainbow", &espRainbow);
        ImGui::SliderFloat("Speed", &espRainbowSpeed, 0.1f, 3.0f, "%.1f");

        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (wallHackEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("WallHack", ImVec2(200, 40))) currentCategory = 9;
    ImGui::PopStyleVar();
    if (wallHackEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) wallHackEnabled = !wallHackEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("WallHack Settings");
    }
    if (ImGui::BeginPopupModal("WallHack Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("WallHackContainer", ImVec2(190, 200), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("PlayerWallHack", &playerwallhackEnabled);
        ImGui::Checkbox("ChestWallHack", &chestwallhackEnabled);
        ImGui::Checkbox("ItemWallHack", &itemwallhackEnabled);

       



        

        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

    if (chamsEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    if (ImGui::Button("Chams", ImVec2(200, 40))) currentCategory = 10;
    ImGui::PopStyleVar();
    if (chamsEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) chamsEnabled = !chamsEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("Chams Settings");
    }
    if (ImGui::BeginPopupModal("Chams Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("ChamsContainer", ImVec2(190, 200), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("PlayerChams", &chamsPlayer);
        ImGui::Checkbox("ChestChams", &chamsChest);
        ImGui::Checkbox("ItemChams", &chamsItem);







        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

    if (tracersEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("Tracers", ImVec2(200, 40))) currentCategory = 11;
    ImGui::PopStyleVar();
    if (tracersEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) tracersEnabled = !tracersEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("Tracers Settings");
    }
    if (ImGui::BeginPopupModal("Tracers Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("TracersContainer", ImVec2(270, 200), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("Tracers", &tracersEnabled);
        if (ImGui::ColorEdit3("", (float*)&tracersColor, ImGuiColorEditFlags_NoAlpha)) {
            tracerColorR = tracersColor.x * 255.0f;
            tracerColorG = tracersColor.y * 255.0f;
            tracerColorB = tracersColor.z * 255.0f;
        }
        ImGui::Checkbox("Rainbow", &tracerRainbow);
        ImGui::SliderFloat("Speed", &tracerRainbowSpeed, 0.1f, 3.0f, "%.1f");
        ImGui::SliderFloat("Opacity", &tracerOpacity, 0, 255, "%.0f");
        ImGui::SliderFloat("Thickness", &tracerColorThickness, 0.1f, 3.0f, "%.1f");
     






        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    
    if (chinaHatEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("ChinaHat", ImVec2(200, 40))) currentCategory = 12;
    ImGui::PopStyleVar();
    if (chinaHatEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) chinaHatEnabled = !chinaHatEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("ChinaHat Settings");
    }
    if (ImGui::BeginPopupModal("ChinaHat Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("ChinaHatContainer", ImVec2(350, 150), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("China Hat", &chinaHatEnabled);
        if (ImGui::ColorEdit3("", (float*)&chinaColor, ImGuiColorEditFlags_NoAlpha)) {
            chinaR = chinaColor.x * 255.0f;
            chinaG = chinaColor.y * 255.0f;
            chinaB = chinaColor.z * 255.0f;
        }
        ImGui::Checkbox("Rainbow", &chinaRainbowEnabled);
        ImGui::SliderFloat("Speed", &chinaRainbowSpeed, 0.1f, 3.0f, "%.1f");
        






        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));


    if (nameTagsEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("NameTags", ImVec2(200, 40))) currentCategory = 13;
    ImGui::PopStyleVar();
    if (nameTagsEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) nameTagsEnabled = !nameTagsEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("NameTags Settings");
    }
    if (ImGui::BeginPopupModal("NameTags Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("NameTags", ImVec2(350, 150), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("NameTags", &nameTagsEnabled);
        ImGui::SliderFloat("", &tagsscale, 0.1f, 3.5f, "%.1f");
        






        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (playerScaleEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("PlayerScale", ImVec2(200, 40))) currentCategory = 14;
    ImGui::PopStyleVar();
    if (playerScaleEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) playerScaleEnabled = !playerScaleEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("EntityScale Settings");
    }
    if (ImGui::BeginPopupModal("EntityScale Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::BeginChild("PlayerScaleContainer", ImVec2(350, 150), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("Entity Scale", &playerScaleEnabled);
        ImGui::SliderFloat("X", &playerx, 0.1f, 3.5f, "%.1f");
        ImGui::SliderFloat("Y", &playery, 0.1f, 3.5f, "%.1f");
        ImGui::SliderFloat("Z", &playerz, 0.1f, 3.5f, "%.1f");







        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (itemScaleEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("ItemScale", ImVec2(200, 40))) currentCategory = 15;
    ImGui::PopStyleVar();
    if (itemScaleEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) itemScaleEnabled = !itemScaleEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("ItemScale Settings");
    }
    if (ImGui::BeginPopupModal("ItemScale Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        
        ImGui::BeginChild("ItemScaleContainer", ImVec2(350, 150), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Separator();
        ImGui::Checkbox("Item Scale", &itemScaleEnabled);
        ImGui::SliderFloat("X", &itemx, 0.1f, 3.5f, "%.1f");
        ImGui::SliderFloat("Y", &itemy, 0.1f, 3.5f, "%.1f");
        ImGui::SliderFloat("Z", &itemz, 0.1f, 3.5f, "%.1f");






        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));


    if (xrayEnabled) {

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.8f, 0.9f)); // Hovered (fare üstündeyken) için renk
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.55f, 0.7f, 0.9f)); // Mavi tonu
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    if (ImGui::Button("CaveFinder", ImVec2(200, 40))) currentCategory = 16;
    if (xrayEnabled) ImGui::PopStyleColor(2);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) xrayEnabled = !xrayEnabled; // Sað týk ile aç/kapa

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

        ImGui::OpenPopup("CaveFinder Settings");
    }
    if (ImGui::BeginPopupModal("CaveFinder Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.9f, 0.9f, 0.9f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85f, 0.85f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));

        ImGui::Checkbox("XRAY", &xrayEnabled);







        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Kapat", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndChild();
        ImGui::EndPopup();

    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PopStyleVar();


    ImGui::End();
    

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GUI::toggleVisibility()
{
    do_draw = !do_draw;
}

void GUI::hud(const std::string& featureName, bool featureEnabled, float& yOffset)
{
    if (featureEnabled) {

        auto draw_list = ImGui::GetBackgroundDrawList();

        ImVec2 featureTextSize = ImGui::CalcTextSize(featureName.c_str());

        ImVec2 featurePos(9.5f, yOffset);

        ImColor bgColor(1.0f, 1.0f, 1.0f, 0.5f); // Þeffaf beyaz arka plan
        ImColor textColor(50, 50, 50, 255); // Koyu gri yazý rengi

        float rounding = 0.0f; // Köþe yuvarlama yarýçapý
        float elementHeight = featureTextSize.y + 4.0f; // Kutunun yüksekliði

        // Arka plan (þeffaf beyaz ve yuvarlatýlmýþ köþeler)
        draw_list->AddRectFilled(
            ImVec2(featurePos.x - 2.0f, featurePos.y - 2.0f),
            ImVec2(featurePos.x + featureTextSize.x + 4.0f, featurePos.y + featureTextSize.y + 2.0f),
            bgColor,
            rounding // Köþe yuvarlama
        );

        // Metin
        draw_list->AddText(featurePos, textColor, featureName.c_str());

        // Sonraki öðe için yOffset'i artýr
        yOffset += elementHeight;
    }
}




std::chrono::steady_clock::time_point lastFpsUpdateTime = std::chrono::steady_clock::now();
int currentFps = 0;
int frameCount = 0;

void GUI::drawHud()
{
    updateRainbowEffects();
    if (!do_draw)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        auto draw_list = ImGui::GetBackgroundDrawList();

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastFpsUpdateTime).count();

        if (elapsed >= 1) {
            currentFps = frameCount;
            frameCount = 0;
            lastFpsUpdateTime = now;
        }
        frameCount++;

        std::string fpsText = std::to_string(currentFps) + " fps";
        std::string watermark = "Discord : sobe00 | " + fpsText;

        ImVec2 textPos(9.5f, 8.5f);
        ImVec2 textSize = ImGui::CalcTextSize(watermark.c_str());
        ImColor bgColor(1.0f, 1.0f, 1.0f, 0.5f); // Þeffaf beyaz arka plan
        ImColor textColor(50, 50, 50, 255);  // Tam opak beyaz metin

        float rounding = 0.0f; // Köþe yumuþatma yarýçapý

        // Arka plan (þeffaf beyaz ve köþeleri yuvarlatýlmýþ)
        draw_list->AddRectFilled(
            ImVec2(textPos.x - 2.0f, textPos.y - 2.0f),
            ImVec2(textPos.x + textSize.x + 7.0f, textPos.y + textSize.y + 7.0f),
            bgColor,
            rounding // Köþe yumuþatma yarýçapý
        );

        // Metin
        draw_list->AddText(textPos, textColor, watermark.c_str());

        float yOffset = textPos.y + textSize.y + 9.0f;

        if (aimbotEnabled) {
            hud("Aimbot [R]", aimbotEnabled, yOffset);
        }
        if (esp2dEnabled) {
            hud("ESP [Z]", esp2dEnabled, yOffset);
        }
        if (chamsEnabled) {
            hud("Chams", chamsEnabled, yOffset);
        }
        if (wallHackEnabled) {
            hud("WallHack", wallHackEnabled, yOffset);
        }
        if (tracersEnabled) {
            hud("Tracers", tracersEnabled, yOffset);
        }
        if (leftclickEnabled) {
            hud("LeftClicker [F]", leftclickEnabled, yOffset);
        }
        if (rightclickEnabled) {
            hud("RightClicker [G]", rightclickEnabled, yOffset);
        }
        if (triggerbotEnabled) {
            hud("TriggerBot [H]", triggerbotEnabled, yOffset);
        }
        if (playerScaleEnabled) {
            hud("PlayerScale", playerScaleEnabled, yOffset);
        }
        if (nameTagsEnabled) {
            hud("NameTags", nameTagsEnabled, yOffset);
        }
        if (chinaHatEnabled) {
            hud("ChinaHat", chinaHatEnabled, yOffset);
        }
        if (xrayEnabled) {
            hud("XRAY [X]", xrayEnabled, yOffset);
        }
        if (huddraw) {
            hud("HUD", huddraw, yOffset);
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        
        
        

        
    }
}

void GUI::updateRainbowEffects()
{
    if (GUI::chinaRainbowEnabled)
    {
        float time = ImGui::GetTime() * GUI::chinaRainbowSpeed;
        float hue = fmodf(time / 10.0f, 1.0f);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        GUI::chinaR = r * 255.0f;
        GUI::chinaG = g * 255.0f;
        GUI::chinaB = b * 255.0f;
    }
    if (GUI::espRainbow)
    {
        float time = ImGui::GetTime() * GUI::espRainbowSpeed;
        float hue = fmodf(time / 10.0f, 1.0f);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        GUI::espR = r * 255.0f;
        GUI::espG = g * 255.0f;
        GUI::espB = b * 255.0f;
    }
    if (GUI::tracerRainbow)
    {
        float time = ImGui::GetTime() * GUI::tracerRainbowSpeed;
        float hue = fmodf(time / 10.0f, 1.0f);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        GUI::tracerColorR = r * 255.0f;
        GUI::tracerColorG = g * 255.0f;
        GUI::tracerColorB = b * 255.0f;
    }
}


bool GUI::getIsInit()
{
    return is_init;
}

bool GUI::getDoDraw()
{
    return do_draw;
}

void GUI::setDoDraw(bool new_value)
{
    do_draw = new_value;
}
