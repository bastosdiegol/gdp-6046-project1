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

    static const char* current_item;
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
            m_fmod_manager->getChannelGroupMuteStatus("music", &m_music->m_isMuted); // Gets the music muted status
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
                if (m_music->m_isPaused) {
                    m_fmod_manager->setPause("music", false);
                    m_music->m_isPaused = false;
                } else {
                    m_fmod_manager->playSound(current_item, "music");
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_PAUSE)) { // Pause Button
                m_fmod_manager->setPause("music", true);
                m_music->m_isPaused = true;
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FAD_STOP)) { // Stop Button
                m_fmod_manager->stopSound("music");
                m_music->m_isPaused = false;
            }
            ImGui::SameLine();
            if (m_music->m_isMuted) {
                if (ImGui::Button(ICON_FAD_MUTE)) { // UnMute Button
                    m_music->m_isMuted = false;
                    m_fmod_manager->setChannelGroupMuteStatus("music", m_music->m_isMuted);
                }
            } else {
                if (ImGui::Button(ICON_FAD_MUTE)) { // Mute Button
                    m_music->m_isMuted = true;
                    m_fmod_manager->setChannelGroupMuteStatus("music", m_music->m_isMuted);
                }
            }
            // *****************************
            // MUSIC CHANNEL DSP TABLE START
            // *****************************
            if (ImGui::BeginTable("tableMusicControl", 13, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                // Music Channel Volume Knob
                m_fmod_manager->getChannelGroupVolume("music", &m_music->m_volume); // Gets the volume
                m_music->m_volume *= 100; // Conversion 0% to 100%
                if (ImGuiKnobs::Knob("Volume", &m_music->m_volume, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_music->m_volume /= 100; // Back to the original float value
                    if (m_music->m_volume > 1.0f) { // Safe Guard to avoid hardware issues
                        m_music->m_volume = 1.0f;
                    }
                    m_fmod_manager->setChannelGroupVolume("music", m_music->m_volume); // Sets new volume
                }
                ImGui::TableNextColumn();
                // Music Channel Pan Knob
                m_fmod_manager->getChannelGroupPan("music", &m_music->m_pan); // Gets the pan
                if (ImGuiKnobs::Knob("Pan", &m_music->m_pan, -1.0f, 1.0f, 0.01f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setChannelGroupPan("music", m_music->m_pan); // Sets new pan
                }
                // DSP CHORUS
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Chorus##Music", &m_music->m_dspChorus)) {
                    if (m_music->m_dspChorus) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_CHORUS);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_CHORUS);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_CHORUS, FMOD_DSP_CHORUS_MIX, &m_music->m_chorusMix); // Gets the chorus mix
                if (ImGuiKnobs::Knob("Chorus Mix", &m_music->m_chorusMix, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_CHORUS, FMOD_DSP_CHORUS_MIX, m_music->m_chorusMix); // Sets new chorus mix
                }
                // DSP COMPRESSOR
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Compressor##Music", &m_music->m_dspCompressor)) {
                    if (m_music->m_dspCompressor) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_COMPRESSOR);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_COMPRESSOR);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_COMPRESSOR, FMOD_DSP_COMPRESSOR_THRESHOLD, &m_music->m_compressorThreshold); // Gets the echo delay
                if (ImGuiKnobs::Knob("Threshold", &m_music->m_compressorThreshold, -60.0f, 0.0f, 0.5f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_COMPRESSOR, FMOD_DSP_COMPRESSOR_THRESHOLD, m_music->m_compressorThreshold); // Sets new echo delay
                }
                // DSP DELAY
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Delay##Music", &m_music->m_dspDelay)) {
                    if (m_music->m_dspDelay) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_DELAY);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_DELAY);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_DELAY, FMOD_DSP_DELAY_MAXDELAY, &m_music->m_delayMax); // Gets the echo delay
                if (ImGuiKnobs::Knob("Max", &m_music->m_delayMax, 0.0f, 10000.0f, 10.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_DELAY, FMOD_DSP_DELAY_MAXDELAY, m_music->m_delayMax); // Sets new echo delay
                }
                // DSP DISTORTION
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Distortion##Music", &m_music->m_dspDistortion)) {
                    if (m_music->m_dspDistortion) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_DISTORTION);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_DISTORTION);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_DISTORTION, FMOD_DSP_DISTORTION_LEVEL, &m_music->m_distortionLevel); // Gets the echo delay
                if (ImGuiKnobs::Knob("Level", &m_music->m_distortionLevel, 0.0f, 1.0f, 0.01f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_DISTORTION, FMOD_DSP_DISTORTION_LEVEL, m_music->m_distortionLevel); // Sets new echo delay
                }
                // DSP ECHO
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Echo##Music", &m_music->m_dspEcho)) {
                    if (m_music->m_dspEcho) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_ECHO);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_ECHO);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_ECHO, FMOD_DSP_ECHO_DELAY, &m_music->m_echoDelay); // Gets the echo delay
                if (ImGuiKnobs::Knob("Delay", &m_music->m_echoDelay, 1.0f, 5000.0f, 10.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_ECHO, FMOD_DSP_ECHO_DELAY, m_music->m_echoDelay); // Sets new echo delay
                }
                // DSP FADER
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Fader##Music", &m_music->m_dspFader)) {
                    if (m_music->m_dspFader) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_FADER);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_FADER);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_FADER, FMOD_DSP_FADER_GAIN, &m_music->m_faderGain); // Gets the echo delay
                if (ImGuiKnobs::Knob("Gain", &m_music->m_faderGain, -80.0f, 10.0f, 1.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_FADER, FMOD_DSP_FADER_GAIN, m_music->m_faderGain); // Sets new echo delay
                }
                // DSP FLANGE
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Flange##Music", &m_music->m_dspFlange)) {
                    if (m_music->m_dspFlange) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_FLANGE);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_FLANGE);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_FLANGE, FMOD_DSP_FLANGE_MIX, &m_music->m_flangeMix); // Gets the echo delay
                if (ImGuiKnobs::Knob("Flange Mix", &m_music->m_flangeMix, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_FLANGE, FMOD_DSP_FLANGE_MIX, m_music->m_flangeMix); // Sets new echo delay
                }
                // DSP NORMALIZE
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Normalize##Music", &m_music->m_dspNormalize)) {
                    if (m_music->m_dspNormalize) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_NORMALIZE);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_NORMALIZE);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_NORMALIZE, FMOD_DSP_NORMALIZE_FADETIME, &m_music->m_normalizeFadetime); // Gets the echo delay
                if (ImGuiKnobs::Knob("Fade Time", &m_music->m_normalizeFadetime, 0.0f, 20000.0f, 10.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_NORMALIZE, FMOD_DSP_NORMALIZE_FADETIME, m_music->m_normalizeFadetime); // Sets new echo delay
                }
                // DSP OSCILLATOR
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Oscillator##Music", &m_music->m_dspOscillator)) {
                    if (m_music->m_dspOscillator) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_OSCILLATOR);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_OSCILLATOR);
                    }
                }
                m_fmod_manager->getIntParameterDSP(FMOD_DSP_TYPE_OSCILLATOR, FMOD_DSP_OSCILLATOR_TYPE, &m_music->m_oscillatorType); // Gets the echo delay
                if (ImGuiKnobs::KnobInt("Type", &m_music->m_oscillatorType, 0, 5, 0.1f, "%d", ImGuiKnobVariant_Stepped)) {
                    m_fmod_manager->setIntParameterDSP(FMOD_DSP_TYPE_OSCILLATOR, FMOD_DSP_OSCILLATOR_TYPE, m_music->m_oscillatorType); // Sets new echo delay
                }
                // DSP SFXREVERB
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("SFX Reverb##Music", &m_music->m_dspSFXReverb)) {
                    if (m_music->m_dspSFXReverb) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_SFXREVERB);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_SFXREVERB);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_SFXREVERB, FMOD_DSP_NORMALIZE_FADETIME, &m_music->m_reverbDecay); // Gets the echo delay
                if (ImGuiKnobs::Knob("Decay", &m_music->m_reverbDecay, 100.0f, 20000.0f, 100.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_SFXREVERB, FMOD_DSP_NORMALIZE_FADETIME, m_music->m_reverbDecay); // Sets new echo delay
                }
                // DSP TREMOLO
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Tremolo##Music", &m_music->m_dspTremolo)) {
                    if (m_music->m_dspTremolo) {
                        m_fmod_manager->addDSPEffect("music", FMOD_DSP_TYPE_TREMOLO);
                    } else {
                        m_fmod_manager->removeDSPEffect("music", FMOD_DSP_TYPE_TREMOLO);
                    }
                }
                m_fmod_manager->getFloatParameterDSP(FMOD_DSP_TYPE_TREMOLO, FMOD_DSP_TREMOLO_FREQUENCY, &m_music->m_tremoloFrequency); // Gets the echo delay
                if (ImGuiKnobs::Knob("Frequency", &m_music->m_tremoloFrequency, 0.1f, 20.0f, 0.1f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                    m_fmod_manager->setFloatParameterDSP(FMOD_DSP_TYPE_TREMOLO, FMOD_DSP_TREMOLO_FREQUENCY, m_music->m_tremoloFrequency); // Sets new echo delay
                }
                ImGui::EndTable();
            }            
            ImGui::EndTabItem();
        }
        // -------------
        //   FX CHANNEL
        // -------------
        if (ImGui::BeginTabItem("FX")) {
            ImGui::Text("FXs Available:");
            static float fxVolume = 0, fxPan = 0;
            static bool isFxPaused = false;
            static bool isFxMuted = false;
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
            if (ImGuiKnobs::Knob("Volume", &fxVolume, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                fxVolume /= 100; // Back to the original float value
                if (fxVolume > 1.0f) { // Safe Guard to avoid hardware issues
                    fxVolume = 1.0f;
                }
                m_fmod_manager->setChannelGroupVolume("fx", fxVolume); // Sets new volume
            }
            // FX Channel Pan Knob
            ImGui::SameLine();
            m_fmod_manager->getChannelGroupPan("fx", &fxPan); // Gets the pan
            if (ImGuiKnobs::Knob("Pan", &fxPan, -1.0f, 1.0f, 0.01f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
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
            if (ImGuiKnobs::Knob("Volume", &masterVolume, 0.0f, 100.0f, 1.0f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                masterVolume /= 100; // Back to the original float value
                if (masterVolume > 1.0f) { // Safe Guard to avoid hardware issues
                    masterVolume = 1.0f;
                }
                m_fmod_manager->setChannelGroupVolume("master", masterVolume); // Sets new volume
            }
            // Master Channel Pan Knob
            ImGui::SameLine();
            m_fmod_manager->getChannelGroupPan("master", &masterPan); // Gets the pan
            if (ImGuiKnobs::Knob("Pan", &masterPan, -1.0f, 1.0f, 0.01f, "%.1f%", ImGuiKnobVariant_WiperDot)) {
                m_fmod_manager->setChannelGroupPan("master", masterPan); // Sets new pan
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}
