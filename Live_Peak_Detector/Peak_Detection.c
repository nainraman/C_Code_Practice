#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

uint16_t getSensorData(void)
{
    static int counter = 0;
    counter++;

    // Simulate a spike every 100 samples
    if (counter % 100 == 0)
    {
        return 10;  // Simulated spike value
    }

    return 1;  // Normal baseline value
}

//----------------------------------------------
// Persistent state structure
//----------------------------------------------
typedef struct
{
    float baseline;
    uint8_t in_spike;
    uint8_t spikecount;
} SpikeState;

static SpikeState st;  // persistent across calls

//----------------------------------------------
// Initialize the detector
//----------------------------------------------
void Spike_DetectorInit(SpikeState *st, float firstSample)
{
    st->baseline   = firstSample;
    st->in_spike   = 0;
    st->spikecount = 0;
}

//----------------------------------------------
// Update baseline (EMA) and detect spikes
//----------------------------------------------
uint8_t Spike_Detector(SpikeState *st,
                       float sample,
                       float alpha,
                       float thresholdHigh,
                       float thresholdLow,
                       uint8_t minSpikeSamples)
{
    // Update baseline
    float b_prev = st->baseline;
    float baseline = (1.0f - alpha) * b_prev + alpha * sample;
    st->baseline = baseline;

    // Compute deviation
    float diff = sample - baseline;

    // Spike detection logic
    if (!st->in_spike)
    {
        // Not in spike: look for high threshold crossing
        if (fabsf(diff) > thresholdHigh)
        {
            st->spikecount++;

            if (st->spikecount >= minSpikeSamples)
            {
                st->in_spike = 1;
                st->spikecount = 0;

                return 1;  // spike event detected
            }
        }
        else
        {
            st->spikecount = 0;
        }
    }
    else
    {
        // Already in spike: wait to drop below low threshold
        if (fabsf(diff) < thresholdLow)
        {
            st->in_spike = 0;
        }
    }

    return 0;  // no new spike
}

//----------------------------------------------
// Run every 10 ms
//----------------------------------------------
void run10ms(void)
{
    float alpha = 0.02f;
    float thresholdHigh = 5.0f;   // tune these for your test signal
    float thresholdLow  = 3.0f;
    uint8_t minSpikeSamples = 2;

    float sample = (float)getSensorData();

    uint8_t spike = Spike_Detector(&st, sample,
                                   alpha,
                                   thresholdHigh,
                                   thresholdLow,
                                   minSpikeSamples);

    if (spike)
    {
        printf("Spike detected!\n");
        fflush(stdout);
    }

    // Debugging logs
    printf("Sample: %.2f, Baseline: %.2f, Diff: %.2f, SpikeCount: %d\n",
           sample, st.baseline, sample - st.baseline, st.spikecount);
    fflush(stdout);
}

//----------------------------------------------
// Main
//----------------------------------------------
int main()
{
    // Initialize with first reading
    Spike_DetectorInit(&st, (float)getSensorData());

    // Simulate 1000 × 10 ms calls
    for (int i = 0; i < 1000; i++)
    {
        run10ms();
    }

    return 0;
}
