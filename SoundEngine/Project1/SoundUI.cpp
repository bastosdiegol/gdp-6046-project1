#include <vector>

#include "SoundUI.h"
#include "imgui/imgui.h"
#include "imgui/IconsFontaudio.h"

void SoundUI::render() {
	// Create a window called "Audio Settings", with a menu bar.
	ImGui::Begin("Sound Engine Tools", &is_my_tool_active, ImGuiWindowFlags_MenuBar );
    
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
            if (ImGui::MenuItem("Close", "Ctrl+W")) { is_my_tool_active = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    std::vector<const char *> musics;
    std::vector<const char*> fx;
    std::map<std::string, Sound*>::iterator itSounds;
    for (itSounds = m_fmod_manager->m_sounds.begin(); itSounds != m_fmod_manager->m_sounds.end(); itSounds++) {
        if (itSounds->second->m_type == "MUSIC")
            musics.push_back(itSounds->first.c_str());
        if (itSounds->second->m_type == "FX")
            fx.push_back(itSounds->first.c_str());
    }


    if (ImGui::BeginTabBar("Sound Library")) {
        if (ImGui::BeginTabItem("Music")) {
            ImGui::Text("Musics Available:");
            static bool isMusicPaused = false;
            static const char* current_item = NULL;
            if (ImGui::BeginCombo("##combo", current_item)) {
                for (int i = 0; i < musics.size(); i++) {
                    bool is_selected = (current_item == musics[i]);
                    if (ImGui::Selectable(musics[i], is_selected))
                        current_item = musics[i];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PLAY)) {
                if (isMusicPaused) {
                    m_fmod_manager->setPause("music", false);
                    isMusicPaused = false;
                } else {
                    m_fmod_manager->playSound(current_item, "music");
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PAUSE)) {
                m_fmod_manager->setPause("music", true);
                isMusicPaused = true;
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_STOP)) {
                m_fmod_manager->stopSound("music");
                isMusicPaused = false;
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("FX")) {
            ImGui::Text("FXs Available:");
            static bool isFxPaused = false;
            static const char* current_item = NULL;
            if (ImGui::BeginCombo("##combo", current_item)) {
                for (int i = 0; i < fx.size(); i++) {
                    bool is_selected = (current_item == fx[i]);
                    if (ImGui::Selectable(fx[i], is_selected))
                        current_item = fx[i];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PLAY)) {
                if (isFxPaused) {
                    m_fmod_manager->setPause("fx", false);
                    isFxPaused = false;
                } else {
                    m_fmod_manager->playSound(current_item, "fx");
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PAUSE)) {
                m_fmod_manager->setPause("fx", true);
                isFxPaused = true;
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_STOP)) {
                m_fmod_manager->stopSound("fx");
                isFxPaused = false;
            }
            ImGui::EndTabItem();
        }
    }
    
    ImGui::EndTabBar();
    ImGui::End();
}
