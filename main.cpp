#include <OpenAL/OpenAL.h>
#include "stb_vorbis.h"

class AudioStream {
public:
    ALuint buffer, source;
    static AudioStream* Create(std::string audioName, float gain);
};

AudioStream* AudioStream::Create(std::string audioName, float gain) {

    ALenum format = AL_FORMAT_MONO16;

    stb_vorbis* result = stb_vorbis_open_filename(audioName.c_str(), nullptr, nullptr);
    stb_vorbis_info info = stb_vorbis_get_info(result);
    int16_t* output;

    int channels, rate;

    int count = stb_vorbis_decode_filename(audioName.c_str(), &channels, &rate, &output);
    int length = count * sizeof(int16_t) * info.channels;

    if (result->channels == 1) format = AL_FORMAT_MONO16;
    if (result->channels == 2) format = AL_FORMAT_STEREO16;

    AudioStream* stream = new AudioStream();
    alGenBuffers(1, &stream->buffer);
    alBufferData(stream->buffer, format, output, length, result->sample_rate);

    alGenSources(1, &stream->source);
    alSourcei(stream->source, AL_BUFFER, stream->buffer);
    alSourcei(stream->source, AL_LOOPING, 1);
    alSourcef(stream->source, AL_GAIN, gain);

    return stream;
}

void PlayAudioStream(AudioStream* stream) {
    alSourcePlay(stream->source);
}

void InitializeOpenAL() {

    DayOfArmistice::Audio::device = alcOpenDevice(nullptr);
    DayOfArmistice::Audio::context = alcCreateContext(DayOfArmistice::Audio::device, nullptr);
    alcMakeContextCurrent(DayOfArmistice::Audio::context);
}





int main() {
    InitializeOpenAL();
    AudioStream* stream = AudioStream::Create(".../something.ogg");
    PlayAudioStream(stream);    
}
