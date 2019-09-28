#ifndef RATE_H
#define RATE_H

#include <time.h>
#include <sys/time.h>
#include <argos3/core/utility/math/general.h>

namespace argos {

  /*
   * Helper to keep a constant rate in loops.
   *
   * This class is a simple helper that allows you to keep a constant rate in a
   * loop such as a robot control loop. Its usage is simple:
   *
   * // Set the number of "ticks" per second
   * CRate cRate(10.0);
   * while(1) {
   *   // do stuff
   *   // ...
   *   // Sleep enough time to complete the tick period
   *   // If the tick period was execeed before this call,
   *   // don't sleep and write a warning message
   *   cRate.Sleep();
   * }
   */
  class CRate {

  public:

    /**
     * Class constructor.
     * @param f_rate The number of ticks per second.
     */
    CRate(Real f_rate);

    /**
     * Class destructor.
     */
    ~CRate() {}

    /**
     * Returns the time elapsed since the last sleep in microseconds.
     */
    UInt64 ElapsedUS() const;

    /**
     * Returns the time elapsed since the last sleep in seconds.
     */
    Real ElapsedS() const;

    /**
     * Sleeps for the appropriate time to complete the period.
     * If the period was exceeded before this call, this method does not
     * sleep and, instead, write a warning on the log.
     */
    void Sleep();

    /**
     * Returns the rate.
     */
    inline Real GetRate() const {
      return m_fNominalRate;
    }

    /**
     * Sets the rate and resets the internal clock.
     * @param f_rate The number of ticks per second.
     */
    void SetRate(Real f_rate);

  private:

    Real m_fNominalRate;
    UInt64 m_unNominalPeriod;
    ::timeval m_tPast;

  };

}

#endif // RATE_H
