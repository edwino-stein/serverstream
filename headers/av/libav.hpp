#if !defined  _LIB_AV_HPP
    #define _LIB_AV_HPP

    #include "types.hpp"

    extern "C" {
        #include <libavcodec/avcodec.h>
        #include <libavdevice/avdevice.h>
        #include <libavfilter/avfilter.h>
        #include <libavformat/avformat.h>
        #include <libavutil/avutil.h>
        #include <libavutil/pixdesc.h>
        #include <libavformat/avio.h>
        #include <libavfilter/buffersink.h>
        #include <libavfilter/buffersrc.h>
    }

    //For missing avio_context_free on old versions
    #if LIBAVFORMAT_VERSION_INT <= AV_VERSION_INT(57, 56, 101)
        void avio_context_free(AVIOContext **ps);
    #endif

    namespace av {

        //Functions
        void initAll();

        //Classes
        class Codec;
        class CodecVideo;
        class Format;
        class FormatInput;
        class FormatOutput;
        class VideoInput;
        class VideoOutput;
        class VideoFileInput;
        class V4L2DeviceInput;
        class VideoFileOutput;
        class VideoMemoryOutput;
        class FilterGraph;
        class Transcoder;

        //Interfaces
        class DecodeListener;
    }

#endif
