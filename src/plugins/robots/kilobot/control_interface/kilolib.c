#include "kilolib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

/*
 * Mersenne-Twister-related constants
 *
 * These variables and constants are used to implement the
 * Mersenne-Twister random number generation algorithm. The algorithm
 * is used in rand_hard().
 */
#define MT_N          624
#define MT_M          397
#define MT_MATRIX_A   0x9908b0dfUL /* constant vector a */
#define MT_UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define MT_LOWER_MASK 0x7fffffffUL /* least significant r bits */
#define MT_INT_MAX    0xFFFFFFFFUL
int32_t* mt_rngstate; /* Random number generator state */
uint32_t mt_rngidx;   /* Random number generator index */

/* Sets the seed of a Mersenne-Twister random number generator */
void mt_setseed(uint32_t seed) {
   mt_rngstate[0] = seed & 0xffffffffUL;
   for(mt_rngidx = 1; mt_rngidx < MT_N; ++mt_rngidx) {
      mt_rngstate[mt_rngidx] =
         (1812433253UL * (mt_rngstate[mt_rngidx-1] ^ (mt_rngstate[mt_rngidx-1] >> 30)) + mt_rngidx);
      mt_rngstate[mt_rngidx] &= 0xffffffffUL;
   }
}

/* Return a random number on 32 bits */
uint32_t mt_uniform32() {
   uint32_t y;
   static uint32_t mag01[2] = { 0x0UL, MT_MATRIX_A };
   /* mag01[x] = x * MT_MATRIX_A  for x=0,1 */
   if (mt_rngidx >= MT_N) { /* generate N words at one time */
      int32_t kk;
      for (kk = 0; kk < MT_N - MT_M; ++kk) {
         y = (mt_rngstate[kk] & MT_UPPER_MASK) | (mt_rngstate[kk+1] & MT_LOWER_MASK);
         mt_rngstate[kk] = mt_rngstate[kk+MT_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      for (; kk < MT_N - 1; ++kk) {
         y = (mt_rngstate[kk] & MT_UPPER_MASK) | (mt_rngstate[kk+1] & MT_LOWER_MASK);
         mt_rngstate[kk] = mt_rngstate[kk+(MT_M-MT_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      y = (mt_rngstate[MT_N-1] & MT_UPPER_MASK) | (mt_rngstate[0] & MT_LOWER_MASK);
      mt_rngstate[MT_N-1] = mt_rngstate[MT_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];
      mt_rngidx = 0;
   }
   y = mt_rngstate[mt_rngidx++];
   /* Tempering */
   y ^= (y >> 11);
   y ^= (y << 7) & 0x9d2c5680UL;
   y ^= (y << 15) & 0xefc60000UL;
   y ^= (y >> 18);
   return y;
}

/* Kilolib original variables */
uint32_t kilo_ticks                = 0;
uint16_t kilo_tx_period            = 100;
uint16_t kilo_uid                  = 0;
uint8_t  kilo_turn_left            = 255;
uint8_t  kilo_turn_right           = 255;
uint8_t  kilo_straight_left        = 255;
uint8_t  kilo_straight_right       = 255;
void message_rx_dummy(message_t* m, distance_measurement_t* d) { }
message_t *message_tx_dummy() { return NULL; }
void message_tx_success_dummy() {}
message_rx_t kilo_message_rx = message_rx_dummy;
message_tx_t kilo_message_tx = message_tx_dummy;
message_tx_success_t kilo_message_tx_success = message_tx_success_dummy;

/* Variables necessary here */
static uint32_t  argos_tick_length = 0;    // length of an argos tick in ms
static uint32_t  kilo_tx_clock     = 0;    // message tx clock
static uint32_t  kilo_delay        = 0;    // delay clock
static uint8_t   kilo_seed         = 0xAA; // default random seed
static uint8_t   kilo_accumulator  = 0;    // rng accumulator
static char*     kilo_str_id       = NULL; // kilobot id as string
static int       kilo_state_fd     = -1;   // shared memory file
kilobot_state_t* kilo_state        = NULL; // shared robot state

void preloop() {
   /* Update tick count */
   // TODO: this is approximated, and works only if (argos_tick_length > TICKS_PER_SEC)
   kilo_ticks += argos_tick_length / TICKS_PER_SEC;
   /* Message sent? */
   if(kilo_state->tx_state != 2) {
      /* No message sent */
      kilo_tx_clock += argos_tick_length;
   } else {
      /* Message sent */
      kilo_state->tx_state = 0;
      kilo_tx_clock = 0;
      kilo_message_tx_success();
   }
   /* Message received? */
   if(kilo_state->rx_state > 0) {
      uint8_t i;
      for(i = 0; i < kilo_state->rx_state; ++i) {
         kilo_message_rx(&kilo_state->rx_message[i], &kilo_state->rx_distance[i]);
      }
      kilo_state->rx_state = 0;
   }
}

void postloop() {
   /* Send message? */
   if(kilo_state->tx_state == 0 &&
      kilo_tx_clock > kilo_tx_period) {
      message_t* msg = kilo_message_tx();
      if(msg) {
         /* Attempt to send message */
         kilo_state->tx_state = 1;
         memcpy(&kilo_state->tx_message, msg, sizeof(message_t));
      }
   }
}

uint8_t estimate_distance(const distance_measurement_t* d) {
   return d->high_gain;
}

void delay(uint16_t ms) {
   /* If the delay is shorter than the tick length, it's no delay at all */
   if(ms < argos_tick_length) return;
   /* Set delay counter and wait */
   kilo_delay = ms;
   postloop();
   while(kilo_delay > 0) {
      /* Suspend process, waiting for ARGoS controller's resume signal */
      raise(SIGTSTP);
      /* Update state */
      preloop();
      /* Are we done waiting? */
      if(kilo_delay > argos_tick_length) {
         /* No, keep going */
         kilo_delay -= argos_tick_length;
         postloop();
      }
      else {
         /* Done waiting! */
         kilo_delay = 0;
      }
   }
}

uint8_t rand_hard() {
   return mt_uniform32();
}

uint8_t rand_soft() {
   kilo_seed ^= kilo_seed << 3;
   kilo_seed ^= kilo_seed >> 5;
   kilo_seed ^= kilo_accumulator++ >> 2;
   return kilo_seed;
}

void rand_seed(uint8_t seed) {
   kilo_seed = seed;
}

int16_t get_ambientlight() {
   return kilo_state->ambientlight;
}

int16_t get_voltage() {
   return kilo_state->voltage;
}

int16_t get_temperature() {
   return kilo_state->temperature;
}

void set_motors(uint8_t left, uint8_t right) {
   kilo_state->left_motor  = left;
   kilo_state->right_motor = right;
}

void spinup_motors() {
   set_motors(255, 255);
}

void set_color(uint8_t color) {
   kilo_state->color = color;
}

void kilo_init() {
}

void cleanup() {
   munmap(kilo_state, sizeof(kilobot_state_t));
   close(kilo_state_fd);
   shm_unlink(kilo_str_id);
}

void sigkill_handler(int s) {
   cleanup();
}

void kilo_start(void (*setup)(void), void (*loop)(void)) {
   /* Install handler for SIGKILL */
   signal(SIGKILL, sigkill_handler);
   /* Execute setup() */
   setup();
   /* Continue working until killed by ARGoS controller */
   while(1) {
      /* Suspend yourself, waiting for ARGoS controller's resume signal */
      raise(SIGTSTP);
      /* Resumed */
      /* Execute loop */
      preloop();
      loop();
      postloop();
   }
}

/*
 * Main function wrapper
 */
static uint16_t argos_id_to_kilo_uid(const char* argos_id) {
   /* Skip all non-digit characters */
   size_t pos = 0;
   while(argos_id[pos] != 0 && !isdigit(argos_id[pos])) ++pos;
   /* Got to the end? */
   if(argos_id[pos] == 0) return 0;
   /* Now get the rest and return it as a number */
   return strtoul(argos_id + pos, NULL, 10);
}

/* main() wrapper */
int __kilobot_main(int argc, char* argv[]);
#undef main
int main(int argc, char* argv[]) {
   /* Parse arguments */
   if(argc != 4) {
      int i;
      fprintf(stderr, "Error: %s was given %d arguments\n", argv[0], argc);
      for(i = 0; i < argc; ++i) {
         fprintf(stderr, "\tARG %d: %s\n", i, argv[i]);
      }
      fprintf(stderr, "Usage: <script> <robot_id> <tick_length> <random_seed>\n");
      exit(1);
   }
   kilo_str_id = strdup(argv[1]);
   /* Open shared memory */
   kilo_state_fd = shm_open(kilo_str_id, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   if(kilo_state_fd < 0) {
      fprintf(stderr, "Opening the shared memory file of %s: %s\n", kilo_str_id, strerror(errno));
      exit(1);
   }
   /* Resize shared memory area to contain the robot state, filling it with zeros */
   ftruncate(kilo_state_fd, sizeof(kilobot_state_t));
   /* Get pointer to shared memory area */
   kilo_state =
      (kilobot_state_t*)mmap(NULL,
                             sizeof(kilobot_state_t),
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED,
                             kilo_state_fd,
                             0);
   if(kilo_state == MAP_FAILED) {
      fprintf(stderr, "Mmapping the shared memory area of %s: %s\n", kilo_str_id, strerror(errno));
      close(kilo_state_fd);
      shm_unlink(kilo_str_id);
      exit(1);
   }
   /* Install cleanup function */
   atexit(cleanup);
   /* Initialize variables */
   kilo_uid = argos_id_to_kilo_uid(kilo_str_id);
   argos_tick_length = strtoul(argv[2], NULL, 10);
   /* Initialize random number generator */
   mt_rngstate = (int32_t*)malloc(MT_N * sizeof(int32_t));
   mt_rngidx = MT_N + 1;
   mt_setseed(strtoul(argv[3], NULL, 10));
   /* Call main of behavior */
   return __kilobot_main(argc, argv);
}
