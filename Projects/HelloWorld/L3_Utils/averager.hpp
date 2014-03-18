/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

#ifndef AVERAGER_HPP_
#define AVERAGER_HPP_

/**
 * Averager class.
 * The purpose of this class is to get average for a bunch of samples
 *
 * @code
 * Average<int> samples(2);
 * samples.storeSample(10);
 * samples.storeSample(20);
 * int avg = samples.getAverage(); // Should be 15
 * @endcode
 */
template <typename TYPE>
class Averager
{
    public:
        Averager(int numSamples) : mSampleArraySize(numSamples), mSampleIndex(0), mSamplesReady(false)
        {
            mSamples = new TYPE[numSamples];
        }

        void storeSample(const TYPE& sample)
        {
            mSamples[mSampleIndex] = sample;
            if(++mSampleIndex >= mSampleArraySize) {
                mSampleIndex = 0;
                mSamplesReady = true;
            }
        }

        TYPE getAverage(void)
        {
            // If not enough samples collected, assume the average is the 1st sample
            if(!mSamplesReady) {
                return mSamples[0];
            }

            TYPE sum = 0;
            for(int i=0; i < mSampleArraySize; i++) {
                sum += mSamples[i];
            }

            return (sum / mSampleArraySize);
        }

    private:
        Averager() {} ///< Do not use this contructor.

        const int mSampleArraySize; ///< Number of samples
        int mSampleIndex;   ///< Index of next sample that will get stored to mSamples array
        bool mSamplesReady; ///< If the whole array is not filled, we can't compute the average
        TYPE* mSamples;     ///< Array of samples
};

#endif /* AVERAGER_HPP_ */
