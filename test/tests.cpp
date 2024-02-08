#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};

TEST(load_process_control_blocks, NullFileName) {
    const char *input_filename = NULL;
    EXPECT_EQ(NULL, load_process_control_blocks(input_filename));
}

TEST (load_process_control_blocks, BadFileName) {
	const char *input_filename = "\n";
	EXPECT_EQ(NULL, load_process_control_blocks(input_filename));
}

TEST(first_come_first_serve, NullReadyQueue) {
    dyn_array_t *ready_queue = NULL;
    ScheduleResult_t *result = new ScheduleResult_t;
    EXPECT_EQ(false, first_come_first_serve(ready_queue, result));
    delete result;
}

TEST(first_come_first_serve, NullResult) {
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ScheduleResult_t *result = NULL;
    EXPECT_EQ(false, first_come_first_serve(ready_queue, result));
    dyn_array_destroy(ready_queue);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
