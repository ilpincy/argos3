#ifndef __KILOLIB_H__
#define __KILOLIB_H__

/**
 * @file kilolib.h
 * @mainpage Kilobot Library API
 *
 * At its core the Kilobot Library library provides the function
 * kilo_init() to initialize the hardware of the kilobots, and the
 * function kilo_start() that uses a basic event loop programming
 * paradigm (this relies on a setup and a loop callback, similar to
 * those used in the arduino software).
 *
 * The API also provides functions to read the various sensors available
 * to the kilobots (get_ambientlight(), get_voltage(), get_temperature()), and
 * also to control the individual pager motors and the RGB led present
 * in each kilobot (set_motors(), set_color()).
 *
 * The user can register callbacks to interact with the messaging
 * subsystem. There are callbacks for the events of message reception
 * (::kilo_message_rx), message transmission (::kilo_message_tx), and
 * notification of successful transmission (::kilo_message_tx_success).
 * By default every kilobot attempts to send message twice per second.
 * Advanced users can modify this through the `kilo_tx_period` variable,
 * although this is discouraged unless you know what you are doing.
 *
 * To prevent collisions the kilobot library uses a basic exponential
 * back-off strategy with carrier sensing. There are no acknowledgement
 * packets, and as such a message is considered to be successfully
 * transmitted when a kilobot is able transmit a message without
 * detecting any contention in the channel.
 */

#include <stdint.h>
#include "message.h"
#include "message_crc.h"

#define RGB(r,g,b) (r&3)|(((g&3)<<2))|((b&3)<<4)
#define RED(c)     ((c     ) & 3)
#define GREEN(c)   ((c >> 2) & 3)
#define BLUE(c)    ((c >> 4) & 3)
#define TICKS_PER_SEC 31

/**
 * Redefine the 'main' symbol so, in the behavior code, the main of the robot is not called first.
 * The actual main() is in kilolib.c.
 */
#define main __kilobot_main

/**
 * @brief Distance measurement.
 *
 * Every time a message is received by a kilobot, it collects two 10 bit
 * measurements that represent the signal strength of the received
 * message after going through an operational amplifier. This data
 * structure stores these two measurements.
 *
 * Using these two measurements it is possible to estimate the distance
 * of the sender.
 */

typedef struct {
    int16_t low_gain;  ///< Low gain 10-bit signal-strength measurement.
    int16_t high_gain; ///< High gain 10-bit signal-strength measurement.
} distance_measurement_t;

typedef void (*message_rx_t)(message_t *, distance_measurement_t *d);
typedef message_t *(*message_tx_t)(void);
typedef void (*message_tx_success_t)(void);

/**
 * @brief Kilobot clock variable.
 *
 * This variable holds a 32-bit unsigned positive integer. This variable
 * is initialized to zero whenever the program run at the kilobot is
 * reset (or when the kilobot is first turned on). It is incremented
 * approximately 32 times per second, or once every 30ms. 
 *
 * @code
 *
 * void setup() {
 *     last_changed = kilo_ticks;
 * }
 *
 * // blink the LED green for 50ms, once every 2 seconds.
 * void loop() {
 *     if (kilo_ticks > last_changed + 64) {
 *         last_changed = kilo_ticks;
 *         set_color(RGB(0,1,0));
 *         delay(50);
 *         set_color(RGB(0,0,0));
 *     }
 * }
 *
 * int main() {
 *     kilo_init();
 *     kilo_start(setup, loop);
 *
 *     return 0;
 * }
 * @endcode
 */

extern uint32_t kilo_ticks;
extern uint16_t kilo_tx_period;
/**
 * @brief Kilobot unique identifier.
 *
 * This variable holds a 16-bit positive integer which is designated as
 * the kilobot's unique identifier during calibration.
 */
extern uint16_t kilo_uid;
/**
 * @brief Calibrated turn left duty-cycle.
 *
 * This variable holds an 8-bit positive integer which is the calibrated
 * duty-cycle for turning left.
 *
 * @see set_motors
 */
extern uint8_t kilo_turn_left;
/**
 * @brief Calibrated turn right duty-cycle.
 *
 * This variable holds an 8-bit positive integer which is the calibrated
 * duty-cycle for turning right.
 *
 * @see set_motors
 */
extern uint8_t kilo_turn_right;
/**
 * @brief Calibrated straight (left motor) duty-cycle.
 *
 * This variable holds an 8-bit positive integer which is the calibrated
 * duty-cycle used for the left motor to go straight. This must be used
 * in conjuction with kilo_straight_right.
 *
 * @see set_motors, kilo_straight_right
 */
extern uint8_t kilo_straight_left;
/**
 * @brief Calibrated straight (right motor) duty-cycle.
 *
 * This variable holds an 8-bit positive integer which is the calibrated
 * duty-cycle used for the right motor to go straight. This must be used
 * in conjuction with kilo_straight_left.
 *
 * @see set_motors, kilo_straight_left
 */
extern uint8_t kilo_straight_right;

/**
 * @brief Callback for message receiption.
 *
 * This callback is triggered every time a message is succesfully
 * decoded. The callback receives two parameters, a pointer to the
 * message decoded, and a pointer to the distance measurements from the
 * message originator.
 *
 * @code
 *  uint8_t recvd_message = 0;
 *
 *  // receive message callback
 *  void rx_message(message_t *msg, distance_measurement_t *d) {
 *      recvd_message = 1;
 *  }
 *
 *  void setup() {
 *      recvd_message = 0;
 *  }
 *
 *  // blink green when a new message is received
 *  void loop() {
 *      if ( recvd_message ) {
 *          recvd_message = 0;
 *          set_color(RGB(0,1,0));
 *          delay(100);
 *          set_color(RGB(0,0,0));
 *      }
 *  }
 *
 *  int main() {
 *      kilo_init();
 *      // register message callback
 *      kilo_message_rx = rx_message;
 *      kilo_start(setup, loop);
 *
 *      return 0;
 *  }
 * @endcode
 *
 * @note You must register a message callback before calling kilo_start.
 * @see message_t, message_crc, kilo_message_tx, kilo_message_tx_success
 */
extern message_rx_t kilo_message_rx;
/**
 * @brief Callback for message transmission.
 *
 * This callback is triggered every time a message is scheduled for
 * transmission (roughly twice every second). This callback returns a
 * pointer to the message that should be sent; if the pointer is null,
 * the no message is sent.
 *
 * @code
 *
 * message_t msg;
 * uint8_t sent_message;
 *
 * // message transmission callback
 * message_t *tx_message() {
 *     return &msg;
 * }
 *
 * // successful transmission callback
 * void tx_message_success() {
 *     sent_message = 1;
 * }
 *
 * void setup() {
 *    msg.type = NORMAL;
 *    msg.crc = message_crc(&msg);
 * }
 *
 * // blink red when a new message is sent
 * void loop() {
 *     if ( sent_message ) {
 *         sent_message = 0;
 *         set_color(RGB(1,0,0));
 *         delay(100);
 *         set_color(RGB(0,0,0));
 *     
 *     }
 * }
 *
 * int main() {
 *     kilo_init();
 *     // register message transmission calback
 *     kilo_message_tx = tx_message;
 *     // register tranmsission success callback
 *     kilo_message_tx_success = tx_message_success;
 *     kilo_start(setup, loop);
 *
 *     return 0;
 * }
 * @endcode
 *
 * @note The message returned by this callback must have a valid CRC
 * value computed using the message_crc() function.
 *
 * @see message_t, message_crc, kilo_message_tx, kilo_message_tx_success
 */
extern message_tx_t kilo_message_tx;

/**
 * @brief Callback for successful message transmission.
 *
 * This callback is triggered every time a message is sent
 * successfully. It receives no parameters and returns no values.
 *
 * @note For example usage see ::kilo_message_tx .
 * @warning The message subsystem has no acknowledgements, therefore
 * successful message reception is not guaranteed. Instead the
 * successful message callback is called when a message is transmitted
 * and no contention is detected on the channel.
 *
 * @see message_t, message_crc, kilo_message_tx, kilo_message_tx_success
 */
extern message_tx_success_t kilo_message_tx_success;

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/**
 * @brief Estimate distance in mm based on signal strength measurements.
 *
 * This function receives as an argument the signal strength measurements @p d taken
 * during message reception, and returns a positive integer that
 * respresents the distance estimate in mm towards the robot that
 * originated the message.
 *
 * @param d Signal strength measurement for a message.
 * @return Positive integer with distance estimate in mm.
 *
 * @note Internally this function relies on a distance calibration table
 * stored on the EEPROM memory of the kilobots. This table is populated
 * using a special calibration rig. Robots purchased from Kteam come
 * precalibrated.
 */

uint8_t estimate_distance(const distance_measurement_t *d);

/**
 * @brief Pauses the program for the specified amount of time.
 *
 * This function receives as an argument a positive 16-bit integer @p ms
 * that represents the number of milliseconds for which to pause the
 * program.
 *
 * @param ms Number of milliseconds to pause the program (there are 1000
 * milliseconds in a second).
 *
 * @note While its easy to create short delays in the program execution
 * using this function, the processor of the kilobot cannot perform
 * other tasks during this delay functions. In general its preferable to
 * use timers to create delays.
 * @see kilo_ticks
 */
void delay(uint16_t ms);

/**
 * @brief Hardware random number generator.
 *
 * This function uses the analog-to-digital converter to generate an
 * 8-bit random number. Specifically, the robot measures its own battery
 * voltage and extracts randomness from the least significant bit by
 * using Von Neumann's fair-coin algorithm. By its nature, this function
 * is slow, use rand_soft() if you want a faster alternative, and you
 * can seed the software random generator using the output of
 * rand_hard().
 *
 * @see rand_soft, rand_seed
 * @return 8-bit random number.
 */
uint8_t rand_hard();

/**
 * @brief Software random number generator.
 *
 * This function implements a linear-shift-register to implement an
 * 8-bit pseudo-random number generator. The seed of the random number
 * generator can be controlled through rand_seed().
 *
 * @return 8-bit random number.
 */
uint8_t rand_soft();

/**
 * @brief Seed software random number generator.
 *
 * This function changes the seed used by the software random
 * number generator implemented by rand_soft().
 *
 * @param 8-bit random seed.
 */
void rand_seed(uint8_t seed);

/**
 * @brief Read the amount of ambient light.
 *
 * This function returns a 10-bit measurement (0 to 1023) that
 * represents the amount of ambient light detected by the photo diode
 * available in the kilobot.
 *
 * @return 10-bit measurement of ambient light.
 * @note All measurements in the kilobot are performed using the same
 * analog-to-digital conversion (ADC) unit of the AVR processor. This
 * ADC unit requires a certain amount of time to change the source of
 * the measurement. As such, if a message is received while the ambient
 * light is being measured, it is possible for either the ambient light
 * measurements or the distance measurements of the message to be
 * inaccurate.
 */
int16_t get_ambientlight();

/**
 * @brief Read the amount of battery voltage.
 *
 * This function returns a 10-bit measurement (0 to 1023) that
 * represents the amount of voltage that remains in the battery. It can
 * be used to determine if the kilobot should be recharged.
 *
 * @return 10-bit measurement of battery voltage.
 */
int16_t get_voltage();

/**
 * @brief Read the temperature of the kilobot.
 *
 * This function returns a 10-bit measurement (0 to 1023) that
 * represents the temperature of the board of the kilobot. This sensor
 * is only capable of detecting large temperature changes (in the order
 * of 2 Celsius degrees or more).
 *
 * As such, it is only useful only to detect drastic changes in the
 * operating environment of the kilobot.
 */
int16_t get_temperature();

/**
 * @brief Set the power of each motor.
 *
 * The power received by the left and right motor is controlled using
 * hardware pulse-width-modulation (PWM) and can be set using this
 * function.
 *
 * The parameter @p left and @p right are 8-bit unsigned integers (0 to
 * 255) that control the duty-cycle of the PWM signal from 0% to 100%.
 * In other words, setting a motor to 0% duty-cycle equates to running
 * off the motor, and setting a motor to 100% duty-cycle equates to
 * running the motor at full power. For the most part, motors should
 * only be set to the calibrated values `kilo_turn_left`,
 * `kilo_turn_right`, `kilo_straight_left` and `kilo_straight_right`.
 *
 * When a motor transitions from being off (0% duty cycle) to being on
 * (> 10% duty cycle) it must first be turned on at full-speed for 15ms
 * to overcome the effects of static friction. 
 *
 * @see kilo_turn_left
 * @see kilo_turn_right
 * @see kilo_straight_left
 * @see kilo_straight_right
 *
 * @code
 * // turn motors off
 * set_motors(0, 0);
 * // spin up left motor for 15ms
 * set_motors(255, 0);
 * delay(15);
 * // turn the kilobot left for 2 seconds
 * set_motors(kilo_turn_left, 0);
 * delay(2000);
 * // go straight for 2 seconds
 * set_motors(kilo_straight_left, kilo_straight_right);
 * delay(2000);
 * @endcode
 *
 * @param left 8-bit integer to control left motor duty-cycle.
 * @param right 8-bit integer to control right motor duty-cycle.
 *
 * @warning It is important that in a 2 second interval no motor is at
 * 100% duty-cycle (255) for more than 50ms at a time, since this might
 * cause the motors to fail permanently. It is recommended that during
 * regular operation the motors are never run to more than a 35%
 * duty-cycle (90).
 */
void set_motors(uint8_t left, uint8_t right);

/**
 * @brief Turn motors at full-speed for 15ms.
 *
 * When the robot transitions from being stationary (motors off) to
 * being mobile (one or both motors on) it must overcome the effects of
 * static friction. For that purpose, the motors can be turned-pn at
 * full-speed during 15ms. This function does precisely that, and is
 * equivalent to the following code:
 *
 * @code
 * set_motors(255, 255);
 * delay(15);
 * @endcode
 *
 * @note Observe that the spinup() function turns both motors on. In
 * some cases (when turning left or turning right) this is not required,
 * and thus to achieve smoother motion you can do manual `spinup` of a
 * motor. See set_motors() for an example.
 *
 * @see set_motors
 */
void spinup_motors();

/**
 * @brief Set the output of the RGB led.
 *
 * This function receives an 8-bit unsigned integer whose bits are used
 * to determine the output of the RGB led mounted on the kilobot. Each
 * color has a 2-bit resolution which allows set each color channel
 * independently from off (0) to full-brightness (3).
 *
 * The convenience macro `RGB` can be used to set the individual bits.
 * For instance `RGB(0,0,0)` turns all color channels off, and therefore
 * the RGB led remains off. Meanwhile `RGB(0,3,0)` turns the green
 * channel to full intensity and turns all other channels off, which
 * results in an RGB led displaying a bright green color.
 *
 * @param color Output of the led. The recommended usage is through the
 * `RGB` macro.
 *
 * @code
 * // blink dim RED once per second
 * while (1) {
 *   set_color(RGB(1,0,0));
 *   delay(500);
 *   set_color(RGB(0,0,0));
 *   delay(500);
 * }
 * @endcode
 */
void set_color(uint8_t color);

/**
 * @brief Initialize kilobot hardware.
 *
 * This function initializes all hardware of the kilobots. This includes
 * calibrating the hardware oscillator, setting hardware timers,
 * configuring ports, setting up analog-to-digital converters,
 * registering system interrupts and the initializing the messaging
 * subsystem.
 * 
 * It is recommended that you call this function as early as possible
 * inside the `main` function of your program.
 */
void kilo_init();

/**
 * @brief Start kilobot event loop.
 *
 * This function receives two parameters. The first parameter @p setup
 * is a function which will be called once to perform any initialization
 * required by your user program. The second parameter @p loop is a
 * function that will be called repeatedly to perform any computations
 * required by your user program.
 *
 * Using the overhead controller it is possible to interrupt the event
 * loop to trigger events such as program start/resume, program pause,
 * and program restart.
 *
 * @param setup put your setup code here, to be run only once
 * @param loop  put your main code here, will be run repeatedly
 *
 * @code
 *
 * uint32_t counter;
 *
 * void setup() {
 *    counter = 0;
 * }
 *
 * void loop() {
 *    counter++;
 * }
 *
 * int main() {
 *   kilo_init();
 *   kilo_start(setup, loop);
 *   return 0;
 * }
 * @endcode
 */
void kilo_start(void (*setup)(void), void (*loop)(void));


/**
 * Maximum number of messages received by a Kilobot in a timestep
 */
#define KILOBOT_MAX_RX 4

/**
 * @brief Kilobot state, used for communication with ARGoS.
 *
 * This data structure is used by ARGoS and by the Kilobot behavior to communicate.
 * The structure contains the status of the sensors and actuators of the Kilobot.
 *
 * Do not use this in your own programs.
 */
typedef struct {
   message_t              tx_message;     // the message to send
   uint8_t                tx_state;       // 0 = none, 1 = sending, 2 = sent
   message_t              rx_message[KILOBOT_MAX_RX];  // the received messages
   distance_measurement_t rx_distance[KILOBOT_MAX_RX]; // distance of message sources
   uint8_t                rx_state;       // 0 = none, >0 # of received messages
   int16_t                ambientlight;   // used by get_ambientlight()
   int16_t                voltage;        // used by get_voltage()
   int16_t                temperature;    // used by get_temperature()
   uint8_t                left_motor;     // used by set_motors()
   uint8_t                right_motor;    // used by set_motors()
   uint8_t                color;          // used by set_color()
} kilobot_state_t;

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif//__KILOLIB_H__
