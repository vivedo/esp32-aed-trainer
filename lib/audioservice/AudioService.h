#pragma once

namespace AudioService {

enum class AudioTracks {
    CONNECT_PADS,
    RYTHM_ANALYSIS_IN_PROGRESS,
    SHOCK_ADVISED_CHARGING,
    SHOCK_DELIVERED,
    SHOCK_NOT_ADVISED
};

void Start();
void QueueTrack(AudioTracks track);

}  // namespace AudioService
