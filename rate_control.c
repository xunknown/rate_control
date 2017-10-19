#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <time.h>
#include<unistd.h>
#define TIME_SLICE_COUNT 100
int main(int argc, char * argv[])
{
    struct  timeval tv;
    struct  timezone tz;
    gettimeofday(&tv, &tz);
    printf("tv_sec: %ld\n", tv.tv_sec);
    printf("tv_usec: %ld\n", tv.tv_usec);
    printf("tz_minuteswest: %d\n", tz.tz_minuteswest);
    printf("tz_dsttime: %d\n", tz.tz_dsttime);
    usleep(1000);
    gettimeofday(&tv, &tz);
    printf("tv_sec: %ld\n", tv.tv_sec);
    printf("tv_usec: %ld\n", tv.tv_usec);
    printf("tz_minuteswest: %d\n", tz.tz_minuteswest);
    printf("tz_dsttime: %d\n", tz.tz_dsttime);
    long pps = 1000 * 1000 * 1000;
    long speeds[TIME_SLICE_COUNT] = { 0 };
    long time_slice = (1000 * 1000) / TIME_SLICE_COUNT; // us

    for (int i = 0; i < TIME_SLICE_COUNT; i++) {
        speeds[i] = pps / TIME_SLICE_COUNT;
    }

    for (int i = 0; i < 10; i++) {
        long pps1 = 0;
        struct  timeval tvb;
        struct  timeval tve;
        gettimeofday(&tvb, NULL);
        printf("tv_sec: %ld, tv_usec: %ld, loop %d\n", tvb.tv_sec, tvb.tv_usec, i);

        for (int j = 0; j < TIME_SLICE_COUNT; j++) {
            struct  timeval tvb;
            struct  timeval tve;
            gettimeofday(&tvb, NULL);
            printf("tv_sec: %ld, tv_usec: %ld, %d, loop %ld\n", tvb.tv_sec, tvb.tv_usec, j, speeds[j]);
            pps1 += speeds[j];
            usleep(rand() % time_slice);
            gettimeofday(&tve, NULL);
            long spend = (tve.tv_sec - tvb.tv_sec) * 1000 * 1000;
            spend += tve.tv_usec - tvb.tv_usec;
            long rem = time_slice - spend;

            if (rem > 0) {
                usleep(rem);
                /*
                struct timespec req = { 0 };
                struct timespec rem1 = { 0 };
                req.tv_sec = rem / (1000 * 1000);
                req.tv_nsec = rem % (1000 * 1000) * 1000;
                int ret = nanosleep(&req, &rem1);
                printf("nanosleep ret: %d, rem: %ld,"
                       " req tv_sec: %ld, req tv_nsec: %ld,"
                       " rem1 tv_sec: %ld, rem1 tv_nsec: %ld\n",
                       ret, rem, req.tv_sec, req.tv_nsec, rem1.tv_sec, rem1.tv_nsec);
                */
            }
        }

        gettimeofday(&tve, NULL);
        long spend = (tve.tv_sec - tvb.tv_sec) * 1000 * 1000;
        spend += tve.tv_usec - tvb.tv_usec;
        printf("loop: %d, pps: %ld, pps1 %ld, spend: %ld\n", i, pps, pps1, spend);
    }

    return 0;
}
