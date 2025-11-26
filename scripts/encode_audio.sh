#!/bin/bash

# Remove existing encoded audio files
#rm -rf data/audio
# Create output directory
mkdir -p data/audio

# Find all audio files in audio/ subdirectories (mp3, wav, flac, m4a, ogg, aac, wma)
find audio -type f \( -iname "*.mp3" -o -iname "*.wav" -o -iname "*.flac" -o -iname "*.m4a" -o -iname "*.ogg" -o -iname "*.aac" -o -iname "*.wma" -o -iname "*.aiff" -o -iname "*.ape" \) | while read -r audio_file; do
    # Get the relative path from audio/ directory
    relative_path="${audio_file#audio/}"
    
    # Remove extension and add .opus
    output_file="data/audio/${relative_path%.*}.opus"
    
    # Create output directory if it doesn't exist
    output_dir=$(dirname "$output_file")
    mkdir -p "$output_dir"
    
    # Convert to Opus
    echo "Converting: $audio_file -> $output_file"
    ffmpeg -i "$audio_file" -c:a libopus -b:a 12k -ac 1 -ar 16000 "$output_file" -y -loglevel error
    
    if [ $? -eq 0 ]; then
        echo "✓ Success: $output_file"
    else
        echo "✗ Failed: $audio_file"
    fi
done

echo ""
echo "Conversion complete!"
echo "Total files converted: $(find data/audio -type f -name "*.opus" | wc -l)"

echo "Total size of encoded audio files:"
du -sh data/audio