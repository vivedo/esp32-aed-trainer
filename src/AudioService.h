#ifndef AUDIO_SERVICE_H
#define AUDIO_SERVICE_H

#include <Logger.h>

#include <AudioTools.h>
#include <AudioTools/AudioLibs/AudioBoardStream.h>
#include <AudioTools/Disk/AudioSourceSD.h>
#include <AudioTools/AudioCodecs/CodecMP3Helix.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

enum class AudioTracks
{
    CONNECT_PADS,
    RYTHM_ANALYSIS_IN_PROGRESS,
    SHOCK_ADVISED_CHARGING,
    SHOCK_DELIVERED,
    SHOCK_NOT_ADVISED
};

void AudioService_Start();
void QueueTrack(AudioTracks track);

#endif // AUDIO_SERVICE_H