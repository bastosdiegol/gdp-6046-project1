#include <vector>

#include "SoundUI.h"
#include "imgui/imgui.h"
#include "imgui/IconsFontaudio.h"
#include "imgui/imgui-knobs.h"

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
        // -------------
        // MUSIC CHANNEL
        // -------------
        if (ImGui::BeginTabItem("Music")) {
            ImGui::Text("Musics Available:");
            static bool isMusicPaused = false;
            static bool isMusicMuted = false;
            static float musicVolume = 0;
            static float musicPan = 0;
            static const char* current_item = NULL;
            m_fmod_manager->getChannelGroupMuteStatus("music", &isMusicMuted); // Gets the music muted status
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
            if (ImGui::Button(ICON_FAD_PLAY)) { // Play Button
                if (isMusicPaused) {
                    m_fmod_manager->setPause("music", false);
                    isMusicPaused = false;
                } else {
                    m_fmod_manager->playSound(current_item, "music");
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PAUSE)) { // Pause Button
                m_fmod_manager->setPause("music", true);
                isMusicPaused = true;
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_STOP)) { // Stop Button
                m_fmod_manager->stopSound("music");
                isMusicPaused = false;
            }
            ImGui::SameLine();
            if (isMusicMuted) {
                if (ImGui::Button(ICON_FAD_MUTE)) { // UnMute Button
                    isMusicMuted = false;
                    m_fmod_manager->setChannelGroupMuteStatus("music", isMusicMuted);
                }
            } else {
                if (ImGui::Button(ICON_FAD_MUTE)) { // Mute Button
                    isMusicMuted = true;
                    m_fmod_manager->setChannelGroupMuteStatus("music", isMusicMuted);
                }
            }
            // Music Channel Volume Knob
            m_fmod_manager->getChannelGroupVolume("music", &musicVolume); // Gets the volume
            musicVolume *= 100; // Conversion 0% to 100%
            if (ImGuiKnobs::Knob("Volume", &musicVolume, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_Tick)) {
                musicVolume /= 100; // Back to the original float value
                if (musicVolume > 1.0f) { // Safe Guard to avoid hardware issues
                    musicVolume = 1.0f;
                }
                m_fmod_manager->setChannelGroupVolume("music", musicVolume); // Sets new volume
            }
            // Music Channel Pan Knob
            ImGui::SameLine();
            m_fmod_manager->getChannelGroupPan("music", &musicPan); // Gets the pan
            if (ImGuiKnobs::Knob("Pan", &musicPan, -1.0f, 1.0f, 0.01f, "%.1f%", ImGuiKnobVariant_Tick)) {
                m_fmod_manager->setChannelGroupPan("music", musicPan); // Sets new pan
            }
            ImGui::EndTabItem();
        }
        // -------------
        //   FX CHANNEL
        // -------------
        if (ImGui::BeginTabItem("FX")) {
            ImGui::Text("FXs Available:");
            static bool isFxPaused = false;
            static float fxVolume = 0;
            static bool isFxMuted = false;
            static float fxPan = 0;
            static const char* current_item = NULL;
            m_fmod_manager->getChannelGroupMuteStatus("fx", &isFxMuted); // Gets the fx muted status
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
            if (ImGui::Button(ICON_FAD_PLAY)) { // Play Button
                if (isFxPaused) {
                    m_fmod_manager->setPause("fx", false);
                    isFxPaused = false;
                } else {
                    m_fmod_manager->playSound(current_item, "fx");
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PAUSE)) { // Pause Button
                m_fmod_manager->setPause("fx", true);
                isFxPaused = true;
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_STOP)) { // Stop Button
                m_fmod_manager->stopSound("fx");
                isFxPaused = false;
            }
            ImGui::SameLine();
            if (isFxMuted) {
                if (ImGui::Button(ICON_FAD_MUTE)) { // UnMute Button
                    isFxMuted = false;
                    m_fmod_manager->setChannelGroupMuteStatus("music", isFxMuted);
                }
            } else {
                if (ImGui::Button(ICON_FAD_MUTE)) { // Mute Button
                    isFxMuted = true;
                    m_fmod_manager->setChannelGroupMuteStatus("music", isFxMuted);
                }
            }
            // FX Channel Volume Knob
            m_fmod_manager->getChannelGroupVolume("fx", &fxVolume); // Gets the volume
            fxVolume *= 100; // Conversion 0% to 100%
            if (ImGuiKnobs::Knob("Volume", &fxVolume, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_Tick)) {
                fxVolume /= 100; // Back to the original float value
                if (fxVolume > 1.0f) { // Safe Guard to avoid hardware issues
                    fxVolume = 1.0f;
                }
                m_fmod_manager->setChannelGroupVolume("fx", fxVolume); // Sets new volume
            }
            // FX Channel Pan Knob
            ImGui::SameLine();
            m_fmod_manager->getChannelGroupPan("fx", &fxPan); // Gets the pan
            if (ImGuiKnobs::Knob("Pan", &fxPan, -1.0f, 1.0f, 0.01f, "%.1f%", ImGuiKnobVariant_Tick)) {
                m_fmod_manager->setChannelGroupPan("fx", fxPan); // Sets new pan
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    // --------------
    // MASTER CHANNEL
    // --------------
    ImGui::Text("   ");
    ImGui::Text("   ");
    if (ImGui::BeginTabBar("Master Channel")) {
        if (ImGui::BeginTabItem("Master")) {
            ImGui::Text("Master Channel");
            // Master Channel Volume Knob
            static float masterVolume = 0;
            static float masterPan = 0;
            static bool isMasterPaused = false;
            static bool isMasterMuted = false;
            m_fmod_manager->getChannelGroupMuteStatus("fx", &isMasterMuted); // Gets the master muted status
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PLAY)) { // Play Button
                if (isMasterPaused) {
                    m_fmod_manager->setPause("master", false);
                    isMasterPaused = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PAUSE)) { // Pause Button
                m_fmod_manager->setPause("master", true);
                isMasterPaused = true;
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_STOP)) { // Stop Button
                m_fmod_manager->stopSound("master");
                isMasterPaused = false;
            }
            ImGui::SameLine();
            if (isMasterMuted) {
                if (ImGui::Button(ICON_FAD_MUTE)) { // UnMute Button
                    isMasterMuted = false;
                    m_fmod_manager->setChannelGroupMuteStatus("master", isMasterMuted);
                }
            } else {
                if (ImGui::Button(ICON_FAD_MUTE)) { // Mute Button
                    isMasterMuted = true;
                    m_fmod_manager->setChannelGroupMuteStatus("master", isMasterMuted);
                }
            }
            m_fmod_manager->getChannelGroupVolume("master", &masterVolume); // Gets the master volume
            masterVolume *= 100; // Conversion 0% to 100%
            if (ImGuiKnobs::Knob("Volume", &masterVolume, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_Tick)) {
                masterVolume /= 100; // Back to the original float value
                if (masterVolume > 1.0f) { // Safe Guard to avoid hardware issues
                    masterVolume = 1.0f;
                }
                m_fmod_manager->setChannelGroupVolume("master", masterVolume); // Sets new volume
            }
            // Master Channel Pan Knob
            ImGui::SameLine();
            m_fmod_manager->getChannelGroupPan("master", &masterPan); // Gets the pan
            if (ImGuiKnobs::Knob("Pan", &masterPan, -1.0f, 1.0f, 0.01f, "%.1f%", ImGuiKnobVariant_Tick)) {
                m_fmod_manager->setChannelGroupPan("master", masterPan); // Sets new pan
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}
