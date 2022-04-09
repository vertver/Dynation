#pragma once

#define ENVELOPEATTACK      1.0     // 1ms
#define ENVELOPERELEASE     1000.0   // 1000 ms


// стаитческий рмс с окном на 1024 семпла независимо от фремретейм рейта 22050=46ms 44100=23ms 88200=11ms
#define RMSWINDOWSIZE  (16)

class RMSDetector {
public:
    void        Reset( ) {
        Index   = 0;
        RMSSumm  = 0.0;
        for ( u32 i = 0; i < RMSWINDOWSIZE; i++ ) Buffer[i] = 0.0;
    }

    void        NextSample( f64 In ) {
        if (Index >= BufferSize - 1) Index = 0;

        f64 In2         = In * In;

        f64 PrevSample  = Buffer[Index];
        Buffer[Index]   = In2;
        Index++;

        RMSSumm         += In2;
        RMSSumm         -= PrevSample;
    }

    f64         GetRMS( ) {
        if (RMSSumm <= 0.000001) return 0.0;
        f64 RMS = sqrt(RMSSumm/(f64)BufferSize);
        return RMS;
    }
private:
    f64     Buffer[RMSWINDOWSIZE];
    u32     BufferSize = RMSWINDOWSIZE;
    u32     Index = 0;
    f64     RMSSumm = 0.0;
};


#define RMSWINDOWSIZEB (256)

class RMSDetectorB {
public:
    void        Reset( ) {
        Index   = 0;
        RMSSumm  = 0.0;
        for ( u32 i = 0; i < RMSWINDOWSIZEB; i++ ) Buffer[i] = 0.0;
    }

    void        NextSample( f64 In ) {
        if (Index >= BufferSize - 1) Index = 0;

        f64 In2         = In * In;

        f64 PrevSample  = Buffer[Index];
        Buffer[Index]   = In2;
        Index++;

        RMSSumm         += In2;
        RMSSumm         -= PrevSample;
    }

    f64         GetRMS( ) {
        if (RMSSumm <= 0.000001) return 0.0;
        f64 RMS = sqrt(RMSSumm/(f64)BufferSize);
        return RMS;
    }
private:
    f64     Buffer[RMSWINDOWSIZEB];
    u32     BufferSize = RMSWINDOWSIZEB; // RMSWINDOWSIZE????????????????
    u32     Index = 0;
    f64     RMSSumm = 0.0;
};


#define RMSWINDOWSIZEC (1024*8)

class RMSDetectorC {
public:
    void        Reset() {
        Index = 0;
        RMSSumm = 0.0;
        for (u32 i = 0; i < RMSWINDOWSIZEC; i++) Buffer[i] = 0.0;
    }

    void        NextSample(f64 In) {
        if (Index >= BufferSize - 1) Index = 0;

        f64 In2 = In * In;

        f64 PrevSample = Buffer[Index];
        Buffer[Index] = In2;
        Index++;

        RMSSumm += In2;
        RMSSumm -= PrevSample;
    }

    f64         GetRMS() {
        if (RMSSumm <= 0.000001) return 0.0;
        f64 RMS = sqrt(RMSSumm / (f64)BufferSize);
        return RMS;
    }
private:
    f64     Buffer[RMSWINDOWSIZEC];
    u32     BufferSize = RMSWINDOWSIZEC;
    u32     Index = 0;
    f64     RMSSumm = 0.0;
};
