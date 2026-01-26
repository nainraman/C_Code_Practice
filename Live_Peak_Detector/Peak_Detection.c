#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>  // Add for bool type

// Spike detection parameters
#define ALPHA_BASELINE      0.02f    // EMA smoothing factor for baseline
#define THRESHOLD_HIGH      5.0f     // High threshold for spike detection
#define THRESHOLD_LOW       3.0f     // Low threshold for spike exit
#define MIN_SPIKE_SAMPLES   2        // Minimum samples required to confirm spike
#define MIN_EXIT_SAMPLES    2        // Minimum samples required to exit spike

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
    uint8_t exitcount;               // Counter for spike exit confirmation
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
    st->exitcount  = 0;              // Initialize exit counter
}

//----------------------------------------------
// Update baseline (EMA) and detect spikes
//----------------------------------------------
bool Spike_Detector(SpikeState *st, float sample)
{
    // Update baseline using the defined constant
    float b_prev = st->baseline;
    float baseline = (1.0f - ALPHA_BASELINE) * b_prev + ALPHA_BASELINE * sample;
    st->baseline = baseline;

    // Compute deviation
    float diff = sample - baseline;

    // Spike detection logic using defined constants
    if (!st->in_spike)
    {
        // Not in spike: look for high threshold crossing
        if (fabsf(diff) > THRESHOLD_HIGH)
        {
            st->spikecount++;
            st->exitcount = 0;       // Reset exit counter when above high threshold

            if (st->spikecount >= MIN_SPIKE_SAMPLES)
            {
                st->in_spike = 1;
                st->spikecount = 0;

                return true;  // spike event detected
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
        if (fabsf(diff) < THRESHOLD_LOW)
        {
            st->exitcount++;
            st->spikecount = 0;      // Reset spike counter when below low threshold
            
            if (st->exitcount >= MIN_EXIT_SAMPLES)
            {
                st->in_spike = 0;
                st->exitcount = 0;
            }
        }
        else
        {
            st->exitcount = 0;       // Reset exit counter if signal goes back above threshold
        }
    }

    return false;  // no new spike
}

//----------------------------------------------
// Run every 10 ms
//----------------------------------------------
void run10ms(void)
{
    float sample = (float)getSensorData();

    bool spike = Spike_Detector(&st, sample);

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
