#include "AudioService.h"

#include <AudioTools.h>
#include <AudioTools/AudioCodecs/CodecMP3Helix.h>
#include <AudioTools/AudioLibs/AudioBoardStream.h>
#include <AudioTools/Disk/AudioSourceSD.h>
#include <Logger.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace AudioService {

static const char* startFilePath = "/";
static const char* ext = "mp3";

static AudioSourceSD source(startFilePath, ext, PIN_AUDIO_KIT_SD_CARD_CS);
static AudioBoardStream kit(AudioKitEs8388V1);
static MP3DecoderHelix decoder;
static AudioPlayer player(source, kit, decoder);

static const char* AUDIO_FILE_CONNECT_PADS = "connect_pads.mp3";
static const char* AUDIO_FILE_RYTHM_ANALYSIS_IN_PROGRESS = "analysis_in_progress.mp3";
static const char* AUDIO_FILE_SHOCK_ADVISED_CHARGING = "shock_advised_charging.mp3";
static const char* AUDIO_FILE_SHOCK_DELIVERED = "shock_delivered.mp3";
static const char* AUDIO_FILE_SHOCK_NOT_ADVISED = "shock_not_advised.mp3";

static QueueHandle_t trackQueue = nullptr;

static void Task(void*);

void Start() {
    trackQueue = xQueueCreate(10, sizeof(AudioTracks));

    AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);

    auto cfg = kit.defaultConfig(TX_MODE);
    cfg.sd_active = true;
    kit.begin(cfg);

    // Enable Amplifiers
    // kit.setPAPower(true);

    // setup player
    player.setVolume(0.7);
    player.setAutoNext(false);
    player.begin();

    xTaskCreatePinnedToCore(Task, "AudioService", 10000, NULL, 1, NULL, 1);
}

static void Task(void*) {
    LOG_D("Audio Service started");

    AudioTracks track;
    for (;;) {
        if (!player.isActive() && xQueueReceive(trackQueue, &track, 0) == pdTRUE) {
            switch (track) {
                case AudioTracks::CONNECT_PADS:
                    player.playPath(AUDIO_FILE_CONNECT_PADS);
                    break;
                case AudioTracks::RYTHM_ANALYSIS_IN_PROGRESS:
                    player.playPath(AUDIO_FILE_RYTHM_ANALYSIS_IN_PROGRESS);
                    break;
                case AudioTracks::SHOCK_ADVISED_CHARGING:
                    player.playPath(AUDIO_FILE_SHOCK_ADVISED_CHARGING);
                    break;
                case AudioTracks::SHOCK_DELIVERED:
                    player.playPath(AUDIO_FILE_SHOCK_DELIVERED);
                    break;
                case AudioTracks::SHOCK_NOT_ADVISED:
                    player.playPath(AUDIO_FILE_SHOCK_NOT_ADVISED);
                    break;
            }
        }

        player.copy();
        kit.processActions();
    }
}

void QueueTrack(AudioTracks track) {
    xQueueSend(trackQueue, &track, 0);
}

}  // namespace AudioService