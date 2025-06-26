#include <iostream>

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/dict.h>
    #include <libavutil/samplefmt.h>
    #include <libavcodec/codec_desc.h>
}

void printVideoStreamInfo(AVStream* stream, size_t index) {
    AVCodecParameters* codecParams = stream->codecpar;
    std::string type;

    if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO) {
        type = "Video";
    } else if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO) {
        type = "Audio";
    } else {
        std::cout << "Unknown codec type for stream " << index << '\n';

        return;
    }

    std::cout << "\nStream number: " << index << '\n';
    std::cout << "Type: " << type << '\n';
    std::cout << "Codec: " << avcodec_get_name(codecParams->codec_id) << '\n';

    if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO) {
        std::cout << "Video resolution: " << codecParams->height << "x" << codecParams->width << '\n';
        std::cout << "Number of frames: " << stream->nb_frames << '\n';

        double frameRate = 0;

        if (stream->r_frame_rate.num && stream->r_frame_rate.den) {
            frameRate = av_q2d(stream->r_frame_rate);
        }

        std::cout << "FPS: " << frameRate << '\n';
    }

    if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO) {
        std::cout << "Audio sample rate: " << codecParams->sample_rate << " Hz\n";
        std::cout << "Channels number: " << codecParams->ch_layout.nb_channels << '\n';

        char layoutStr[128];
        std::cout << "Channel layout: " << av_channel_layout_describe(
                    &codecParams->ch_layout,
                    layoutStr,
                    sizeof(layoutStr)
                ) << '\n';

        std::cout << "Sample format: " << av_get_sample_fmt_name(
                    static_cast<AVSampleFormat>(codecParams->format)
                ) << '\n';
    }

    std::cout << "Stream duration: " << (double)stream->duration * av_q2d(stream->time_base) << " seconds\n";
}

void printVideoInfo(const std::string& filename) {
    AVFormatContext* pFormatContext = avformat_alloc_context();

    if (!pFormatContext) {
        std::cerr << "Error while allocating memory for format context\n";

        return;
    }

    if (avformat_open_input(&pFormatContext, filename.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Error while opening file\n";
        avformat_free_context(pFormatContext);

        return;
    }

    if (avformat_find_stream_info(pFormatContext, nullptr) < 0) {
        std::cerr << "Error while reading stream info\n";
        avformat_close_input(&pFormatContext);

        return;
    }

    std::cout << "File: " << filename << '\n';
    std::cout << "File format: " << pFormatContext->iformat->long_name << " (" << pFormatContext->iformat->name << ")\n";
    std::cout << "Entire video duration: " << (double)pFormatContext->duration / AV_TIME_BASE << " seconds\n";

    for (size_t i = 0; i < pFormatContext->nb_streams; i++) {
        AVStream* stream = pFormatContext->streams[i];
        printVideoStreamInfo(stream, i);
    }

    avformat_close_input(&pFormatContext);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Error! No file specified\n";

        return -1;
    }

    const char* filename = argv[1];

    printVideoInfo(filename);
}
